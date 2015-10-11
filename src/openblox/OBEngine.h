/*
 * Copyright 2015 John M. Harris, Jr.
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
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenBlox.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file OBEngine.h
 * @author John M. Harris, Jr.
 * @date October 2015
 *
 * This file is defines the OBEngine singleton.
 */

#ifndef OPENBLOX_OBENGINE_H_
#define OPENBLOX_OBENGINE_H_

#include "OpenBlox.h"

#include "OBGame.h"

namespace OpenBlox{
	class OBEngine{
		public:
			OBEngine();
			virtual ~OBEngine();

			/**
			 * Returns the global instance of OBEngine
			 * @returns OBEngine* engine
			 * @author John M. Harris, Jr.
			 */
			static OBEngine* getInstance();

			#ifndef OB_NO_GRAPHICS

			/**
			 * @brief Used to set the Ogre::Root* to be used by OpenBlox.
			 *
			 * If this is set to NULL, OpenBlox will not attempt to render anything and will not bind instances to a scenegraph.
			 * THIS MUST BE SET BEFORE YOU RUN OBEngine::init() !
			 * @param Ogre::Root* root
			 * @author John M. Harris, Jr.
			 */
			void setOgreRoot(Ogre::Root* root);

			/**
			 * Returns the currently set Ogre::Root*
			 * @returns Ogre::Root*
			 * @author John M. Harris, Jr.
			 */
			Ogre::Root* getOgreRoot();

			/**
			 * @brief Used to set the Ogre::RenderWindow* to be used by OpenBlox.
			 *
			 * THIS MUST BE SET BEFORE YOU RUN OBEngine::init() !
			 * @param Ogre::RenderWindow* renderWindow
			 * @author John M. Harris, Jr.
			 */
			void setRenderWindow(Ogre::RenderWindow* renderWindow);

			/**
			 * Returns the currently set Ogre::RenderWindow*
			 * @returns Ogre::RenderWindow*
			 * @author John M. Harris, Jr.
			 */
			Ogre::RenderWindow* getRenderWindow();

			/**
			 * Returns the current Ogre::SceneManager*
			 * @returns Ogre::SceneManager*
			 * @author John M. Harris, Jr.
			 */
			Ogre::SceneManager* getSceneManager();

			/**
			 * Returns the current Ogre::Camera*
			 * @returns Ogre::Camera*
			 * @author John M. Harris, Jr.
			 */
			Ogre::Camera* getCamera();

			/**
			 * Returns the current Ogre::Viewport*
			 * @returns Ogre::Viewport*
			 * @author John M. Harris, Jr.
			 */
			Ogre::Viewport* getViewport();

			#endif

			/**
			 * @brief Sets whether or not this is a server instance.
			 *
			 * If this is set to true, server-only instances will be available.
			 * Server instances can still connect to other servers, so this may be useful for some people building clients.
			 * @param bool isServer
			 * @author John M. Harris, Jr.
			 */
			void setServer(bool isServer);

			/**
			 * Returns true if this is a server instance, otherwise false.
			 * @returns bool isServer
			 * @author John M. Harris, Jr.
			 */
			bool isServer();

			/**
			 * Returns true if this OBEngine instance has been initialized, otherwise false.
			 * @returns bool isInitialized
			 * @author John M. Harris, Jr.
			 */
			bool isInitialized();

			/**
			 * @brief Initializes this OBEngine instance.
			 *
			 * Make sure you've set all of OBEngine's settings to your liking before calling this.
			 *
			 * If you haven't set the Ogre::Root* and Ogre::RenderWindow* to be used by OpenBlox before calling this,
			 * you're going to want to have another look at your code. Perhaps read the documentation for OBEngine::setOgreRoot and OBEngine::setRenderWindow.
			 *
			 * @see OBEngine::setOgreRoot OBEngine::setRenderWindow
			 * @author John M. Harris, Jr.
			 */
			void init();

			/**
			 * Returns the current OBGame* instance, or NULL if not initialized.
			 * @returns OBGame* game instance
			 * @author John M. Harris, Jr.
			 */
			OBGame* getGame();

			/**
			 * @brief Runs the logic of the OpenBlox engine.
			 *
			 * Equivalent to OBGame::tick
			 *
			 * @author John M. Harris, Jr.
			 */
			void tick();

			/**
			 * @brief Renders one frame.
			 *
			 * Equivalent to Ogre::Root::renderOneFrame
			 *
			 * @author John M. Harris, Jr.
			 */
			void render();

			/**
			 * Called when the window is resized.
			 * @author John M. Harris, Jr.
			 */
			void resized(unsigned int width, unsigned int height);

			//Input Injection Methods
			void mousePress(uint8_t btn, QPoint pos);
			void mouseRelease(uint8_t btn, QPoint pos);
			void mouseMove(QPoint pos);
			void mouseWheel(int delta, QPoint pos);
			void keyDown(int32_t key, uint16_t mods);
			void keyUp(int32_t key, uint16_t mods);

			lua_State* getLuaState();

			qint64 getStartTime();

			void info(QString output);
			void print(QString output);
			void warn(QString output);
			void print_error(QString output);

		private:
			bool started;
			bool is_Server;

			#ifndef OB_NO_GRAPHICS
			Ogre::Root* root;
			Ogre::RenderWindow* renderWindow;

			Ogre::SceneManager* sceneMgr;
			Ogre::Camera* cam;
			Ogre::Viewport* vp;
			#endif

			qint64 startTime;

			lua_State* mainLuaState;

			OBGame* game;

			static OBEngine* inst;
	};
}

#endif
