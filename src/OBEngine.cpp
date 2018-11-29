/*
 * Copyright (C) 2016-2018 John M. Harris, Jr. <johnmh@openblox.org>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "OBEngine.h"

#include "config.h"

#include "OBException.h"
#include "utility.h"

#include "TaskScheduler.h"
#include "ClassFactory.h"

#include "OBRenderUtils.h"

#include "lua/OBLua.h"

#include <string>

#include "instance/Lighting.h"

#include "type/Type.h"
#include "type/Color3.h"

#ifndef _MSC_VER
#include <unistd.h>
#else
#include <windows.h>
#endif

#if HAVE_ENET
#include <enet/enet.h>
#endif

#if HAVE_SDL2
#include <SDL_syswm.h>
#endif

namespace OB{
	OBEngine::OBEngine(){
#if HAVE_PUGIXML
		serializer = make_shared<OBSerializer>(this);
#endif

		logger = make_shared<OBLogger>(this);

		ClassFactory::registerCoreClasses();

		initialized = false;
		startTime = currentTimeMillis();
		_isRunning = false;
		exitCode = 0;

		doRendering = true;
		startWidth = 640;
		startHeight = 480;
		vsync = false;
		resizable = false;

		globalState = NULL;

		windowId = NULL;

		custPostRender = NULL;

#if HAVE_IRRLICHT
		irrDev = NULL;
		irrDriv = NULL;
		irrSceneMgr = NULL;
#endif

		eventReceiver = new OBInputEventReceiver(this);

		renderUtils = NULL;

#if HAVE_ENET
		enet_initialize();
#endif
	}

	OBEngine::~OBEngine(){
		// Assumptions like this are bad, oh well.
#if HAVE_ENET
		enet_deinitialize();
#endif

#if HAVE_SDL2
		if(sdl_window){
			SDL_DestroyWindow(sdl_window);
		}
#endif
	}

	shared_ptr<TaskScheduler> OBEngine::getTaskScheduler(){
		return taskSched;
	}

	shared_ptr<TaskScheduler> OBEngine::getSecondaryTaskScheduler(){
		return secondaryTaskSched;
	}

	shared_ptr<OBSerializer> OBEngine::getSerializer(){
		return serializer;
	}

	void* _ob_eng_secondaryTaskThread(void* vobEng){
		OBEngine* eng = (OBEngine*)vobEng;
		shared_ptr<TaskScheduler> taskS = eng->getSecondaryTaskScheduler();

		while(eng->isRunning()){
			taskS->tick();

			usleep(10000);
		}

		pthread_exit(NULL);
		return NULL;
	}

	void OBEngine::init(){
		if(initialized){
			throw new OBException("OBEngine has already been initialized.");
		}

		_isRunning = true;

		std::string verString = std::string(PACKAGE_STRING) + " initializing";

		logger->log(verString);

#if HAVE_SDL2
		if(!windowId){
			SDL_SetMainReady();
			SDL_Init(SDL_INIT_VIDEO);

		    Uint32 sdlWinFlags = SDL_WINDOW_OPENGL;

			if(resizable){
				sdlWinFlags = SDL_WINDOW_RESIZABLE;
			}

			sdl_window = SDL_CreateWindow("OpenBlox", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, startWidth, startHeight, sdlWinFlags);
			if(sdl_window == NULL){
				throw new OBException("Failed to create window");
			}

			SDL_SysWMinfo wwmInfo;
			SDL_VERSION(&wwmInfo.version);

			if(SDL_GetWindowWMInfo(sdl_window, &wwmInfo)){
				switch(wwmInfo.subsystem){
#if defined(SDL_VIDEO_DRIVER_X11)
					case SDL_SYSWM_X11: {
						windowId = (void*)wwmInfo.info.x11.window;
						break;
					}
#endif
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
					case SDL_SYSWM_WINDOWS: {
						windowId = (void*)wwmInfo.info.win.window;
						break;
					}
#endif
#if defined(SDL_VIDEO_DRIVER_WINRT)
					case SDL_SYSWM_WINRT: {
						windowId = (void*)wwmInfo.info.winrt.window;
						break;
					}
#endif
#if defined(SDL_VIDEO_DRIVER_DIRECTFB)
					case SDL_SYSWM_DIRECTFB: {
						windowId = (void*)wwmInfo.info.dfb.window;
						break;
					}
#endif
#if defined(SDL_VIDEO_DRIVER_COCOA)
					case SDL_SYSWM_COCOA: {
						windowId = (void*)wwmInfo.info.cocoa.window;
						break;
					}
#endif
#if defined(SDL_VIDEO_DRIVER_UIKIT)
					case SDL_SYSWM_UIKIT: {
						windowId = (void*)wwmInfo.info.uikit.window;
						break;
					}
#endif
#if defined(SDL_VIDEO_DRIVER_WAYLAND)
					case SDL_SYSWM_WAYLAND: {
						windowId = (void*)wwmInfo.info.wl.surface;
						break;
					}
#endif
#if defined(SDL_VIDEO_DRIVER_MIR)
					case SDL_SYSWM_MIR: {
						windowId = (void*)wwmInfo.info.mir.surface;
						break;
					}
#endif
#if defined(SDL_VIDEO_DRIVER_ANDROID)
					case SDL_SYSWM_ANDROID: {
						windowId = (void*)wwmInfo.info.android.window;
						break;
					}
#endif
#if defined(SDL_VIDEO_DRIVER_VIVANTE)
					case SDL_SYSWM_VIVANTE: {
						windowId = (void*)wwmInfo.vivante.window;
						break;
					}
#endif
					default: {
						throw new OBException("Unknown window subsystem");
					}
				}
			}else{
				throw new OBException("Failed to get window handle information");
			}
		}
#endif

#if HAVE_IRRLICHT
		if(doRendering){
			irr::SIrrlichtCreationParameters p;
			p.DriverType = irr::video::EDT_OPENGL;

			p.WindowSize = irr::core::dimension2d<irr::u32>(startWidth, startHeight);
			p.Vsync = vsync;
			p.WindowId = windowId;

			p.LoggingLevel = irr::ELL_WARNING;

			irrDev = irr::createDeviceEx(p);

			if(!irrDev){
				throw new OBException("Failed to create Irrlicht Device");
			}

			if(!windowId){
				irrDev->setWindowCaption(L"OpenBlox");
			}

			irrDev->setResizable(resizable);

#if HAVE_SDL2
			if(!windowId && !sdl_window){
				irrDev->setEventReceiver(eventReceiver);
			}
#else
			irrDev->setEventReceiver(eventReceiver);
#endif

			irrDriv = irrDev->getVideoDriver();
			irrSceneMgr = irrDev->getSceneManager();

			renderUtils = make_shared<OBRenderUtils>(this);

			//Log at INFO level
			std::string renderTag("[RENDERER] ");

			std::string irrVer = renderTag + std::string("Irrlicht: ") + std::string(irrDev->getVersion());

			logger->log(irrVer);

			std::wstring wsName(irrDriv->getName());
			std::string renderVersion = renderTag + std::string("Version: ") + std::string(wsName.begin(), wsName.end());

			logger->log(renderVersion);

			std::string renderVendor = renderTag + std::string("Vendor: ") + std::string(irrDriv->getVendorInfo().c_str());

			logger->log(renderVendor);

			unsigned int shaderLangVersion = irrDriv->getDriverAttributes().getAttributeAsInt("ShaderLanguageVersion");

			char buf[32];
			unsigned int shaderLangVerMaj = shaderLangVersion/100;
			snprintf(buf, 32, "%u.%u", shaderLangVerMaj, shaderLangVersion - shaderLangVerMaj * 100);

			std::string renderShadingLangVer = renderTag + std::string("Shading Language Version: ") + std::string(buf);

			logger->log(renderShadingLangVer);

			irrSceneMgr->addLightSceneNode();
		}
#endif

		taskSched = make_shared<TaskScheduler>(this);

		secondaryTaskSched = make_shared<TaskScheduler>(this);
		secondaryTaskSched->SetSortsTasks(false);

		assetLocator = make_shared<AssetLocator>(this);

		pluginManager = make_shared<PluginManager>(this);

		globalState = OB::Lua::initGlobal(this);

		dm = make_shared<Instance::DataModel>(this);
		dm->initServices();

		// Initialize Lua types
		Type::Type::_ob_init(this);

		ClassFactory::initClasses(this);

		pthread_create(&secondaryTaskThread, NULL, _ob_eng_secondaryTaskThread, this);

		initialized = true;
	}

	void OBEngine::setExitCode(int exitCode){
		this->exitCode = exitCode;
	}

	int OBEngine::getExitCode(){
		return exitCode;
	}

	void OBEngine::shutdown(){
		_isRunning = false;
	}

	bool OBEngine::isRunning(){
		return _isRunning;
	}

	void OBEngine::tick(){
#if HAVE_IRRLICHT
		if(doRendering){
			if(!irrDev->run()){
				_isRunning = false;

				void* _stat;

				pthread_join(secondaryTaskThread, &_stat);

				return;// Early return, we're not running anymore!
			}
		}

#if HAVE_SDL2
		if(sdl_window){
			SDL_Event evt;
			while(SDL_PollEvent(&evt)){
			    switch(evt.type){
					case SDL_QUIT: {
						_isRunning = false;

						void* _stat;

						pthread_join(secondaryTaskThread, &_stat);

						return;// Early return, we're not running anymore!
					}
					case SDL_WINDOWEVENT: {
					    // Sanity check
						if(SDL_GetWindowID(sdl_window) == evt.window.windowID){
							switch(evt.window.event){
								case SDL_WINDOWEVENT_RESIZED: {
									resized(evt.window.data1, evt.window.data2);
									break;
								}
								case SDL_WINDOWEVENT_FOCUS_GAINED: {
									eventReceiver->focus();
								}
								case SDL_WINDOWEVENT_FOCUS_LOST: {
									eventReceiver->unfocus();
								}
							}
						}
						break;
					}
					default: {
						eventReceiver->processSDL2Event(evt);
					}
				}
			}
		}
#endif
#endif

		taskSched->tick();
		dm->tick();

		if(!doRendering){
			// If we aren't rendering, there's no wait and we end up in a busy loop doing nothing.
			usleep(10000);
		}
	}

	void OBEngine::render(){
#if HAVE_IRRLICHT
		if(doRendering){
			if(!dm){
				// Prevents segfault when render() is called before init()
				return;
			}
			shared_ptr<Instance::Lighting> light = dm->getLighting();

			shared_ptr<Type::Color3> skyCol = light->getSkyColor();
			irr::video::SColor irrSkyCol;
			if(light->isSkyTransparent()){
				irrSkyCol = irr::video::SColor(0, 0, 0, 0);
			}else{
				if(skyCol != NULL){
					irrSkyCol = skyCol->toIrrlichtSColor();
				}else{
					irrSkyCol = irr::video::SColor(255, 0, 0, 0);
				}
			}

			irrDriv->beginScene(true, true, irrSkyCol);

			dm->preRender();

			irrSceneMgr->drawAll();

			dm->render();

			if(custPostRender != NULL){
				custPostRender(irrDriv);
			}

			irrDriv->endScene();
		}
#endif
	}

	lua_State* OBEngine::getGlobalLuaState(){
		return globalState;
	}

	ob_uint64 OBEngine::getStartTime(){
		return startTime;
	}

	bool OBEngine::doesRendering(){
		return doRendering;
	}

	void OBEngine::setRendering(bool renders){
		if(initialized){
			throw new OBException("You can't call setRendering after init is called.");
		}
		doRendering = renders;
	}

	int OBEngine::getInitWidth(){
		return startWidth;
	}

	void OBEngine::setInitWidth(int w){
		if(initialized){
			throw new OBException("You can't call setInitWidth after init is called.");
		}
		startWidth = w;
	}

	int OBEngine::getInitHeight(){
		return startHeight;
	}

	void OBEngine::setInitHeight(int h){
		if(initialized){
			throw new OBException("You can't call setInitHeight after init is called.");
		}
		startHeight = h;
	}

	bool OBEngine::getUseVsync(){
		return vsync;
	}

	void OBEngine::setUseVsync(bool useVsync){
		if(initialized){
			throw new OBException("You can't call setUsesVsync after init is called.");
		}
		vsync = useVsync;
	}

	bool OBEngine::getResizable(){
		return resizable;
	}

	void OBEngine::setResizable(bool Resizable){
		if(initialized){
			#if HAVE_SDL2
			if(sdl_window){
				resizable = Resizable;

				SDL_SetWindowResizable(sdl_window, resizable ? SDL_TRUE : SDL_FALSE);
				return;
			}
			#endif
			throw new OBException("You can't call setResizable after init is called.");
		}
		resizable = Resizable;
	}

	void* OBEngine::getWindowId(){
		return windowId;
	}

	void OBEngine::setWindowId(void* wId){
		if(initialized){
			throw new OBException("You can't call setWindowId after init is called.");
		}
		windowId = wId;
	}

	void OBEngine::resized(int width, int height){
#if HAVE_IRRLICHT
		if(irrDriv){
			irrDriv->OnResize(irr::core::dimension2d<irr::u32>(width, height));
		}
#endif
	}

#if HAVE_IRRLICHT
	irr::IrrlichtDevice* OBEngine::getIrrlichtDevice(){
		return irrDev;
	}

	post_render_func_t OBEngine::getPostRenderFunc(){
		return custPostRender;
	}

	void OBEngine::setPostRenderFunc(post_render_func_t prf){
		custPostRender = prf;
	}

	shared_ptr<OBRenderUtils> OBEngine::getRenderUtils(){
		return renderUtils;
	}
#endif

	shared_ptr<Instance::DataModel> OBEngine::getDataModel(){
		return dm;
	}

	shared_ptr<AssetLocator> OBEngine::getAssetLocator(){
		return assetLocator;
	}

	shared_ptr<PluginManager> OBEngine::getPluginManager(){
		return pluginManager;
	}

	shared_ptr<OBLogger> OBEngine::getLogger(){
		return logger;
	}

	OBInputEventReceiver* OBEngine::getInputEventReceiver(){
		return eventReceiver;
	}
}
