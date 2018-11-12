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

#include "type/InputEvent.h"

#include "instance/Instance.h"

#include "type/Vector2.h"

namespace OB{
	namespace Type{
		DEFINE_TYPE(InputMouseButtonEvent){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}

		InputMouseButtonEvent::InputMouseButtonEvent(){
			Button = Enum::MouseButton::Unknown;
			State = false;
		}

		InputMouseButtonEvent::~InputMouseButtonEvent(){}

		bool InputMouseButtonEvent::equals(shared_ptr<Type> other){
			shared_ptr<InputMouseButtonEvent> imbe = dynamic_pointer_cast<InputMouseButtonEvent>(other);
			if(!imbe){
				return false;
			}

			// InputMouseButtonEvents can't be copied or replicated
			// Compare addresses
			return imbe.get() == this;
		}

		std::string InputMouseButtonEvent::toString(){
			return "Input Event";
		}

		Enum::MouseButton InputMouseButtonEvent::getButton(){
			return Button;
		}

		void InputMouseButtonEvent::setButton(Enum::MouseButton btn){
		    Button = btn;
		}

		bool InputMouseButtonEvent::getState(){
			return State;
		}

		void InputMouseButtonEvent::setState(bool state){
			State = state;
		}

		int InputMouseButtonEvent::lua_getButton(lua_State* L){
			shared_ptr<InputMouseButtonEvent> LuaInputMouseButtonEvent = checkInputMouseButtonEvent(L, 1, false);
			if(!LuaInputMouseButtonEvent){
				return 0;
			}

			return Enum::LuaMouseButton->getEnumItem((int)LuaInputMouseButtonEvent->getButton())->wrap_lua(L);
		}

		int InputMouseButtonEvent::lua_getState(lua_State* L){
			shared_ptr<InputMouseButtonEvent> LuaInputMouseButtonEvent = checkInputMouseButtonEvent(L, 1, false);
			if(!LuaInputMouseButtonEvent){
				return 0;
			}

			lua_pushboolean(L, LuaInputMouseButtonEvent->getState());
			return 1;
		}

		int InputMouseButtonEvent::lua_eq(lua_State* L){
			shared_ptr<InputMouseButtonEvent> LuaInputMouseButtonEvent = checkInputMouseButtonEvent(L, 1, false, false);
			if(LuaInputMouseButtonEvent){
				if(lua_isuserdata(L, 2)){
					shared_ptr<InputMouseButtonEvent> OtherInputMouseButtonEvent = checkInputMouseButtonEvent(L, 2, false);
					lua_pushboolean(L, LuaInputMouseButtonEvent->equals(OtherInputMouseButtonEvent));
					return 1;
				}
			}

			lua_pushboolean(L, false);
			return 1;
		}

