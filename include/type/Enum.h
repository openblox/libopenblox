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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "type/LuaEnum.h"
#include "type/LuaEnumItem.h"

#ifndef OB_TYPE_ENUM
#define OB_TYPE_ENUM

namespace OB{
	namespace Enum{
#define LENUM(name, ...) \
		extern shared_ptr<Type::LuaEnum> Lua##name; \
		enum class name{__VA_ARGS__, __COUNT}

#define DENUM(name, ...) \
		shared_ptr<Type::LuaEnum> Lua##name = Type::LuaEnum::createLuaEnum(#name, \
																		__VA_ARGS__, \
																		NULL \
																		); \

		void registerLuaEnums(lua_State* L);

		// Start Enum Definitions

		LENUM(MessageType,
			  MessageOutput,
			  MessageInfo,
			  MessageWarning,
			  MessageError);

		LENUM(UserInputType,
			  MouseButton,
			  MouseWheel,
			  MouseMovement,
			  Touch,
			  Keyboard,
			  Focus,
			  Gamepad,
			  TextInput,
			  Unknown);

		LENUM(MouseButton,
			  Left,
			  Middle,
			  Right,
			  Unknown);

		LENUM(ScriptLanguage,
			  Lua,
			  JavaScript);
	}
}

#endif // OB_TYPE_ENUM


// Local Variables:
// mode: c++
// End:
