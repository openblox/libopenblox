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
	/*
	 * Convenience function to handle type errors, used in check*
	 * functions.
	 *
	 * Basically luaL_typeerror/luaL_typerror
	 *
	 * @param L Lua state
	 * @param arg Argument
	 * @param tname Type name expected
	 * @author John M. Harris, Jr.
	 */
	inline int luaO_typeerror(lua_State* L, int arg, const char* tname){
		const char* typearg; //Name of type of the given argument

		if(luaL_getmetafield(L, arg, "__name") == LUA_TSTRING){
			typearg = lua_tostring(L, -1); //Use given type name
		}else if(lua_type(L, arg) == LUA_TLIGHTUSERDATA){
			typearg = "light userdata";
		}else{
			typearg = luaL_typename(L, arg);
		}
		
		const char* msg = lua_pushfstring(L, "%s expected, got %s", tname, typearg);
		return luaL_argerror(L, arg, msg);
	}
	
	namespace Lua{
		struct OBLState{
			lua_State* L;
			int ref;
			int numChildStates;
			bool initUseOver;
			OBLState* parent;
		};

		static OBLState* globalOBLState = NULL;

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
		 * Handles closing a state.
		 *
		 * @param L Lua state
		 * @author John M. Harris, Jr.
		 */
		void close_state(lua_State* L);

		/**
		 * Used internally to handle errors. Returns a Lua error as a
		 * string.
		 *
		 * @param L Lua state
		 * @returns std::string Lua error message
		 * @author John M. Harris, Jr.
		 */
		std::string handle_errors(lua_State* L);

		//Lua functions and misc

		/**
		 * Used as the redefined "print" function for Lua.
		 *
		 * @param L Lua state
		 * @returns int 0
		 * @author John M. Harris, Jr.
		 */
		int lua_print(lua_State* L);

		/**
		 * Basic warn function.
		 *
		 * @param L Lua state
		 * @returns int 0
		 * @author John M. Harris, Jr.
		 */
		int lua_warn(lua_State* L);

		/**
		 * Used to yield the current coroutine for a certain amount of time.
		 *
		 * @param L Lua state
		 * @returns int 0
		 * @author John M. Harris, Jr.
		 */
		int lua_wait(lua_State* L);

		/**
		 * Used to create an Instance from Lua.
		 *
		 * @param L Lua state
		 * @returns int 1
		 * @author John M. Harris, Jr.
		 */
		int lua_newInstance(lua_State* L);

		/**
		 * Used to create a Color3 from Lua.
		 *
		 * @param L Lua state
		 * @returns int 1
		 * @author John M. Harris, Jr.
		 */
		int lua_newColor3(lua_State* L);

		/**
		 * Used to create a Vector3 from Lua.
		 *
		 * @param L Lua state
		 * @returns int 1
		 * @author John M. Harris, Jr.
		 */
		int lua_newVector3(lua_State* L);

		/**
		 * Used to get a list of all Instance classes from Lua.
		 *
		 * @param L Lua state
		 * @returns int 1
		 * @author John M. Harris, Jr.
		 */
		int lua_listInstanceClasses(lua_State* L);
	}
}

#endif // OB_LUA_OBLUA

// Local Variables:
// mode: c++
// End:
