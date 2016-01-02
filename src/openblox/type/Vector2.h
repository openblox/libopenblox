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

#ifndef OPENBLOX_TYPE_VECTOR2_H_
#define OPENBLOX_TYPE_VECTOR2_H_

#include <OpenBlox.h>
#include "OBType.h"

BEGIN_OB_TYPE

#define lua_vec2_name "luaL_Vector2"

class Vector2{
	public:
		Vector2();
		Vector2(double x, double y);
		virtual ~Vector2();

		DECLARE_GAME_STATIC_INIT(Vector2);

		double x;
		double y;
		double magnitude;

		Vector2* getNormalized();//unit

		bool equals(Vector2* other);

		Vector2* clone();

		Vector2* add(double v);
		Vector2* add(Vector2* other);
		Vector2* sub(double v);
		Vector2* sub(Vector2* other);
		Vector2* mul(double v);
		Vector2* mul(Vector2* other);
		Vector2* div(double v);
		Vector2* div(Vector2* other);
		Vector2* neg();

		Vector2* lerp(Vector2* goal, double alpha);
		double dot(Vector2* other);
		Vector2* cross(Vector2* other);
		bool isClose(Vector2* other, double epsilon = 1e-6);

		static int lua_index(lua_State* L);
		static int lua_newindex(lua_State* L);

		static int lua_toString(lua_State* L);
		static int lua_getUnit(lua_State* L);
		static int lua_getMagnitude(lua_State* L);
		static int lua_getX(lua_State* L);
		static int lua_getY(lua_State* L);
		static int lua_lerp(lua_State* L);
		static int lua_dot(lua_State* L);
		static int lua_cross(lua_State* L);
		static int lua_isClose(lua_State* L);

		static int lua_unm(lua_State* L);
		static int lua_add(lua_State* L);
		static int lua_sub(lua_State* L);
		static int lua_mul(lua_State* L);
		static int lua_div(lua_State* L);
		static int lua_eq(lua_State* L);

		int wrap_lua(lua_State* L);
};

Vector2* checkVector2(lua_State* L, int n);

END_OB_TYPE

#endif
