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

#include "instance/Instance.h"

#include "oblibconfig.h"

#include "BitStream.h"

#if HAVE_ENET
#include <enet/enet.h>

#ifndef OB_INST_NETWORKREPLICATOR
#define OB_INST_NETWORKREPLICATOR

namespace OB{
	namespace Instance{
		class NetworkReplicator: public Instance{
			public:
			    NetworkReplicator();
				NetworkReplicator(ENetPeer* peer);
				virtual ~NetworkReplicator();

				void _initReplicator();
				void _dropPeer();

			    int getHighestRoundTripTimeVariance();
				int getLastReceiveTime();
				int getLastRoundTripTime();
				int getLastRoundTripTimeVariance();
				int getLastSendTime();
				int getLowestRoundTripTime();
				int getMTU();
				int getPacketLoss();
				int getPacketLossVariance();
				int getPacketsLost();
				int getPacketsSent();
				int getRoundTripTime();
				int getRoundTripTimeVariance();

				void Disconnect();
				void Ping();
				void Reset();
				void Send(enet_uint8 channel, shared_ptr<BitStream> bs);

				DECLARE_CLASS(NetworkReplicator);

				ENetPeer* enet_peer;
		};
	}
}

#endif // OB_INST_NETWORKREPLICATOR

#endif // HAVE_ENET

// Local Variables:
// mode: c++
// End:
