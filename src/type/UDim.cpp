/*
 * Copyright (C) 2017 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "type/UDim.h"

#include "instance/Instance.h"
#include "utility.h"

#include <sstream>

#include <math.h>

namespace OB{
	namespace Type{
		DEFINE_TYPE(UDim){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}
		
	    UDim::UDim(){
			scale = 0;
			offset = 0;
		}

	    UDim::UDim(double scale, double offset){
			this->scale = scale;
			this->offset = offset;
		}

	    UDim::UDim(std::string str){
			std::vector<std::string> fields = split(str);
			if(fields.size() == 2){
				std::string scaleStr = trim(fields[0]);
				std::string offsetStr = trim(fields[1]);

				if(scaleStr.length() > 0){
					scale = atof(scaleStr.c_str());
				}else{
					scale = 0;
				}
				
				if(offsetStr.length() > 0){
					offset = atof(offsetStr.c_str());
				}else{
					offset = 0;
				}
			}else{
				scale = 0;
				offset = 0;
			}
		}

	    UDim::~UDim(){}

		double UDim::getScale(){
			return scale;
		}
		
		double UDim::getOffset(){
			return offset;
		}

		bool UDim::equals(shared_ptr<Type> other){
		    shared_ptr<UDim> co = dynamic_pointer_cast<UDim>(other);
			if(!co){
				return false;
			}
			
			return co->scale == scale && co->offset == offset;
		}

	    std::string UDim::toString(){
		    return ((std::ostringstream&)(std::ostringstream() << std::dec << scale)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << offset)).str();
		}

		int UDim::lua_getScale(lua_State* L){
			shared_ptr<UDim> LuaUDim = checkUDim(L, 1, false);
			if(!LuaUDim){
				return 0;
			}
			
			lua_pushnumber(L, LuaUDim->scale);
			return 1;
		}

		int UDim::lua_getOffset(lua_State* L){
		    shared_ptr<UDim> LuaUDim = checkUDim(L, 1, false);
			if(!LuaUDim){
				return 0;
			}
			
			lua_pushnumber(L, LuaUDim->offset);
			return 1;
		}

		int UDim::lua_eq(lua_State* L){
			shared_ptr<UDim> LuaUDim = checkUDim(L, 1, false);
			
			if(LuaUDim){
			    shared_ptr<UDim> OtherUDim = checkUDim(L, 2, false);
			   	lua_pushboolean(L, LuaUDim->equals(OtherUDim));
			}
			
			lua_pushboolean(L, false);
			return 1;
		}

		void UDim::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__gc", Type::lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void UDim::register_lua_methods(lua_State* L){
			luaL_Reg methods[] = {
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}

		void UDim::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"Scale", Instance::Instance::lua_readOnlyProperty},
				{"Offset", Instance::Instance::lua_readOnlyProperty},
				{"scale", Instance::Instance::lua_readOnlyProperty},
				{"offset", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void UDim::register_lua_property_getters(lua_State* L){
		    luaL_Reg properties[] = {
				{"Scale", lua_getScale},
				{"Offset", lua_getOffset},
				{"scale", lua_getScale},
				{"offset", lua_getOffset},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

	    shared_ptr<UDim> checkUDim(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}
			
			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_UDim");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<UDim>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "UDim");
			}
			return NULL;
		}
	}
}
