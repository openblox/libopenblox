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
#include "utility.h"

#include <sstream>

namespace OB{
	namespace Type{
		DEFINE_TYPE(Color3){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
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

		Color3::Color3(int r, int g, int b){
			double rr = ((double)r) / 255;
			double gg = ((double)g) / 255;
			double bb = ((double)b) / 255;

		    if(rr > 1){ rr = 1; }
			if(gg > 1){ gg = 1; }
			if(bb > 1){ bb = 1; }
			if(rr < 0){ rr = 0; }
			if(gg < 0){ gg = 0; }
			if(bb < 0){ bb = 0; }

			this->r = rr;
			this->g = gg;
			this->b = bb;
		}

	    Color3::Color3(std::string str){
			std::vector<std::string> fields = split(fields, str, ",");
			if(fields.size() == 3){
				std::string rStr = trim(fields[0]);
				std::string gStr = trim(fields[1]);
				std::string bStr = trim(fields[2]);

				if(rStr.length() > 0){
					r = atof(rStr.c_str());
				}else{
					r = 0;
				}
				
				if(gStr.length() > 0){
					g = atof(gStr.c_str());
				}else{
					g = 0;
				}
				
				if(bStr.length() > 0){
					b = atof(bStr.c_str());
				}else{
					b = 0;
				}
			}else{
				r = 0;
				g = 0;
				b = 0;
			}
		}

		Color3::~Color3(){}

		double Color3::getR(){
			return r;
		}
		
		double Color3::getG(){
			return g;
		}
		
		double Color3::getB(){
			return b;
		}

	    int Color3::getRi(){
			int rr = r * 255;
			if(rr > 255){ rr = 255; }
			if(rr < 0){ rr = 0; }
			return rr;
		}
		
	    int Color3::getGi(){
		    int gg = g * 255;
			if(gg > 255){ gg = 255; }
			if(gg < 0){ gg = 0; }
			return gg;
		}
		
	    int Color3::getBi(){
			int bb = b * 255;
			if(bb > 255){ bb = 255; }
			if(bb < 0){ bb = 0; }
			return bb;
		}

		#if HAVE_IRRLICHT
		
		irr::video::SColor Color3::toIrrlichtSColor(unsigned int alpha){
			return irr::video::SColor(alpha, getRi(), getGi(), getBi());
		}
		
		#endif

		bool Color3::equals(shared_ptr<Type> other){
			shared_ptr<Color3> co = dynamic_pointer_cast<Color3>(other);
			if(!co){
				return false;
			}
			
			return co->r == r && co->g == g && co->b == b;
		}

	    std::string Color3::toString(){
		    return std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b);
		}

		int Color3::lua_getR(lua_State* L){
			shared_ptr<Color3> LuaColor3 = checkColor3(L, 1, false, false);
			if(!LuaColor3){
				return 0;
			}
			
			lua_pushnumber(L, LuaColor3->r);
			return 1;
		}

		int Color3::lua_getG(lua_State* L){
			shared_ptr<Color3> LuaColor3 = checkColor3(L, 1, false, false);
			if(!LuaColor3){
				return 0;
			}
			
			lua_pushnumber(L, LuaColor3->g);
			return 1;
		}

		int Color3::lua_getB(lua_State* L){
			shared_ptr<Color3> LuaColor3 = checkColor3(L, 1, false, false);
			if(!LuaColor3){
				return 0;
			}
			
			lua_pushnumber(L, LuaColor3->b);
			return 1;
		}

		int Color3::lua_eq(lua_State* L){
			shared_ptr<Color3> LuaColor3 = checkColor3(L, 1, false, false);
			if(LuaColor3){
				if(lua_isuserdata(L, 2)){
					shared_ptr<Color3> OtherColor3 = checkColor3(L, 2, false);
					lua_pushboolean(L, LuaColor3->equals(OtherColor3));
					return 1;
				}
			}
			
			lua_pushboolean(L, false);
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
				{"R", Instance::Instance::lua_readOnlyProperty},
				{"G", Instance::Instance::lua_readOnlyProperty},
				{"B", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Color3::register_lua_property_getters(lua_State* L){
		    luaL_Reg properties[] = {
				{"R", lua_getR},
				{"G", lua_getG},
				{"B", lua_getB},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

	    shared_ptr<Color3> checkColor3(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}
			
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
			}
			
			if(errIfNot){
				luaO_typeerror(L, index, "Color3");
			}
			return NULL;
		}
	}
}
