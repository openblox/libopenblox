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

#include "type/Enum.h"

namespace OB{
	namespace Enum{
		DENUM(MessageType,
			  "MessageOutput",
			  "MessageInfo",
			  "MessageWarning",
			  "MessageError");

		DENUM(UserInputType,
			  "Unknown",
			  "MouseButton",
			  "MouseWheel",
			  "MouseMovement",
			  "Touch",
			  "Keyboard",
			  "Focus",
			  "Gamepad",
			  "TextInput");

		DENUM(MouseButton,
			  "Unknown",
			  "Left",
			  "Middle",
			  "Right",
			  "X1",
			  "X2");

		DENUM(KeyCode,
			  "Unknown",
			  "Backspace",
			  "Tab",
			  "Clear",
			  "Return",
			  "Return2",
			  "Pause",
			  "Escape",
			  "Space",
			  "DoubleQuote",
			  "Hash",
			  "Dollar",
			  "Percent",
			  "Ampersand",
			  "Quote",
			  "LeftParenthesis",
			  "RightParenthesis",
			  "Asterisk",
			  "Plus",
			  "Comma",
			  "Minus",
			  "Period",
			  "Slash",
			  "Zero",
			  "One",
			  "Two",
			  "Three",
			  "Four",
			  "Five",
			  "Six",
			  "Seven",
			  "Eight",
			  "Nine",
			  "Colon",
			  "Semicolon",
			  "LessThan",
			  "Equals",
			  "GreaterThan",
			  "Question",
			  "Exclamation",
			  "At",
			  "LeftBracket",
			  "Backslash",
			  "RightBracket",
			  "Caret",
			  "Underscore",
			  "Backquote",
			  "A",
			  "B",
			  "C",
			  "D",
			  "E",
			  "F",
			  "G",
			  "H",
			  "I",
			  "J",
			  "K",
			  "L",
			  "M",
			  "N",
			  "O",
			  "P",
			  "Q",
			  "R",
			  "S",
			  "T",
			  "U",
			  "V",
			  "W",
			  "X",
			  "Y",
			  "Z",
			  "Delete",
			  "NumpadZero",
			  "NumpadOne",
			  "NumpadTwo",
			  "NumpadThree",
			  "NumpadFour",
			  "NumpadFive",
			  "NumpadSix",
			  "NumpadSeven",
			  "NumpadEight",
			  "NumpadNine",
			  "NumpadPeriod",
			  "NumpadDivide",
			  "NumpadMultiply",
			  "NumpadMinus",
			  "NumpadPlus",
			  "NumpadEnter",
			  "NumpadEquals",
			  "Up",
			  "Down",
			  "Left",
			  "Right",
			  "Insert",
			  "Select",
			  "Home",
			  "End",
			  "PageUp",
			  "PageDown",
			  "LeftShift",
			  "RightShift",
			  "LeftMeta",
			  "RightMeta",
			  "LeftAlt",
			  "RightAlt",
			  "LeftControl",
			  "RightControl",
			  "CapsLock",
			  "NumLock",
			  "ScrollLock",
			  "LeftSuper",
			  "RightSuper",
			  "Mode",
			  "Compose",
			  "Help",
			  "Print",
			  "PrintSc",
			  "Execute",
			  "SysRq",
			  "Menu",
			  "Power",
			  "Euro",
			  "Undo",
			  "Redo",
			  "Cut",
			  "Copy",
			  "Paste",
			  "F1",
			  "F2",
			  "F3",
			  "F4",
			  "F5",
			  "F6",
			  "F7",
			  "F8",
			  "F9",
			  "F10",
			  "F11",
			  "F12",
			  "F13",
			  "F14",
			  "F15",
			  "F16",
			  "F17",
			  "F18",
			  "F19",
			  "F20",
			  "F21",
			  "F22",
			  "F23",
			  "F24",
			  "OEM1",
			  "OEM2",
			  "OEM3",
			  "OEM4",
			  "OEM5",
			  "OEM6",
			  "OEM7",
			  "OEM8",
			  "OEM9",
			  "OEM_AX",
			  "OEM102",
			  "WWW",
			  "MediaPrevious",
			  "MediaNext",
			  "MediaPlayPause",
			  "MediaStop",
			  "ButtonX",
			  "ButtonY",
			  "ButtonA",
			  "ButtonB",
			  "ButtonR1",
			  "ButtonL1",
			  "ButtonR2",
			  "ButtonL2",
			  "ButtonR3",
			  "ButtonL3",
			  "ButtonStart",
			  "ButtonSelect",
			  "DPadLeft",
			  "DPadRight",
			  "DPadUp",
			  "DPadDown",
			  "Thumbstick1",
			  "Thumbstick2");

		DENUM(ScriptLanguage,
			  "Lua",
			  "JavaScript");

		DENUM(BorderMode,
			  "Outline",
			  "Middle",
			  "Inset");

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
