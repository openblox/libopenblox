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

#include "lua/OBLua_OBBase.h"

namespace OB{
	namespace Lua{
		static const luaL_Reg base_funcs[] = {
			{"assert", luaB_assert},
			{"collectgarbage", luaB_collectgarbage},
			//{"dofile", luaB_dofile},
			{"error", luaB_error},
			{"getmetatable", luaB_getmetatable},
			{"ipairs", luaB_ipairs},
			//{"loadfile", luaB_loadfile},
			{"load", luaB_load},
			{"loadstring", luaB_load},
			{"next", luaB_next},
			{"pairs", luaB_pairs},
			{"pcall", luaB_pcall},
			//{"print", luaB_print},
			{"rawequal", luaB_rawequal},
			{"rawlen", luaB_rawlen},
			{"rawget", luaB_rawget},
			{"rawset", luaB_rawset},
			{"select", luaB_select},
			{"setmetatable", luaB_setmetatable},
			{"tonumber", luaB_tonumber},
			{"tostring", luaB_tostring},
			{"xpcall", luaB_xpcall},
			/* placeholders */
			{"type", NULL},
			{"_G", NULL},
			{"_VERSION", NULL},
			{NULL, NULL}
		};

		int luaopen_obbase(lua_State *L){
			int i;
			/* open lib into global table */
			lua_pushglobaltable(L);
			luaL_setfuncs(L, base_funcs, 0);
			/* set global _G */
			lua_pushvalue(L, -1);
			lua_setfield(L, -2, "_G");
			/* set global _VERSION */
			lua_pushliteral(L, LUA_VERSION);
			lua_setfield(L, -2, "_VERSION");
			/* set function 'type' with proper upvalues */
			for(i = 0; i < LUA_NUMTAGS; i++){ /* push all type names as upvalues */
				lua_pushstring(L, lua_typename(L, i));
			}
			lua_pushcclosure(L, luaB_type, LUA_NUMTAGS);
			lua_setfield(L, -2, "type");
			return 1;
		}
	}
}
