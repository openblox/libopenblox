/*
 * Copyright 2016 John M. Harris, Jr.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenBlox.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OB_LUA_OB_LUA_H_
#define OB_LUA_OB_LUA_H_

extern "C"{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

namespace ob_lua{
	#define OB_G_FUNC(L, funcName) lua_register(L, #funcName, lua_##funcName)

	struct LuaState{
		lua_State* L;
		int ref;
	};

	extern lua_State* init();
	extern LuaState* newState();
	extern LuaState* newthread(lua_State* L);

	//extern int lua_require(lua_State* L);

	extern int lua_print(lua_State* L);
	extern int lua_warn(lua_State* L);

	extern int lua_wait(lua_State* L);
	extern int lua_delay(lua_State* L);
	extern int lua_spawn(lua_State* L);

	extern int lua_elapsedTime(lua_State* L);
	extern int lua_tick(lua_State* L);

	extern int lua_LoadLibrary(lua_State* L);

	extern int lua_newInstance(lua_State* L);
	extern int lua_newVector2(lua_State* L);
	extern int lua_newVector3(lua_State* L);
	extern int lua_newColor3(lua_State* L);

	extern LuaState* getStateInfo(lua_State* L);
	extern void killState(lua_State* L);

	extern void handle_lua_errors(lua_State* L);
}

#endif
