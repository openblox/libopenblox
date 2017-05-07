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

#include "instance/NetworkServer.h"

#include "OBEngine.h"

#include "OBException.h"

#if HAVE_ENET
namespace OB{
	namespace Instance{
		DEFINE_CLASS(NetworkServer, false, isDataModel, NetworkPeer){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    NetworkServer::NetworkServer(){
			Name = ClassName;

			Port = -1;
		}

	    NetworkServer::~NetworkServer(){}

		shared_ptr<Instance> NetworkServer::cloneImpl(){
			return NULL;
		}

		void NetworkServer::tick(){
			if(enet_host){
				ENetEvent evt;
				int numEvts = enet_host_check_events(enet_host, &evt);
				if(numEvts > 0){
				    processEvent(evt);

					if(numEvts > 1){
						for(int i = 0; i > numEvts - 1; i++){
							int r = enet_host_check_events(enet_host, &evt);
							if(r >= 0){
								processEvent(evt);
							}
						}
					}
				}
			}
		}

		int NetworkServer::getPort(){
			return Port;
		}

		void NetworkServer::Start(int port){
			if(!enet_host){
				ENetAddress address;
				address.host = ENET_HOST_ANY;

				if(port != 0){
					address.port = port;
				}

				//Hard coded max peer count! Eww!
				enet_host = enet_host_create(&address, OB_NET_MAX_PEERS, OB_NET_CHANNELS, 0, 0);
				if(!enet_host){
					throw new OBException("An error occurred while creating the ENet host.");
				}
			}
		}

		void NetworkServer::Stop(int port){
			if(enet_host){
			
			}
		}

		void NetworkServer::processEvent(ENetEvent evt){
			switch(evt.type){
				case ENET_EVENT_TYPE_CONNECT: {
					puts("Connect");
					break;
				}
				case ENET_EVENT_TYPE_RECEIVE: {
					puts("Recv");
					break;
				}
				case ENET_EVENT_TYPE_DISCONNECT: {
					puts("Disconnect");
					break;
				}
			}
		}

		int NetworkServer::lua_Start(lua_State* L){
		    shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(shared_ptr<NetworkServer> ns = dynamic_pointer_cast<NetworkServer>(inst)){
				try{
					if(!lua_isnoneornil(L, 2)){
						ns->Start(luaL_checkinteger(L, 2));
					}else{
						ns->Start();
					}
				}catch(OBException& ex){
					return luaL_error(L, ex.getMessage().c_str());
				}
				
				return 0;
			}
			
			return luaL_error(L, COLONERR, "Start");
		}

		int NetworkServer::lua_Stop(lua_State* L){
		    shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(shared_ptr<NetworkServer> ns = dynamic_pointer_cast<NetworkServer>(inst)){
			    try{
					if(!lua_isnoneornil(L, 2)){
						ns->Stop(luaL_checkinteger(L, 2));
					}else{
						ns->Stop();
					}
				}catch(OBException& ex){
					return luaL_error(L, ex.getMessage().c_str());
				}
				
				return 0;
			}
			
			return luaL_error(L, COLONERR, "Stop");
		}

		void NetworkServer::register_lua_methods(lua_State* L){
		    Instance::register_lua_methods(L);

			luaL_Reg methods[] = {
				{"Start", lua_Start},
				{"Stop", lua_Stop},
			    {NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}
	}
}
#endif
