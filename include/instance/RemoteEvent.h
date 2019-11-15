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

#include "instance/Instance.h"

#if HAVE_ENET

#ifndef OB_INST_REMOTEEVENT
#define OB_INST_REMOTEEVENT

namespace OB{
	namespace Instance{
		/**
		 * The RemoteEvent class provides a simple method for
		 * developers to create their own network enabled events
		 * in the DataModel.
		 *
		 * @author John M. Harris, Jr.
		 */
		class RemoteEvent: public Instance{
			public:
			    RemoteEvent(OBEngine* eng);
				virtual ~RemoteEvent();

				shared_ptr<Type::Event> getServerEvent();
				shared_ptr<Type::Event> getClientEvent();

				void FireServer(std::vector<shared_ptr<Type::VarWrapper>> argList);
				void FireClient(shared_ptr<Instance> plr_conn, std::vector<shared_ptr<Type::VarWrapper>> argList);
				void FireAllClients(std::vector<shared_ptr<Type::VarWrapper>> argList);

				DECLARE_LUA_METHOD(FireServer);
				DECLARE_LUA_METHOD(FireClient);
				DECLARE_LUA_METHOD(FireAllClients);

				static void register_lua_methods(lua_State* L);
				static void register_lua_events(lua_State* L);

				DECLARE_CLASS(RemoteEvent);

				shared_ptr<Type::Event> ServerEvent;
				shared_ptr<Type::Event> ClientEvent;
		};
	}
}

#endif // OB_INST_REMOTEEVENT

#endif


// Local Variables:
// mode: c++
// End:
