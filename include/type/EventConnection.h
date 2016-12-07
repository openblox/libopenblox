/*
 * Copyright (C) 2016 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "obtype.h"

extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#ifndef OB_TYPE_EVENTCONNECTION
#define OB_TYPE_EVENTCONNECTION

namespace OB{
	namespace Type{
		class Event;
		
		class EventConnection{
			public:
			    EventConnection(Event* evt, void* ud, eventConnectFunction fnc);
				virtual ~EventConnection();

				void Disconnect();
				bool isConnected();

			    static void init();
				
			protected:
			    Event* evt;
				void* ud;
				eventConnectFunction fnc;
		};

		EventConnection* checkEventConnection(lua_State* L, int n);
	}
}

#endif // OB_TYPE_EVENTCONNECTION


// Local Variables:
// mode: c++
// End:
