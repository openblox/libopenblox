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

#ifndef OB_TYPE_LUAENUMITEM
#define OB_TYPE_LUAENUMITEM

namespace OB{
	namespace Type{
		class LuaEnumItem: public Type{
		public:
			LuaEnumItem(std::string type, std::string name, int value);
			virtual ~LuaEnumItem();

			virtual bool equals(shared_ptr<Type> other);

			virtual std::string toString();

			std::string getType();
			std::string getName();
			int getValue();

			template<class T> T toEnum();

			DECLARE_TYPE();

			static int lua_index(lua_State* L);

			std::string type;
			std::string name;
			int value;
		};

		class LuaEnum;

		shared_ptr<LuaEnumItem> checkLuaEnumItem(lua_State* L, int n, shared_ptr<LuaEnum> enum_type);
		shared_ptr<LuaEnumItem> checkAnyLuaEnumItem(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);
	}
}

#endif // OB_TYPE_LUAENUMITEM


// Local Variables:
// mode: c++
// End:
