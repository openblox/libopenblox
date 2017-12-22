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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "type/Type.h"

#include "type/VarWrapper.h"

#include <vector>

#include <functional>

#ifndef OB_TYPE_EVENT
#define OB_TYPE_EVENT

namespace OB{
	namespace Type{
		class EventConnection;

		class Event: public Type{
			public:
				Event(std::string name, bool canFireFromLua = false, bool blockLogService = false);
				virtual ~Event();

				shared_ptr<EventConnection> Connect(std::function<void(std::vector<shared_ptr<VarWrapper>>)> fnc);
				shared_ptr<EventConnection> Connect(void (*fnc)(std::vector<shared_ptr<VarWrapper>>, void*), void* ud);

				void disconnectAll();
				void disconnect(shared_ptr<EventConnection> conn);
				bool isConnected(shared_ptr<EventConnection> conn);
				bool doesBlockLogService();

				void FireLater(std::vector<shared_ptr<VarWrapper>> argList);
				void Fire(OBEngine* eng, std::vector<shared_ptr<VarWrapper>> argList);
				void Fire(OBEngine* eng);

				virtual std::string toString();

				DECLARE_TYPE();

				bool canFireFromLua;
				bool blockLogService;
				std::string name;
				std::vector<shared_ptr<EventConnection>> conns;

				static int lua_fire(lua_State* L);
				static int lua_connect(lua_State* L);
				static int lua_wait(lua_State* L);

				static void register_lua_methods(lua_State* L);
		};

		shared_ptr<Event> checkEvent(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);
	}
}

#endif // OB_TYPE_EVENT


// Local Variables:
// mode: c++
// End:
