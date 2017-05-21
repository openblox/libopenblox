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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox.	 If not, see <https://www.gnu.org/licenses/>.
 */

#include "instance/ClientReplicator.h"

#if HAVE_ENET
namespace OB{
	namespace Instance{
		DEFINE_CLASS(ClientReplicator, false, false, NetworkReplicator){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    ClientReplicator::ClientReplicator(OBEngine* eng) : NetworkReplicator(eng){
			Name = ClassName;
			netId = OB_NETID_NOT_REPLICATED;
		}

	    ClientReplicator::ClientReplicator(ENetPeer* peer, OBEngine* eng) : NetworkReplicator(peer, eng){
			Name = ClassName;
		    netId = OB_NETID_NOT_REPLICATED;
		}

	    ClientReplicator::~ClientReplicator(){}

		shared_ptr<Instance> ClientReplicator::cloneImpl(){
			return NULL;
		}
	}
}
#endif
