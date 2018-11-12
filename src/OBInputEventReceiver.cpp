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

namespace OB{
    OBInputEventReceiver::OBInputEventReceiver(OBEngine* eng){
		this->eng = eng;
	}

	OBInputEventReceiver::~OBInputEventReceiver(){}

#if HAVE_IRRLICHT
	Enum::KeyCode OBInputEventReceiver::irrlictKeyToOB(irr::EKEY_CODE irrKey){
		switch(irrKey){
			case irr::KEY_BACK: {
				return Enum::KeyCode::Backspace;
			}
			case irr::KEY_TAB: {
				return Enum::KeyCode::Tab;
			}
			case irr::KEY_CLEAR: {
				return Enum::KeyCode::Clear;
			}
			case irr::KEY_RETURN: {
				return Enum::KeyCode::Return;
			}
			case irr::KEY_SHIFT: {
				return Enum::KeyCode::LeftShift;
			}
			case irr::KEY_CONTROL: {
				return Enum::KeyCode::LeftControl;
			}
			case irr::KEY_MENU: {
				return Enum::KeyCode::Menu;
			}
			case irr::KEY_PAUSE: {
				return Enum::KeyCode::Pause;
			}
			case irr::KEY_CAPITAL: {
				return Enum::KeyCode::CapsLock;
			}
			case irr::KEY_ESCAPE: {
				return Enum::KeyCode::Escape;
			}
			case irr::KEY_SPACE: {
				return Enum::KeyCode::Space;
			}
			case irr::KEY_PRIOR: {
				return Enum::KeyCode::PageUp;
			}
			case irr::KEY_NEXT: {
				return Enum::KeyCode::PageDown;
			}
			case irr::KEY_END: {
				return Enum::KeyCode::End;
			}
			case irr::KEY_HOME: {
				return Enum::KeyCode::Home;
			}
			case irr::KEY_LEFT: {
				return Enum::KeyCode::Left;
			}
			case irr::KEY_UP: {
				return Enum::KeyCode::Up;
			}
			case irr::KEY_RIGHT: {
				return Enum::KeyCode::Right;
			}
			case irr::KEY_DOWN: {
				return Enum::KeyCode::Down;
			}
			case irr::KEY_SELECT: {
				return Enum::KeyCode::Select;
			}
			case irr::KEY_PRINT: {
				return Enum::KeyCode::Print;
			}
			case irr::KEY_EXECUT: {
				return Enum::KeyCode::Execute;
			}
			case irr::KEY_SNAPSHOT: {
				return Enum::KeyCode::PrintSc;
			}
			case irr::KEY_INSERT: {
				return Enum::KeyCode::Insert;
			}
			case irr::KEY_DELETE: {
				return Enum::KeyCode::Delete;
			}
			case irr::KEY_HELP: {
				return Enum::KeyCode::Help;
			}
			case irr::KEY_KEY_0: {
				return Enum::KeyCode::Zero;
			}
			case irr::KEY_KEY_1: {
				return Enum::KeyCode::One;
			}
			case irr::KEY_KEY_2: {
				return Enum::KeyCode::Two;
			}
			case irr::KEY_KEY_3: {
				return Enum::KeyCode::Three;
			}
			case irr::KEY_KEY_4: {
				return Enum::KeyCode::Four;
			}
			case irr::KEY_KEY_5: {
				return Enum::KeyCode::Five;
			}
			case irr::KEY_KEY_6: {
				return Enum::KeyCode::Six;
			}
			case irr::KEY_KEY_7: {
				return Enum::KeyCode::Seven;
			}
			case irr::KEY_KEY_8: {
				return Enum::KeyCode::Eight;
			}
			case irr::KEY_KEY_9: {
				return Enum::KeyCode::Nine;
			}
			case irr::KEY_KEY_A: {
				return Enum::KeyCode::A;
			}
			case irr::KEY_KEY_B: {
				return Enum::KeyCode::B;
			}
			case irr::KEY_KEY_C: {
				return Enum::KeyCode::C;
			}
			case irr::KEY_KEY_D: {
				return Enum::KeyCode::D;
			}
			case irr::KEY_KEY_E: {
				return Enum::KeyCode::E;
			}
			case irr::KEY_KEY_F: {
				return Enum::KeyCode::F;
			}
			case irr::KEY_KEY_G: {
				return Enum::KeyCode::G;
			}
			case irr::KEY_KEY_H: {
				return Enum::KeyCode::H;
			}
			case irr::KEY_KEY_I: {
				return Enum::KeyCode::I;
			}
			case irr::KEY_KEY_J: {
				return Enum::KeyCode::J;
			}
			case irr::KEY_KEY_K: {
				return Enum::KeyCode::K;
			}
			case irr::KEY_KEY_L: {
				return Enum::KeyCode::L;
			}
			case irr::KEY_KEY_M: {
				return Enum::KeyCode::M;
			}
			case irr::KEY_KEY_N: {
				return Enum::KeyCode::N;
			}
			case irr::KEY_KEY_O: {
				return Enum::KeyCode::O;
			}
			case irr::KEY_KEY_P: {
				return Enum::KeyCode::P;
			}
			case irr::KEY_KEY_Q: {
				return Enum::KeyCode::Q;
			}
			case irr::KEY_KEY_R: {
				return Enum::KeyCode::R;
			}
			case irr::KEY_KEY_S: {
				return Enum::KeyCode::S;
			}
			case irr::KEY_KEY_T: {
				return Enum::KeyCode::T;
			}
			case irr::KEY_KEY_U: {
				return Enum::KeyCode::U;
			}
			case irr::KEY_KEY_V: {
				return Enum::KeyCode::V;
			}
			case irr::KEY_KEY_W: {
				return Enum::KeyCode::W;
			}
			case irr::KEY_KEY_X: {
				return Enum::KeyCode::X;
			}
			case irr::KEY_KEY_Y: {
				return Enum::KeyCode::Y;
			}
			case irr::KEY_KEY_Z: {
				return Enum::KeyCode::Z;
			}
			case irr::KEY_LWIN: {
				return Enum::KeyCode::LeftSuper;
			}
			case irr::KEY_RWIN: {
				return Enum::KeyCode::RightSuper;
			}
			case irr::KEY_NUMPAD0: {
				return Enum::KeyCode::NumpadZero;
			}
			case irr::KEY_NUMPAD1: {
				return Enum::KeyCode::NumpadOne;
			}
			case irr::KEY_NUMPAD2: {
				return Enum::KeyCode::NumpadTwo;
			}
			case irr::KEY_NUMPAD3: {
				return Enum::KeyCode::NumpadThree;
			}
			case irr::KEY_NUMPAD4: {
				return Enum::KeyCode::NumpadFour;
			}
			case irr::KEY_NUMPAD5: {
				return Enum::KeyCode::NumpadFive;
			}
			case irr::KEY_NUMPAD6: {
				return Enum::KeyCode::NumpadSix;
			}
			case irr::KEY_NUMPAD7: {
				return Enum::KeyCode::NumpadSeven;
			}
			case irr::KEY_NUMPAD8: {
				return Enum::KeyCode::NumpadEight;
			}
			case irr::KEY_NUMPAD9: {
				return Enum::KeyCode::NumpadNine;
			}
			case irr::KEY_MULTIPLY: {
				return Enum::KeyCode::NumpadMultiply;
			}
			case irr::KEY_ADD: {
				return Enum::KeyCode::NumpadPlus;
			}
			case irr::KEY_SEPARATOR: {
				puts("SEPARATOR");
			}
			case irr::KEY_SUBTRACT: {
				return Enum::KeyCode::NumpadMinus;
			}
			case irr::KEY_DECIMAL: {
				return Enum::KeyCode::NumpadPeriod;
			}
			case irr::KEY_DIVIDE: {
				return Enum::KeyCode::NumpadDivide;
			}
			case irr::KEY_F1: {
				return Enum::KeyCode::F1;
			}
			case irr::KEY_F2: {
				return Enum::KeyCode::F2;
			}
			case irr::KEY_F3: {
				return Enum::KeyCode::F3;
			}
			case irr::KEY_F4: {
				return Enum::KeyCode::F4;
			}
			case irr::KEY_F5: {
				return Enum::KeyCode::F5;
			}
			case irr::KEY_F6: {
				return Enum::KeyCode::F6;
			}
			case irr::KEY_F7: {
				return Enum::KeyCode::F7;
			}
			case irr::KEY_F8: {
				return Enum::KeyCode::F8;
			}
			case irr::KEY_F9: {
				return Enum::KeyCode::F9;
			}
			case irr::KEY_F10: {
				return Enum::KeyCode::F10;
			}
			case irr::KEY_F11: {
				return Enum::KeyCode::F11;
			}
			case irr::KEY_F12: {
				return Enum::KeyCode::F12;
			}
			case irr::KEY_F13: {
				return Enum::KeyCode::F13;
			}
			case irr::KEY_F14: {
				return Enum::KeyCode::F14;
			}
			case irr::KEY_F15: {
				return Enum::KeyCode::F15;
			}
			case irr::KEY_F16: {
				return Enum::KeyCode::F16;
			}
			case irr::KEY_F17: {
				return Enum::KeyCode::F17;
			}
			case irr::KEY_F18: {
				return Enum::KeyCode::F18;
			}
			case irr::KEY_F19: {
				return Enum::KeyCode::F19;
			}
			case irr::KEY_F20: {
				return Enum::KeyCode::F20;
			}
			case irr::KEY_F21: {
				return Enum::KeyCode::F21;
			}
			case irr::KEY_F22: {
				return Enum::KeyCode::F22;
			}
			case irr::KEY_F23: {
				return Enum::KeyCode::F23;
			}
			case irr::KEY_F24: {
				return Enum::KeyCode::F24;
			}
			case irr::KEY_NUMLOCK: {
				return Enum::KeyCode::NumLock;
			}
			case irr::KEY_SCROLL: {
				return Enum::KeyCode::ScrollLock;
			}
			case irr::KEY_LSHIFT: {
				return Enum::KeyCode::LeftShift;
			}
			case irr::KEY_RSHIFT: {
				return Enum::KeyCode::RightShift;
			}
			case irr::KEY_LCONTROL: {
				return Enum::KeyCode::LeftControl;
			}
			case irr::KEY_RCONTROL: {
				return Enum::KeyCode::RightControl;
			}
			case irr::KEY_RMENU: {
				return Enum::KeyCode::Menu;
			}
			case irr::KEY_OEM_1: {
				return Enum::KeyCode::OEM1;
			}
			case irr::KEY_OEM_2: {
				return Enum::KeyCode::OEM2;
			}
			case irr::KEY_OEM_3: {
				return Enum::KeyCode::OEM3;
			}
			case irr::KEY_OEM_4: {
				return Enum::KeyCode::OEM4;
			}
			case irr::KEY_OEM_5: {
				return Enum::KeyCode::OEM5;
			}
			case irr::KEY_OEM_6: {
				return Enum::KeyCode::OEM6;
			}
			case irr::KEY_OEM_7: {
				return Enum::KeyCode::OEM7;
			}
			case irr::KEY_OEM_8: {
				return Enum::KeyCode::OEM8;
			}
			case irr::KEY_OEM_AX: {
				return Enum::KeyCode::OEM_AX;
			}
			case irr::KEY_OEM_102: {
				return Enum::KeyCode::OEM102;
			}
			case irr::KEY_PLUS: {
				return Enum::KeyCode::Plus;
			}
			case irr::KEY_COMMA: {
				return Enum::KeyCode::Comma;
			}
			case irr::KEY_MINUS: {
				return Enum::KeyCode::Minus;
			}
			case irr::KEY_PERIOD: {
				return Enum::KeyCode::Period;
			}
			case 0xAD: {
				return Enum::KeyCode::MediaPrevious;
			}
			case 0xAC: {
				return Enum::KeyCode::MediaPlayPause;
			}
			case 0xAB: {
				return Enum::KeyCode::MediaNext;
			}
			case 0xAE: {
				return Enum::KeyCode::MediaStop;
			}
			default: {
				printf("[INPUTRECEIVER] UNKNOWN KEY: %i", irrKey);
			}
		}
		
	    return Enum::KeyCode::Unknown;
	}
	
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
						Enum::KeyCode keyCode = irrlictKeyToOB(evt.KeyInput.Key);
						uis->input_keyEvent(keyCode, evt.KeyInput.PressedDown);
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
#endif

	OBEngine* OBInputEventReceiver::getEngine(){
		return eng;
	}
}
