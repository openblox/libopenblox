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

#ifndef OPENBLOX_ENUM_ENUMS_H_
#define OPENBLOX_ENUM_ENUMS_H_

#include "LuaEnum.h"

namespace OpenBlox{ namespace Enum{
	#define LENUM(name, ...) \
	extern LuaEnum* Lua##name; \
	enum class name{__VA_ARGS__, __COUNT}

	#define DENUM(name, ...) \
		LuaEnum* Lua##name = new LuaEnum(#name, \
			__VA_ARGS__, \
			NULL \
	); \

	void registerLuaEnums(lua_State* L);

	//Start Enum definitions

	LENUM(HttpContentType,
		ApplicationJson,
		ApplicationXml,
		ApplicationUrlEncoded,
		TextPlain,
		TextXml
	);

	LENUM(OrientationMode,
		Degree_180,
		Portrait,
		LandscapeRight,
		LandscapeLeft
	);
}}

#endif
