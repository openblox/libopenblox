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
#include <irrlicht/matrix4.h>
#endif

#if HAVE_BULLET
#include <btBulletDynamicsCommon.h>
#endif

#ifndef OB_TYPE_CFRAME
#define OB_TYPE_CFRAME

namespace OB{
	namespace Type{
		class Vector3;

		/**
		 * A CFrame object is an abstraction of a 4×4 matrix that represents a position and a rotation.
		 *
		 * The position is represented by the
		 * \f$
		 * 	\begin{bmatrix}
		 * 		x & y & z
		 * 	\end{bmatrix}
		 * \f$
		 * row vector and the rotation is represented by the upper 3×3 submatrix of the CFrame:
		 * \f[
		 * 	\begin{bmatrix}
		 * 		R_{00} & R_{01} & R_{02} & 0 \\
		 * 		R_{10} & R_{11} & R_{12} & 0 \\
		 * 		R_{20} & R_{21} & R_{22} & 0 \\
		 * 		x & y & z & 1
		 * 	\end{bmatrix}.
		 * \f]
		 */
		class CFrame: public Type{
			public:
				/**
				 * Creates a blank identity CFrame
				 */
				CFrame();
				/**
				 * Creates a CFrame from a Vector3
				 */
				CFrame(shared_ptr<Vector3> pos);
				/**
				 * Creates a CFrame positioned at pos, looking at lookAt
				 */
				CFrame(shared_ptr<Vector3> pos, shared_ptr<Vector3> lookAt);
				/**
				 * Creates a CFrame from a position
				 */
				CFrame(double x, double y, double z);
				/**
				 * Creates a copy of a CFrame
				 */
				CFrame(shared_ptr<CFrame> cf);
				/**
				 * Creates a CFrame from a position (x, y, z) and
				 * quaternion (qX, qY, qZ, qW)
				 */
				CFrame(double x, double y, double z,
					   double qX, double qY, double qZ, double qW);

				CFrame(int);

				/*
				 * Creates a CFrame from a position (x, y, z) with
				 * orientation specified by the rotation matrix.
				 */
				CFrame(double x, double y, double z,
					   double R00, double R01, double R02,
					   double R10, double R11, double R12,
					   double R20, double R21, double R22);

#if HAVE_IRRLICHT
				CFrame(irr::core::matrix4 m4p);
#endif

				virtual ~CFrame();

				double getX();
				double getY();
				double getZ();
				shared_ptr<Vector3> getPosition();

#if HAVE_IRRLICHT
				irr::core::matrix4 toIrrlichtMatrix4();
#endif

#if HAVE_BULLET
				//btVector3 toBulletVector3();
#endif

				virtual bool equals(shared_ptr<Type> other);

				shared_ptr<CFrame> add(shared_ptr<Vector3> v);
				shared_ptr<CFrame> sub(shared_ptr<Vector3> v);
				// TODO: shared_ptr<CFrame> mul(shared_ptr<Vector3> v);
				shared_ptr<CFrame> mul(shared_ptr<CFrame> c);

				shared_ptr<CFrame> lerp(shared_ptr<CFrame> goal, double alpha);

				/**
				 * Returns the Tait-Bryan Euler angles for the CFrame's orientation in x-y-z order.
				 *
				 * @returns Vector of (x, y, z) angles in radians
				 * @author Mark Otaris
				 */
				shared_ptr<Vector3> toEulerAnglesXYZ();
				/**
				 * Returns the Tait-Bryan Euler angles for the CFrame's orientation in x-z-y order.
				 *
				 * @returns Vector of (x, z, y) angles in radians
				 * @author Mark Otaris
				 */
				shared_ptr<Vector3> toEulerAnglesXZY();
				/**
				 * Returns the Tait-Bryan Euler angles for the CFrame's orientation in y-x-z order.
				 *
				 * @returns Vector of (y, x, z) angles in radians
				 * @author Mark Otaris
				 */
				shared_ptr<Vector3> toEulerAnglesYXZ();
				/**
				 * Returns the Tait-Bryan Euler angles for the CFrame's orientation in y-z-x order.
				 *
				 * @returns Vector of (y, z, x) angles in radians
				 * @author Mark Otaris
				 */
				shared_ptr<Vector3> toEulerAnglesYZX();
				/**
				 * Returns the Tait-Bryan Euler angles for the CFrame's orientation in z-x-y order.
				 *
				 * @returns Vector of (z, x, y) angles in radians
				 * @author Mark Otaris
				 */
				shared_ptr<Vector3> toEulerAnglesZXY();
				/**
				 * Returns the Tait-Bryan Euler angles for the CFrame's orientation in z-y-x order.
				 *
				 * @returns Vector of (z, y, x) angles in radians
				 * @author Mark Otaris
				 */
				shared_ptr<Vector3> toEulerAnglesZYX();

