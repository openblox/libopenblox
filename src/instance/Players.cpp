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

#include "instance/Players.h"

#include "instance/Player.h"

#include "utility.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Players, false, isDataModel, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Players::Players(OBEngine* eng) : Instance(eng){
			Name = ClassName;
			netId = OB_NETID_PLAYERS;

			Archivable = false;
			PlayerAdded = make_shared<Type::Event>("PlayerAdded");
			PlayerRemoving = make_shared<Type::Event>("PlayerRemoving");
		}

		Players::~Players(){}

		std::string Players::fixedSerializedID(){
			return "Players";
		}

		shared_ptr<Instance> Players::cloneImpl(){
			return NULL;
		}

		void Players::removeChild(shared_ptr<Instance> kid){
			if(kid){
				if(shared_ptr<Player> kplr = dynamic_pointer_cast<Player>(kid)){
					std::vector<shared_ptr<Type::VarWrapper>> args = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(kplr)});
					PlayerRemoving->Fire(eng, args);
				}

				Instance::removeChild(kid);
			}
		}

		void Players::addChild(shared_ptr<Instance> kid){
			if(kid){
				Instance::addChild(kid);

				if(shared_ptr<Player> kplr = dynamic_pointer_cast<Player>(kid)){
					std::vector<shared_ptr<Type::VarWrapper>> args = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(kplr)});
					PlayerAdded->Fire(eng, args);
				}
			}
		}

		shared_ptr<Type::Event> Players::getPlayerAdded(){
			return PlayerAdded;
		}

		shared_ptr<Type::Event> Players::getPlayerRemoving(){
			return PlayerRemoving;
		}

		void Players::register_lua_events(lua_State* L){
			Instance::register_lua_events(L);

			luaL_Reg events[] = {
				{"PlayerAdded", WRAP_EVT(Players, PlayerAdded)},
				{"PlayerRemoving", WRAP_EVT(Players, PlayerRemoving)},
				{NULL, NULL}
			};
			luaL_setfuncs(L, events, 0);
		}
	}
}
