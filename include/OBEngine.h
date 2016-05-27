/*
 * Copyright (C) 2016 John M. Harris, Jr. <johnmh@openblox.org>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file OBEngine.h
 * @author John M. Harris, Jr.
 * @date May 2016
 * 
 * This file defines the OBEngine singleton, which is the root of the
 * OpenBlox game engine.
 */

#ifndef OB_OBENGINE
#define OB_OBENGINE

#include "obtype.h"

#include <lua.h>

#ifndef __I_IRRLICHT_DEVICE_H_INCLUDED__
namespace irr{
	class IrrlichtDevice;
}
#endif

namespace OB{
	/**
	 * This is the main class of the OpenBlox engine. This class is
	 * the hierarchical parent of everything else in the engine,
	 * including the DataModel, InputManager and SoundManager.
	 *
	 * author John M. Harris, Jr.
	 * @date May 2016
	 */
	class OBEngine{
		public:
			OBEngine();
			virtual ~OBEngine();

			/**
			 * Returns the global instance of OBEngine.
			 * @returns OBEngine*
			 * @author John M. Harris, Jr.
			 */
			static OBEngine* getInstance();

			/**
			 * @brief Initializes the OpenBlox engine.
			 *
			 * You must have set all initialization parameters
			 * before calling this, otherwise you will not be
			 * able to change many of them.
			 *
			 * @author John M. Harris, Jr.
			 */
			void init();

			/**
			 * @brief Runs the logic of the OpenBlox engine.
			 *
			 * This runs a single "tick" of the logic thread,
			 * including running physics and the Lua task scheduler.
			 *
			 * @author John M. Harris, Jr.
			 */
			void tick();

			/**
			 * @brief Renders one frame of the DataModel.
			 * 
			 * This is normally called after OBEngine::tick
			 *
			 * @author John M. Harris, Jr.
			 */
			void render();

			/**
			 * Returns the global Lua state, which all states
			 * used by the engine are coroutines of.
			 *
			 * @returns lua_State*
			 * @author John M. Harris, Jr.
			 */
			lua_State* getGlobalLuaState();

			/**
			 * Returns the time the OpenBlox engine was started,
			 * in milliseconds.
			 *
			 * @returns ob_int64
			 * @author John M. Harris, Jr.
			 */
			ob_int64 getStartTime();

			/**
			 * Returns true if OBEngine is meant to create its
			 * own window on initialization.
			 *
			 * @returns bool
			 * @author John M. Harris, Jr.
			 */
			bool getCreateOwnWindow();

			/*
			 * Sets whether or not OBEngine is meant to create
			 * its own window on initialization.
			 *
			 * @param bool createOwnWindow
			 * @author John M. Harris, Jr.
			 */
			void setCreateOwnWindow(bool cow);
			
		private:
			//State helpers
			bool initialized;
			ob_int64 startTime;

			//Init options
			bool createOwnWindow;
			int startWidth;
			int startHeight;
		
			lua_State* globalState;

			static OBEngine* inst;
	};
}

#endif // OB_OBENGINE

// Local Variables:
// mode: c++
// End:
