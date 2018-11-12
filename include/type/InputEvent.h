/*
 * Copyright (C) 2018 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "type/Type.h"

#include "type/Enum.h"

#ifndef OB_TYPE_INPUTEVENT
#define OB_TYPE_INPUTEVENT

namespace OB{
	namespace Type{
		#ifndef OB_TYPE_VECTOR2
		class Vector2;
		#endif

		class InputMouseButtonEvent: public Type{
			public:
			    InputMouseButtonEvent();
				virtual ~InputMouseButtonEvent();

				virtual bool equals(shared_ptr<Type> other);

				virtual std::string toString();

				Enum::MouseButton getButton();
				void setButton(Enum::MouseButton btn);

				bool getState();
				void setState(bool state);

				static int lua_getButton(lua_State* L);
				static int lua_getState(lua_State* L);

				static int lua_eq(lua_State* L);

				static void register_lua_metamethods(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				static void register_lua_property_getters(lua_State* L);

				DECLARE_TYPE();

				Enum::MouseButton Button;
				bool State;
		};

		shared_ptr<InputMouseButtonEvent> checkInputMouseButtonEvent(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);

		class InputMouseWheelEvent: public Type{
			public:
			    InputMouseWheelEvent();
				virtual ~InputMouseWheelEvent();

				virtual bool equals(shared_ptr<Type> other);

				virtual std::string toString();

			    shared_ptr<Vector2> getDelta();
				void setDelta(shared_ptr<Vector2> delta);

				static int lua_getDelta(lua_State* L);

				static int lua_eq(lua_State* L);

				static void register_lua_metamethods(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				static void register_lua_property_getters(lua_State* L);

				DECLARE_TYPE();

			    shared_ptr<Vector2> Delta;
		};

		shared_ptr<InputMouseWheelEvent> checkInputMouseWheelEvent(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);

		class InputMouseMovementEvent: public Type{
			public:
			    InputMouseMovementEvent();
				virtual ~InputMouseMovementEvent();

				virtual bool equals(shared_ptr<Type> other);

				virtual std::string toString();

				shared_ptr<Vector2> getPosition();
				void setPosition(shared_ptr<Vector2> position);

			    shared_ptr<Vector2> getDelta();
				void setDelta(shared_ptr<Vector2> delta);

				static int lua_getPosition(lua_State* L);
				static int lua_getDelta(lua_State* L);

				static int lua_eq(lua_State* L);

				static void register_lua_metamethods(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				static void register_lua_property_getters(lua_State* L);

				DECLARE_TYPE();

				shared_ptr<Vector2> Position;
			    shared_ptr<Vector2> Delta;
		};

		shared_ptr<InputMouseMovementEvent> checkInputMouseMovementEvent(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);

		class InputKeyEvent: public Type{
			public:
			    InputKeyEvent();
				virtual ~InputKeyEvent();

				virtual bool equals(shared_ptr<Type> other);

				virtual std::string toString();

				Enum::KeyCode getKeyCode();
				void setKeyCode(Enum::KeyCode keyCode);

			    bool getState();
			    void setState(bool state);

				static int lua_getKeyCode(lua_State* L);
				static int lua_getState(lua_State* L);

				static int lua_eq(lua_State* L);

				static void register_lua_metamethods(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				static void register_lua_property_getters(lua_State* L);

				DECLARE_TYPE();

				Enum::KeyCode KeyCode;
			    bool State;
		};

		shared_ptr<InputKeyEvent> checkInputKeyEvent(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);

		class InputEvent: public Type{
			public:
				InputEvent();
				virtual ~InputEvent();

				virtual bool equals(shared_ptr<Type> other);

				virtual std::string toString();

				Enum::UserInputType getEventType();
				void setEventType(Enum::UserInputType evtType);

				shared_ptr<InputMouseButtonEvent> getMouseButton();
				void setMouseButton(shared_ptr<InputMouseButtonEvent> mouseButton);

				shared_ptr<InputMouseWheelEvent> getMouseWheel();
				void setMouseWheel(shared_ptr<InputMouseWheelEvent> mouseWheel);

				shared_ptr<InputMouseMovementEvent> getMouseMovement();
				void setMouseMovement(shared_ptr<InputMouseMovementEvent> mouseMovement);

				shared_ptr<InputKeyEvent> getKey();
				void setKey(shared_ptr<InputKeyEvent> key);

				static int lua_getMouseButton(lua_State* L);
				static int lua_getMouseWheel(lua_State* L);
				static int lua_getMouseMovement(lua_State* L);
				static int lua_getKey(lua_State* L);

				static int lua_getEventType(lua_State* L);

				static int lua_eq(lua_State* L);

				static void register_lua_metamethods(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				static void register_lua_property_getters(lua_State* L);

				DECLARE_TYPE();

				Enum::UserInputType EventType;

				shared_ptr<InputMouseButtonEvent> MouseButton;
				shared_ptr<InputMouseWheelEvent> MouseWheel;
				shared_ptr<InputMouseMovementEvent> MouseMovement;
				shared_ptr<InputKeyEvent> Key;
		};

		shared_ptr<InputEvent> checkInputEvent(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);
	}
}

#endif // OB_TYPE_INPUTEVENT


// Local Variables:
// mode: c++
// End:
