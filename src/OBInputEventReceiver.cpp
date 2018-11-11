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

#include "OBInputEventReceiver.h"

#include "OBEngine.h"

#include "instance/UserInputService.h"
#include "type/InputEvent.h"

#if HAVE_IRRLICHT
namespace OB{
    OBInputEventReceiver::OBInputEventReceiver(OBEngine* eng){
		this->eng = eng;
	}

	OBInputEventReceiver::~OBInputEventReceiver(){}

	bool OBInputEventReceiver::OnEvent(const irr::SEvent& evt){
		shared_ptr<Instance::DataModel> dm = eng->getDataModel();
		if(dm){
			shared_ptr<Instance::UserInputService> uis = dm->getUserInputService();
			if(uis){
				switch(evt.EventType){
					case irr::EET_MOUSE_INPUT_EVENT: {
					    switch(evt.MouseInput.Event){
							case irr::EMIE_LMOUSE_PRESSED_DOWN: {
							    uis->input_mouseButton(Enum::MouseButton::Left, true);
								break;
							}
							case irr::EMIE_RMOUSE_PRESSED_DOWN: {
								uis->input_mouseButton(Enum::MouseButton::Right, true);
								break;
							}
							case irr::EMIE_MMOUSE_PRESSED_DOWN: {
								uis->input_mouseButton(Enum::MouseButton::Middle, true);
								break;
							}
							case irr::EMIE_LMOUSE_LEFT_UP: {
								uis->input_mouseButton(Enum::MouseButton::Left, false);
								break;
							}
							case irr::EMIE_RMOUSE_LEFT_UP: {
								uis->input_mouseButton(Enum::MouseButton::Right, false);
								break;
							}
							case irr::EMIE_MMOUSE_LEFT_UP: {
								uis->input_mouseButton(Enum::MouseButton::Middle, false);
								break;
							}
							case irr::EMIE_MOUSE_WHEEL: {
								uis->input_mouseWheel(evt.MouseInput.Wheel);
								break;
							}
							case irr::EMIE_MOUSE_MOVED: {
								uis->input_mouseMoved(evt.MouseInput.X, evt.MouseInput.Y);
								break;
							}
						}
				
						return true;
						break;
					}
					case irr::EET_KEY_INPUT_EVENT: {
						return true;
						break;
					}
					case irr::EET_JOYSTICK_INPUT_EVENT: {
						break;
					}
				}
			}
		}

		return false;
	}

	OBEngine* OBInputEventReceiver::getEngine(){
		return eng;
	}
}
#endif
