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

#include "type/StatusEffect.h"

#include "instance/Instance.h"

namespace OB{
	namespace Type{
		DEFINE_TYPE(StatusEffect){
			registerLuaType(LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}
		
	    StatusEffect::StatusEffect(std::string name){
		    this->name = name;
		}

		StatusEffect::~StatusEffect(){}

		bool StatusEffect::equals(shared_ptr<StatusEffect> other){
			if(other == NULL){
				return false;
			}
			
			return other->name == name;
		}

	    std::string StatusEffect::toString(){
			return "StatusEffect: " + name;
		}

		int StatusEffect::lua_getName(lua_State* L){
			shared_ptr<StatusEffect> LuaStatusEffect = checkStatusEffect(L, 1, false, false);
			if(!LuaStatusEffect){
				return 0;
			}
			
			lua_pushstring(L, LuaStatusEffect->name.c_str());
			return 1;
		}

		int StatusEffect::lua_eq(lua_State* L){
			shared_ptr<StatusEffect> LuaStatusEffect = checkStatusEffect(L, 1, false, false);
			if(LuaStatusEffect){
				if(lua_isuserdata(L, 2)){
					shared_ptr<StatusEffect> OtherStatusEffect = checkStatusEffect(L, 2, false);
					lua_pushboolean(L, LuaStatusEffect->equals(OtherStatusEffect));
					return 1;
				}
			}
			
			lua_pushboolean(L, false);
			return 1;
		}

		void StatusEffect::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__gc", Type::lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void StatusEffect::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"Name", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void StatusEffect::register_lua_property_getters(lua_State* L){
		    luaL_Reg properties[] = {
				{"Name", lua_getName},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

	    shared_ptr<StatusEffect> checkStatusEffect(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}
			
			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_StatusEffect");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<StatusEffect>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}
			
			if(errIfNot){
				luaO_typeerror(L, index, "StatusEffect");
			}
			return NULL;
		}
	}
}
