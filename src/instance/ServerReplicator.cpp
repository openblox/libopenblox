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

#include "instance/ServerReplicator.h"

#include "instance/Player.h"
#include "instance/Players.h"

#if HAVE_ENET
namespace OB{
	namespace Instance{
		DEFINE_CLASS(ServerReplicator, false, false, NetworkReplicator){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		ServerReplicator::ServerReplicator(OBEngine* eng) : NetworkReplicator(eng){
			Name = ClassName;
			netId = OB_NETID_NOT_REPLICATED;

			Archivable = false;
		}

		ServerReplicator::ServerReplicator(ENetPeer* peer, OBEngine* eng) : NetworkReplicator(peer, eng){
			Name = ClassName;
			netId = OB_NETID_NOT_REPLICATED;
		}

		ServerReplicator::~ServerReplicator(){
			if(plr){
				plr->Destroy();
			}
		}

		shared_ptr<Instance> ServerReplicator::cloneImpl(){
			return NULL;
		}

		shared_ptr<Player> ServerReplicator::CreatePlayer(){
			if(!plr){
				shared_ptr<DataModel> dm = getEngine()->getDataModel();
				if(dm){
					shared_ptr<Players> plrs = dm->getPlayers();
					plr = make_shared<Player>(getEngine());
					plr->setParent(plrs, true);
					plr->ParentLocked = true;
					return plr;
				}
			}else{
				return plr;
			}
			return NULL;
		}

		shared_ptr<Player> ServerReplicator::GetPlayer(){
			return plr;
		}

		int ServerReplicator::lua_CreatePlayer(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(shared_ptr<ServerReplicator> sr = dynamic_pointer_cast<ServerReplicator>(inst)){
				shared_ptr<Player> plr = sr->CreatePlayer();
				if(plr){
					return plr->wrap_lua(L);
				}else{
					lua_pushnil(L);
					return 1;
				}
			}

			return luaL_error(L, COLONERR, "CreatePlayer");
		}

		int ServerReplicator::lua_GetPlayer(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(shared_ptr<ServerReplicator> sr = dynamic_pointer_cast<ServerReplicator>(inst)){
				shared_ptr<Player> plr = sr->GetPlayer();
				if(plr){
					return plr->wrap_lua(L);
				}else{
					lua_pushnil(L);
					return 1;
				}
			}

			return luaL_error(L, COLONERR, "GetPlayer");
		}

		void ServerReplicator::register_lua_methods(lua_State* L){
			Instance::register_lua_methods(L);

			luaL_Reg methods[] = {
				{"CreatePlayer", lua_CreatePlayer},
				{"GetPlayer", lua_GetPlayer},
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}
	}
}
#endif
