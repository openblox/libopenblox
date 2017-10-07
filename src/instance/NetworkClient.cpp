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

#include "OBException.h"

#include "utility.h"

#include "instance/ClientReplicator.h"

#if HAVE_ENET
namespace OB{
	namespace Instance{
		DEFINE_CLASS(NetworkClient, false, isDataModel, NetworkPeer){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		NetworkClient::NetworkClient(OBEngine* eng) : NetworkPeer(eng){
			Name = ClassName;
			netId = OB_NETID_NOT_REPLICATED;

			Archivable = false;

			server_peer = NULL;
		}

		NetworkClient::~NetworkClient(){}

		shared_ptr<Instance> NetworkClient::cloneImpl(){
			return NULL;
		}

		void NetworkClient::tick(){
			if(enet_host){
				ENetEvent evt;
				while(enet_host && enet_host_service(enet_host, &evt, 10) > 0){
					processEvent(evt);
				}
			}

			if(!heldInstances.empty()){
				ob_int64 curTime = currentTimeMillis();

				while(!heldInstances.empty()){
					HeldInstance hi = heldInstances.front();
					if(hi.holdEnd < curTime){
						hi.inst.reset();
						heldInstances.pop();
					}else{
						break;
					}
				}
			}

			tickChildren();
		}

		void NetworkClient::Connect(std::string server, int serverPort, int clientPort){
			if(!enet_host){
				ENetAddress address;
				address.host = ENET_HOST_ANY;
				address.port = clientPort;

				enet_host = enet_host_create(&address, 1, OB_NET_CHANNELS, 0, 0);
				if(!enet_host){
					throw new OBException("An error occurred while creating the ENet host.");
				}

				ENetAddress servAddr;
				enet_address_set_host(&servAddr, server.c_str());
				servAddr.port = serverPort;

				server_peer = enet_host_connect(enet_host, &servAddr, OB_NET_CHANNELS, 0);
				if(!server_peer){
					enet_host_destroy(enet_host);
					enet_host = NULL;
					throw new OBException("No available peers for connection attempt.");
				}
			}
		}

		void NetworkClient::Disconnect(int blockDuration){
			if(enet_host && server_peer){
				enet_peer_disconnect(server_peer, 0);

				ENetEvent evt;
				while(enet_host && enet_host_service(enet_host, &evt, blockDuration) > 0){
					processEvent(evt);
				}


				server_peer = NULL;
				if(enet_host){
					enet_host_destroy(enet_host);
					enet_host = NULL;
				}
			}
		}

		void NetworkClient::processPacket(ENetEvent evt, BitStream &bs){
			size_t pkt_type = bs.readSizeT();

			switch(pkt_type){
			case OB_NET_PKT_CREATE_INSTANCE: {
				ob_uint64 netId = bs.readUInt64();
				std::string className = bs.readString();

				shared_ptr<DataModel> dm = eng->getDataModel();
				if(dm){
					weak_ptr<Instance> lookedUpInst = dm->lookupInstance(netId);
					if(lookedUpInst.expired()){
						shared_ptr<Instance> createdInst = ClassFactory::createReplicate(className, eng);
						if(createdInst){
							createdInst->setNetworkID(netId);

							HeldInstance hi;
							// We hold instances for up to 10 seconds
							hi.holdEnd = currentTimeMillis() + (10 * 1000);
							hi.inst = createdInst;

							heldInstances.push(hi);
						}
					}
				}
				break;
			}
			case OB_NET_PKT_SET_PARENT: {
				ob_uint64 netId = bs.readUInt64();
				ob_uint64 parentNetId = bs.readUInt64();

				shared_ptr<DataModel> dm = eng->getDataModel();
				if(dm){
					weak_ptr<Instance> lookedUpInst = dm->lookupInstance(netId);

					if(lookedUpInst.expired()){
						return;
					}

					if(shared_ptr<Instance> kid = lookedUpInst.lock()){
						bool wasParentLocked = false;
						if(kid->ParentLocked){
							wasParentLocked = true;
							kid->ParentLocked = false;
						}

						if(parentNetId > OB_NETID_NULL){
							weak_ptr<Instance> lookedUpParent = dm->lookupInstance(parentNetId);
							if(lookedUpParent.expired()){
								return;
							}

							if(shared_ptr<Instance> par = lookedUpParent.lock()){
								kid->setParent(par, true);
							}
						}else{
							kid->setParent(NULL, true);
						}

						if(wasParentLocked){
							kid->ParentLocked = true;
						}
					}
				}
				break;
			}
			case OB_NET_PKT_SET_PROPERTY: {
				ob_uint64 netId = bs.readUInt64();
				std::string prop = bs.readString();
				shared_ptr<Type::VarWrapper> val = bs.readVar(eng);

				shared_ptr<DataModel> dm = eng->getDataModel();
				if(dm){
					weak_ptr<Instance> lookedUpInst = dm->lookupInstance(netId);
					if(lookedUpInst.expired()){
						return;
					}

					if(shared_ptr<Instance> kid = lookedUpInst.lock()){
						kid->setProperty(prop, val);
					}
				}
				break;
			}
			default: {
				printf("Unknown packet type: %i\n", pkt_type);
			}
			}
		}

		void NetworkClient::processEvent(ENetEvent evt){
			switch(evt.type){
			case ENET_EVENT_TYPE_CONNECT: {
				shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();

				shared_ptr<ClientReplicator> cliRep = make_shared<ClientReplicator>(evt.peer, eng);
				cliRep->_initReplicator();
				cliRep->setParent(sharedThis, false);
				cliRep->ParentLocked = true;
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE: {
				ENetPacket* pkt = evt.packet;

				BitStream bs(pkt->data, pkt->dataLength, true);

				try{
					processPacket(evt, bs);
				}catch(OBException* ex){
					printf("Error reading packet: %s\n", ex->getMessage().c_str());
				}

				enet_packet_destroy(pkt);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT: {
				ENetPeer* peer = evt.peer;
				if(peer->data){
					shared_ptr<Instance> dataInst = (*static_cast<shared_ptr<Instance>*>(peer->data));

					if(shared_ptr<NetworkReplicator> netRep = dynamic_pointer_cast<NetworkReplicator>(dataInst)){
						netRep->_dropPeer();
					}
				}

				enet_host_destroy(enet_host);
				enet_host = NULL;
				server_peer = NULL;
				break;
			}
			}
		}

#if HAVE_PUGIXML
		std::string NetworkClient::serializedID(){
			shared_ptr<OBSerializer> serializer = eng->getSerializer();
			serializer->SetID(shared_from_this(), "NetworkClient");

			return Instance::serializedID();
		}
#endif

		int NetworkClient::lua_Connect(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(shared_ptr<NetworkClient> nc = dynamic_pointer_cast<NetworkClient>(inst)){
				std::string servAddr = std::string(luaL_checkstring(L, 2));
				int servPort = luaL_checkinteger(L, 3);

				try{
					if(!lua_isnoneornil(L, 4)){
						nc->Connect(servAddr, servPort, luaL_checkinteger(L, 4));
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
