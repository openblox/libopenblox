/*
 * Copyright (C) 2016 John M. Harris, Jr. <johnmh@openblox.org>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox.	 If not, see <https://www.gnu.org/licenses/>.
 */

#include "OBEngine.h"

#include "config.h"

#include "OBException.h"
#include "utility.h"

#include "TaskScheduler.h"
#include "ClassFactory.h"

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

namespace OB{
	OBEngine* OBEngine::inst = NULL;

	OBEngine::OBEngine(){
		if(inst != NULL){
			throw new OBException("Only one instance of OBEngine can exist.");
		}
		inst = this;

	    #if HAVE_PUGIXML
		serializer = make_shared<OBSerializer>(this);
		#endif

		ClassFactory::registerCoreClasses();

		initialized = false;
		startTime = currentTimeMillis();
		_isRunning = false;
		exitCode = 0;

		doRendering = true;
		startWidth = 640;
		startHeight = 480;
		vsync = false;

		globalState = NULL;

		windowId = NULL;

		#if HAVE_IRRLICHT
		
		irrDev = NULL;
		irrDriv = NULL;
		irrSceneMgr = NULL;
		
		#endif

		#if HAVE_ENET
		enet_initialize();
		#endif
	}

	OBEngine::~OBEngine(){
		//Assumptions like this are bad, oh well.
		#if HAVE_ENET
		enet_deinitialize();
		#endif
	}

	OBEngine* OBEngine::getInstance(){
		return inst;
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
		
		OBLogger::log(verString);

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

			irrDev->setWindowCaption(L"OpenBlox");

			irrDriv = irrDev->getVideoDriver();
			irrSceneMgr = irrDev->getSceneManager();

			//Log at INFO level
			std::string renderTag("[RENDERER] ");

			std::string irrVer = renderTag + std::string("Irrlicht: ") + std::string(irrDev->getVersion());

			OBLogger::log(irrVer);
			
			std::wstring wsName(irrDriv->getName());
			std::string renderVersion = renderTag + std::string("Version: ") + std::string(wsName.begin(), wsName.end());

			OBLogger::log(renderVersion);

			std::string renderVendor = renderTag + std::string("Vendor: ") + std::string(irrDriv->getVendorInfo().c_str());
			
			OBLogger::log(renderVendor);

			unsigned int shaderLangVersion = irrDriv->getDriverAttributes().getAttributeAsInt("ShaderLanguageVersion");

			char buf[32];
			unsigned int shaderLangVerMaj = shaderLangVersion/100;
			snprintf(buf, 32, "%u.%u", shaderLangVerMaj, shaderLangVersion - shaderLangVerMaj * 100);

			std::string renderShadingLangVer = renderTag + std::string("Shading Language Version: ") + std::string(buf);
			
			OBLogger::log(renderShadingLangVer);

			irrSceneMgr->addLightSceneNode();
			irrSceneMgr->addCameraSceneNode(0, irr::core::vector3df(0,30,-40), irr::core::vector3df(0,5,0));
		}
		#endif
		
		taskSched = make_shared<TaskScheduler>();
		
		secondaryTaskSched = make_shared<TaskScheduler>();
		secondaryTaskSched->SetSortsTasks(false);
		
		assetLocator = make_shared<AssetLocator>();
		
		globalState = OB::Lua::initGlobal();
		
		dm = make_shared<Instance::DataModel>();
		dm->initServices();

		//Initialize Lua types
		Type::Type::_ob_init();
		
		ClassFactory::initClasses();

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
				
				return;//Early return, we're not running anymore!
			}
		}
		
		#endif
		
	    taskSched->tick();
		dm->tick();

		if(!doRendering){
			//If we aren't rendering, there's no wait and we end up in a busy loop doing nothing.
			usleep(10000);
		}
	}

	void OBEngine::render(){
		#if HAVE_IRRLICHT
		
		if(doRendering){
			if(!dm){
				//Prevents segfault when render() is called before init()
				return;
			}
			shared_ptr<Instance::Lighting> light = dm->getLighting();
			
			shared_ptr<Type::Color3> skyCol = light->getSkyColor();
			irr::video::SColor irrSkyCol;
			if(skyCol != NULL){
				irrSkyCol = skyCol->toIrrlichtSColor();
			}else{
				irrSkyCol = irr::video::SColor(255, 0, 0, 0);
			}
			
			irrDriv->beginScene(true, true, irrSkyCol);
			irrSceneMgr->drawAll();
			irrDriv->endScene();
		}
		
		#endif
	}

	lua_State* OBEngine::getGlobalLuaState(){
		return globalState;
	}

	ob_int64 OBEngine::getStartTime(){
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

	#endif

	shared_ptr<Instance::DataModel> OBEngine::getDataModel(){
		return dm;
	}

	shared_ptr<AssetLocator> OBEngine::getAssetLocator(){
		return assetLocator;
	}
}
