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

#include "type/CFrame.h"
#include "type/Vector3.h"

#include "instance/Instance.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace OB{
	namespace Type{
		DEFINE_TYPE(CFrame){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}
		
		CFrame::CFrame(){
			setIdentity();
		}

		CFrame::CFrame(shared_ptr<Vector3> pos){
			setIdentity();
			if(pos){
				translate(pos->getX(), pos->getY(), pos->getZ());
			}
		}

		CFrame::CFrame(shared_ptr<Vector3> pos, shared_ptr<Vector3> at){
			lookAt(pos, at);
		}

		CFrame::CFrame(double x, double y, double z){
			setIdentity();
			
		    m[3][0] = x;
			m[3][1] = y;
			m[3][2] = z;
			fB = Translation;
		}

		CFrame::CFrame(double x, double y, double z,
					   double qX, double qY, double qZ, double qW){
			setIdentity();
			
		    m[3][0] = x;
			m[3][1] = y;
			m[3][2] = z;
			fB = Translation;
			
		    rotateQ(qX, qY, qZ, qW);
		}

		CFrame::CFrame(double x, double y, double z,
					   double R00, double R01, double R02,
					   double R10, double R11, double R12,
					   double R20, double R21, double R22){
			m[0][0] = R00;
			m[0][1] = R01;
			m[0][2] = R02;
			m[0][3] = x;
			m[1][0] = R10;
			m[1][1] = R11;
			m[1][2] = R12;
			m[1][3] = y;
			m[2][0] = R20;
			m[2][1] = R21;
			m[2][2] = R22;
			m[2][3] = z;
			m[3][0] = 0;
			m[3][1] = 0;
			m[3][2] = 0;
			m[3][3] = 1;
			
			fB = General;
		}

		CFrame::CFrame(int){
			// This is used in some operations such as rotate
			// We use this to keep from setting the identity
			fB = General;
		}

		CFrame::~CFrame(){}

		void CFrame::setIdentity(){
			m[0][0] = 1;
			m[0][1] = 0;
			m[0][2] = 0;
			m[0][3] = 0;
			m[1][0] = 0;
			m[1][1] = 1;
			m[1][2] = 0;
			m[1][3] = 0;
			m[2][0] = 0;
			m[2][1] = 0;
			m[2][2] = 1;
			m[2][3] = 0;
			m[3][0] = 0;
			m[3][1] = 0;
			m[3][2] = 0;
			m[3][3] = 1;

			fB = Identity;
		}

		void CFrame::translate(double x, double y, double z){
			if(fB == Identity){
				m[3][0] = x;
				m[3][1] = y;
				m[3][2] = z;
				fB = Translation;
			}else if(fB == Translation){
				m[3][0] += x;
				m[3][1] += y;
				m[3][2] += z;
			}else if(fB == Scale){
				m[3][0] = m[0][0] * x;
				m[3][1] = m[1][1] * y;
				m[3][2] = m[2][2] * z;
				fB |= Translation;
			}else if(fB == (Scale | Translation)){
				m[3][0] += m[0][0] * x;
				m[3][1] += m[1][1] * y;
				m[3][2] += m[2][2] * z;
			}else{
				m[3][0] += m[0][0] * x + m[1][0] * y + m[2][0] * z;
				m[3][1] += m[0][1] * x + m[1][1] * y + m[2][1] * z;
				m[3][2] += m[0][2] * x + m[1][2] * y + m[2][2] * z;
				m[3][3] += m[0][3] * x + m[1][3] * y + m[2][3] * z;
				if(fB == Rotation){
					fB |= Translation;
				}else if(fB != (Rotation | Translation)){
					fB = General;
				}
			}
		}

		double CFrame::getX(){
			return m[3][0];
		}
			
		double CFrame::getY(){
			return m[3][1];
		}
		
		double CFrame::getZ(){
			return m[3][2];
		}
		
		shared_ptr<Vector3> CFrame::getPosition(){
			return make_shared<Vector3>(m[3][0], m[3][1], m[3][2]);
		}

		bool _ob_cf_nearZero(double d){
			return abs(d) <= 0.000000000001;
		}

		void CFrame::rotate(double angle, double x, double y, double z){
			if(angle == 0){
				return;
			}
			shared_ptr<CFrame> mm = make_shared<CFrame>(0);
			double c, s, i;
			if(angle == 90 || angle == -270){
				s = 1;
				c = 0;
			}else if(angle == -90 || angle == 270){
				s = -1;
				c = 0;
			}else if(angle == 180 || angle == -180){
				s = 0;
				c = -1;
			}else{
				double a = angle * M_PI / 180;
				c = cos(a);
				s = sin(a);
			}
			bool q = false;
			if(x == 0){
				if(y == 0){
					if(z != 0){
						mm->setIdentity();
						mm->m[0][0] = c;
						mm->m[1][1] = c;
						if(z < 0){
							mm->m[1][0] = s;
							mm->m[0][1] = -s;
						}else{
							mm->m[1][0] = -s;
							mm->m[0][1] = s;
						}
						mm->fB = General;
						quick = true;
					}
				}else if(z == 0){
					mm->setIdentity();
					mm->m[0][0] = c;
					mm->m[2][2] = c;
					if(y < 0){
						mm->m[2][0] = -s;
						mm->m[0][2] = s;
					}else{
						mm->m[2][0] = s;
						mm->m[0][2] = -s;
					}
					mm->fB = General;
					quick = true;
				}
			}else if(y == 0 && z == 0){
				mm->setIdentity();
				mm->m[1][1] = c;
				mm->m[2][2] = c;
				if(x < 0){
					mm->m[2][1] = s;
					mm->m[1][2] = -s;
				}else{
					mm->m[2][1] = -s;
					mm->m[1][2] = s;
				}
				mm->fB = General;
				quick = true;
			}
			if(!quick){
				double len = x * x + y * y + z * z;
				if(!_ob_cf_nearZero(len - 1) && !_ob_cf_nearZero(len)){
					len = sqrt(len);
					x /= len;
					y /= len;
					z /= len;
				}
				i = 1 - c;
				mm->m[0][0] = x * x * i + c;
				mm->m[1][0] = x * y * i - z * s;
				mm->m[2][0] = x * z * i + y * s;
				mm->m[3][0] = 0;
				mm->m[0][1] = y * x * i + z * s;
				mm->m[1][1] = y * y * i + c;
				mm->m[2][1] = y * z * i - x * s;
				mm->m[3][1] = 0;
				mm->m[0][2] = x * z * i - y * s;
				mm->m[1][2] = y * z * i + x * s;
				mm->m[2][2] = z * z * i + c;
				mm->m[3][2] = 0;
				mm->m[0][3] = 0;
				mm->m[1][3] = 0;
				mm->m[2][3] = 0;
				mm->m[3][3] = 1;
			}
			
			int flag = fB;
			multiplyInternal(mm);
			if(flag != Identity){
				fB = flag | Rotation;
			}else{
				fB = Rotation;
			}
		}

		void CFrame::rotateQ(double qX, double qY, double qZ, double qW){
			shared_ptr<CFrame> mm = make_shared<CFrame>(0);

			double xx = qX * qX;
			double xy = qX * qY;
			double xz = qX * qZ;
			double xw = qX * qW;
			double yy = qY * qY;
			double yz = qY * qZ;
			double yw = qY * qW;
			double zz = qZ * qZ;
			double zw = qZ * qW;

			mm->m[0][0] = 1 - 2 * (yy + zz);
			mm->m[1][0] =     2 * (xy - zw);
			mm->m[2][0] =     2 * (xz + yw);
			mm->m[3][0] = 0;
			mm->m[0][1] =     2 * (xy + zw)
			mm->m[1][1] = 1 - 2 * (xx + zz);
			mm->m[2][1] =     2 * (yz - xw);
			mm->m[3][1] = 0;
			mm->m[0][2] =     2 * (xz - yw);
			mm->m[1][2] =     2 * (yz + xw);
			mm->m[2][2] = 1 - 2 * (xx + yy);
			mm->m[3][2] = 0;
			mm->m[0][3] = 0;
			mm->m[1][3] = 0;
			mm->m[2][3] = 0;
			mm->m[3][3] = 1;
			
			int flag = fB;
			multiplyInternal(mm);
			if(flag != Identity){
				fB = flag | Rotation;
			}else{
				fB = Rotation;
			}
		}

		void CFrame::multiplyInternal(shared_ptr<CFrame> other){
			if(!other){
				return;
			}
			if(other->fB == Identity){
				return;
			}
			if(fB == Identity){
				m[0][0] = other->m[0][0];
				m[1][0] = other->m[1][0];
				m[2][0] = other->m[2][0];
				m[3][0] = other->m[3][0];
				m[0][1] = other->m[0][1];
				m[1][1] = other->m[1][1];
				m[2][1] = other->m[2][1];
				m[3][1] = other->m[3][1];
				m[0][2] = other->m[0][2];
				m[1][2] = other->m[1][2];
				m[2][2] = other->m[2][2];
				m[3][2] = other->m[3][2];
				m[0][3] = other->m[0][3];
				m[1][3] = other->m[1][3];
				m[2][3] = other->m[2][3];
				m[3][3] = other->m[3][3];
			}else{
				double tm[4][4];
				tm[0][0] =
					m[0][0] * other->m[0][0] +
					m[1][0] * other->m[0][1] +
					m[2][0] * other->m[0][2] +
					m[3][0] * other->m[0][3];
				tm[0][1] =
					m[0][1] * other->m[0][0] +
					m[1][1] * other->m[0][1] +
					m[2][1] * other->m[0][2] +
					m[3][1] * other->m[0][3];
				tm[0][2] =
					m[0][2] * other->m[0][0] +
					m[1][2] * other->m[0][1] +
					m[2][2] * other->m[0][2] +
					m[3][2] * other->m[0][3];
				tm[0][3] =
					m[0][3] * other->m[0][0] +
					m[1][3] * other->m[0][1] +
					m[2][3] * other->m[0][2] +
					m[3][3] * other->m[0][3];
				tm[1][0] =
					m[0][0] * other->m[1][0] +
					m[1][0] * other->m[1][1] +
					m[2][0] * other->m[1][2] +
					m[3][0] * other->m[1][3];
				tm[1][1] =
					m[0][1] * other->m[1][0] +
					m[1][1] * other->m[1][1] +
					m[2][1] * other->m[1][2] +
					m[3][1] * other->m[1][3];
				tm[1][2] =
					m[0][2] * other->m[1][0] +
					m[1][2] * other->m[1][1] +
					m[2][2] * other->m[1][2] +
					m[3][2] * other->m[1][3];
				tm[1][3] =
					m[0][3] * other->m[1][0] +
					m[1][3] * other->m[1][1] +
					m[2][3] * other->m[1][2] +
					m[3][3] * other->m[1][3];
				tm[2][0] =
					m[0][0] * other->m[2][0] +
					m[1][0] * other->m[2][1] +
					m[2][0] * other->m[2][2] +
					m[3][0] * other->m[2][3];
				tm[2][1] =
					m[0][1] * other->m[2][0] +
					m[1][1] * other->m[2][1] +
					m[2][1] * other->m[2][2] +
					m[3][1] * other->m[2][3];
				tm[2][2] =
					m[0][2] * other->m[2][0] +
					m[1][2] * other->m[2][1] +
					m[2][2] * other->m[2][2] +
					m[3][2] * other->m[2][3];
				tm[2][3] =
					m[0][3] * other->m[2][0] +
					m[1][3] * other->m[2][1] +
					m[2][3] * other->m[2][2] +
					m[3][3] * other->m[2][3];
				tm[3][0] =
					m[0][0] * other->m[3][0] +
					m[1][0] * other->m[3][1] +
					m[2][0] * other->m[3][2] +
					m[3][0] * other->m[3][3];
				tm[3][1] =
					m[0][1] * other->m[3][0] +
					m[1][1] * other->m[3][1] +
					m[2][1] * other->m[3][2] +
					m[3][1] * other->m[3][3];
				tm[3][2] =
					m[0][2] * other->m[3][0] +
					m[1][2] * other->m[3][1] +
					m[2][2] * other->m[3][2] +
					m[3][2] * other->m[3][3];
				tm[3][3] =
					m[0][3] * other->m[3][0] +
					m[1][3] * other->m[3][1] +
					m[2][3] * other->m[3][2] +
					m[3][3] * other->m[3][3];

				m[0][0] = tm[0][0];
				m[1][0] = tm[1][0];
				m[2][0] = tm[2][0];
				m[3][0] = tm[3][0];
				m[0][1] = tm[0][1];
				m[1][1] = tm[1][1];
				m[2][1] = tm[2][1];
				m[3][1] = tm[3][1];
				m[0][2] = tm[0][2];
				m[1][2] = tm[1][2];
				m[2][2] = tm[2][2];
				m[3][2] = tm[3][2];
				m[0][3] = tm[0][3];
				m[1][3] = tm[1][3];
				m[2][3] = tm[2][3];
				m[3][3] = tm[3][3];
			}
		}

		void CFrame::lookAt(shared_ptr<Vector3> pos, shared_ptr<Vector3> at){
			shared_ptr<Vector3> fwd = (at->sub(pos))->normalize();
			shared_ptr<Vector3> side = fwd->cross(make_shared<Vector3>(0, 1, 0));
			shared_ptr<Vector3> up = side->cross(fwd);

			shared_ptr<CFrame> mm = make_shared<CFrame>(0);
			mm->m[0][0] = side->getX();
			mm->m[1][0] = side->getY();
			mm->m[2][0] = side->getZ();
			mm->m[3][0] = 0;
			mm->m[0][1] = up->getX();
			mm->m[1][1] = up->getY();
			mm->m[2][1] = up->getZ();
			mm->m[3][1] = 0;
			mm->m[0][2] = -fwd->getX();
			mm->m[1][2] = -fwd->getY();
			mm->m[2][2] = -fwd->getZ();
			mm->m[3][2] = 0;
			mm->m[0][3] = 0;
			mm->m[1][3] = 0;
			mm->m[2][3] = 0;
			mm->m[3][3] = 1;

			multiplyInternal(mm);

			translate(-pos->getX(), -pos->getY(), -pos->getZ());
		}

		#if HAVE_IRRLICHT

		/*
		irr::core::vector3d<double> Vector3::toIrrlichtVector3d(){
			return irr::core::vector3d<double>(x, y, z);
		}*/
		
		#endif

		#if HAVE_BULLET
		/*
		btVector3 Vector3::toBulletVector3(){
			return btVector3(x, y, z);
		}
		*/
		#endif

		bool CFrame::equals(shared_ptr<CFrame> other){
			shared_ptr<CFrame> co = dynamic_pointer_cast<CFrame>(other);
			if(!co){
				return false;
			}
			
			return
				(m[0][0] == other->m[0][0]) &&
				(m[0][1] == other->m[0][1]) &&
				(m[0][2] == other->m[0][2]) &&
				(m[0][3] == other->m[0][3]) &&
				(m[1][0] == other->m[1][0]) &&
				(m[1][1] == other->m[1][1]) &&
				(m[1][2] == other->m[1][2]) &&
				(m[1][3] == other->m[1][3]) &&
				(m[2][0] == other->m[2][0]) &&
				(m[2][1] == other->m[2][1]) &&
				(m[2][2] == other->m[2][2]) &&
				(m[2][3] == other->m[2][3]) &&
				(m[3][0] == other->m[3][0]) &&
				(m[3][1] == other->m[3][1]) &&
				(m[3][2] == other->m[3][2]) &&
				(m[3][3] == other->m[3][3]);
		}
		shared_ptr<CFrame> CFrame::add(shared_ptr<Vector3> v){
			if(!v){
				return NULL;
			}
			return ;
		}
	    
		shared_ptr<CFrame> CFrame::sub(shared_ptr<Vector3> v){
			if(!v){
			    return NULL;
			}
			return add(v->neg());
		}
		
		shared_ptr<Vector3> Vector3::mul(double v){
			return make_shared<Vector3>(x * v, y * v, z * v);
		}
		
		shared_ptr<Vector3> Vector3::mul(shared_ptr<Vector3> v){
			if(!v){
				return make_shared<Vector3>(0, 0, 0);
			}
			return make_shared<Vector3>(x * v->x, y * v->y, z * v->z);
		}
		
		shared_ptr<Vector3> Vector3::div(double v){
			if(v == 0){
				return NULL;
			}
			return make_shared<Vector3>(x / v, y / v, z / v);
		}
		
		shared_ptr<Vector3> Vector3::div(shared_ptr<Vector3> v){
			if(!v){
				return NULL;
			}
			if(v->x == 0 || v->y == 0 || v->z == 0){//Divide by 0
				return NULL;
			}
			return make_shared<Vector3>(x / v->x, y / v->y, z / v->z);
		}
		
		shared_ptr<Vector3> Vector3::neg(){
			return make_shared<Vector3>(-x, -y, -z);
		}

		shared_ptr<Vector3> Vector3::lerp(shared_ptr<Vector3> goal, double alpha){
			if(goal == NULL){
				return NULL;
			}

			return make_shared<Vector3>((x + alpha) * (goal->x - x),
										(y + alpha) * (goal->y - y),
										(z + alpha) * (goal->z - z));
		}

		double Vector3::dot(shared_ptr<Vector3> v){
			if(v == NULL){
				return 0;
			}
			return x * v->x + y * v->y + z * v->z;
		}

		shared_ptr<Vector3> Vector3::cross(shared_ptr<Vector3> v){
			if(v == NULL){
				return NULL;
			}
			return make_shared<Vector3>(y * v->z - z * v->y,
										z * v->x - x * v->z,
										x * v->y - y * v->x);
		}

		bool Vector3::isClose(shared_ptr<Vector3> v, double epsilon){
			if(v == NULL){
				return NULL;
			}
			return sub(v)->getLength() <= epsilon;
		}

		std::string Vector3::toString(){
			return ((std::ostringstream&)(std::ostringstream() << std::dec << x)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << y)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << z)).str();
		}

		int Vector3::lua_getX(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			lua_pushnumber(L, LuaVector3->x);
			return 1;
		}

		int Vector3::lua_getY(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			lua_pushnumber(L, LuaVector3->y);
			return 1;
		}

		int Vector3::lua_getZ(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			lua_pushnumber(L, LuaVector3->z);
			return 1;
		}

		int Vector3::lua_getLength(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			lua_pushnumber(L, LuaVector3->getLength());
			return 1;
		}

		int Vector3::lua_getLengthSquared(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			lua_pushnumber(L, LuaVector3->getLengthSquared());
			return 1;
		}

		int Vector3::lua_getUnit(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			shared_ptr<Vector3> normalized = LuaVector3->normalize();
			if(normalized){
				return normalized->wrap_lua(L);
			}else{
				lua_pushnil(L);
			}
			
			return 1;
		}

		int Vector3::lua_lerp(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return luaL_error(L, COLONERR, "Lerp");
			}
			
			shared_ptr<Vector3> Vec3 = checkVector3(L, 2);
			double alpha = luaL_checknumber(L, 3);
			
			return LuaVector3->lerp(Vec3, alpha)->wrap_lua(L);
		}

		int Vector3::lua_dot(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return luaL_error(L, COLONERR, "Dot");
			}
			
			shared_ptr<Vector3> Vec3 = checkVector3(L, 2);
			
			lua_pushnumber(L, LuaVector3->dot(Vec3));
			return 1;
		}

		int Vector3::lua_cross(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return luaL_error(L, COLONERR, "Cross");
			}
			
			shared_ptr<Vector3> Vec3 = checkVector3(L, 2);
			
			return LuaVector3->cross(Vec3)->wrap_lua(L);
		}

		int Vector3::lua_isClose(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return luaL_error(L, COLONERR, "IsClose");
			}
			
			shared_ptr<Vector3> Vec3 = checkVector3(L, 2);
			double epsilon = luaL_checknumber(L, 3);
			
			lua_pushboolean(L, LuaVector3->isClose(Vec3, epsilon));
			return 1;
		}

		int Vector3::lua_eq(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			
			if(LuaVector3){
				shared_ptr<Vector3> OtherVector3 = checkVector3(L, 2, false);
				lua_pushboolean(L, LuaVector3->equals(OtherVector3));
			}
			
			lua_pushboolean(L, false);
			return 1;
		}

		int Vector3::lua_unm(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			return LuaVector3->neg()->wrap_lua(L);
		}

		int Vector3::lua_add(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			if(lua_isnumber(L, 2)){
				double numAdd = lua_tonumber(L, 2);
				return LuaVector3->add(numAdd)->wrap_lua(L);
			}else{
				shared_ptr<Vector3> OtherVec3 = checkVector3(L, 2, false);
				return LuaVector3->add(OtherVec3)->wrap_lua(L);
			}
			
			return 0;
		}

		int Vector3::lua_sub(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			if(lua_isnumber(L, 2)){
				double numSub = lua_tonumber(L, 2);
				return LuaVector3->sub(numSub)->wrap_lua(L);
			}else{
				shared_ptr<Vector3> OtherVec3 = checkVector3(L, 2, false);
				return LuaVector3->sub(OtherVec3)->wrap_lua(L);
			}
			
			return 0;
		}

		int Vector3::lua_mul(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			if(lua_isnumber(L, 2)){
				double numMul = lua_tonumber(L, 2);
				return LuaVector3->mul(numMul)->wrap_lua(L);
			}else{
				shared_ptr<Vector3> OtherVec3 = checkVector3(L, 2, false);
				return LuaVector3->mul(OtherVec3)->wrap_lua(L);
			}
			
			return 0;
		}

		int Vector3::lua_div(lua_State* L){
			shared_ptr<Vector3> LuaVector3 = checkVector3(L, 1, false);
			if(!LuaVector3){
				return 0;
			}
			
			if(lua_isnumber(L, 2)){
				double numDiv = lua_tonumber(L, 2);
				return LuaVector3->div(numDiv)->wrap_lua(L);
			}else{
				shared_ptr<Vector3> OtherVec3 = checkVector3(L, 2, false);
				return LuaVector3->div(OtherVec3)->wrap_lua(L);
			}
			
			return 0;
		}

		void Vector3::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__unm", lua_unm},
				{"__add", lua_add},
				{"__sub", lua_sub},
				{"__mul", lua_mul},
				{"__div", lua_div},
				{"__gc", Type::lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void Vector3::register_lua_methods(lua_State* L){
			luaL_Reg methods[] = {
				{"Lerp", lua_lerp},
				{"Dot", lua_dot},
				{"Cross", lua_cross},
				{"IsClose", lua_isClose},
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}

		void Vector3::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"X", Instance::Instance::lua_readOnlyProperty},
				{"Y", Instance::Instance::lua_readOnlyProperty},
				{"Z", Instance::Instance::lua_readOnlyProperty},
				{"length", Instance::Instance::lua_readOnlyProperty},
				{"magnitude", Instance::Instance::lua_readOnlyProperty},
				{"lengthSquared", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Vector3::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[] = {
				{"X", lua_getX},
				{"Y", lua_getY},
				{"Z", lua_getZ},
				{"length", lua_getLength},
				{"magnitude", lua_getLength},
				{"lengthSquared", lua_getLengthSquared},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		shared_ptr<Vector3> checkVector3(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}
			
			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_Vector3");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<Vector3>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "Vector3");
			}
			return NULL;
		}
	}
}
