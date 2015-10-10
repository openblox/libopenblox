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
 * @file OpenBloxGlobal.h
 * @author John M. Harris, Jr.
 * @date July 2015
 *
 * This file is included into OpenBlox.h, allowing anything here to be globally included.
 */

#ifndef OPENBLOX_OPENBLOXGLOBAL_H_
#define OPENBLOX_OPENBLOXGLOBAL_H_

//C++ stdlib
#include <string>
#include <vector>

//SDL
#ifndef __ANDROID__
	#define SDL_MAIN_HANDLED
#endif
#include <SDL2/SDL.h>

#ifndef OB_NO_GRAPHICS
	//OGRE
	#include <OGRE/Ogre.h>

	namespace OpenBlox{
		extern Ogre::Root* root;
		extern Ogre::RenderWindow* renderWindow;
		extern Ogre::SceneManager* sceneMgr;
		extern Ogre::Camera* cam;
		extern Ogre::Viewport* vp;

		extern SDL_Window* mw;
		extern SDL_GLContext maincontext;
	}
#endif

namespace OpenBlox{
	extern bool shouldQuit;
	extern int taskThreadFunc(void* ptr);
}

#define OB_UNUSED(any_thing) (void)any_thing

#include <ob_lua.h>

#include "static_init.h"

#include "OBException.h"
#include "OBGame.h"

#endif
