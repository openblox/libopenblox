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

#ifndef OB_LUA_OBLUA
#define OB_LUA_OBLUA

extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <string>

namespace OB{
	namespace Lua{
		struct OBLState{
			lua_State* L;
			int ref;
		};

		/**
		 * Returns a global Lua state, which is only used to handle
		 * global variables and state management (i.e. though the
		 * TaskScheduler).
		 *
		 * @returns Global a global Lua state.
		 * @author John M. Harris, Jr.
		 */
		lua_State* initGlobal();

		/**
		 * Returns a Lua state under the global Lua state with the
		 * altered OpenBlox standard library. OpenBlox provides an
		 * altered base library, coroutine library and os library. The
		 * other libraries loaded are as follows: table, string, math,
		 * utf8 
		 *
		 * @param gL Global Lua state
		 * @returns Lua state under the global Lua state
		 * @author John M. Harris, Jr.
		 */
		lua_State* initThread(lua_State* gL);

		/**
		 * Returns a Lua state under a parent Lua state. This replaces
		 * Lua's default coroutine creation, because OpenBlox needs to
		 * keep track of certain metadata about each Lua state, for
		 * example the 'script' value. Event handlers are also passed
		 * through this.
		 *
		 * @param pL Parent Lua state
		 * @returns New Lua state under the parent Lua state
		 * @author John M. Harris, Jr.
		 */
		lua_State* initCoroutine(lua_State* pL);

		/**
		 * Used internally to handle errors. Returns a Lua error as a
		 * string.
		 *
		 * @param L Lua state
		 * @returns std::string Lua error message
		 * @author John M. Harris, Jr.
		 */
		std::string handle_errors(lua_State* L);
	}
}

#endif // OB_LUA_OBLUA

// Local Variables:
// mode: c++
// End:
