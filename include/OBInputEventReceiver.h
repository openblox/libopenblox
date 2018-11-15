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

#ifndef OB_OBINPUTEVENTRECEIVER
#define OB_OBINPUTEVENTRECEIVER

#include "oblibconfig.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#endif

#include "type/Enum.h"
#include "type/Vector2.h"

#if HAVE_SDL2
#define SDL_MAIN_HANDLED
#include <SDL.h>
#ifdef vector
#undef vector
#endif
#ifdef pixel
#undef pixel
#endif
#ifdef bool
#undef bool
#endif
#endif

namespace OB{
#ifndef OB_OBENGINE
	class OBEngine;
#endif

	/**
	 * This class handles basic input from Irrlicht.
	 *
	 * @author John M. Harris, Jr.
	 * @date November 2018
	 */
#if HAVE_IRRLICHT
	class OBInputEventReceiver: public irr::IEventReceiver{
#else
	class OBInputEventReceiver{
#endif
		public:
			OBInputEventReceiver(OBEngine* eng);
			virtual ~OBInputEventReceiver();

			#if HAVE_IRRLICHT
			/**
			 * Converts an Irrlicht key code to an OB KeyCode
			 *
			 * @author John M. Harris, Jr.
			 */
			static Enum::KeyCode irrlictKeyToOB(irr::EKEY_CODE irrKey);

			/**
			 * Process an Irrlicht event
			 *
			 * @author John M. Harris, Jr.
			 */
			virtual bool OnEvent(const irr::SEvent& evt);
			#endif

			#if HAVE_SDL2
			static Enum::KeyCode sdl2KeyToOB(SDL_Keysym& ksym);

			/**
			 * Process an SDL input event
			 *
			 * Non-input events should be handled elsewhere
			 *
			 * @author John M. Harris, Jr.
			 */
		    void processSDL2Event(SDL_Event& evt);
			#endif

			/**
			 * Convenience method that fires the WindowFocused event of
			 * Instance::UserInputService
			 *
			 * @author John M. Harris, Jr.
			 */
			void focus();

		    /**
			 * Convenience method that fires the WindowFocusReleased
			 * event of Instance::UserInputService
			 *
			 * @author John M. Harris, Jr.
			 */
			void unfocus();

			/**
			 * Convenience method that calls
			 * Instance::UserInputService::input_mouseButton
			 *
			 * @author John M. Harris, Jr.
			 */
			void input_mouseButton(Enum::MouseButton btn, bool state);

			/**
			 * Convenience method that calls
			 * Instance::UserInputService::input_mouseWheel
			 *
			 * @author John M. Harris, Jr.
			 */
			void input_mouseWheel(shared_ptr<Type::Vector2> delta);

			/**
			 * Convenience method that calls
			 * Instance::UserInputService::input_mouseMoved
			 *
			 * @author John M. Harris, Jr.
			 */
			void input_mouseMoved(shared_ptr<Type::Vector2> pos, shared_ptr<Type::Vector2> delta);

			/**
			 * Convenience method that calls
			 * Instance::UserInputService::input_keyEvent
			 *
			 * @author John M. Harris, Jr.
			 */
			void input_keyEvent(Enum::KeyCode keyCode, bool state);

			OBEngine* getEngine();

		private:
			OBEngine* eng;
	};
}

#endif // OB_OBINPUTEVENTRECEIVER

// Local Variables:
// mode: c++
// End:
