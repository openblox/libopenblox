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

#include "type/Type.h"

#ifndef OB_TYPE_EVENTCONNECTION
#define OB_TYPE_EVENTCONNECTION

namespace OB{
	namespace Type{
		class Event;
		
		class EventConnection: public Type{
			public:
			    EventConnection(Event* evt, void* ud, void (*fnc)(std::vector<VarWrapper>, void*));
				virtual ~EventConnection();

				void Disconnect();
				bool isConnected();

				void fire(std::vector<VarWrapper> args);

				virtual std::string toString();
				
				DECLARE_TYPE();

				static int lua_disconnect(lua_State* L);
				static int lua_isConnected(lua_State* L);

				static void register_lua_methods(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				static void register_lua_property_getters(lua_State* L);
				
			    Event* evt;
				void* ud;
			    void (*fnc)(std::vector<VarWrapper>, void*);
		};

		EventConnection* checkEventConnection(lua_State* L, int n);
	}
}

#endif // OB_TYPE_EVENTCONNECTION


// Local Variables:
// mode: c++
// End:
