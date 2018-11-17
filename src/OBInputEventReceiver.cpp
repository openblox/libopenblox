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
				printf("[INPUTRECEIVER] [IRRLICHT] UNKNOWN KEY: %i\n", irrKey);
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
								uis->input_mouseWheel(make_shared<Type::Vector2>(0, evt.MouseInput.Wheel));
								break;
							}
							case irr::EMIE_MOUSE_MOVED: {
								uis->input_mouseMoved(make_shared<Type::Vector2>(evt.MouseInput.X, evt.MouseInput.Y), NULL);
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
				}
			}
		}

		return false;
	}
#endif

#if HAVE_SDL2
    Enum::KeyCode OBInputEventReceiver::sdl2KeyToOB(SDL_Keysym& ksym){
		switch(ksym.sym){
			case SDLK_BACKSPACE: {
				return Enum::KeyCode::Backspace;
			}
			case SDLK_TAB: {
				return Enum::KeyCode::Tab;
			}
			case SDLK_CLEAR: {
				return Enum::KeyCode::Clear;
			}
			case SDLK_RETURN: {
				return Enum::KeyCode::Return;
			}
			case SDLK_RETURN2: {
				return Enum::KeyCode::Return2;
			}
			case SDLK_PAUSE: {
				return Enum::KeyCode::Pause;
			}
			case SDLK_ESCAPE: {
				return Enum::KeyCode::Escape;
			}
			case SDLK_SPACE: {
				return Enum::KeyCode::Space;
			}
			case SDLK_QUOTEDBL: {
				return Enum::KeyCode::DoubleQuote;
			}
			case SDLK_HASH: {
				return Enum::KeyCode::Hash;
			}
			case SDLK_DOLLAR: {
				return Enum::KeyCode::Dollar;
			}
			case SDLK_PERCENT: {
				return Enum::KeyCode::Percent;
			}
			case SDLK_AMPERSAND: {
				return Enum::KeyCode::Ampersand;
			}
			case SDLK_QUOTE: {
				return Enum::KeyCode::Quote;
			}
			case SDLK_LEFTPAREN: {
				return Enum::KeyCode::LeftParenthesis;
			}
			case SDLK_RIGHTPAREN: {
				return Enum::KeyCode::RightParenthesis;
			}
			case SDLK_ASTERISK: {
				return Enum::KeyCode::Asterisk;
			}
			case SDLK_PLUS: {
				return Enum::KeyCode::Plus;
			}
			case SDLK_COMMA: {
				return Enum::KeyCode::Comma;
			}
			case SDLK_MINUS: {
				return Enum::KeyCode::Minus;
			}
			case SDLK_PERIOD: {
				return Enum::KeyCode::Period;
			}
			case SDLK_SLASH: {
				return Enum::KeyCode::Hash;
			}
			case SDLK_0: {
				return Enum::KeyCode::Zero;
			}
			case SDLK_1: {
				return Enum::KeyCode::One;
			}
			case SDLK_2: {
				return Enum::KeyCode::Two;
			}
			case SDLK_3: {
				return Enum::KeyCode::Three;
			}
			case SDLK_4: {
				return Enum::KeyCode::Four;
			}
			case SDLK_5: {
				return Enum::KeyCode::Five;
			}
			case SDLK_6: {
				return Enum::KeyCode::Six;
			}
			case SDLK_7: {
				return Enum::KeyCode::Seven;
			}
			case SDLK_8: {
				return Enum::KeyCode::Eight;
			}
			case SDLK_9: {
				return Enum::KeyCode::Nine;
			}
			case SDLK_COLON: {
				return Enum::KeyCode::Colon;
			}
			case SDLK_SEMICOLON: {
				return Enum::KeyCode::Semicolon;
			}
			case SDLK_GREATER: {
				return Enum::KeyCode::GreaterThan;
			}
			case SDLK_LESS: {
				return Enum::KeyCode::LessThan;
			}
			case SDLK_EQUALS: {
				return Enum::KeyCode::Equals;
			}
			case SDLK_QUESTION: {
				return Enum::KeyCode::Question;
			}
			case SDLK_EXCLAIM: {
				return Enum::KeyCode::Exclamation;
			}
			case SDLK_AT: {
				return Enum::KeyCode::At;
			}
			case SDLK_LEFTBRACKET: {
				return Enum::KeyCode::LeftBracket;
			}
			case SDLK_RIGHTBRACKET: {
				return Enum::KeyCode::RightBracket;
			}
			case SDLK_BACKSLASH: {
				return Enum::KeyCode::Backslash;
			}
			case SDLK_CARET: {
				return Enum::KeyCode::Caret;
			}
			case SDLK_UNDERSCORE: {
				return Enum::KeyCode::Underscore;
			}
			case SDLK_BACKQUOTE: {
				return Enum::KeyCode::Backquote;
			}
			case SDLK_a: {
				return Enum::KeyCode::A;
			}
			case SDLK_b: {
				return Enum::KeyCode::B;
			}
			case SDLK_c: {
				return Enum::KeyCode::C;
			}
			case SDLK_d: {
				return Enum::KeyCode::D;
			}
			case SDLK_e: {
				return Enum::KeyCode::E;
			}
			case SDLK_f: {
				return Enum::KeyCode::F;
			}
			case SDLK_g: {
				return Enum::KeyCode::G;
			}
			case SDLK_h: {
				return Enum::KeyCode::H;
			}
			case SDLK_i: {
				return Enum::KeyCode::I;
			}
			case SDLK_j: {
				return Enum::KeyCode::J;
			}
			case SDLK_k: {
				return Enum::KeyCode::K;
			}
			case SDLK_l: {
				return Enum::KeyCode::L;
			}
			case SDLK_m: {
				return Enum::KeyCode::M;
			}
			case SDLK_n: {
				return Enum::KeyCode::N;
			}
			case SDLK_o: {
				return Enum::KeyCode::O;
			}
			case SDLK_p: {
				return Enum::KeyCode::P;
			}
			case SDLK_q: {
				return Enum::KeyCode::Q;
			}
			case SDLK_r: {
				return Enum::KeyCode::R;
			}
			case SDLK_s: {
				return Enum::KeyCode::S;
			}
			case SDLK_t: {
				return Enum::KeyCode::T;
			}
			case SDLK_u: {
				return Enum::KeyCode::U;
			}
			case SDLK_v: {
				return Enum::KeyCode::V;
			}
			case SDLK_w: {
				return Enum::KeyCode::W;
			}
			case SDLK_x: {
				return Enum::KeyCode::X;
			}
			case SDLK_y: {
				return Enum::KeyCode::Y;
			}
			case SDLK_z: {
				return Enum::KeyCode::Z;
			}
			case SDLK_DELETE: {
				return Enum::KeyCode::Delete;
			}
			case SDLK_KP_0: {
				return Enum::KeyCode::NumpadZero;
			}
			case SDLK_KP_1: {
				return Enum::KeyCode::NumpadOne;
			}
			case SDLK_KP_2: {
				return Enum::KeyCode::NumpadTwo;
			}
			case SDLK_KP_3: {
				return Enum::KeyCode::NumpadThree;
			}
			case SDLK_KP_4: {
				return Enum::KeyCode::NumpadFour;
			}
			case SDLK_KP_5: {
				return Enum::KeyCode::NumpadFive;
			}
			case SDLK_KP_6: {
				return Enum::KeyCode::NumpadSix;
			}
			case SDLK_KP_7: {
				return Enum::KeyCode::NumpadSeven;
			}
			case SDLK_KP_8: {
				return Enum::KeyCode::NumpadEight;
			}
			case SDLK_KP_9: {
				return Enum::KeyCode::NumpadNine;
			}
			case SDLK_KP_PERIOD: {
				return Enum::KeyCode::NumpadPeriod;
			}
			case SDLK_KP_DIVIDE: {
				return Enum::KeyCode::NumpadDivide;
			}
			case SDLK_KP_MULTIPLY: {
				return Enum::KeyCode::NumpadMultiply;
			}
			case SDLK_KP_MINUS: {
				return Enum::KeyCode::NumpadMinus;
			}
			case SDLK_KP_PLUS: {
				return Enum::KeyCode::NumpadPlus;
			}
			case SDLK_KP_ENTER: {
				return Enum::KeyCode::NumpadEnter;
			}
			case SDLK_KP_EQUALS: {
				return Enum::KeyCode::NumpadEquals;
			}
			case SDLK_UP: {
				return Enum::KeyCode::Up;
			}
			case SDLK_DOWN: {
				return Enum::KeyCode::Down;
			}
			case SDLK_LEFT: {
				return Enum::KeyCode::Left;
			}
			case SDLK_RIGHT: {
				return Enum::KeyCode::Right;
			}
			case SDLK_INSERT: {
				return Enum::KeyCode::Insert;
			}
			case SDLK_HOME: {
				return Enum::KeyCode::Home;
			}
			case SDLK_END: {
				return Enum::KeyCode::End;
			}
			case SDLK_PAGEUP: {
				return Enum::KeyCode::PageUp;
			}
			case SDLK_PAGEDOWN: {
				return Enum::KeyCode::PageDown;
			}
			case SDLK_LSHIFT: {
				return Enum::KeyCode::LeftShift;
			}
			case SDLK_RSHIFT: {
				return Enum::KeyCode::RightShift;
			}
			case SDLK_LGUI: {
				return Enum::KeyCode::LeftSuper;
			}
			case SDLK_RGUI: {
				return Enum::KeyCode::RightSuper;
			}
			case SDLK_LALT: {
				return Enum::KeyCode::LeftAlt;
			}
			case SDLK_RALT: {
				return Enum::KeyCode::RightAlt;
			}
			case SDLK_LCTRL: {
				return Enum::KeyCode::LeftControl;
			}
			case SDLK_RCTRL: {
				return Enum::KeyCode::RightControl;
			}
			case SDLK_CAPSLOCK: {
				return Enum::KeyCode::CapsLock;
			}
			case SDLK_NUMLOCKCLEAR: {
				return Enum::KeyCode::NumLock;
			}
			case SDLK_SCROLLLOCK: {
				return Enum::KeyCode::ScrollLock;
			}
			case SDLK_MODE: {
				return Enum::KeyCode::Mode;
			}
			case SDLK_APPLICATION: {
				return Enum::KeyCode::Compose;
			}
			case SDLK_HELP: {
				return Enum::KeyCode::Help;
			}
			case SDLK_PRINTSCREEN: {
				return Enum::KeyCode::PrintSc;
			}
			case SDLK_SYSREQ: {
				return Enum::KeyCode::SysRq;
			}
			case SDLK_MENU: {
				return Enum::KeyCode::Menu;
			}
			case SDLK_POWER: {
			    return Enum::KeyCode::Power;
			}
			case SDLK_UNDO: {
				return Enum::KeyCode::Undo;
			}
			case SDLK_AGAIN: {
				return Enum::KeyCode::Redo;
			}
			case SDLK_CUT: {
				return Enum::KeyCode::Cut;
			}
			case SDLK_COPY: {
				return Enum::KeyCode::Copy;
			}
			case SDLK_PASTE: {
				return Enum::KeyCode::Paste;
			}
			case SDLK_F1: {
				return Enum::KeyCode::F1;
			}
			case SDLK_F2: {
				return Enum::KeyCode::F2;
			}
			case SDLK_F3: {
				return Enum::KeyCode::F3;
			}
			case SDLK_F4: {
				return Enum::KeyCode::F4;
			}
			case SDLK_F5: {
				return Enum::KeyCode::F5;
			}
			case SDLK_F6: {
				return Enum::KeyCode::F6;
			}
			case SDLK_F7: {
				return Enum::KeyCode::F7;
			}
			case SDLK_F8: {
				return Enum::KeyCode::F8;
			}
			case SDLK_F9: {
				return Enum::KeyCode::F9;
			}
			case SDLK_F10: {
				return Enum::KeyCode::F10;
			}
			case SDLK_F11: {
				return Enum::KeyCode::F11;
			}
			case SDLK_F12: {
				return Enum::KeyCode::F12;
			}
			case SDLK_F13: {
				return Enum::KeyCode::F13;
			}
			case SDLK_F14: {
				return Enum::KeyCode::F14;
			}
			case SDLK_F15: {
				return Enum::KeyCode::F15;
			}
			case SDLK_F16: {
				return Enum::KeyCode::F16;
			}
			case SDLK_F17: {
				return Enum::KeyCode::F17;
			}
			case SDLK_F18: {
				return Enum::KeyCode::F18;
			}
			case SDLK_F19: {
				return Enum::KeyCode::F19;
			}
			case SDLK_F20: {
				return Enum::KeyCode::F20;
			}
			case SDLK_F21: {
				return Enum::KeyCode::F21;
			}
			case SDLK_F22: {
				return Enum::KeyCode::F22;
			}
			case SDLK_F23: {
				return Enum::KeyCode::F23;
			}
			case SDLK_F24: {
				return Enum::KeyCode::F24;
			}
			case SDLK_WWW: {
				return Enum::KeyCode::WWW;
			}
			case SDLK_AUDIOPREV: {
				return Enum::KeyCode::MediaPrevious;
			}
			case SDLK_AUDIONEXT: {
				return Enum::KeyCode::MediaNext;
			}
			case SDLK_AUDIOPLAY: {
				return Enum::KeyCode::MediaPlayPause;
			}
			case SDLK_AUDIOSTOP: {
				return Enum::KeyCode::MediaStop;
			}
			default: {
				printf("[INPUTRECEIVER] [SDL2] UNKNOWN KEY: %i\n", ksym.sym);
			}
		}
		return Enum::KeyCode::Unknown;
	}

    void OBInputEventReceiver::processSDL2Event(SDL_Event& evt){
		shared_ptr<Instance::DataModel> dm = eng->getDataModel();
		if(dm){
			shared_ptr<Instance::UserInputService> uis = dm->getUserInputService();
			if(uis){
				switch(evt.type){
					case SDL_KEYDOWN: {
						// For now, throw out repeat events
						if(!evt.key.repeat){
							Enum::KeyCode keyCode = sdl2KeyToOB(evt.key.keysym);
							uis->input_keyEvent(keyCode, true);
						}
						break;
					}
					case SDL_KEYUP: {
					    // For now, throw out repeat events
						if(!evt.key.repeat){
							Enum::KeyCode keyCode = sdl2KeyToOB(evt.key.keysym);
							uis->input_keyEvent(keyCode, false);
						}
						break;
					}
					case SDL_MOUSEMOTION: {
						uis->input_mouseMoved(make_shared<Type::Vector2>(evt.motion.x, evt.motion.y), make_shared<Type::Vector2>(evt.motion.xrel, evt.motion.yrel));
						break;
					}
					case SDL_MOUSEBUTTONDOWN: {
						Enum::MouseButton mbtn = Enum::MouseButton::Unknown;

						switch(evt.button.button){
							case SDL_BUTTON_LEFT: {
								mbtn = Enum::MouseButton::Left;
								break;
							}
							case SDL_BUTTON_MIDDLE: {
								mbtn = Enum::MouseButton::Middle;
								break;
							}
							case SDL_BUTTON_RIGHT: {
								mbtn = Enum::MouseButton::Right;
								break;
							}
							case SDL_BUTTON_X1: {
								mbtn = Enum::MouseButton::X1;
								break;
							}
							case SDL_BUTTON_X2: {
								mbtn = Enum::MouseButton::X2;
								break;
							}
						}

						uis->input_mouseButton(mbtn, true);
						break;
					}
					case SDL_MOUSEBUTTONUP: {
						Enum::MouseButton mbtn = Enum::MouseButton::Unknown;

						switch(evt.button.button){
							case SDL_BUTTON_LEFT: {
								mbtn = Enum::MouseButton::Left;
								break;
							}
							case SDL_BUTTON_MIDDLE: {
								mbtn = Enum::MouseButton::Middle;
								break;
							}
							case SDL_BUTTON_RIGHT: {
								mbtn = Enum::MouseButton::Right;
							}
							case SDL_BUTTON_X1: {
								mbtn = Enum::MouseButton::X1;
							}
							case SDL_BUTTON_X2: {
								mbtn = Enum::MouseButton::X2;
							}
						}

						uis->input_mouseButton(mbtn, false);
					}
					case SDL_MOUSEWHEEL: {
						uis->input_mouseWheel(make_shared<Type::Vector2>(evt.wheel.x, evt.wheel.y));
						break;
					}
				}
			}
		}
	}
