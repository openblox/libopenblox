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

#include "OBException.h"
#include "utility.h"

#include "TaskScheduler.h"
#include "ClassFactory.h"

#include "lua/OBLua.h"

#include <string>

#include <irrlicht/irrlicht.h>

#include "type/Type.h"

namespace OB{
	OBEngine* OBEngine::inst = NULL;

	OBEngine::OBEngine(){
		if(inst != NULL){
			throw new OBException("Only one instance of OBEngine can exist.");
		}
		inst = this;

		initialized = false;
		startTime = currentTimeMillis();
		_isRunning = true;
		exitCode = 0;

		doRendering = true;
		startWidth = 640;
		startHeight = 480;
		vsync = false;

		globalState = NULL;

		windowId = NULL;
		irrDev = NULL;
		irrDriv = NULL;
		irrSceneMgr = NULL;
	}

	OBEngine::~OBEngine(){}

	OBEngine* OBEngine::getInstance(){
		return inst;
	}

	shared_ptr<TaskScheduler> OBEngine::getTaskScheduler(){
		return taskSched;
	}

	void OBEngine::init(){
		if(initialized){
			throw new OBException("OBEngine has already been initialized.");
		}
		
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
		}
		
		taskSched = make_shared<TaskScheduler>();
		globalState = OB::Lua::initGlobal();
		dm = make_shared<Instance::DataModel>();
		dm->initServices();

		//Initialize Lua types
		Type::Type::_ob_init();
		
		ClassFactory::initClasses();

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
		if(doRendering){
			if(!irrDev->run()){
				_isRunning = false;
				return;//Early return, we're not running anymore!
			}
		}
		
	    taskSched->tick();
	}

	void OBEngine::render(){
		if(doRendering){
			irrDriv->beginScene(true, true, irr::video::SColor(255, 0, 255, 0));
			irrSceneMgr->drawAll();
			irrDriv->endScene();
		}
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

	irr::IrrlichtDevice* OBEngine::getIrrlichtDevice(){
		return irrDev;
	}

	shared_ptr<Instance::DataModel> OBEngine::getDataModel(){
		return dm;
	}
}