				virtual std::string toString();

				static int lua_getPosition(lua_State* L);
				static int lua_getX(lua_State* L);
				static int lua_getY(lua_State* L);
				static int lua_getZ(lua_State* L);
				static int lua_getP(lua_State* L);

				static int lua_lerp(lua_State* L);

				/**
				 * Returns (on the %Lua stack) the Euler angles in x-y-z order, or nil.
				 *
				 * Throws a %Lua error if not invoked as a member.
				 *
				 * @param L The %Lua state
				 * @returns 1
				 * @sa toEulerAnglesXYZ
				 * @author Mark Otaris
				 */
				static int lua_toEulerAnglesXYZ(lua_State* L);
				/**
				 * Returns (on the %Lua stack) the Euler angles in x-z-y order, or nil.
				 *
				 * Throws a %Lua error if not invoked as a member.
				 *
				 * @param L The %Lua state
				 * @returns 1
				 * @sa toEulerAnglesXZY
				 * @author Mark Otaris
				 */
				static int lua_toEulerAnglesXZY(lua_State* L);
				/**
				 * Returns (on the %Lua stack) the Euler angles in y-x-z order, or nil.
				 *
				 * Throws a %Lua error if not invoked as a member.
				 *
				 * @param L The %Lua state
				 * @returns 1
				 * @sa toEulerAnglesYXZ
				 * @author Mark Otaris
				 */
				static int lua_toEulerAnglesYXZ(lua_State* L);
				/**
				 * Returns (on the %Lua stack) the Euler angles in y-z-x order, or nil.
				 *
				 * Throws a %Lua error if not invoked as a member.
				 *
				 * @param L The %Lua state
				 * @returns 1
				 * @sa toEulerAnglesYZX
				 * @author Mark Otaris
				 */
				static int lua_toEulerAnglesYZX(lua_State* L);
				/**
				 * Returns (on the %Lua stack) the Euler angles in z-x-y order, or nil.
				 *
				 * Throws a %Lua error if not invoked as a member.
				 *
				 * @param L The %Lua state
				 * @returns 1
				 * @sa toEulerAnglesZXY
				 * @author Mark Otaris
				 */
				static int lua_toEulerAnglesZXY(lua_State* L);
				/**
				 * Returns (on the %Lua stack) the Euler angles in z-y-x order, or nil.
				 *
				 * Throws a %Lua error if not invoked as a member.
				 *
				 * @param L The %Lua state
				 * @returns 1
				 * @sa toEulerAnglesZYX
				 * @author Mark Otaris
				 */
				static int lua_toEulerAnglesZYX(lua_State* L);

				static int lua_eq(lua_State* L);
				static int lua_add(lua_State* L);
				static int lua_sub(lua_State* L);
				static int lua_mul(lua_State* L);

				static void register_lua_metamethods(lua_State* L);
				static void register_lua_methods(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				static void register_lua_property_getters(lua_State* L);

				DECLARE_TYPE();

				void setIdentity();
				void multiplyInternal(shared_ptr<CFrame> other);
				void translate(double x, double y, double z);
				void rotate(double angle, double x, double y, double z);
				void rotateQ(double qX, double qY, double qZ, double qW);
				void lookAt(shared_ptr<Vector3> pos, shared_ptr<Vector3> at);

				double m[4][4];
				int fB;

				enum CFPerfT{
					Identity = 0x0001, // Identity matrix
					Unknown = 0x0002, // Unknown contents, make no assumptions
					Translation = 0x0003, // Simple translation
					Scale = 0x0004, // Simple scale
					Rotation = 0x0005 // Simple rotation
				};

		};

		shared_ptr<CFrame> checkCFrame(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);
	}
}

#endif // OB_TYPE_CFRAME


// Local Variables:
// mode: c++
// End:
