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

#include "type/Color3.h"

#include "instance/Instance.h"

#include <sstream>

namespace OB{
	namespace Type{
		DEFINE_TYPE(Color3){
			registerLuaType(LuaTypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}
		
	    Color3::Color3(){
			r = 0;
			g = 0;
			b = 0;
		}

		Color3::Color3(double r, double g, double b){
			this->r = r;
			this->g = g;
			this->b = b;
		}

		Color3::~Color3(){}

		bool Color3::equals(shared_ptr<Color3> other){
			return other->r == r && other->g == g && other->b == b;
		}

	    std::string Color3::toString(){
		    return ((std::ostringstream&)(std::ostringstream() << std::dec << r)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << g)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << b)).str();
		}

		int Color3::lua_getR(lua_State* L){
			shared_ptr<Color3> LuaColor3 = checkColor3(L, 1);
			lua_pushnumber(L, LuaColor3->r);
			return 1;
		}

		int Color3::lua_getG(lua_State* L){
			shared_ptr<Color3> LuaColor3 = checkColor3(L, 1);
			lua_pushnumber(L, LuaColor3->g);
			return 1;
		}

		int Color3::lua_getB(lua_State* L){
			shared_ptr<Color3> LuaColor3 = checkColor3(L, 1);
			lua_pushnumber(L, LuaColor3->b);
			return 1;
		}

		int Color3::lua_eq(lua_State* L){
			shared_ptr<Color3> LuaColor3 = checkColor3(L, 1);
			if(lua_isuserdata(L, 2)){
				shared_ptr<Color3> OtherColor3 = checkColor3(L, 2);
				lua_pushboolean(L, LuaColor3->equals(OtherColor3));
			}else{
				lua_pushboolean(L, false);
			}
			return 1;
		}

		void Color3::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__gc", Type::lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void Color3::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"r", Instance::Instance::lua_readOnlyProperty},
				{"R", Instance::Instance::lua_readOnlyProperty},
				{"g", Instance::Instance::lua_readOnlyProperty},
				{"G", Instance::Instance::lua_readOnlyProperty},
				{"b", Instance::Instance::lua_readOnlyProperty},
				{"B", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Color3::register_lua_property_getters(lua_State* L){
		    luaL_Reg properties[] = {
				{"r", lua_getR},
				{"R", lua_getR},
				{"g", lua_getG},
				{"G", lua_getG},
				{"b", lua_getB},
				{"B", lua_getB},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

	    shared_ptr<Color3> checkColor3(lua_State* L, int index){
			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_Color3");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<Color3>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
				return NULL;
			}
			return NULL;
		}
	}
}
