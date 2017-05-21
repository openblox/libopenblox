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

#include "instance/DataModel.h"

#include "BitStream.h"

#if HAVE_ENET

#ifndef OB_INST_NETWORKSERVER
#define OB_INST_NETWORKSERVER

namespace OB{
	namespace Instance{
		class NetworkServer: public NetworkPeer{
			public:
			    NetworkServer(OBEngine* eng);
				virtual ~NetworkServer();

			    virtual void tick();

				int getPort();

				void Start(int port = 0);
				void Stop(int blockDuration = 1000);

				void broadcast(enet_uint8 channel, BitStream &bs);

				#if HAVE_PUGIXML
				virtual std::string serializedID();
				#endif

				DECLARE_LUA_METHOD(Start);
				DECLARE_LUA_METHOD(Stop);

				void processEvent(ENetEvent evt);

				static void register_lua_methods(lua_State* L);

				DECLARE_CLASS(NetworkServer);

				int Port;
		};
	}
}

#endif // OB_INST_NETWORKSERVER

#endif // HAVE_ENET

// Local Variables:
// mode: c++
// End:
