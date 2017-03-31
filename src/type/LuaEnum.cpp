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

#include "type/LuaEnum.h"

#include "type/LuaEnumItem.h"

#include "OBEngine.h"

namespace OB{
	namespace Type{
		std::map<std::string, shared_ptr<LuaEnum>>* LuaEnum::enums = NULL;
		
		DEFINE_TYPE(LuaEnum){
		    typeList.push_back(LuaTypeName);
			
			lua_State* L = OB::OBEngine::getInstance()->getGlobalLuaState();

			luaL_newmetatable(L, LuaTypeName.c_str());
		    register_lua_metamethods(L);

			lua_pushstring(L, "__metatable");
			lua_pushstring(L, "This metatable is locked");
			lua_rawset(L, -3);

			//Item get
			lua_pushstring(L, "__index");
			lua_pushcfunction(L, lua_index);
			lua_rawset(L, -3);

			//Item set
			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, Instance::Instance::lua_readOnlyProperty);
			lua_rawset(L, -3);

			lua_pop(L, 1);
		}
		
	    LuaEnum::LuaEnum(std::string type, va_list args){
			this->type = type;

			int i = 0;
			while(true){
				const char* arg = va_arg(args, const char*);
				if(arg == NULL){
					break;
				}
				enumValues.insert(std::pair<std::string, shared_ptr<LuaEnumItem>>(std::string(arg), make_shared<LuaEnumItem>(type, arg, i)));
				i++;
			}
			va_end(args);
		}

	    LuaEnum::~LuaEnum(){}

		shared_ptr<LuaEnum> LuaEnum::createLuaEnum(std::string type, ...){
			if(enums == NULL){
				enums = new std::map<std::string, shared_ptr<LuaEnum>>();
			}

			va_list args;
			va_start(args, type);

			shared_ptr<LuaEnum> sharedThing = make_shared<LuaEnum>(type, args);
			enums->insert(std::pair<std::string, shared_ptr<LuaEnum>>(type, sharedThing));
			return sharedThing;
		}

		std::string LuaEnum::getType(){
			return type;
		}

	    shared_ptr<LuaEnumItem> LuaEnum::getEnumItem(int value){
			for(std::map<std::string, shared_ptr<LuaEnumItem>>::iterator it = enumValues.begin(); it != enumValues.end(); ++it){
				if(it->second->getValue() == value){
					return it->second;
				}
			}
			return NULL;
		}

	    std::string LuaEnum::toString(){
			return "Enum." + type;
		}

		int LuaEnum::lua_getEnumItems(lua_State* L){
		    shared_ptr<LuaEnum> itm = checkLuaEnum(L, 1);
			if(itm != NULL){
				lua_newtable(L);

				int i = 1;

				for(std::map<std::string, shared_ptr<LuaEnumItem>>::iterator it = itm->enumValues.begin(); it != itm->enumValues.end(); ++it){
					it->second->wrap_lua(L);
					lua_rawseti(L, -2, i);
					i++;
				}

				return 1;
			}
			return 0;
		}

		int LuaEnum::lua_index(lua_State* L){
		    shared_ptr<LuaEnum> con = checkLuaEnum(L, 1);
			if(con != NULL){
				std::string propname = std::string(luaL_checkstring(L, 2));
				if(propname == "GetEnumItems"){
					lua_pushcfunction(L, lua_getEnumItems);
					return 1;
				}else{
				    shared_ptr<LuaEnumItem> enm = con->enumValues[propname];
					if(enm != NULL){
						return enm->wrap_lua(L);
					}
				}
			}
			return 0;
		}

	    shared_ptr<LuaEnum> checkLuaEnum(lua_State* L, int index){
			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_LuaEnum");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<LuaEnum>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
				return NULL;
			}
			return NULL;
		}
	}
}