#endif

    void OBInputEventReceiver::focus(){
	    shared_ptr<Instance::DataModel> dm = eng->getDataModel();
		if(dm){
			shared_ptr<Instance::UserInputService> uis = dm->getUserInputService();
			if(uis){
				uis->getWindowFocused()->Fire(eng, std::vector<shared_ptr<Type::VarWrapper>>());
			}
		}
	}

	void OBInputEventReceiver::unfocus(){
		shared_ptr<Instance::DataModel> dm = eng->getDataModel();
		if(dm){
			shared_ptr<Instance::UserInputService> uis = dm->getUserInputService();
			if(uis){
				uis->getWindowFocusReleased()->Fire(eng, std::vector<shared_ptr<Type::VarWrapper>>());
			}
		}
	}

	void OBInputEventReceiver::input_mouseButton(Enum::MouseButton btn, bool state){
		shared_ptr<Instance::DataModel> dm = eng->getDataModel();
		if(dm){
			shared_ptr<Instance::UserInputService> uis = dm->getUserInputService();
			if(uis){
				uis->input_mouseButton(btn, state);
			}
		}
	}

	void OBInputEventReceiver::input_mouseWheel(shared_ptr<Type::Vector2> delta){
		shared_ptr<Instance::DataModel> dm = eng->getDataModel();
		if(dm){
			shared_ptr<Instance::UserInputService> uis = dm->getUserInputService();
			if(uis){
				uis->input_mouseWheel(delta);
			}
		}
	}

	void OBInputEventReceiver::input_mouseMoved(shared_ptr<Type::Vector2> pos, shared_ptr<Type::Vector2> delta){
		shared_ptr<Instance::DataModel> dm = eng->getDataModel();
		if(dm){
			shared_ptr<Instance::UserInputService> uis = dm->getUserInputService();
			if(uis){
				uis->input_mouseMoved(pos, delta);
			}
		}
	}

	void OBInputEventReceiver::input_keyEvent(Enum::KeyCode keyCode, bool state){
		shared_ptr<Instance::DataModel> dm = eng->getDataModel();
		if(dm){
			shared_ptr<Instance::UserInputService> uis = dm->getUserInputService();
			if(uis){
				uis->input_keyEvent(keyCode, state);
			}
		}
	}

	OBEngine* OBInputEventReceiver::getEngine(){
		return eng;
	}
}
