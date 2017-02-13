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

#include "instance/ServiceProvider.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS_WCLONE(ServiceProvider, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    ServiceProvider::ServiceProvider(){
			Name = ClassName;
		}

		ServiceProvider::~ServiceProvider(){}

		shared_ptr<Instance> ServiceProvider::FindService(std::string className){
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != children.size(); i++){
				shared_ptr<Instance> kid = children[i];
				if(kid != NULL){
					if(kid->getClassName() == className){
						return kid;
					}
				}
			}
			return NULL;
		}

		shared_ptr<Instance> ServiceProvider::GetService(std::string className){
			shared_ptr<Instance> foundService = FindService(className);
			if(foundService != NULL){
				return foundService;
			}
		    shared_ptr<Instance> newGuy = ClassFactory::createService(className, false);
			if(newGuy){
				newGuy->setParent(std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this(), false);
				newGuy->ParentLocked = true;
			}
			return newGuy;
		}

		int ServiceProvider::lua_FindService(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(shared_ptr<ServiceProvider> sp = dynamic_pointer_cast<ServiceProvider>(inst)){
				std::string serviceName = std::string(luaL_checkstring(L, 2));
				shared_ptr<Instance> foundGuy = sp->FindService(serviceName);
				if(foundGuy != NULL){
					return foundGuy->wrap_lua(L);
				}
				lua_pushnil(L);
				return 1;
			}
			return luaL_error(L, COLONERR, "FindService");
		}

		int ServiceProvider::lua_GetService(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(shared_ptr<ServiceProvider> sp = dynamic_pointer_cast<ServiceProvider>(inst)){
				std::string serviceName = std::string(luaL_checkstring(L, 2));
				shared_ptr<Instance> foundGuy = sp->GetService(serviceName);
				if(foundGuy != NULL){
					return foundGuy->wrap_lua(L);
				}
				lua_pushnil(L);
				return 1;
			}
			return luaL_error(L, COLONERR, "GetService");
		}

		void ServiceProvider::register_lua_methods(lua_State* L){
			Instance::register_lua_methods(L);

			luaL_Reg methods[] = {
				{"FindService", lua_FindService},
				{"GetService", lua_GetService},
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}
	}
}
