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

#include "instance/NetworkServer.h"

#include "OBException.h"

#include "instance/ServerReplicator.h"
#include "instance/RemoteEvent.h"
#include "instance/Player.h"

#if HAVE_ENET
namespace OB{
	namespace Instance{
		DEFINE_CLASS(NetworkServer, false, isDataModel, NetworkPeer){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		NetworkServer::NetworkServer(OBEngine* eng) : NetworkPeer(eng){
			Name = ClassName;
			netId = OB_NETID_NOT_REPLICATED;

			Archivable = false;

			Port = -1;
		}

		NetworkServer::~NetworkServer(){}

		shared_ptr<Instance> NetworkServer::cloneImpl(){
			return NULL;
		}

		void NetworkServer::tick(){
			if(enet_host){
				ENetEvent evt;
				while(enet_host_service(enet_host, &evt, 10) > 0){
					processEvent(evt);
				}
			}

			tickChildren();
		}

		int NetworkServer::getPort(){
			return Port;
		}

		void NetworkServer::Start(int port){
			if(!enet_host){
				ENetAddress address;
				address.host = ENET_HOST_ANY;
				address.port = port;

				// Hard coded max peer count! Eww!
				enet_host = enet_host_create(&address, OB_NET_MAX_PEERS, OB_NET_CHANNELS, 0, 0);
				if(!enet_host){
					throw new OBException("An error occurred while creating the ENet host.");
				}
			}
		}

		void NetworkServer::Stop(int blockDuration){
			if(enet_host){
				for(int i = 0; i > enet_host->connectedPeers; i++){
					ENetPeer* peer = &(enet_host->peers[i]);
					if(peer){
						enet_peer_disconnect(peer, 0);
					}
				}

				ENetEvent evt;
				while(enet_host && enet_host_service(enet_host, &evt, blockDuration) > 0){
					processEvent(evt);
				}

				if(enet_host){
					for(int i = 0; i > enet_host->connectedPeers; i++){
						ENetPeer* peer = &(enet_host->peers[i]);
						if(peer){
							if(peer->data){
								shared_ptr<Instance> dataInst = (*static_cast<shared_ptr<Instance>*>(peer->data));

								if(shared_ptr<NetworkReplicator> netRep = dynamic_pointer_cast<NetworkReplicator>(dataInst)){
									netRep->_dropPeer();
								}
							}
						}
					}

					enet_host_destroy(enet_host);
					enet_host = NULL;
				}
			}
		}

		void NetworkServer::broadcast(enet_uint8 channel, BitStream &bs){
			if(enet_host){
				ENetPacket* pkt = enet_packet_create(bs.getData(), bs.getNumBytesUsed(), ENET_PACKET_FLAG_RELIABLE);
				if(!pkt){
					throw new OBException("Failed to create ENet packet.");
				}

				enet_host_broadcast(enet_host, channel, pkt);
			}
		}

#if HAVE_PUGIXML
		std::string NetworkServer::fixedSerializedID(){
			return "NetworkServer";
		}
#endif

		void NetworkServer::processPacket(ENetEvent evt, BitStream &bs){
			size_t pkt_type = bs.readSizeT();

			if(evt.channelID == OB_NET_CHAN_PROTOCOL){
				switch(pkt_type){
					case OB_NET_PKT_FIRE_REMOTE_EVENT: {
						ob_uint64 netId = bs.readUInt64();

						shared_ptr<DataModel> dm = eng->getDataModel();
						if(dm){
							weak_ptr<Instance> lookedUpInst = dm->lookupInstance(netId);
							if(lookedUpInst.expired()){
								return;
							}

							if(shared_ptr<Instance> ki = lookedUpInst.lock()){
								if(shared_ptr<RemoteEvent> re = dynamic_pointer_cast<RemoteEvent>(ki)){
								    // First argument for ServerEvent events will always be the client
									// If ServerReplicator->getPlayer() is non-NULL, we use the Player
									shared_ptr<Instance> dataInst = (*static_cast<shared_ptr<Instance>*>(evt.peer->data));

									if(shared_ptr<ServerReplicator> sr = dynamic_pointer_cast<ServerReplicator>(dataInst)){
										shared_ptr<Instance> peerInst = sr;

										if(shared_ptr<Player> plr = sr->GetPlayer()){
											peerInst = plr;
										}

										std::vector<shared_ptr<Type::VarWrapper>> argList;

										argList.push_back(make_shared<Type::VarWrapper>(peerInst));

										size_t numArgs = bs.readSizeT();

										for(size_t i = 0; i < numArgs; i++){
											shared_ptr<Type::VarWrapper> nVal = bs.readVar(eng);
											if(nVal){
												argList.push_back(nVal);
											}else{
												argList.push_back(NULL);
											}
										}

										re->getServerEvent()->Fire(eng, argList);
									}
								}
							}
						}

						break;
					}
					default: {
						printf("Unknown packet type: %i\n", pkt_type);
					}
				}
			}else{
				printf("Unknown network channel: %i\n", evt.channelID);
			}
		}

		void NetworkServer::processEvent(ENetEvent evt){
			switch(evt.type){
				case ENET_EVENT_TYPE_CONNECT: {
					shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();

					shared_ptr<ServerReplicator> servRep = make_shared<ServerReplicator>(evt.peer, eng);
					servRep->_initReplicator();
					servRep->setParent(sharedThis, false);
					servRep->ParentLocked = true;

					shared_ptr<DataModel> dm = eng->getDataModel();
					if(dm){
						dm->replicate(servRep);
					}
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
				}catch(OBException* ex){
					return luaL_error(L, ex->getMessage().c_str());
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
				}catch(OBException* ex){
					return luaL_error(L, ex->getMessage().c_str());
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
