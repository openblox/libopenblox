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
#include <irrlicht/vector3d.h>
#endif

#if HAVE_BULLET
#include <btBulletDynamicsCommon.h>
#endif

#ifndef OB_TYPE_VECTOR3
#define OB_TYPE_VECTOR3

namespace OB{
	namespace Type{
		class Vector3: public Type{
			public:
				Vector3();
				Vector3(double x, double y, double z);
				Vector3(std::string str);
				virtual ~Vector3();

				double getX();
				double getY();
				double getZ();

#if HAVE_IRRLICHT
				irr::core::vector3d<double> toIrrlichtVector3d();
				irr::core::vector3d<float> toIrrlichtVector3df();
#endif

#if HAVE_BULLET
				btVector3 toBulletVector3();
#endif

				virtual bool equals(shared_ptr<Type> other);

				double getLength();
				double getLengthSquared();
				shared_ptr<Vector3> normalize();

				shared_ptr<Vector3> add(double v);
				shared_ptr<Vector3> add(shared_ptr<Vector3> v);
				shared_ptr<Vector3> sub(double v);
				shared_ptr<Vector3> sub(shared_ptr<Vector3> v);
				shared_ptr<Vector3> mul(double v);
				shared_ptr<Vector3> mul(shared_ptr<Vector3> v);
				shared_ptr<Vector3> div(double v);
				shared_ptr<Vector3> div(shared_ptr<Vector3> v);
				shared_ptr<Vector3> neg();

				shared_ptr<Vector3> lerp(shared_ptr<Vector3> goal, double alpha);
				double dot(shared_ptr<Vector3> v);
				shared_ptr<Vector3> cross(shared_ptr<Vector3> v);
				bool isClose(shared_ptr<Vector3> v, double epsilon = 1e-6);

				virtual std::string toString();

				static int lua_getX(lua_State* L);
				static int lua_getY(lua_State* L);
				static int lua_getZ(lua_State* L);

				static int lua_getLength(lua_State* L);
				static int lua_getLengthSquared(lua_State* L);
				static int lua_getUnit(lua_State* L);

				static int lua_lerp(lua_State* L);
				static int lua_dot(lua_State* L);
				static int lua_cross(lua_State* L);
				static int lua_isClose(lua_State* L);

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
				double z;

		};

		shared_ptr<Vector3> checkVector3(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);
	}
}

#endif // OB_TYPE_VECTOR3


// Local Variables:
// mode: c++
// End:
