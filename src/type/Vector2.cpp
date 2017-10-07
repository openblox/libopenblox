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

#include "type/Vector2.h"

#include "instance/Instance.h"
#include "utility.h"

#include <sstream>

#include <math.h>

namespace OB{
	namespace Type{
		DEFINE_TYPE(Vector2){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}

		Vector2::Vector2(){
			x = 0;
			y = 0;
		}

		Vector2::Vector2(double x, double y){
			this->x = x;
			this->y = y;
		}

		Vector2::Vector2(std::string str){
			std::vector<std::string> fields = split(str);
			if(fields.size() == 2){
				std::string xStr = trim(fields[0]);
				std::string yStr = trim(fields[1]);

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
			}else{
				x = 0;
				y = 0;
			}
		}

		Vector2::~Vector2(){}

		double Vector2::getX(){
			return x;
		}

		double Vector2::getY(){
			return y;
		}

#if HAVE_IRRLICHT

		irr::core::vector2d<double> Vector2::toIrrlichtVector2d(){
			return irr::core::vector2d<double>(x, y);
		}

		irr::core::vector2d<float> Vector2::toIrrlichtVector2df(){
			return irr::core::vector2d<float>(x, y);
		}

#endif

		bool Vector2::equals(shared_ptr<Type> other){
			shared_ptr<Vector2> co = dynamic_pointer_cast<Vector2>(other);
			if(!co){
				return false;
			}

			return co->x == x && co->y == y;
		}

		double Vector2::getLength(){
			return sqrt(getLengthSquared());
		}

		double Vector2::getLengthSquared(){
			return x*x + y*y;
		}

		shared_ptr<Vector2> Vector2::normalize(){
			double len = getLength();
			if(len == 0){//Prevents NaN in sqrt
				return make_shared<Vector2>(x, y);
			}
			len = 1.0 / sqrt(len);

			double X = (double)(x * len);
			double Y = (double)(y * len);

			return make_shared<Vector2>(X, Y);
		}

		shared_ptr<Vector2> Vector2::add(double v){
			return make_shared<Vector2>(x + v, y + v);
		}

		shared_ptr<Vector2> Vector2::add(shared_ptr<Vector2> v){
			if(!v){
				return make_shared<Vector2>(x, y);
			}
			return make_shared<Vector2>(x + v->x, y + v->y);
		}

		shared_ptr<Vector2> Vector2::sub(double v){
			return make_shared<Vector2>(x - v, y - v);
		}

		shared_ptr<Vector2> Vector2::sub(shared_ptr<Vector2> v){
			if(!v){
				return make_shared<Vector2>(x, y);
			}
			return make_shared<Vector2>(x - v->x, y - v->y);
		}

		shared_ptr<Vector2> Vector2::mul(double v){
			return make_shared<Vector2>(x * v, y * v);
		}

		shared_ptr<Vector2> Vector2::mul(shared_ptr<Vector2> v){
			if(!v){
				return make_shared<Vector2>(0, 0);
			}
			return make_shared<Vector2>(x * v->x, y * v->y);
		}

		shared_ptr<Vector2> Vector2::div(double v){
			if(v == 0){
				return NULL;
			}
			return make_shared<Vector2>(x / v, y / v);
		}

		shared_ptr<Vector2> Vector2::div(shared_ptr<Vector2> v){
			if(!v){
				return NULL;
			}
			if(v->x == 0 || v->y == 0){//Divide by 0
				return NULL;
			}
			return make_shared<Vector2>(x / v->x, y / v->y);
		}

		shared_ptr<Vector2> Vector2::neg(){
			return make_shared<Vector2>(-x, -y);
		}

		shared_ptr<Vector2> Vector2::lerp(shared_ptr<Vector2> goal, double alpha){
			if(goal == NULL){
				return NULL;
			}

			return make_shared<Vector2>((x + alpha) * (goal->x - x),
						    (y + alpha) * (goal->y - y));
		}

		double Vector2::dot(shared_ptr<Vector2> v){
			if(v == NULL){
				return 0;
			}
			return x * v->x + y * v->y;
		}

		std::string Vector2::toString(){
			return ((std::ostringstream&)(std::ostringstream() << std::dec << x)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << y)).str();
		}

		int Vector2::lua_getX(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return 0;
			}

