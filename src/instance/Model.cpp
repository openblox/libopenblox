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

#include "instance/Model.h"

#include "OBEngine.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Model, true, false, PVInstance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Model::Model(){
			Name = ClassName;

		    OBEngine* eng = OBEngine::getInstance();
			if(eng){
				irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
				if(irrDev){
					irr::scene::ISceneManager* sceneMgr = irrDev->getSceneManager();
					if(sceneMgr){
						irrNode = sceneMgr->addEmptySceneNode();
					}
				}
			}
		}

	    Model::~Model(){}
	}
}
