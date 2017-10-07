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

#include "instance/NetworkPeer.h"

#include <queue>

#include "BitStream.h"

#if HAVE_ENET

#ifndef OB_INST_NETWORKCLIENT
#define OB_INST_NETWORKCLIENT

namespace OB{
	namespace Instance{
		class NetworkClient: public NetworkPeer{
		public:
			NetworkClient(OBEngine* eng);
			virtual ~NetworkClient();

			virtual void tick();

			int getPort();

			void Connect(std::string server, int serverPort, int clientPort = 0);
			void Disconnect(int blockDuration = 1000);

#if HAVE_PUGIXML
			virtual std::string serializedID();
#endif

			DECLARE_LUA_METHOD(Connect);
			DECLARE_LUA_METHOD(Disconnect);

			void processPacket(ENetEvent evt, BitStream &bs);
			void processEvent(ENetEvent evt);

			static void register_lua_methods(lua_State* L);

			class HeldInstance{
			public:
				shared_ptr<Instance> inst;
				ob_int64 holdEnd;
			};
			std::queue<HeldInstance> heldInstances;

			DECLARE_CLASS(NetworkClient);

			ENetPeer* server_peer;
		};
	}
}

#endif // OB_INST_NETWORKCLIENT

#endif // HAVE_ENET

// Local Variables:
// mode: c++
// End:
