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

#include "OBEngine.h"

#include "OBException.h"

#include "AssetLocator.h"
#include "ClassFactory.h"
#include "TaskScheduler.h"

namespace OpenBlox{
	OBEngine* OBEngine::inst = NULL;

	OBEngine::OBEngine(){
		if(inst != NULL){
			throw new OBException("Only one instance of OBEngine can exist, at one time.");
		}
		inst = this;

		if(!QCoreApplication::instance()){
			throw new OBException("You must have an instance of QCoreApplication (Or QGuiApplication/QApplication) before creating OpenBlox::OBEngine.");
		}

		started = false;
		is_Server = false;

		root = NULL;
		renderWindow = NULL;

		sceneMgr = NULL;
		cam = NULL;
		vp = NULL;

		game = NULL;
		mainLuaState = NULL;

		startTime = QDateTime::currentMSecsSinceEpoch();

		new ClassFactory();
		new TaskScheduler();
		new AssetLocator();
	}

	OBEngine::~OBEngine(){
		delete game;
	}

	OBEngine* OBEngine::getInstance(){
		return inst;
	}

	#ifndef OB_NO_GRAPHICS

	void OBEngine::setOgreRoot(Ogre::Root* _root){
		if(started){
			throw new OBException("This OBEngine is already initialized.");
		}
		root = _root;
	}

	Ogre::Root* OBEngine::getOgreRoot(){
		return root;
	}

	void OBEngine::setRenderWindow(Ogre::RenderWindow* _renderWindow){
		if(started){
			throw new OBException("This OBEngine is already initialized.");
		}
		renderWindow = _renderWindow;
	}

	Ogre::RenderWindow* OBEngine::getRenderWindow(){
		return renderWindow;
	}

	Ogre::SceneManager* OBEngine::getSceneManager(){
		return sceneMgr;
	}

	Ogre::Camera* OBEngine::getCamera(){
		return cam;
	}

	Ogre::Viewport* OBEngine::getViewport(){
		return vp;
	}

	#endif

	void OBEngine::setServer(bool isServer){
		if(started){
			throw new OBException("This OBEngine is already initialized.");
		}
		is_Server = isServer;
	}

	bool OBEngine::isServer(){
		return is_Server;
	}

	bool OBEngine::isInitialized(){
		return started;
	}

	//The meat on this skeleton of a class

	void OBEngine::init(){
		if(started){
			throw new OBException("This OBEngine is already initialized.");
		}
		started = true;

		#ifndef OB_NO_GRAPHICS
		if(root && !renderWindow){
			throw new OBException("If you set an Ogre::Root*, you must also set an Ogre::RenderWindow* before initializing OBEngine.");
		}
		if(root){
			sceneMgr = root->createSceneManager(Ogre::ST_GENERIC);

			cam = sceneMgr->createCamera("MainCam");
			cam->setPosition(Ogre::Vector3(0, 0, 80));
			cam->lookAt(Ogre::Vector3(0, 0, -300));
			cam->setNearClipDistance(5);

			vp = renderWindow->addViewport(cam);
			cam->setAutoAspectRatio(true);
		}
		#endif

		static_init::execute(false);

		game = new OBGame(this);

		//Init Lua
		mainLuaState = ob_lua::init();

		static_init::execute(true);
	}

	void OBEngine::tick(){
		if(game){
			//TODO: Update DataModel, then physics, then handle resized GUIs (if applicable)
		}
	}

	void OBEngine::render(){
		#ifndef OB_NO_GRAPHICS
		if(root){
			root->renderOneFrame();
		}
		#endif
	}

	void OBEngine::resized(unsigned int width, unsigned int height){

	}

	void(*OBEngine::getShutdownHook())(){
		return shutdown_hook;
	}

	void OBEngine::setShutdownHook(void(*shutdown_hook)()){
		this->shutdown_hook = shutdown_hook;
	}

	//Input Injection Methods
	void OBEngine::mousePress(uint8_t btn, QPoint pos){

	}

	void OBEngine::mouseRelease(uint8_t btn, QPoint pos){

	}

	void OBEngine::mouseMove(QPoint pos){

	}

	void OBEngine::mouseWheel(int delta, QPoint pos){

	}

	void OBEngine::keyDown(int32_t key, uint16_t mods){

	}

	void OBEngine::keyUp(int32_t key, uint16_t mods){

	}

	lua_State* OBEngine::getLuaState(){
		return mainLuaState;
	}

	qint64 OBEngine::getStartTime(){
		return startTime;
	}

	void OBEngine::info(QString output){
		std::cout << "[INFO] " << output.toStdString() << std::endl;
	}

	void OBEngine::print(QString output){
		std::cout << output.toStdString() << std::endl;
	}

	void OBEngine::warn(QString output){
		std::cout << "[WARN] " << output.toStdString() << std::endl;
	}

	void OBEngine::print_error(QString output){
		std::cerr << output.toStdString() << std::endl;
	}
}
