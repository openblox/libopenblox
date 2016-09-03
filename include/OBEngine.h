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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox.	 If not, see <https://www.gnu.org/licenses/>.
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

#include "OBLogger.h"

#include <lua.h>

#ifndef __I_IRRLICHT_DEVICE_H_INCLUDED__
namespace irr{
	class IrrlichtDevice;

	namespace video{
		class IVideoDriver;
	}

	namespace scene{
		class ISceneManager;
	}
}
#endif

namespace OB{
	#ifndef OB_TASKSCHEDULER
	class TaskScheduler;
	#endif
	
	/**
	 * This is the main class of the OpenBlox engine. This class is
	 * the hierarchical parent of everything else in the engine,
	 * including the DataModel, InputManager and SoundManager.
	 *
	 * @author John M. Harris, Jr.
	 * @date May 2016
	 */
	class OBEngine{
		public:
			OBEngine();
			virtual ~OBEngine();

			/**
			 * Returns the global instance of OBEngine.
			 *
			 * @returns Global instance of OBEngine
			 * @author John M. Harris, Jr.
			 */
			static OBEngine* getInstance();

			/**
			 * Initializes the OpenBlox engine.
			 *
			 * You must have set all initialization parameters
			 * before calling this, otherwise you will not be
			 * able to change many of them.
			 *
			 * @author John M. Harris, Jr.
			 */
			void init();

			/** 
			 * Returns true if OpenBlox is still running.
			 *
			 * @returns bool isRunning
			 * @author John M. Harris, Jr.
			 */
			bool isRunning();

			/**
			 * Runs the logic of the OpenBlox engine.
			 *
			 * This runs a single "tick" of the logic thread,
			 * including running physics and the Lua task scheduler.
			 *
			 * When running with rendering enabled, this also processes
			 * messages from the operating system and input.
			 *
			 * @author John M. Harris, Jr.
			 */
			void tick();

			/**
			 * Renders one frame of the DataModel.
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
			 * @returns Global Lua state
			 * @author John M. Harris, Jr.
			 */
			lua_State* getGlobalLuaState();

			/**
			 * Returns the time the OpenBlox engine was started,
			 * in milliseconds.
			 *
			 * @returns Start time started in milliseconds
			 * @author John M. Harris, Jr.
			 */
			ob_int64 getStartTime();

			/**
			 * Returns true if OBEngine is meant to handle
			 * rendering.
			 *
			 * @returns true if this instance of OBEngine supports rendering
			 * @author John M. Harris, Jr.
			 */
			bool doesRendering();

			/**
			 * Sets whether or not OBEngine is meant to handle
			 * rendering.
			 *
			 * @param renders true if this instance of OBEngine is supposed to support rendering, otherwise false
			 * @author John M. Harris, Jr.
			 */
			void setRendering(bool renders);

			/**
			 * Gets the initial width of the render window.
			 * Defaults to 640.
			 * 
			 * @returns Initial width of render window
			 * @author John M. Harris, Jr.
			 */
			int getInitWidth();

			/**
			 * Sets the initial width of the render window.
			 *
			 * @param w Initial width of render window
			 * @author John M. Harris, Jr.
			 */
			void setInitWidth(int w);

			/**
			 * Gets the initial height of the render window.
			 * Defaults to 480.
			 *
			 * @returns Initial height of render window
			 * @author John M. Harris, Jr.
			 */
			int getInitHeight();

			/**
			 * Sets the initial height of the render window.
			 *
			 * @param h Initial height of render window
			 * @author John M. Harris, Jr.
			 */
			void setInitHeight(int h);

			/**
			 * Returns true if the OpenBlox engine will use
			 * vertical synchronization, otherwise false.
			 *
			 * @returns true if OpenBlox is supposed to use vsync
			 * @author John M. Harris, Jr.
			 */
			bool getUseVsync();

			/**
			 * Sets whether or not the OpenBlox engine will
			 * use vertical synchronization.
			 *
			 * @param useVsync true if OpenBlox is supposed to use vsync, otherwise false
			 * @author John M. Harris, Jr.
			 */
			void setUseVsync(bool useVsync);

			/**
			 * Gets the current underlying window ID. With X
			 * this is a Window handle (A.K.A. XID A.K.A.
			 * unsigned long int), on Windows this is an
			 * HWND, their equivalent of a window handle.
			 * 
			 * Defaults to NULL.
			 *
			 * @returns System-dependent window ID
			 * @author John M. Harris, Jr.
			 */
			void* getWindowId();

			/**
			 * Sets the current underlying window ID. The
			 * same types apply as used with OBEngine::getWindowId
			 *
			 * Setting this to NULL will mean that the
			 * engine will create a window for itself.
			 *
			 * @param System-dependent window ID
			 * @author John M. Harris, Jr.
			 */
			void setWindowId(void* wId);
		
		private:
			//State helpers
			bool initialized;
			ob_int64 startTime;
			bool _isRunning;

			//Init options
			bool doRendering;
			int startWidth;
			int startHeight;
			bool vsync;
			void* windowId;

			lua_State* globalState;

			irr::IrrlichtDevice* irrDev;
			irr::video::IVideoDriver* irrDriv;
			irr::scene::ISceneManager* irrSceneMgr;

			TaskScheduler* taskSched;

			static OBEngine* inst;
	};
}

#endif // OB_OBENGINE

// Local Variables:
// mode: c++
// End:
