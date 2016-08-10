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

#include "lua/OBLua.h"

#include "lua/OBLua_OBBase.h"

#include <map>

namespace OB{
	namespace Lua{
		void* l_alloc(void* ud, void* ptr, size_t osize, size_t nsize){
			(void)ud;
			(void)osize;

			if(nsize == 0){
				free(ptr);
				return NULL;
			}else{
				return realloc(ptr, nsize);
			}
		}
		
		//Stores information about Lua states used by OpenBlox, for example the 'script' value.
		static std::map<lua_State*, struct OBLState*> lStates;
		
		lua_State* initGlobal(){
			//Don't put anything on the global state, its one purpose
			//is to be the parent of coroutines.
			return lua_newstate(l_alloc, NULL);
		}

		lua_State* initThread(lua_State* gL){
			lua_State* L = lua_newthread(gL);

			struct OBLState* LState = new struct OBLState;
			LState->L = L;
			LState->ref = luaL_ref(gL, LUA_REGISTRYINDEX);

			lStates[L] = LState;

			//Load altered standard lib
			luaL_requiref(L, "_G", luaopen_obbase, 1);//OB version of Lua's base lib
			luaL_requiref(L, LUA_COLIBNAME, luaopen_coroutine, 1);
			luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);
			luaL_requiref(L, LUA_OSLIBNAME, luaopen_obos, 1);//OB version of Lua's os lib
			luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
			luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 1);
			luaL_requiref(L, LUA_UTF8LIBNAME, luaopen_utf8, 1);

			lua_pop(L, 7);
			
			return L;
		}

		lua_State* initCoroutine(lua_State* pL){
			//Unlike "initThread", we don't load in our altered standard library.
			//We want this coroutine to use the environment of the parent state.

			lua_State* L = lua_newthread(pL);

			struct OBLState* LState = new struct OBLState;
			LState->L = L;
			LState->ref = luaL_ref(pL, LUA_REGISTRYINDEX);

			lStates[L] = LState;
			
			return L;
		}
	}
}
