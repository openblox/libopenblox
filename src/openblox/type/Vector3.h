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

#ifndef OPENBLOX_TYPE_VECTOR3_H_
#define OPENBLOX_TYPE_VECTOR3_H_

#include <OpenBlox.h>
#include "OBType.h"

BEGIN_OB_TYPE

#define lua_vec3_name "luaL_Vector3"

class Vector3{
	public:
		Vector3();
		Vector3(double x, double y, double z);
		virtual ~Vector3();

		DECLARE_GAME_STATIC_INIT(Vector3);

		double x;
		double y;
		double z;
		double magnitude;

		Vector3* getNormalized();//unit

		bool equals(Vector3* other);

		Vector3* clone();

		Vector3* add(double v);
		Vector3* add(Vector3* other);
		Vector3* sub(double v);
		Vector3* sub(Vector3* other);
		Vector3* mul(double v);
		Vector3* mul(Vector3* other);
		Vector3* div(double v);
		Vector3* div(Vector3* other);
		Vector3* neg();

		Vector3* lerp(Vector3* goal, double alpha);
		double dot(Vector3* other);
		Vector3* cross(Vector3* other);
		bool isClose(Vector3* other, double epsilon = 1e-6);

		static int lua_index(lua_State* L);
		static int lua_newindex(lua_State* L);

		static int lua_toString(lua_State* L);
		static int lua_getUnit(lua_State* L);
		static int lua_getMagnitude(lua_State* L);
		static int lua_getX(lua_State* L);
		static int lua_getY(lua_State* L);
		static int lua_getZ(lua_State* L);
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

Vector3* checkVector3(lua_State* L, int n);

END_OB_TYPE

#endif