			lua_pushnumber(L, LuaVector2->x);
			return 1;
		}

		int Vector2::lua_getY(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return 0;
			}

			lua_pushnumber(L, LuaVector2->y);
			return 1;
		}

		int Vector2::lua_getLength(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return 0;
			}

			lua_pushnumber(L, LuaVector2->getLength());
			return 1;
		}

		int Vector2::lua_getLengthSquared(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return 0;
			}

			lua_pushnumber(L, LuaVector2->getLengthSquared());
			return 1;
		}

		int Vector2::lua_getUnit(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return 0;
			}

			shared_ptr<Vector2> normalized = LuaVector2->normalize();
			if(normalized){
				return normalized->wrap_lua(L);
			}else{
				lua_pushnil(L);
			}

			return 1;
		}

		int Vector2::lua_lerp(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return luaL_error(L, COLONERR, "Lerp");
			}

			shared_ptr<Vector2> Vec2 = checkVector2(L, 2);
			double alpha = luaL_checknumber(L, 3);

			return LuaVector2->lerp(Vec2, alpha)->wrap_lua(L);
		}

		int Vector2::lua_dot(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return luaL_error(L, COLONERR, "Dot");
			}

			shared_ptr<Vector2> Vec2 = checkVector2(L, 2);

			lua_pushnumber(L, LuaVector2->dot(Vec2));
			return 1;
		}

		int Vector2::lua_eq(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);

			if(LuaVector2){
				shared_ptr<Vector2> OtherVector2 = checkVector2(L, 2, false);
			   	lua_pushboolean(L, LuaVector2->equals(OtherVector2));
			}

			lua_pushboolean(L, false);
			return 1;
		}

		int Vector2::lua_unm(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return 0;
			}

			return LuaVector2->neg()->wrap_lua(L);
		}

		int Vector2::lua_add(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return 0;
			}

			if(lua_isnumber(L, 2)){
				double numAdd = lua_tonumber(L, 2);
				return LuaVector2->add(numAdd)->wrap_lua(L);
			}else{
				shared_ptr<Vector2> OtherVec2 = checkVector2(L, 2, false);
				return LuaVector2->add(OtherVec2)->wrap_lua(L);
			}

			return 0;
		}

		int Vector2::lua_sub(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return 0;
			}

			if(lua_isnumber(L, 2)){
				double numSub = lua_tonumber(L, 2);
				return LuaVector2->sub(numSub)->wrap_lua(L);
			}else{
				shared_ptr<Vector2> OtherVec2 = checkVector2(L, 2, false);
				return LuaVector2->sub(OtherVec2)->wrap_lua(L);
			}

			return 0;
		}

		int Vector2::lua_mul(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return 0;
			}

			if(lua_isnumber(L, 2)){
				double numMul = lua_tonumber(L, 2);
				return LuaVector2->mul(numMul)->wrap_lua(L);
			}else{
				shared_ptr<Vector2> OtherVec2 = checkVector2(L, 2, false);
				return LuaVector2->mul(OtherVec2)->wrap_lua(L);
			}

			return 0;
		}

		int Vector2::lua_div(lua_State* L){
			shared_ptr<Vector2> LuaVector2 = checkVector2(L, 1, false);
			if(!LuaVector2){
				return 0;
			}

			if(lua_isnumber(L, 2)){
				double numDiv = lua_tonumber(L, 2);
				return LuaVector2->div(numDiv)->wrap_lua(L);
			}else{
				shared_ptr<Vector2> OtherVec2 = checkVector2(L, 2, false);
				return LuaVector2->div(OtherVec2)->wrap_lua(L);
			}

			return 0;
		}

		void Vector2::register_lua_metamethods(lua_State* L){
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

		void Vector2::register_lua_methods(lua_State* L){
			luaL_Reg methods[] = {
				{"Lerp", lua_lerp},
				{"Dot", lua_dot},
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}

		void Vector2::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"X", Instance::Instance::lua_readOnlyProperty},
				{"Y", Instance::Instance::lua_readOnlyProperty},
				{"length", Instance::Instance::lua_readOnlyProperty},
				{"magnitude", Instance::Instance::lua_readOnlyProperty},
				{"lengthSquared", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Vector2::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[] = {
				{"X", lua_getX},
				{"Y", lua_getY},
				{"length", lua_getLength},
				{"magnitude", lua_getLength},
				{"lengthSquared", lua_getLengthSquared},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		shared_ptr<Vector2> checkVector2(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}

			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_Vector2");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<Vector2>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "Vector2");
			}
			return NULL;
		}
	}
}
