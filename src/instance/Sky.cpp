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

#include "instance/Sky.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS_WCLONE(Sky, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Sky::Sky(OBEngine* eng) : Instance(eng){
			Name = ClassName;

			Archivable = false;
		}

	    Sky::~Sky(){}

#if HAVE_ENET
		void Sky::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
		}
#endif

		std::map<std::string, _PropertyInfo> Sky::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			return propMap;
		}

		void Sky::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> Sky::getProperty(std::string prop){
			return Instance::getProperty(prop);
		}

		void Sky::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);

			luaL_Reg properties[] = {
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Sky::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);

			luaL_Reg properties[] = {
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
