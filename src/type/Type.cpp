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

#include "OBException.h"
#include "OBEngine.h"

#include "type/Color3.h"
#include "type/Vector3.h"
#include "type/Vector2.h"
#include "type/UDim.h"
#include "type/UDim2.h"

#include "type/Event.h"
#include "type/EventConnection.h"
#include "type/LuaEnum.h"
#include "type/LuaEnumItem.h"

namespace OB{
	namespace Type{
		DEFINE_TYPE(Type){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);

			Color3::_ob_init(eng);
			Vector3::_ob_init(eng);
			Vector2::_ob_init(eng);
			UDim::_ob_init(eng);
			UDim2::_ob_init(eng);

			Event::_ob_init(eng);
			EventConnection::_ob_init(eng);
			LuaEnum::_ob_init(eng);
			LuaEnumItem::_ob_init(eng);
		}

		std::vector<std::string> Type::typeList;

		Type::Type(){}

		Type::~Type(){}

		bool Type::equals(shared_ptr<Type> other){
			return false;
		}

		void Type::registerLuaType(OBEngine* eng, std::string typeName, std::string className, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters){
			typeList.push_back(typeName);

			lua_State* L = eng->getGlobalLuaState();

			luaL_newmetatable(L, typeName.c_str());
			register_metamethods(L);

			lua_pushstring(L, "__metatable");
			lua_pushstring(L, "This metatable is locked");
			lua_rawset(L, -3);

			// Methods
			lua_pushstring(L, "__methods");
			lua_newtable(L);
			register_methods(L);
			lua_rawset(L, -3);

			// Property getters
			lua_pushstring(L, "__propertygetters");
			lua_newtable(L);
			register_getters(L);
			lua_rawset(L, -3);

			// Property setters
			lua_pushstring(L, "__propertysetters");
			lua_newtable(L);
			register_setters(L);
			lua_rawset(L, -3);

			// Item get
			lua_pushstring(L, "__index");
			lua_pushcfunction(L, lua_index);
			lua_rawset(L, -3);

			// Item set
			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, lua_newindex);
			lua_rawset(L, -3);

			// Set name
			lua_pushstring(L, "__name");
			lua_pushstring(L, className.c_str());
			lua_rawset(L, -3);

			lua_pop(L, 1);
		}

		std::string Type::toString(){
			return TypeName;
		}

		void Type::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__gc", lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void Type::register_lua_methods(lua_State* L){
			luaL_Reg methods[] = {
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}

		void Type::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Type::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[] = {
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		shared_ptr<Type> Type::checkType(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}

			if(lua_isuserdata(L, index)){
				unsigned size = typeList.size();
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					for(unsigned i = 0; i<size; i++){
						luaL_getmetatable(L, typeList[i].c_str());
						if(lua_rawequal(L, -1, -2)){
							lua_pop(L, 2);
							return *static_cast<shared_ptr<Type>*>(udata);
						}
						lua_pop(L, 1);
					}
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "Type");
			}
			return NULL;
		}

		int Type::lua_newindex(lua_State* L){
			shared_ptr<Type> t = checkType(L, 1, false);
			if(t){
				const char* name = luaL_checkstring(L, 2);
				lua_getmetatable(L, 1);//-3
				lua_getfield(L, -1, "__propertysetters");//-2
				lua_getfield(L, -1, name);//-1
				if (lua_iscfunction(L, -1)){
					lua_remove(L, -2);
					lua_remove(L, -2);

					lua_pushvalue(L, 1);
					lua_pushvalue(L, 3);
					lua_call(L, 2, 0);

					return 0;
				}else{
					lua_pop(L, 3);

					return luaL_error(L, "attempt to index '%s' (a nil value)", name);
				}
			}
			return 0;
		}

		int Type::lua_index(lua_State* L){
			shared_ptr<Type> t = checkType(L, 1, false);
			if(!t){
				return 0;
			}

			const char* name = luaL_checkstring(L, 2);

			lua_getmetatable(L, 1);//-3
			lua_getfield(L, -1, "__propertygetters");//-2
			lua_getfield(L, -1, name);//-1
			if(lua_iscfunction(L, -1)){
				lua_remove(L, -2);
				lua_remove(L, -2);

				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				return 1;
			}else{
				lua_pop(L, 2);
				// Check methods
				lua_getfield(L, -1, "__methods");//-2
				lua_getfield(L, -1, name);//-1
				if(lua_iscfunction(L, -1)){
					lua_remove(L, -2);
					lua_remove(L, -3);

					return 1;
				}else{
					return luaL_error(L, "attempt to index '%s' (a nil value)", name);
				}
			}
		}

		int Type::lua_eq(lua_State* L){
			shared_ptr<Type> t = checkType(L, 1, false);
			if(t){
				shared_ptr<Type> ot = checkType(L, 2, false);
				if(ot){
					lua_pushboolean(L, t == ot);
					return 1;
				}
			}

			lua_pushboolean(L, false);
			return 1;
		}

		int Type::lua_gc(lua_State* L){
			if(lua_isuserdata(L, 1)){
				unsigned size = typeList.size();
				void* udata = lua_touserdata(L, 1);
				int meta = lua_getmetatable(L, 1);
				if(meta != 0){
					for(unsigned i = 0; i<size; i++){
						luaL_getmetatable(L, typeList[i].c_str());
						if(lua_rawequal(L, -1, -2)){
							lua_pop(L, 2);

							(*static_cast<shared_ptr<Type>*>(udata)).reset();
						}
						lua_pop(L, 1);
					}
				}
			}

			return 0;
		}

		int Type::lua_toString(lua_State* L){
			shared_ptr<Type> t = checkType(L, 1, false);
			if(!t){
				return 0;
			}

			lua_pushstring(L, t->toString().c_str());
			return 1;
		}

		int Type::wrap_lua(lua_State* L){
			shared_ptr<Type>* udata = static_cast<shared_ptr<Type>*>(lua_newuserdata(L, sizeof(shared_ptr<Type>)));
			new(udata) shared_ptr<Type>(shared_from_this());

			luaL_getmetatable(L, getLuaClassName().c_str());
			lua_setmetatable(L, -2);
			return 1;
		}
	}
}