		void InputMouseButtonEvent::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__gc", Type::lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void InputMouseButtonEvent::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[] = {
				{"Button", lua_getButton},
				{"State", lua_getState},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void InputMouseButtonEvent::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"Button", Instance::Instance::lua_readOnlyProperty},
				{"State", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		shared_ptr<InputMouseButtonEvent> checkInputMouseButtonEvent(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}

			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_InputMouseButtonEvent");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<InputMouseButtonEvent>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "InputMouseButtonEvent");
			}
			return NULL;
		}

		// BEGIN INPUTMOUSEWHEELEVENT

		DEFINE_TYPE(InputMouseWheelEvent){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}

		InputMouseWheelEvent::InputMouseWheelEvent(){
		    Delta = NULL;
		}

		InputMouseWheelEvent::~InputMouseWheelEvent(){}

		bool InputMouseWheelEvent::equals(shared_ptr<Type> other){
			shared_ptr<InputMouseWheelEvent> imwe = dynamic_pointer_cast<InputMouseWheelEvent>(other);
			if(!imwe){
				return false;
			}

			// InputMouseWheelEvents can't be copied or replicated
			// Compare addresses
			return imwe.get() == this;
		}

		std::string InputMouseWheelEvent::toString(){
			return "Input Event";
		}

	    shared_ptr<Vector2> InputMouseWheelEvent::getDelta(){
			return Delta;
		}

		void InputMouseWheelEvent::setDelta(shared_ptr<Vector2>  delta){
			Delta = delta;
		}

		int InputMouseWheelEvent::lua_getDelta(lua_State* L){
			shared_ptr<InputMouseWheelEvent> LuaInputMouseWheelEvent = checkInputMouseWheelEvent(L, 1, false);
			if(!LuaInputMouseWheelEvent){
				return 0;
			}

			shared_ptr<Vector2> delta = LuaInputMouseWheelEvent->getDelta();
			if(delta){
				return delta->wrap_lua(L);
			}

			lua_pushnil(L);
			return 1;
		}

		int InputMouseWheelEvent::lua_eq(lua_State* L){
			shared_ptr<InputMouseWheelEvent> LuaInputMouseWheelEvent = checkInputMouseWheelEvent(L, 1, false, false);
			if(LuaInputMouseWheelEvent){
				if(lua_isuserdata(L, 2)){
					shared_ptr<InputMouseWheelEvent> OtherInputMouseWheelEvent = checkInputMouseWheelEvent(L, 2, false);
					lua_pushboolean(L, LuaInputMouseWheelEvent->equals(OtherInputMouseWheelEvent));
					return 1;
				}
			}

			lua_pushboolean(L, false);
			return 1;
		}

		void InputMouseWheelEvent::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__gc", Type::lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void InputMouseWheelEvent::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[] = {
				{"Delta", lua_getDelta},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void InputMouseWheelEvent::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"Delta", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		shared_ptr<InputMouseWheelEvent> checkInputMouseWheelEvent(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}

			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_InputMouseWheelEvent");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<InputMouseWheelEvent>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "InputMouseWheelEvent");
			}
			return NULL;
		}

		// BEGIN INPUTMOUSEMOVEMENTEVENT

		DEFINE_TYPE(InputMouseMovementEvent){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}

		InputMouseMovementEvent::InputMouseMovementEvent(){
			Position = NULL;
		    Delta = NULL;
		}

		InputMouseMovementEvent::~InputMouseMovementEvent(){}

		bool InputMouseMovementEvent::equals(shared_ptr<Type> other){
			shared_ptr<InputMouseMovementEvent> imme = dynamic_pointer_cast<InputMouseMovementEvent>(other);
			if(!imme){
				return false;
			}

			// InputMouseMovementEvents can't be copied or replicated
			// Compare addresses
			return imme.get() == this;
		}

		std::string InputMouseMovementEvent::toString(){
			return "Input Event";
		}

		shared_ptr<Vector2> InputMouseMovementEvent::getPosition(){
			return Position;
		}

		void InputMouseMovementEvent::setPosition(shared_ptr<Vector2> pos){
			Position = pos;
		}

		shared_ptr<Vector2> InputMouseMovementEvent::getDelta(){
			return Delta;
		}

		void InputMouseMovementEvent::setDelta(shared_ptr<Vector2> delta){
			Delta = delta;
		}

		int InputMouseMovementEvent::lua_getPosition(lua_State* L){
			shared_ptr<InputMouseMovementEvent> LuaInputMouseMovementEvent = checkInputMouseMovementEvent(L, 1, false);
			if(!LuaInputMouseMovementEvent){
				return 0;
			}

		    shared_ptr<Vector2> pos = LuaInputMouseMovementEvent->getPosition();
			if(pos){
				return pos->wrap_lua(L);
			}

			lua_pushnil(L);
			return 1;
		}

		int InputMouseMovementEvent::lua_getDelta(lua_State* L){
			shared_ptr<InputMouseMovementEvent> LuaInputMouseMovementEvent = checkInputMouseMovementEvent(L, 1, false);
			if(!LuaInputMouseMovementEvent){
				return 0;
			}

		    shared_ptr<Vector2> delta = LuaInputMouseMovementEvent->getDelta();
			if(delta){
				return delta->wrap_lua(L);
			}

			lua_pushnil(L);
			return 1;
		}

		int InputMouseMovementEvent::lua_eq(lua_State* L){
			shared_ptr<InputMouseMovementEvent> LuaInputMouseMovementEvent = checkInputMouseMovementEvent(L, 1, false, false);
			if(LuaInputMouseMovementEvent){
				if(lua_isuserdata(L, 2)){
					shared_ptr<InputMouseMovementEvent> OtherInputMouseMovementEvent = checkInputMouseMovementEvent(L, 2, false);
					lua_pushboolean(L, LuaInputMouseMovementEvent->equals(OtherInputMouseMovementEvent));
					return 1;
				}
			}

			lua_pushboolean(L, false);
			return 1;
		}

		void InputMouseMovementEvent::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__gc", Type::lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void InputMouseMovementEvent::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[] = {
				{"Position", lua_getPosition},
				{"Delta", lua_getDelta},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void InputMouseMovementEvent::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"Position", Instance::Instance::lua_readOnlyProperty},
				{"Delta", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		shared_ptr<InputMouseMovementEvent> checkInputMouseMovementEvent(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}

			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_InputMouseMovementEvent");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<InputMouseMovementEvent>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "InputMouseMovementEvent");
			}
			return NULL;
		}

		// BEGIN INPUTKEYEVENT
		
		DEFINE_TYPE(InputKeyEvent){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}

		InputKeyEvent::InputKeyEvent(){
			KeyCode = Enum::KeyCode::Unknown;
		    State = false;
		}

		InputKeyEvent::~InputKeyEvent(){}

		bool InputKeyEvent::equals(shared_ptr<Type> other){
			shared_ptr<InputKeyEvent> ike = dynamic_pointer_cast<InputKeyEvent>(other);
			if(!ike){
				return false;
			}

			// InputKeyEvents can't be copied or replicated
			// Compare addresses
			return ike.get() == this;
		}

		std::string InputKeyEvent::toString(){
			return "Input Event";
		}

		Enum::KeyCode InputKeyEvent::getKeyCode(){
			return KeyCode;
		}

		void InputKeyEvent::setKeyCode(Enum::KeyCode keyCode){
			KeyCode = keyCode;
		}

		bool InputKeyEvent::getState(){
			return State;
		}

		void InputKeyEvent::setState(bool state){
			State = state;
		}

		int InputKeyEvent::lua_getKeyCode(lua_State* L){
			shared_ptr<InputKeyEvent> LuaInputKeyEvent = checkInputKeyEvent(L, 1, false);
			if(!LuaInputKeyEvent){
				return 0;
			}

		    return Enum::LuaKeyCode->getEnumItem((int)LuaInputKeyEvent->getKeyCode())->wrap_lua(L);
			return 1;
		}

		int InputKeyEvent::lua_getState(lua_State* L){
			shared_ptr<InputKeyEvent> LuaInputKeyEvent = checkInputKeyEvent(L, 1, false);
			if(!LuaInputKeyEvent){
				return 0;
			}

			lua_pushboolean(L, LuaInputKeyEvent->getState());
			return 1;
		}

		int InputKeyEvent::lua_eq(lua_State* L){
			shared_ptr<InputKeyEvent> LuaInputKeyEvent = checkInputKeyEvent(L, 1, false, false);
			if(LuaInputKeyEvent){
				if(lua_isuserdata(L, 2)){
					shared_ptr<InputKeyEvent> OtherInputKeyEvent = checkInputKeyEvent(L, 2, false);
					lua_pushboolean(L, LuaInputKeyEvent->equals(OtherInputKeyEvent));
					return 1;
				}
			}

			lua_pushboolean(L, false);
			return 1;
		}

		void InputKeyEvent::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__gc", Type::lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void InputKeyEvent::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[] = {
				{"KeyCode", lua_getKeyCode},
				{"State", lua_getState},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void InputKeyEvent::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"KeyCode", Instance::Instance::lua_readOnlyProperty},
				{"State", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		shared_ptr<InputKeyEvent> checkInputKeyEvent(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}

			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_InputKeyEvent");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<InputKeyEvent>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "InputKeyEvent");
			}
			return NULL;
		}
		
		// BEGIN INPUTEVENT
		
		DEFINE_TYPE(InputEvent){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}

		InputEvent::InputEvent(){
			EventType = Enum::UserInputType::Unknown;

			MouseButton = NULL;
			MouseWheel = NULL;
			MouseMovement = NULL;
			Key = NULL;
		}

		InputEvent::~InputEvent(){}

		bool InputEvent::equals(shared_ptr<Type> other){
			shared_ptr<InputEvent> ie = dynamic_pointer_cast<InputEvent>(other);
			if(!ie){
				return false;
			}

			// InputEvents can't be copied or replicated
			// Compare addresses
			return ie.get() == this;
		}

		std::string InputEvent::toString(){
			return "Input Event";
		}

		Enum::UserInputType InputEvent::getEventType(){
			return EventType;
		}
		
		void InputEvent::setEventType(Enum::UserInputType evtType){
			EventType = evtType;
		}

		shared_ptr<InputMouseButtonEvent> InputEvent::getMouseButton(){
			return MouseButton;
		};
		
		void InputEvent::setMouseButton(shared_ptr<InputMouseButtonEvent> mouseButton){
			MouseButton = mouseButton;
		}

		shared_ptr<InputMouseWheelEvent> InputEvent::getMouseWheel(){
			return MouseWheel;
		}
		
		void InputEvent::setMouseWheel(shared_ptr<InputMouseWheelEvent> mouseWheel){
			MouseWheel = mouseWheel;
		}

		shared_ptr<InputMouseMovementEvent> InputEvent::getMouseMovement(){
			return MouseMovement;
		}
		
		void InputEvent::setMouseMovement(shared_ptr<InputMouseMovementEvent> mouseMovement){
			MouseMovement = mouseMovement;
		}

		shared_ptr<InputKeyEvent> InputEvent::getKey(){
			return Key;
		}
		
		void InputEvent::setKey(shared_ptr<InputKeyEvent> key){
			Key = key;
		}

		int InputEvent::lua_getMouseButton(lua_State* L){
			shared_ptr<InputEvent> LuaInputEvent = checkInputEvent(L, 1, false);
			if(!LuaInputEvent){
				return 0;
			}

			shared_ptr<InputMouseButtonEvent> mbe = LuaInputEvent->getMouseButton();
			if(mbe){
				return mbe->wrap_lua(L);
			}else{
				lua_pushnil(L);
			}

			return 1;
		}

		int InputEvent::lua_getMouseWheel(lua_State* L){
			shared_ptr<InputEvent> LuaInputEvent = checkInputEvent(L, 1, false);
			if(!LuaInputEvent){
				return 0;
			}

			shared_ptr<InputMouseWheelEvent> mwe = LuaInputEvent->getMouseWheel();
			if(mwe){
				return mwe->wrap_lua(L);
			}else{
				lua_pushnil(L);
			}

			return 1;
		}

		int InputEvent::lua_getMouseMovement(lua_State* L){
			shared_ptr<InputEvent> LuaInputEvent = checkInputEvent(L, 1, false);
			if(!LuaInputEvent){
				return 0;
			}

			shared_ptr<InputMouseMovementEvent> mme = LuaInputEvent->getMouseMovement();
			if(mme){
				return mme->wrap_lua(L);
			}else{
				lua_pushnil(L);
			}

			return 1;
		}

		int InputEvent::lua_getKey(lua_State* L){
			shared_ptr<InputEvent> LuaInputEvent = checkInputEvent(L, 1, false);
			if(!LuaInputEvent){
				return 0;
			}

			shared_ptr<InputKeyEvent> ke = LuaInputEvent->getKey();
			if(ke){
				return ke->wrap_lua(L);
			}else{
				lua_pushnil(L);
			}

			return 1;
		}

		int InputEvent::lua_getEventType(lua_State* L){
			shared_ptr<InputEvent> LuaInputEvent = checkInputEvent(L, 1, false);
			if(!LuaInputEvent){
				return 0;
			}

			return Enum::LuaUserInputType->getEnumItem((int)LuaInputEvent->getEventType())->wrap_lua(L);
		}
		

		int InputEvent::lua_eq(lua_State* L){
			shared_ptr<InputEvent> LuaInputEvent = checkInputEvent(L, 1, false, false);
			if(LuaInputEvent){
				if(lua_isuserdata(L, 2)){
					shared_ptr<InputEvent> OtherInputEvent = checkInputEvent(L, 2, false);
					lua_pushboolean(L, LuaInputEvent->equals(OtherInputEvent));
					return 1;
				}
			}

			lua_pushboolean(L, false);
			return 1;
		}

		void InputEvent::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__gc", Type::lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void InputEvent::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"MouseButton", Instance::Instance::lua_readOnlyProperty},
				{"MouseWheel", Instance::Instance::lua_readOnlyProperty},
				{"MouseMovement", Instance::Instance::lua_readOnlyProperty},
				{"Key", Instance::Instance::lua_readOnlyProperty},
				{"EventType", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void InputEvent::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[] = {
				{"MouseButton", lua_getMouseButton},
				{"MouseWheel", lua_getMouseWheel},
				{"MouseMovement", lua_getMouseMovement},
				{"Key", lua_getKey},
				{"EventType", lua_getEventType},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		shared_ptr<InputEvent> checkInputEvent(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}

			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_InputEvent");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<InputEvent>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "InputEvent");
			}
			return NULL;
		}
	}
}
