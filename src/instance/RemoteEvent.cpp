/*
 * Copyright (C) 2018 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "instance/RemoteEvent.h"

#if HAVE_ENET

#include "instance/NetworkServer.h"
#include "instance/NetworkClient.h"
#include "instance/ServerReplicator.h"
#include "instance/Player.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(RemoteEvent, true, false, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    RemoteEvent::RemoteEvent(OBEngine* eng) : Instance(eng){
			Name = ClassName;

			ServerEvent = make_shared<Type::Event>("ServerEvent");
			ClientEvent = make_shared<Type::Event>("ClientEvent");
		}

	    RemoteEvent::~RemoteEvent(){}

		shared_ptr<Instance> RemoteEvent::cloneImpl(){
			shared_ptr<RemoteEvent> re = make_shared<RemoteEvent>(eng);
			re->Archivable = Archivable;
			re->Name = Name;
			re->ParentLocked = ParentLocked;

			return re;
		}

		shared_ptr<Type::Event> RemoteEvent::getServerEvent(){
			return ServerEvent;
		}

		shared_ptr<Type::Event> RemoteEvent::getClientEvent(){
			return ClientEvent;
		}

		BitStream _ob_instance_remoteevent_buildpacket(RemoteEvent* re, std::vector<shared_ptr<Type::VarWrapper>> argList){
			BitStream bsOut;
			bsOut.writeSizeT(OB_NET_PKT_FIRE_REMOTE_EVENT);
			bsOut.writeUInt64(re->GetNetworkID());

			size_t numArgs = argList.size();

			bsOut.writeSizeT(numArgs);

			for(size_t i = 0; i < numArgs; i++){
				bsOut.writeVar(argList[i]);
			}

			return bsOut;
		}

		void RemoteEvent::FireServer(std::vector<shared_ptr<Type::VarWrapper>> argList){
			shared_ptr<Instance> ncInst = eng->getDataModel()->FindService("NetworkClient");
		    if(shared_ptr<NetworkClient> nc = dynamic_pointer_cast<NetworkClient>(ncInst)){
				BitStream bsOut = _ob_instance_remoteevent_buildpacket(this, argList);

				nc->send(OB_NET_CHAN_PROTOCOL, bsOut);
			}
		}

		void RemoteEvent::FireClient(shared_ptr<Instance> plr_conn, std::vector<shared_ptr<Type::VarWrapper>> argList){
			shared_ptr<ServerReplicator> sr;

			if(shared_ptr<ServerReplicator> srT = dynamic_pointer_cast<ServerReplicator>(plr_conn)){
				sr = srT;
			}else if(shared_ptr<Player> plr = dynamic_pointer_cast<Player>(plr_conn)){
			    if(shared_ptr<ServerReplicator> srT = plr->getServerReplicator()){
					sr = srT;
				}else{
					throw new OBException("A Player was specified as the first argument, but was not a network player.");
				}
			}else{
				throw new OBException("First argument must be either a Player or ServerReplicator.");
			}

			shared_ptr<Instance> nsInst = eng->getDataModel()->FindService("NetworkServer");
		    if(shared_ptr<NetworkServer> ns = dynamic_pointer_cast<NetworkServer>(nsInst)){
				BitStream bsOut = _ob_instance_remoteevent_buildpacket(this, argList);

				sr->Send(OB_NET_CHAN_PROTOCOL, bsOut);
			}
		}

		void RemoteEvent::FireAllClients(std::vector<shared_ptr<Type::VarWrapper>> argList){
			shared_ptr<Instance> nsInst = eng->getDataModel()->FindService("NetworkServer");
		    if(shared_ptr<NetworkServer> ns = dynamic_pointer_cast<NetworkServer>(nsInst)){
				BitStream bsOut = _ob_instance_remoteevent_buildpacket(this, argList);

				ns->broadcast(OB_NET_CHAN_PROTOCOL, bsOut);
			}
		}

		int RemoteEvent::lua_FireClient(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(shared_ptr<RemoteEvent> re = dynamic_pointer_cast<RemoteEvent>(inst)){
			    shared_ptr<Instance> cliInst = checkInstance(L, 2, false);

				std::vector<shared_ptr<Type::VarWrapper>> fireArgs;

				int nargs = lua_gettop(L);
				if(nargs > 2){
					for(int i = 3; i <= nargs; i++){
						int ltype = lua_type(L, i);

						switch(ltype){
							case LUA_TNIL: {
								fireArgs.push_back(make_shared<Type::VarWrapper>((void*)NULL, Type::TYPE_NULL));
								break;
							}
							case LUA_TNUMBER: {
								fireArgs.push_back(make_shared<Type::VarWrapper>(lua_tonumber(L, i)));
								break;
							}
							case LUA_TBOOLEAN: {
								fireArgs.push_back(make_shared<Type::VarWrapper>(lua_toboolean(L, i)));
								break;
							}
							case LUA_TSTRING: {
								const char* tmpStr = lua_tostring(L, i);
								fireArgs.push_back(make_shared<Type::VarWrapper>(std::string(tmpStr)));
								break;
							}
							case LUA_TUSERDATA: {
								shared_ptr<Type::Type> tl = Type::Type::checkType(L, i, false);
								if(tl){
									fireArgs.push_back(make_shared<Type::VarWrapper>(tl));
									break;
								}

								shared_ptr<Instance> ti = checkInstance(L, i, false);
								if(ti){
									fireArgs.push_back(make_shared<Type::VarWrapper>(ti));
									break;
								}
							}
							case LUA_TTABLE:
							case LUA_TFUNCTION:
							case LUA_TTHREAD:
							case LUA_TLIGHTUSERDATA: {
								lua_pushvalue(L, i);
								int ref = luaL_ref(L, LUA_REGISTRYINDEX);
								fireArgs.push_back(make_shared<Type::VarWrapper>(L, ref));
								break;
							}
						}
					}

					re->FireClient(cliInst, fireArgs);
					return 0;
				}
			}

			return luaL_error(L, COLONERR, "FireClient");
		}

	    int RemoteEvent::lua_FireAllClients(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(shared_ptr<RemoteEvent> re = dynamic_pointer_cast<RemoteEvent>(inst)){
				std::vector<shared_ptr<Type::VarWrapper>> fireArgs;

				int nargs = lua_gettop(L);
				if(nargs > 1){
					for(int i = 2; i <= nargs; i++){
						int ltype = lua_type(L, i);

						switch(ltype){
							case LUA_TNIL: {
								fireArgs.push_back(make_shared<Type::VarWrapper>((void*)NULL, Type::TYPE_NULL));
								break;
							}
							case LUA_TNUMBER: {
								fireArgs.push_back(make_shared<Type::VarWrapper>(lua_tonumber(L, i)));
								break;
							}
							case LUA_TBOOLEAN: {
								fireArgs.push_back(make_shared<Type::VarWrapper>(lua_toboolean(L, i)));
								break;
							}
							case LUA_TSTRING: {
								const char* tmpStr = lua_tostring(L, i);
								fireArgs.push_back(make_shared<Type::VarWrapper>(std::string(tmpStr)));
								break;
							}
							case LUA_TUSERDATA: {
								shared_ptr<Type::Type> tl = Type::Type::checkType(L, i, false);
								if(tl){
									fireArgs.push_back(make_shared<Type::VarWrapper>(tl));
									break;
								}

								shared_ptr<Instance> ti = checkInstance(L, i, false);
								if(ti){
									fireArgs.push_back(make_shared<Type::VarWrapper>(ti));
									break;
								}
							}
							case LUA_TTABLE:
							case LUA_TFUNCTION:
							case LUA_TTHREAD:
							case LUA_TLIGHTUSERDATA: {
								lua_pushvalue(L, i);
								int ref = luaL_ref(L, LUA_REGISTRYINDEX);
								fireArgs.push_back(make_shared<Type::VarWrapper>(L, ref));
								break;
							}
						}
					}

					re->FireAllClients(fireArgs);
					return 0;
				}
			}

			return luaL_error(L, COLONERR, "FireAllClients");
		}

		int RemoteEvent::lua_FireServer(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(shared_ptr<RemoteEvent> re = dynamic_pointer_cast<RemoteEvent>(inst)){
				std::vector<shared_ptr<Type::VarWrapper>> fireArgs;

				int nargs = lua_gettop(L);
				if(nargs > 1){
					for(int i = 2; i <= nargs; i++){
						int ltype = lua_type(L, i);

						switch(ltype){
							case LUA_TNIL: {
								fireArgs.push_back(make_shared<Type::VarWrapper>((void*)NULL, Type::TYPE_NULL));
								break;
							}
							case LUA_TNUMBER: {
								fireArgs.push_back(make_shared<Type::VarWrapper>(lua_tonumber(L, i)));
								break;
							}
							case LUA_TBOOLEAN: {
								fireArgs.push_back(make_shared<Type::VarWrapper>(lua_toboolean(L, i)));
								break;
							}
							case LUA_TSTRING: {
								const char* tmpStr = lua_tostring(L, i);
								fireArgs.push_back(make_shared<Type::VarWrapper>(std::string(tmpStr)));
								break;
							}
							case LUA_TUSERDATA: {
								shared_ptr<Type::Type> tl = Type::Type::checkType(L, i, false);
								if(tl){
									fireArgs.push_back(make_shared<Type::VarWrapper>(tl));
									break;
								}

								shared_ptr<Instance> ti = checkInstance(L, i, false);
								if(ti){
									fireArgs.push_back(make_shared<Type::VarWrapper>(ti));
									break;
								}
							}
							case LUA_TTABLE:
							case LUA_TFUNCTION:
							case LUA_TTHREAD:
							case LUA_TLIGHTUSERDATA: {
								lua_pushvalue(L, i);
								int ref = luaL_ref(L, LUA_REGISTRYINDEX);
								fireArgs.push_back(make_shared<Type::VarWrapper>(L, ref));
								break;
							}
						}
					}

					re->FireServer(fireArgs);
					return 0;
				}
			}

			return luaL_error(L, COLONERR, "FireServer");
		}

		void RemoteEvent::register_lua_methods(lua_State* L){
			luaL_Reg methods[] = {
                {"FireClient", lua_FireClient},
			    {"FireAllClients", lua_FireAllClients},
				{"FireServer", lua_FireServer}
			};
			luaL_setfuncs(L, methods, 0);
		}

	    void RemoteEvent::register_lua_events(lua_State* L){
			Instance::register_lua_events(L);

			luaL_Reg events[] = {
				{"ServerEvent", WRAP_EVT(RemoteEvent, ServerEvent)},
				{"ClientEvent", WRAP_EVT(RemoteEvent, ClientEvent)},
				{NULL, NULL}
			};
			luaL_setfuncs(L, events, 0);
		}
	}
}

#endif
