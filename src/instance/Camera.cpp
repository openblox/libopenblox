/*
 * Copyright (C) 2017 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "instance/Camera.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Camera, true, false, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Camera::Camera(OBEngine* eng) : Instance(eng){
			Name = ClassName;

			Archivable = false;
		}

		Camera::~Camera(){}

		bool Camera::SaveScreenshot(std::string file){
			return getEngine()->saveScreenshot(file);
		}

		shared_ptr<Instance> Camera::cloneImpl(){
			return NULL;
		}

#if HAVE_ENET
		void Camera::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
		}
#endif

		std::map<std::string, _PropertyInfo> Camera::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			return propMap;
		}

		void Camera::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> Camera::getProperty(std::string prop){
			return Instance::getProperty(prop);
		}

		int Camera::lua_SaveScreenshot(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Camera> instC = dynamic_pointer_cast<Camera>(inst);
				if(instC){
					std::string desired = std::string(luaL_checkstring(L, 2));
					lua_pushboolean(L, instC->SaveScreenshot(desired));
					return 1;
				}
			}

			return 0;
		}

		void Camera::register_lua_methods(lua_State* L){
			Instance::register_lua_methods(L);

			luaL_Reg methods[] = {
				{"SaveScreenshot", lua_SaveScreenshot},
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}

		void Camera::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);

			luaL_Reg properties[] = {
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Camera::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);

			luaL_Reg properties[] = {
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
