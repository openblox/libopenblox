/*
 * Copyright 2015 John M. Harris, Jr.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenBlox.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LuaEnumItem.h"

#include "LuaEnum.h"

namespace OpenBlox{ namespace Enum{
	STATIC_GAME_INIT(LuaEnumItem){
		lua_State* L = OpenBlox::OBEngine::getInstance()->getLuaState();

		luaL_newmetatable(L, lua_enumitem_name);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, OB_TRANSLATE("LuaType", "This metatable is locked"));
		lua_rawset(L, -3);

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, LuaEnumItem::lua_index);
		lua_rawset(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, LuaEnumItem::lua_toString);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	LuaEnumItem::LuaEnumItem(QString type, QString name, int value){
		this->type = type;
		this->name = name;
		this->value = value;
	}

	LuaEnumItem::~LuaEnumItem(){}

	/**
	 * Returns this LuaEnumItem cast to the enum class of T.
	 * @returns The enum value of type T.
	 * @author John M. Harris, Jr.
	 */
	template<class T> T LuaEnumItem::toEnum(){
		return (T)(this->value);
	}

	/**
	 * Wraps this LuaEnumItem for Lua.
	 * @param lua_State* L The Lua state to wrap this LuaEnumItem for.
	 * @returns int Number of returned values.
	 * @author DigiTechs
	 */
	int LuaEnumItem::wrap_lua(lua_State* L){
		LuaEnumItem** enm = (LuaEnumItem**)lua_newuserdata(L, sizeof(*this));
		*enm = this;

		luaL_getmetatable(L, lua_enumitem_name);
		lua_setmetatable(L, -2);

		return 1;
	}

	/**
	 * Returns properties of this type.
	 * @param lua_State* L The Lua state indexing this LuaEnumItem.
	 * @returns int Number of returned values.
	 * @author DigiTechs
	 * @author John M. Harris, Jr.
	 */
	int LuaEnumItem::lua_index(lua_State* L){
		LuaEnumItem* itm = checkEnumItem(L, 1, NULL);
		QString propname = QString(luaL_checkstring(L, 2));

		if(propname == "Name"){
			lua_pushstring(L, itm->name.toStdString().c_str());
			return 1;
		}else if(propname == "Value"){
			lua_pushinteger(L, itm->value);
			return 1;
		}
		return luaL_error(L, "attempt to index '%s' (a nil value)", propname.toStdString().c_str());
	}

	/**
	 * Returns a string representation of this LuaEnumItem.
	 * @param lua_State* L The Lua state calling tostring.
	 * @returns int Number of returned values.
	 * @author DigiTechs
	 * @author John M. Harris, Jr.
	 */
	int LuaEnumItem::lua_toString(lua_State* L){
		LuaEnumItem* itm = checkEnumItem(L, 1, NULL);
		QString name = "Enum.";
		name += itm->type;
		name += ".";
		name += itm->name;
		lua_pushstring(L, name.toStdString().c_str());
		return 1;
	}

	/**
	 * Checks that the object at this index on the Lua stack is a LuaEnumItem, or can be cast to one.
	 * @param lua_State* L The Lua state.
	 * @param int n Index on the stack.
	 * @param LuaEnum* enum_type Type of LuaEnum to check for.
	 * @returns LuaEnumItem* LuaEnumItem from index on stack or errors on the Lua side.
	 * @author DigiTechs
	 * @author John M. Harris, Jr.
	 */
	LuaEnumItem* checkEnumItem(lua_State* L, int n, LuaEnum* enum_type){
		if(enum_type != NULL){
			if(lua_isstring(L, n)){
				QString item_name = QString(lua_tostring(L, n));
				for(std::map<QString, LuaEnumItem*>::iterator it = enum_type->enumValues.begin(); it != enum_type->enumValues.end(); ++it){
					if(it->second->name == item_name){
						return it->second;
					}
				}
				return NULL;
			}
			if(lua_isnumber(L, n)){
				return enum_type->getEnumItem(lua_tointeger(L, n));
			}
		}
		return *(LuaEnumItem**)luaL_checkudata(L, n, lua_enumitem_name);
	}

	/**
	 * Checks that the object at this index on the Lua stack is a LuaEnumItem, or can be cast to one.
	 * @param lua_State* L The Lua state.
	 * @param int n Index on the stack.
	 * @returns LuaEnumItem* LuaEnumItem from index on stack or errors on the Lua side.
	 * @author DigiTechs
	 * @author John M. Harris, Jr.
	 */
	LuaEnumItem* checkAnyEnumItem(lua_State* L, int n){
		return *(LuaEnumItem**)luaL_checkudata(L, n, lua_enumitem_name);
	}
}}
