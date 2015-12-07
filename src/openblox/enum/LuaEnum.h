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

#ifndef OPENBLOX_ENUM_LUAENUM_H_
#define OPENBLOX_ENUM_LUAENUM_H_

#include "LuaEnumItem.h"

#define lua_enum_name "luaL_Enum"

namespace OpenBlox{ namespace Enum{
	/**
	 * Lua representation of enumeration classes.
	 * @author John M. Harris, Jr.
	 * @author DigiTechs
	 */
	class LuaEnum{
		public:
			LuaEnum(QString type, ...);
			virtual ~LuaEnum();

			std::map<QString, LuaEnumItem*> enumValues;
			QString type;
			LuaEnumItem* getEnumItem(int value);

			int wrap_lua(lua_State* L);

			static int lua_index(lua_State* L);
			static int lua_toString(lua_State* L);
			static int lua_getEnumItems(lua_State* L);

			static std::map<QString, LuaEnum*>* enums;

			DECLARE_GAME_STATIC_INIT(LuaEnum);
	};

	LuaEnum* checkEnum(lua_State* L, int idx);
}}

#endif
