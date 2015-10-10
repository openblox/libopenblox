/*
 * Copyright 2015 John M. Harris, Jr.
 *
 * This file is part of OpenBlox.
 *
 * OpenBlox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenBlox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenBlox.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "OpenBlox.h"

#include "OBException.h"

#include <QtCore>
//#include <QtCore/qcoreapplication.h>

#include <iostream>
#include <string>

#include <unistd.h>

namespace OpenBlox{
	bool shouldQuit = false;

	#ifndef OB_NO_GRAPHICS
	Ogre::Root* root;
	Ogre::RenderWindow* renderWindow;
	Ogre::SceneManager* sceneMgr;
	Ogre::Camera* cam;
	Ogre::Viewport* vp;

	SDL_Window* mw;
	SDL_GLContext maincontext;
	#endif

	int taskThreadFunc(void* ptr){
		OB_UNUSED(ptr);

		try{
			OpenBlox::OBGame* game = OpenBlox::OBGame::getInstance();
			if(!game){
				throw new OpenBlox::OBException("game is NULL!");
			}

			while(!shouldQuit){
				bool result = game->tick();
				if(!result){ //Error or shutdown, we don't care which here. (Errors should have already been handled)
					break;
				}
				SDL_Delay(10); //Sleep for 10ms
			}
		}catch(OpenBlox::OBException &e){
			std::cout << "A runtime exception occured: " << e.getMessage() << std::endl;

			#if !defined(OB_NO_GRAPHICS) && !defined(OB_EMBEDDED)
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Runtime Exception", e.getMessage().c_str(), NULL);
			#endif
		}catch(...){
			std::cout << "An unknown runtime exception occured." << std::endl;
			#if !defined(OB_NO_GRAPHICS) && !defined(OB_EMBEDDED)
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Runtime Exception", "An unknown error occured.", NULL);
			#endif
		}

		shouldQuit = true;

		return 0;
	}
}

#if !defined(OB_DUMPMODE) && !defined(OB_EMBEDDED)

#ifndef OB_NO_GRAPHICS
void setupIcon(){
	QFile iconFile(":rc/icon.png");
	if(iconFile.exists()){
		QFileInfo info(iconFile);

		qint64 file_size = info.size();
		char* file_contents = new char[file_size + 1];

		if(iconFile.open(QIODevice::ReadOnly)){
			if(iconFile.read(file_contents, file_size) == file_size){
				file_contents[file_size] = '\0';

				SDL_RWops* iconOps = SDL_RWFromConstMem(file_contents, file_size);
				if(iconOps){
					SDL_Surface* iconSurf = IMG_Load_RW(iconOps, 0);
					if(iconSurf){
						SDL_SetWindowIcon(OpenBlox::mw, iconSurf);
					}
				}
			}
			return;
		}
	}
}

void saveState(QSettings* settings){
	if(!OpenBlox::mw){
		return;
	}

	int x;
	int y;
	int width;
	int height;

	SDL_GetWindowPosition(OpenBlox::mw, &x, &y);
	SDL_GetWindowSize(OpenBlox::mw, &width, &height);

	QRect geom = QRect(x, y, width, height);

	int screenNumber = SDL_GetWindowDisplayIndex(OpenBlox::mw);

	Uint32 windowState = SDL_GetWindowFlags(OpenBlox::mw);
	bool maximized = false;
	if((windowState & SDL_WINDOW_MAXIMIZED) == SDL_WINDOW_MAXIMIZED){
		maximized = true;
	}
	bool isfullscreen = false;
	if((windowState & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN){
		isfullscreen = true;
	}

	QByteArray array;//Emulate the way Qt stores geometry, even if it is just for looks. (That nobody will notice) There is a practical sense to this, being that we give ourselves the option to use Qt windows in the future, if we really want to.
	QDataStream stream(&array, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_4_0);
	const quint32 magicNumber = 0x1D9D0CB;
	quint16 majorVersion = 2;
	quint16 minorVersion = 0;
	stream << magicNumber << majorVersion << minorVersion;
	stream << geom << geom;
	stream << qint32(screenNumber);
	if(maximized){
		stream << quint8(Qt::WindowMaximized & Qt::WindowMaximized);
	}else{
		stream << quint8(0);
	}
	if(isfullscreen){
		stream << quint8(Qt::WindowFullScreen & Qt::WindowFullScreen);
	}else{
		stream << quint8(0);
	}
	SDL_DisplayMode dm;
	if(SDL_GetDesktopDisplayMode(screenNumber, &dm) == 0){
		stream << qint32(dm.w);
	}else{
		stream << qint32(0);//If we can't get display info, why not put false data in?
		//Pretend there isn't even an error!
	}

	settings->beginGroup("client_window");
	{
		settings->setValue("geometry", array);
	}
	settings->endGroup();
}

void restoreState(QSettings* settings){
	if(!OpenBlox::mw){
		return;
	}
	settings->beginGroup("client_window");
	{
		if(settings->contains("geometry")){
			QByteArray array = settings->value("geometry").toByteArray();
			if(array.size() < 4){
				return;
			}
			QDataStream stream(array);
			stream.setVersion(QDataStream::Qt_4_0);

			const quint32 magicNumber = 0x1D9D0CB;
			quint32 storedMagicNumber;
			stream >> storedMagicNumber;
			if(storedMagicNumber != magicNumber){
				return;
			}

			const quint16 currentMajorVersion = 2;
			quint16 majorVersion = 0;
			quint16 minorVersion = 0;

			stream >> majorVersion >> minorVersion;

			if(majorVersion > currentMajorVersion){
				return;
			}

			QRect restoredFrameGeometry;
			QRect _ignoredGeometry;
			qint32 restoredScreenNumber;
			quint8 maximized;
			quint8 fullScreen;
			qint32 restoredScreenWidth = 0;

			stream >> restoredFrameGeometry;
			stream >> _ignoredGeometry;
			stream >> restoredScreenNumber;
			stream >> maximized;
			stream >> fullScreen;

			if(majorVersion > 1){
				stream >> restoredScreenWidth;
			}

			if(restoredFrameGeometry.isValid()){
				//Sanity check, if the display size is smaller than it was when saved don't restore!
				SDL_DisplayMode dm;
				if(SDL_GetDesktopDisplayMode(restoredScreenNumber, &dm) == 0){
					if(dm.w >= restoredScreenWidth){
						SDL_SetWindowPosition(OpenBlox::mw, restoredFrameGeometry.x(), restoredFrameGeometry.y());
						SDL_SetWindowSize(OpenBlox::mw, restoredFrameGeometry.width(), restoredFrameGeometry.height());
					}
				}

				if(maximized){
					SDL_MaximizeWindow(OpenBlox::mw);
				}

				if(fullScreen){
					SDL_SetWindowFullscreen(OpenBlox::mw, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
			}
		}
	}
	settings->endGroup();
}
#endif

int main(int argc, char* argv[]){
	std::string initScript = "res://init.lua"; //TODO: Remove init script in favor of game files

	QCoreApplication app(argc, argv);
	#ifdef OB_SERVER
		app.setApplicationName("OpenBlox server");
	#else
		app.setApplicationName("OpenBlox client");
	#endif
	app.setApplicationVersion(OB_VERSION_STR);
	app.setOrganizationDomain("myzillawr.tk");
	app.setOrganizationName("Myzilla Web Resources");

	QCommandLineParser parser;
	parser.setApplicationDescription("OpenBlox game engine");
	QCommandLineOption help_opt = parser.addHelpOption();
	QCommandLineOption ver_opt = parser.addVersionOption();

	QCommandLineOption initScriptOption("script", "Script to run on initialization.", "res://init.lua");
	parser.addOption(initScriptOption);

	bool parse_success = parser.parse(app.arguments());
	if(!parse_success){
		std::cout << parser.errorText().toStdString() << std::endl;
		return EXIT_FAILURE;
	}

	if(parser.isSet(help_opt)){
		parser.showHelp(0);
	}

	if(parser.isSet(ver_opt)){
		std::cout
			#ifdef OB_SERVER
			<< "OpenBlox server "
			#else
			<< "OpenBlox client"
			#endif
			<< OB_VERSION_STR << std::endl;

		std::cout << "Copyright (C) 2015 John M. Harris, Jr." << std::endl;
		std::cout << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>." << std::endl;
		std::cout << "This is free software: you are free to change and redistribute it." << std::endl;
		std::cout << "This is NO WARRANTY, to the extend permitted by law." << std::endl;
		std::cout << std::endl;
		std::cout << "OpenBlox home page: <https://gitlab.com/myzillawr/openblox>" << std::endl;
		std::cout << "Report bugs to: <https://myzillawr.tk/bugzilla>" << std::endl;
		return EXIT_SUCCESS;
	}

	if(parser.isSet(initScriptOption)){
		initScript = parser.value(initScriptOption).trimmed().toStdString();
	}

	OpenBlox::static_init::execute(false); //Runs the "static" initializer.

	#ifndef OB_NO_GRAPHICS
	SDL_SetMainReady();
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "Unable to initialize SDL" << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); //May need to change to 16 for some systems, perhaps 32 on others.

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2); //Perhaps make this a runtime option, or possibly compile-time.

	SDL_Window* mw = SDL_CreateWindow("OpenBlox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(!mw){
		std::cout << "Failed to create window." << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_SetWindowMinimumSize(mw, 640, 480);

	SDL_GLContext ctx = SDL_GL_CreateContext(mw);
	if(!ctx){
		std::cout << "Failed to create GL context." << std::endl;
		SDL_Quit();
		return 1;
	}

	OpenBlox::mw = mw;

	//Init OGRE
	Ogre::Root* root = new Ogre::Root("", "", "ogre.log");
	Ogre::GLPlugin* glPlugin = new Ogre::GLPlugin();
	root->installPlugin(glPlugin);
	root->setRenderSystem(glPlugin->getRenderSystem());

	root->initialise(false);

	OpenBlox::root = root;

	Ogre::NameValuePairList params;
	params["currentGLContext"] = Ogre::String("True");

	Ogre::RenderWindow* renderWindow = root->createRenderWindow("", 640, 480, false, &params);

	OpenBlox::renderWindow = renderWindow;

	Ogre::SceneManager* sceneMgr = root->createSceneManager(Ogre::ST_GENERIC);

	OpenBlox::sceneMgr = sceneMgr;

	Ogre::Camera* cam = sceneMgr->createCamera("Cam");
	cam->setPosition(Ogre::Vector3(0, 0, 80));
	cam->lookAt(Ogre::Vector3(0, 0, -300));
	cam->setNearClipDistance(5);

	OpenBlox::cam = cam;

	Ogre::Viewport* vp = renderWindow->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(1, 0, 0));
	cam->setAutoAspectRatio(true);

	OpenBlox::vp = vp;
	#endif

	OpenBlox::OBGame* gameInst = new OpenBlox::OBGame();

	#ifndef OB_NO_GRAPHICS
	QSettings settings;
	restoreState(&settings);

	setupIcon();
	#endif

	OpenBlox::static_init::execute(true); //Runs the "static" initializer, this time for any static functions that require game.

	SDL_Thread* taskthread = SDL_CreateThread(OpenBlox::taskThreadFunc, "TaskThread", NULL);
	if(taskthread == NULL){
		std::cout << "Failed to create task thread." << std::endl;
		SDL_Quit();
		return 1;
	}

	while(!OpenBlox::shouldQuit){
		#ifndef OB_NO_GRAPHICS
			SDL_Event event;
			while(SDL_PollEvent(&event)){
				gameInst->processEvent(event);
			}
		#endif

		app.processEvents();



		#ifndef OB_NO_GRAPHICS
			root->renderOneFrame();
			SDL_GL_SwapWindow(mw);
		#endif

		SDL_Delay(10); //Sleep for 10ms
	}

	#ifndef OB_NO_GRAPHICS
	saveState(&settings);

	SDL_GL_DeleteContext(OpenBlox::maincontext);
	SDL_DestroyWindow(OpenBlox::mw);
	SDL_Quit();
	#endif

	OpenBlox::shouldQuit = true;

	SDL_WaitThread(taskthread, NULL);

	delete gameInst;

	return 0;
}

#endif
