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

#include "type/Type.h"

#include <map>

#ifndef OB_TYPE_LUAENUM
#define OB_TYPE_LUAENUM

namespace OB{
	namespace Type{
		class LuaEnumItem;
		
		class LuaEnum: public Type{
			public:
			    LuaEnum(std::string type, va_list args);
				virtual ~LuaEnum();

				static shared_ptr<LuaEnum> createLuaEnum(std::string type, ...);

				virtual std::string toString();

				std::string getType();
			    shared_ptr<LuaEnumItem> getEnumItem(int value);

				std::map<std::string, shared_ptr<LuaEnumItem>> enumValues;

				static std::map<std::string, shared_ptr<LuaEnum>>* enums;
				
				DECLARE_TYPE();

				static int lua_getEnumItems(lua_State* L);
				static int lua_index(lua_State* L);

				std::string type;
		};

		shared_ptr<LuaEnum> checkLuaEnum(lua_State* L, int n);
	}
}

#endif // OB_TYPE_LUAENUM


// Local Variables:
// mode: c++
// End:
