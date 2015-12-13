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

#ifndef OPENBLOX_TYPE_VIEWPORT_H_
#define OPENBLOX_TYPE_VIEWPORT_H_

#include <OpenBlox.h>
#include "OBType.h"

#include <enum/Enums.h>

BEGIN_OB_TYPE

#define lua_viewport_name "luaL_Viewport"

class Viewport{
	public:
		Viewport(Ogre::Viewport* realVP);
		virtual ~Viewport();

		DECLARE_GAME_STATIC_INIT(Viewport);

		Ogre::Viewport* realVP;

		bool equals(Viewport* other);

		Viewport* clone();

		static int lua_index(lua_State* L);
		static int lua_newindex(lua_State* L);

		static int lua_toString(lua_State* L);

		static int lua_getBackgroundColor(lua_State* L);
		static int lua_setBackgroundColor(lua_State* L);

		static int lua_getWidth(lua_State* L);
		static int lua_getHeight(lua_State* L);
		static int lua_getLeft(lua_State* L);
		static int lua_getTop(lua_State* L);
		static int lua_getActualWidth(lua_State* L);
		static int lua_getActualHeight(lua_State* L);
		static int lua_getActualLeft(lua_State* L);
		static int lua_getActualTop(lua_State* L);

		static int lua_getShadowsEnabled(lua_State* L);
		static int lua_getSkiesEnabled(lua_State* L);
		static int lua_setShadowsEnabled(lua_State* L);
		static int lua_setSkiesEnabled(lua_State* L);

		static int lua_getOrientationMode(lua_State* L);
		static int lua_setOrientationMode(lua_State* L);

		static int lua_eq(lua_State* L);

		int wrap_lua(lua_State* L);
};

Viewport* checkViewport(lua_State* L, int n);

END_OB_TYPE

#endif
