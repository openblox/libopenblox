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

#ifndef OPENBLOX_TYPE_COLOR3_H_
#define OPENBLOX_TYPE_COLOR3_H_

#include <OpenBlox.h>
#include "OBType.h"

BEGIN_OB_TYPE

#define lua_color3_name "luaL_Color3"

class Color3{
	public:
		Color3();
		Color3(double r, double g, double b);
		virtual ~Color3();

		DECLARE_GAME_STATIC_INIT(Color3);

		double r;
		double g;
		double b;

		bool equals(Color3* other);

		Color3* clone();

		static int lua_index(lua_State* L);
		static int lua_newindex(lua_State* L);

		static int lua_toString(lua_State* L);

		static int lua_getR(lua_State* L);
		static int lua_getG(lua_State* L);
		static int lua_getB(lua_State* L);

		static int lua_eq(lua_State* L);

		int wrap_lua(lua_State* L);
};

Color3* checkColor3(lua_State* L, int n);

END_OB_TYPE

#endif
