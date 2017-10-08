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

#include "instance/UserInputService.h"

#include "utility.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(UserInputService, false, isDataModel, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		UserInputService::UserInputService(OBEngine* eng) : Instance(eng){
			Name = ClassName;
			netId = OB_NETID_NOT_REPLICATED;

			Archivable = false;

			GamepadConnected = make_shared<Type::Event>("GamepadConnected");
			GamepadDisconnected = make_shared<Type::Event>("GamepadDisconnected");
		}

		UserInputService::~UserInputService(){}

#if HAVE_PUGIXML
		std::string UserInputService::serializedID(){
			shared_ptr<OBSerializer> serializer = eng->getSerializer();
			serializer->SetID(shared_from_this(), getClassName());

			return Instance::serializedID();
		}
#endif

		shared_ptr<Instance> UserInputService::cloneImpl(){
			return NULL;
		}

		void UserInputService::register_lua_events(lua_State* L){
			Instance::register_lua_events(L);

			luaL_Reg events[] = {
				{"GamepadConnected", WRAP_EVT(UserInputService, GamepadConnected)},
				{"GamepadDisconnected", WRAP_EVT(UserInputService, GamepadDisconnected)},
				{NULL, NULL}
			};
			luaL_setfuncs(L, events, 0);
		}
	}
}
