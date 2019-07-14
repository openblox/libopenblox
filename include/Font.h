/*
 * Copyright (C) 2019 John M. Harris, Jr. <johnmh@openblox.org>
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

#ifndef OB_FONT
#define OB_FONT

#include "oblibconfig.h"

#include "obtype.h"
#include "mem.h"

#if HAVE_IRRLICHT && HAVE_SDL2 && HAVE_FONTCONFIG
#include <irrlicht/irrlicht.h>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>

#ifdef vector
#undef vector
#endif
#ifdef pixel
#undef pixel
#endif
#ifdef bool
#undef bool
#endif

#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

namespace OB{
	/**
	 * OpenGL Font class
	 *
	 * @author John M. Harris, Jr.
	 * @date July 2019
	 */
	class Font{
		public:
			Font(const char* fileName, unsigned int h);
			virtual ~Font();

			void print(float x, float y, size_t maxlen, const char* fmt, ...);

		private:
			float height;
			GLuint* textures;
			GLuint list_base;
	};
}

#endif // HAVE_IRRLICHT && HAVE_SDL2 && HAVE_FONTCONFIG

#endif // OB_FONTMANAGER

// Local Variables:
// mode: c++
// End:
