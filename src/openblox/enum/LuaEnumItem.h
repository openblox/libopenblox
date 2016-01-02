/*
 * Copyright 2016 John M. Harris, Jr.
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

#ifndef OPENBLOX_ENUM_LUAENUMITEM_H_
#define OPENBLOX_ENUM_LUAENUMITEM_H_

#include <OpenBlox.h>

#define lua_enumitem_name "luaL_EnumItem"

namespace OpenBlox{ namespace Enum{
	/**
	 * Lua representation of enumeration values.
	 * @author John M. Harris, Jr.
	 * @author DigiTechs
	 */
	class LuaEnumItem{
		public:
			LuaEnumItem(QString type, QString name, int value);
			virtual ~LuaEnumItem();

			QString type;
			QString name;
			int value;

			template<class T> T toEnum();

			int wrap_lua(lua_State* L);

			static int lua_index(lua_State* L);
			static int lua_toString(lua_State* L);

			DECLARE_GAME_STATIC_INIT(LuaEnumItem);
	};

	class LuaEnum;

	LuaEnumItem* checkEnumItem(lua_State* L, int idx, LuaEnum* enum_type);
	LuaEnumItem* checkAnyEnumItem(lua_State* L, int idx);
}}

#endif
