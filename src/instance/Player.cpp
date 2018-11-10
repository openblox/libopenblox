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

#include "instance/Player.h"

#include "utility.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Player, false, false, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Player::Player(OBEngine* eng) : Instance(eng){
			Name = ClassName;
			netId = OB_NETID_PLAYERS;

			Archivable = false;

			#if HAVE_ENET
			serverReplicator = NULL;
			#endif
		}

		Player::~Player(){}

		shared_ptr<Instance> Player::cloneImpl(){
			return NULL;
		}

#if HAVE_ENET
		void Player::setServerReplicator(shared_ptr<ServerReplicator> sR){
			serverReplicator = sR;
		}

		shared_ptr<ServerReplicator> Player::getServerReplicator(){
			return serverReplicator;
		}
#endif
	}
}
