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

#include "type/Vector3.h"

#include "instance/Instance.h"
#include "utility.h"

#include <sstream>

#include <math.h>

namespace OB{
	namespace Type{
		DEFINE_TYPE(Vector3){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}
		
	    Vector3::Vector3(){
			x = 0;
			y = 0;
			z = 0;
		}

	    Vector3::Vector3(double x, double y, double z){
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector3::Vector3(std::string str){
		    std::vector<std::string> fields = split(str);
			if(fields.size() == 3){
				std::string xStr = trim(fields[0]);
				std::string yStr = trim(fields[1]);
				std::string zStr = trim(fields[2]);

				if(xStr.length() > 0){
					x = atof(xStr.c_str());
				}else{
					x = 0;
				}
				
				if(yStr.length() > 0){
					y = atof(yStr.c_str());
				}else{
					y = 0;
				}
				
				if(zStr.length() > 0){
					z = atof(zStr.c_str());
				}else{
					z = 0;
				}
			}else{
				x = 0;
				y = 0;
				z = 0;
			}
		}

	    Vector3::~Vector3(){}

		double Vector3::getX(){
			return x;
		}
		
		double Vector3::getY(){
			return y;
		}
		
		double Vector3::getZ(){
			return z;
		}

		#if HAVE_IRRLICHT
		
		irr::core::vector3d<double> Vector3::toIrrlichtVector3d(){
			return irr::core::vector3d<double>(x, y, z);
		}

		irr::core::vector3d<float> Vector3::toIrrlichtVector3df(){
			return irr::core::vector3d<float>(x, y, z);
		}
		
		#endif

		#if HAVE_BULLET
		btVector3 Vector3::toBulletVector3(){
			return btVector3(x, y, z);
		}
		#endif

		bool Vector3::equals(shared_ptr<Type> other){
			shared_ptr<Vector3> co = dynamic_pointer_cast<Vector3>(other);
			if(!co){
				return false;
			}
			
			return co->x == x && co->y == y && co->z == z;
		}

		double Vector3::getLength(){
			return sqrt(getLengthSquared());
		}
		
		double Vector3::getLengthSquared(){
			return x*x + y*y + z*z;
		}

	    shared_ptr<Vector3> Vector3::normalize(){
		    double len = getLength();
			if(len == 0){//Prevents NaN in sqrt
				return make_shared<Vector3>(x, y, z);
			}
		    len = 1.0 / sqrt(len);

			double X = (double)(x * len);
			double Y = (double)(y * len);
			double Z = (double)(z * len);
			
		    return make_shared<Vector3>(X, Y, Z);
		}

		shared_ptr<Vector3> Vector3::add(double v){
			return make_shared<Vector3>(x + v, y + v, z + v);
		}
		
		shared_ptr<Vector3> Vector3::add(shared_ptr<Vector3> v){
			if(!v){
				return make_shared<Vector3>(x, y, z);
			}
			return make_shared<Vector3>(x + v->x, y + v->y, z + v->z);
		}
		
		shared_ptr<Vector3> Vector3::sub(double v){
			return make_shared<Vector3>(x - v, y - v, z - v);
		}
		
		shared_ptr<Vector3> Vector3::sub(shared_ptr<Vector3> v){
			if(!v){
				return make_shared<Vector3>(x, y, z);
			}
			return make_shared<Vector3>(x - v->x, y - v->y, z - v->z);
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
