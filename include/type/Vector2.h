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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "type/Type.h"

#include "oblibconfig.h"

#if HAVE_IRRLICHT
#include <irrlicht/vector2d.h>
#endif

#ifndef OB_TYPE_VECTOR2
#define OB_TYPE_VECTOR2

namespace OB{
	namespace Type{
		class Vector2: public Type{
			public:
			    Vector2();
			    Vector2(double x, double y);
				virtual ~Vector2();

				double getX();
				double getY();

				#if HAVE_IRRLICHT
				
				irr::core::vector2d<double> toIrrlichtVector2d();
				irr::core::vector2d<float> toIrrlichtVector2df();

				#endif

				bool equals(shared_ptr<Vector2> other);

				double getLength();
				double getLengthSquared();
				shared_ptr<Vector2> normalize();

			    shared_ptr<Vector2> add(double v);
				shared_ptr<Vector2> add(shared_ptr<Vector2> v);
				shared_ptr<Vector2> sub(double v);
				shared_ptr<Vector2> sub(shared_ptr<Vector2> v);
				shared_ptr<Vector2> mul(double v);
				shared_ptr<Vector2> mul(shared_ptr<Vector2> v);
				shared_ptr<Vector2> div(double v);
				shared_ptr<Vector2> div(shared_ptr<Vector2> v);
				shared_ptr<Vector2> neg();

				shared_ptr<Vector2> lerp(shared_ptr<Vector2> goal, double alpha);
				double dot(shared_ptr<Vector2> v);
				shared_ptr<Vector2> cross(shared_ptr<Vector2> v);
				bool isClose(shared_ptr<Vector2> v, double epsilon = 1e-6);

				virtual std::string toString();

				static int lua_getX(lua_State* L);
				static int lua_getY(lua_State* L);

				static int lua_getLength(lua_State* L);
				static int lua_getLengthSquared(lua_State* L);
				static int lua_getUnit(lua_State* L);

				static int lua_lerp(lua_State* L);
				static int lua_dot(lua_State* L);

				static int lua_eq(lua_State* L);
				static int lua_unm(lua_State* L);
				static int lua_add(lua_State* L);
				static int lua_sub(lua_State* L);
				static int lua_mul(lua_State* L);
				static int lua_div(lua_State* L);

				static void register_lua_metamethods(lua_State* L);
				static void register_lua_methods(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				static void register_lua_property_getters(lua_State* L);
				
				DECLARE_TYPE();

				double x;
				double y;

		};

		shared_ptr<Vector2> checkVector2(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);
	}
}

#endif // OB_TYPE_VECTOR2


// Local Variables:
// mode: c++
// End:
