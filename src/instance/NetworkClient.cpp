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

#include "instance/NetworkClient.h"

#include "OBEngine.h"

#include "OBException.h"

#if HAVE_ENET
namespace OB{
	namespace Instance{
		DEFINE_CLASS(NetworkClient, false, isDataModel, NetworkPeer){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    NetworkClient::NetworkClient(){
			Name = ClassName;

			server_peer = NULL;
		}

	    NetworkClient::~NetworkClient(){}

		shared_ptr<Instance> NetworkClient::cloneImpl(){
			return NULL;
		}

		void NetworkClient::tick(){
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

		void NetworkClient::Connect(std::string server, int serverPort, int clientPort){
			if(!enet_host){
				ENetAddress address;
				address.host = ENET_HOST_ANY;

				if(clientPort != 0){
					address.port = clientPort;
				}

				enet_host = enet_host_create(&address, 1, OB_NET_CHANNELS, 0, 0);
				if(!enet_host){
					throw new OBException("An error occurred while creating the ENet host.");
				}

				ENetAddress servAddr;
				enet_address_set_host(&servAddr, server.c_str());
				servAddr.port = serverPort;

			    server_peer = enet_host_connect(enet_host, &servAddr, OB_NET_CHANNELS, 0);
				if(!server_peer){
					throw new OBException("No available peers for connection attempt.");
				}
			}
		}

		void NetworkClient::Disconnect(int blockDuration){
		}

		void NetworkClient::processEvent(ENetEvent evt){
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

		int NetworkClient::lua_Connect(lua_State* L){
		    shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(shared_ptr<NetworkClient> nc = dynamic_pointer_cast<NetworkClient>(inst)){
				std::string servAddr = std::string(luaL_checkstring(L, 2));
				int servPort = luaL_checkinteger(L, 4);
				
				try{
					if(!lua_isnoneornil(L, 2)){
						nc->Connect(servAddr, servPort, luaL_checkinteger(L, 5));
					}else{
						nc->Connect(servAddr, servPort);
					}
				}catch(OBException& ex){
					return luaL_error(L, ex.getMessage().c_str());
				}
				
				return 0;
			}
			
			return luaL_error(L, COLONERR, "Connect");
		}

		int NetworkClient::lua_Disconnect(lua_State* L){
		    shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(shared_ptr<NetworkClient> nc = dynamic_pointer_cast<NetworkClient>(inst)){
			    try{
					if(!lua_isnoneornil(L, 2)){
						nc->Disconnect(luaL_checkinteger(L, 2));
					}else{
						nc->Disconnect();
					}
				}catch(OBException& ex){
					return luaL_error(L, ex.getMessage().c_str());
				}
				
				return 0;
			}
			
			return luaL_error(L, COLONERR, "Disconnect");
		}

		void NetworkClient::register_lua_methods(lua_State* L){
		    Instance::register_lua_methods(L);

			luaL_Reg methods[] = {
				{"Connect", lua_Connect},
				{"Disconnect", lua_Disconnect},
			    {NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}
	}
}
#endif
