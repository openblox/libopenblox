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

#include "instance/PVInstance.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS_WCLONE(PVInstance, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		PVInstance::PVInstance(){
			Name = ClassName;
			irrNode = NULL;
		}

		PVInstance::~PVInstance(){
			if(irrNode){
				irr::scene::ISceneNode* irrPar = irrNode->getParent();
				if(irrPar){
					irrPar->removeChild(irrNode);
				}
				
				delete irrNode;
				irrNode = NULL;
			}
		}

		void PVInstance::removeChild(Instance* kid){
			if(kid){
				if(PVInstance* oInst = dynamic_cast<PVInstance*>(kid)){
					if(irrNode && oInst->irrNode){
						irrNode->removeChild(oInst->irrNode);
					}
				}
				Instance::removeChild(kid);
			}
		}

		void PVInstance::addChild(Instance* kid){
			if(kid){
				if(PVInstance* oInst = dynamic_cast<PVInstance*>(kid)){
					if(irrNode && oInst->irrNode){
				    	irrNode->addChild(oInst->irrNode);
					}
				}
				Instance::addChild(kid);
			}
		}
	}
}
