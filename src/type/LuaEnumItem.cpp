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

#include "type/LuaEnumItem.h"

#include "type/LuaEnum.h"

#include "OBEngine.h"

namespace OB{
	namespace Type{
		DEFINE_TYPE(LuaEnumItem){
		    typeList.push_back(LuaTypeName);
			
			lua_State* L = OB::OBEngine::getInstance()->getGlobalLuaState();

			luaL_newmetatable(L, LuaTypeName.c_str());
			luaL_Reg metamethods[] = {
				{"__tostring", lua_toString},
				{"__eq", lua_eq},
				{"__gc", lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);

			lua_pushstring(L, "__metatable");
			lua_pushstring(L, "This metatable is locked");
			lua_rawset(L, -3);

			//Item get
			lua_pushstring(L, "__index");
			lua_pushcfunction(L, lua_index);
			lua_rawset(L, -3);

			/*
			//Item set
			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, Instance::Instance::lua_readOnlyProperty);
			lua_rawset(L, -3);
			*/

			//Item get
			lua_pushstring(L, "__name");
			lua_pushstring(L, "EnumItem");
			lua_rawset(L, -3);

			lua_pop(L, 1);
		}
		
	    LuaEnumItem::LuaEnumItem(std::string type, std::string name, int value){
			this->type = type;
			this->name = name;
			this->value = value;
		}

	    LuaEnumItem::~LuaEnumItem(){}

		bool LuaEnumItem::equals(shared_ptr<Type> other){
			shared_ptr<LuaEnumItem> co = dynamic_pointer_cast<LuaEnumItem>(other);
			if(!co){
				return false;
			}

			return (co->getType() == type) && (co->getName() == name);
		}

		std::string LuaEnumItem::getType(){
			return type;
		}

		std::string LuaEnumItem::getName(){
			return name;
		}

		int LuaEnumItem::getValue(){
			return value;
		}

		template<class T> T LuaEnumItem::toEnum(){
			return (T)(this->value);
		}

	    std::string LuaEnumItem::toString(){
			return "Enum." + type + "." + name;
		}

		int LuaEnumItem::lua_index(lua_State* L){
		    shared_ptr<LuaEnumItem> con = checkAnyLuaEnumItem(L, 1, false);
			if(!con){
				return 0;
			}
			
			std::string propname = std::string(luaL_checkstring(L, 2));
			if(propname == "Name"){
				lua_pushstring(L, con->getName().c_str());
				return 1;
			}else if(propname == "Value"){
				lua_pushinteger(L, con->value);
				return 1;
			}
			
			return 0;
		}

	    shared_ptr<LuaEnumItem> checkAnyLuaEnumItem(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}
			
			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_LuaEnumItem");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<LuaEnumItem>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "EnumItem");
			}
			return NULL;
		}

		shared_ptr<LuaEnumItem> checkLuaEnumItem(lua_State* L, int index, shared_ptr<LuaEnum> enum_type){
			if(enum_type != NULL){
				if(lua_isstring(L, index)){
					std::string item_name = std::string(lua_tostring(L, index));
					for(std::map<std::string, shared_ptr<LuaEnumItem>>::iterator it = enum_type->enumValues.begin(); it != enum_type->enumValues.end(); ++it){
						if(it->second->getName() == item_name){
							return it->second;
						}
					}
					luaO_typeerror(L, index, "EnumItem");
					return NULL;
				}
				
				if(lua_isnumber(L, index)){
					return enum_type->getEnumItem(lua_tointeger(L, index));
				}
			}
			
			return checkAnyLuaEnumItem(L, index, false);
		}
	}
}
