/*
 * Copyright 2015 John M. Harris, Jr.
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

#include "ob_lua.h"

#include <cstdlib>

namespace ob_lua{
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

	/**
	 * Used to initialize Lua and get the main lua_State*
	 * @returns Main lua_State*
	 * @author John M. Harris, Jr.
	 */
	lua_State* init(){
		return lua_newstate(l_alloc, NULL);
	}
}
