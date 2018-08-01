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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
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

#include "oblibconfig.h"

#include "obtype.h"
#include "mem.h"

#include "OBLogger.h"
#include "AssetLocator.h"
#include "OBSerializer.h"
#include "PluginManager.h"

#include <lua/OBLua.h>

#include <instance/DataModel.h>

#include <pthread.h>

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#endif

namespace OB{
#ifndef OB_TASKSCHEDULER
	class TaskScheduler;
#endif
#ifndef OB_ASSETLOCATOR
	class AssetLocator;
#endif
#ifndef OB_PLUGINMANAGER
	class PluginManager;
#endif

#if HAVE_IRRLICHT
	//typedef void (*post_render_func_t)(irr::video::IVideoDriver*);
	typedef std::function<void(irr::video::IVideoDriver*)> post_render_func_t;
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
			 * Returns primary TaskScheduler.
			 *
			 * @returns Instance of TaskScheduler
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<TaskScheduler> getTaskScheduler();

			/**
			 * Returns secondary TaskScheduler.
			 *
			 * @returns Instance of TaskScheduler
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<TaskScheduler> getSecondaryTaskScheduler();

			/**
			 * Returns the serializer.
			 *
			 * @returns Instance of OBSerializer
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<OBSerializer> getSerializer();

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
			 * Sets the exit code of the OpenBlox engine.
			 *
			 * @param exitCode Exit code
			 *
			 * @author John M. Harris, Jr.
			 */
			void setExitCode(int exitCode);

			/**
			 * Gets the exit code of the OpenBlox engine.
			 *
			 * @returns Exit code
			 *
			 * @author John M. Harris, Jr.
			 */
			int getExitCode();

			/**
			 * Starts the shutdown process of the OpenBlox engine.
			 *
			 *
			 * @author John M. Harris, Jr.
			 */
			void shutdown();

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
			 * Prepares the rendering pipeline for 2D rendering.
			 *
			 * @author John M. Harris, Jr.
			 */
			void prepare2DMode();

			/**
			 * Save an image of the last frame to a file.
			 *
			 * @param file File to write to
			 * @author John M. Harris, Jr.
			 */
			bool saveScreenshot(std::string file);

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
			ob_uint64 getStartTime();

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
			* Returns true if the OpenBlox engine window
			* will be resizable.
			*
			* @returns true if OpenBlox is supposed to be resizable
			* @author Tochigi
			*/
			bool getResizable();

			/**
			* Sets wether or not the OpenBlox engine window
			* will be resizable.
			*
			* @param Resizable true if the Openblox window is supposed to be resizable, otherwise false
			* @author Tochigi
			*/
			void setResizable(bool Resizable);

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
			 * @param wId System-dependent window ID
			 * @author John M. Harris, Jr.
			 */
			void setWindowId(void* wId);

			/**
			 * Used to inform the engine that a resize has occurred,
			 * this is only necessary (and should only be used) when
			 * OpenBlox is embedded in an external window.
			 *
			 * @param width Width
			 * @param height Height
			 * @author John M. Harris, Jr.
			 */
			void resized(int width, int height);

#if HAVE_IRRLICHT
			/**
			 * Returns the currently active Irrlicht device, if any.
			 *
			 * @returns Irrlicht device
			 * @author John M. Harris, Jr.
			 */
			irr::IrrlichtDevice* getIrrlichtDevice();

			/**
			 * Returns the post-render function.
			 *
			 * @author John M. Harris, Jr.
			 */
			post_render_func_t getPostRenderFunc();

			/**
			 * Sets the post-render function.
			 *
			 * As the name would suggest, this function will run after
			 * rendering is complete, but not before the buffer is
			 * swapped.
			 *
			 * @author John M. Harris, Jr.
			 */
			void setPostRenderFunc(post_render_func_t prf);
#endif

			/**
			 * Returns the DataModel.
			 *
			 * @returns DataModel
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<Instance::DataModel> getDataModel();

			/**
			 * Returns the AssetLocator.
			 *
			 * @returns AssetLocator
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<AssetLocator> getAssetLocator();

			/**
			 * Returns the PluginManager associated with this OBEngine
			 * instance.
			 *
			 * @returns PluginManager
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<PluginManager> getPluginManager();

			/**
			 * Returns the logger.
			 *
			 * @returns OBLogger
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<OBLogger> getLogger();

		private:
			// State helpers
			bool initialized;
			ob_uint64 startTime;
			bool _isRunning;
			int exitCode;
			pthread_t secondaryTaskThread;

			// Init options
			bool doRendering;
			int startWidth;
			int startHeight;
			bool vsync;
			void* windowId;
			bool resizable;

			lua_State* globalState;

#if HAVE_IRRLICHT

			post_render_func_t custPostRender;

			irr::IrrlichtDevice* irrDev;
			irr::video::IVideoDriver* irrDriv;
			irr::scene::ISceneManager* irrSceneMgr;

#endif

			shared_ptr<TaskScheduler> taskSched;
			shared_ptr<TaskScheduler> secondaryTaskSched;
			shared_ptr<AssetLocator> assetLocator;
			shared_ptr<PluginManager> pluginManager;
			shared_ptr<OBSerializer> serializer;
			shared_ptr<OBLogger> logger;
			shared_ptr<Instance::DataModel> dm;
	};
}

#endif // OB_OBENGINE

// Local Variables:
// mode: c++
// End:
