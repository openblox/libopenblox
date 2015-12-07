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

#include "Enums.h"

namespace OpenBlox{ namespace Enum{
	DENUM(HttpContentType,
		"ApplicationJson",
		"ApplicationXml",
		"ApplicationUrlEncoded",
		"TextPlain",
		"TextXml"
	);

	void registerLuaEnums(lua_State* L){
		lua_newtable(L);

		for(std::map<QString, LuaEnum*>::iterator it = LuaEnum::enums->begin(); it != LuaEnum::enums->end(); ++it){
			LuaEnum* en = it->second;

			lua_pushstring(L, en->type.toStdString().c_str());
			en->wrap_lua(L);
			lua_rawset(L, -3);
		}

		lua_setglobal(L, "Enum");
	}
}}
