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

#ifndef OB_LUA_OBBASE
#define OB_LUA_OBBASE

#include "OBLua.h"

/**
 * @file OBLua_OBBase.h
 * @author John M. Harris, Jr.
 * @date August 2016
 *
 * This file defines the OpenBlox base lib, which replaces the usual
 * base library of Lua 5.3.
 */

namespace OB{
	namespace Lua{
		int luaB_assert(lua_State* L);
		int luaB_collectgarbage(lua_State* L);
		int luaB_error(lua_State* L);
		int luaB_getmetatable(lua_State* L);
		int luaB_ipairs(lua_State* L);
		int luaB_load(lua_State* L);
		int luaB_next(lua_State* L);
		int luaB_pairs(lua_State* L);
		int luaB_pcall(lua_State* L);
		int luaB_rawequal(lua_State* L);
		int luaB_rawlen(lua_State* L);
		int luaB_rawget(lua_State* L);
		int luaB_rawset(lua_State* L);
		int luaB_select(lua_State* L);
		int luaB_setmetatable(lua_State* L);
		int luaB_tonumber(lua_State* L);
		int luaB_tostring(lua_State* L);
		int luaB_xpcall(lua_State* L);
		int luaB_type(lua_State* L);
		
		int luaopen_obbase(lua_State* L);
	}
}

#endif // OB_LUA_OBBASE

// Local Variables:
// mode: c++
// End:
