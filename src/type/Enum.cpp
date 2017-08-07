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

#include "type/Enum.h"

namespace OB{
	namespace Enum{
		DENUM(MessageType,
			"MessageOutput",
			"MessageInfo",
			"MessageWarning",
			"MessageError"
		);

		DENUM(UserInputType,
			  "MouseButton",
			  "MouseWheel",
			  "MouseMovement",
			  "Touch",
			  "Keyboard",
			  "Focus",
			  "Gamepad",
			  "TextInput",
			  "Unknown");

		void registerLuaEnums(lua_State* L){
			lua_newtable(L);

			for(std::map<std::string, shared_ptr<Type::LuaEnum>>::iterator it = Type::LuaEnum::enums->begin(); it != Type::LuaEnum::enums->end(); ++it){
				shared_ptr<Type::LuaEnum> en = it->second;

				lua_pushstring(L, en->getType().c_str());
				en->wrap_lua(L);
				lua_rawset(L, -3);
			}

			lua_setglobal(L, "Enum");
		}
	}
}
