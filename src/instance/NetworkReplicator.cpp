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

#include "instance/NetworkReplicator.h"

#if HAVE_ENET
namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS_WCLONE(NetworkReplicator, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		NetworkReplicator::NetworkReplicator(){
			Name = ClassName;

			enet_peer = NULL;
		}

		NetworkReplicator::NetworkReplicator(ENetPeer* peer){
			Name = ClassName;

			enet_peer = peer;
		}

	    NetworkReplicator::~NetworkReplicator(){}

		void NetworkReplicator::_initReplicator(){
			shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
			
			void* udata = malloc(sizeof(shared_ptr<Instance>));
			new(udata) shared_ptr<Instance>(sharedThis);

			enet_peer->data = udata;
		}

		void NetworkReplicator::_dropPeer(){
			if(enet_peer->data){
				(*static_cast<shared_ptr<Instance>*>(enet_peer->data)).reset();
				enet_peer->data = NULL;
			}
			enet_peer = NULL;

			ParentLocked = false;
			Destroy();
		}

		int NetworkReplicator::getHighestRoundTripTimeVariance(){
		    if(enet_peer){
			    return enet_peer->highestRoundTripTimeVariance;
			}
			return -1;
		}
		
		int NetworkReplicator::getLastReceiveTime(){
			if(enet_peer){
			    return enet_peer->lastReceiveTime;
			}
			return -1;
		}
		
		int NetworkReplicator::getLastRoundTripTime(){
			if(enet_peer){
			    return enet_peer->lastRoundTripTime;
			}
			return -1;
		}
		
		int NetworkReplicator::getLastRoundTripTimeVariance(){
			if(enet_peer){
			    return enet_peer->lastRoundTripTimeVariance;
			}
			return -1;
		}
		
		int NetworkReplicator::getLastSendTime(){
			if(enet_peer){
			    return enet_peer->lastSendTime;
			}
			return -1;
		}
		
		int NetworkReplicator::getLowestRoundTripTime(){
			if(enet_peer){
			    return enet_peer->lowestRoundTripTime;
			}
			return -1;
		}
		
		int NetworkReplicator::getMTU(){
			if(enet_peer){
			    return enet_peer->mtu;
			}
			return -1;
		}
		
		int NetworkReplicator::getPacketLoss(){
			if(enet_peer){
			    return enet_peer->packetLoss;
			}
			return -1;
		}
		
		int NetworkReplicator::getPacketLossVariance(){
			if(enet_peer){
			    return enet_peer->packetLossVariance;
			}
			return -1;
		}
		
		int NetworkReplicator::getPacketsLost(){
			if(enet_peer){
			    return enet_peer->packetsLost;
			}
			return -1;
		}
		
		int NetworkReplicator::getPacketsSent(){
			if(enet_peer){
			    return enet_peer->packetsSent;
			}
			return -1;
		}
		
		int NetworkReplicator::getRoundTripTime(){
			if(enet_peer){
			    return enet_peer->roundTripTime;
			}
			return -1;
		}
		
		int NetworkReplicator::getRoundTripTimeVariance(){
			if(enet_peer){
			    return enet_peer->packetsLost;
			}
			return -1;
		}

		void NetworkReplicator::Disconnect(){
			if(enet_peer){
				enet_peer_disconnect(enet_peer, 0);
			}
		}

		void NetworkReplicator::Ping(){
			if(enet_peer){
				enet_peer_ping(enet_peer);
			}
		}

		void NetworkReplicator::Reset(){
			if(enet_peer){
				enet_peer_reset(enet_peer);
			}
		}

		void _ob_networking_packet_free_cb(ENetPacket* pkt){
			if(pkt){
				pkt->data = NULL;

			    if(pkt->userData){
					(*static_cast<shared_ptr<BitStream>*>(pkt->userData)).reset();
					free(pkt->userData);
					pkt->userData = NULL;
				}
			}
		}

		void NetworkReplicator::Send(enet_uint8 channel, shared_ptr<BitStream> bs){
			if(enet_peer){
				ENetPacket* pkt = enet_packet_create(NULL, bs->getLength(), ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
				if(!pkt){
					throw new OBException("Failed to create ENet packet.");
				}

			    void* udata = malloc(sizeof(shared_ptr<BitStream>));
				new(udata) shared_ptr<BitStream>(bs);
				
				pkt->data = bs->getData();
				pkt->userData = udata;
				pkt->freeCallback = _ob_networking_packet_free_cb;
			    
				enet_peer_send(enet_peer, channel, pkt);
			}
		}
	}
}
#endif
