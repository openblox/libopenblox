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

#include "type/VarWrapper.h"

#include <vector>

#ifndef OB_TYPE_EVENT
#define OB_TYPE_EVENT

namespace OB{
	namespace Type{	
		class EventConnection;
		
		class Event: public Type{
			public:
			    Event(std::string name);
				Event(std::string name, bool canFireFromLua);
				virtual ~Event();

			    EventConnection* Connect(void (*fnc)(std::vector<VarWrapper>, void*), void* ud);

				void disconnectAll();
				void disconnect(EventConnection* conn);
			    bool isConnected(EventConnection* conn);

				void FireLater(std::vector<VarWrapper>* argList);
				void Fire(std::vector<VarWrapper>* argList);
				void Fire();

				virtual std::string toString();
				
				DECLARE_TYPE();

				bool canFireFromLua;
				std::string name;
				std::vector<EventConnection*> conns;

				static int lua_fire(lua_State* L);
				static int lua_connect(lua_State* L);
				static int lua_wait(lua_State* L);

				static void register_lua_methods(lua_State* L);
		};

		Event* checkEvent(lua_State* L, int n);
	}
}

#endif // OB_TYPE_EVENT


// Local Variables:
// mode: c++
// End:
