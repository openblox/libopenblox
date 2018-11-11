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

#include "instance/UserInputService.h"

#include "utility.h"

#include "type/Vector2.h"
#include "type/InputEvent.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(UserInputService, false, isDataModel, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		UserInputService::UserInputService(OBEngine* eng) : Instance(eng){
			Name = ClassName;
			netId = OB_NETID_NOT_REPLICATED;

			Archivable = false;

			GamepadConnected = make_shared<Type::Event>("GamepadConnected");
			GamepadDisconnected = make_shared<Type::Event>("GamepadDisconnected");

		    InputBegan = make_shared<Type::Event>("InputBegan");
		    InputChanged = make_shared<Type::Event>("InputChanged");
		    InputEnded = make_shared<Type::Event>("InputEnded");

		    WindowFocusReleased = make_shared<Type::Event>("WindowFocusReleased");
		    WindowFocused = make_shared<Type::Event>("WindowFocused");

			mouseX = 0;
			mouseY = 0;
		}

		UserInputService::~UserInputService(){}

#if HAVE_PUGIXML
		std::string UserInputService::serializedID(){
			shared_ptr<OBSerializer> serializer = eng->getSerializer();
			serializer->SetID(shared_from_this(), getClassName());

			return Instance::serializedID();
		}
#endif

		shared_ptr<Instance> UserInputService::cloneImpl(){
			return NULL;
		}

		shared_ptr<Type::Event> UserInputService::getGamepadConnected(){
			return GamepadConnected;
		}

		shared_ptr<Type::Event> UserInputService::getGamepadDisconnected(){
			return GamepadDisconnected;
		}

		shared_ptr<Type::Event> UserInputService::getInputBegan(){
			return InputBegan;
		}

		shared_ptr<Type::Event> UserInputService::getInputChanged(){
			return InputChanged;
		}

		shared_ptr<Type::Event> UserInputService::getInputEnded(){
			return InputEnded;
		}

		shared_ptr<Type::Event> UserInputService::getWindowFocusReleased(){
			return WindowFocusReleased;
		}

		shared_ptr<Type::Event> UserInputService::getWindowFocused(){
			return WindowFocused;
		}

		void UserInputService::register_lua_events(lua_State* L){
			Instance::register_lua_events(L);

			luaL_Reg events[] = {
				{"GamepadConnected", WRAP_EVT(UserInputService, GamepadConnected)},
				{"GamepadDisconnected", WRAP_EVT(UserInputService, GamepadDisconnected)},
				{"InputBegan", WRAP_EVT(UserInputService, InputBegan)},
				{"InputChanged", WRAP_EVT(UserInputService, InputChanged)},
				{"InputEnded", WRAP_EVT(UserInputService, InputEnded)},
				{"WindowFocusReleased", WRAP_EVT(UserInputService, WindowFocusReleased)},
				{"WindowFocused", WRAP_EVT(UserInputService, WindowFocused)},
				{NULL, NULL}
			};
			luaL_setfuncs(L, events, 0);
		}

		void UserInputService::input_mouseButton(Enum::MouseButton btn, bool state){
			shared_ptr<Type::InputEvent> ie = make_shared<Type::InputEvent>();

			shared_ptr<Type::InputMouseButtonEvent> imbe = make_shared<Type::InputMouseButtonEvent>();
			imbe->setButton(btn);
			imbe->setState(state);

			ie->setMouseButton(imbe);
			ie->setEventType(Enum::UserInputType::MouseButton);

			std::vector<shared_ptr<Type::VarWrapper>> argList = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(ie)});

			if(state){
				InputBegan->Fire(eng, argList);
			}else{
				InputEnded->Fire(eng, argList);
			}
			InputChanged->Fire(eng, argList);
		}

		void UserInputService::input_mouseWheel(double delta){
			shared_ptr<Type::InputEvent> ie = make_shared<Type::InputEvent>();

			shared_ptr<Type::InputMouseWheelEvent> imwe = make_shared<Type::InputMouseWheelEvent>();
			imwe->setDelta(delta);

			ie->setMouseWheel(imwe);
			ie->setEventType(Enum::UserInputType::MouseWheel);

			std::vector<shared_ptr<Type::VarWrapper>> argList = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(ie)});

			InputChanged->Fire(eng, argList);
		}

		void UserInputService::input_mouseMoved(int x, int y){
			shared_ptr<Type::InputEvent> ie = make_shared<Type::InputEvent>();

			shared_ptr<Type::InputMouseMovementEvent> imme = make_shared<Type::InputMouseMovementEvent>();
		    imme->setPosition(make_shared<Type::Vector2>(x, y));

			int deltaX = x - mouseX;
			int deltaY = y - mouseY;

			imme->setDelta(make_shared<Type::Vector2>(deltaX, deltaY));

			mouseX = x;
			mouseY = y;

			ie->setMouseMovement(imme);
			ie->setEventType(Enum::UserInputType::MouseMovement);

			std::vector<shared_ptr<Type::VarWrapper>> argList = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(ie)});

			InputChanged->Fire(eng, argList);
		}
	}
}
