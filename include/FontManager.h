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

#ifndef OB_FONTMANAGER
#define OB_FONTMANAGER

#include "oblibconfig.h"

#include "obtype.h"
#include "mem.h"

#if HAVE_IRRLICHT && HAVE_SDL2 && HAVE_FONTCONFIG
#include <irrlicht/irrlicht.h>

struct _FcConfig;
typedef struct _FcConfig FcConfig;

namespace OB{
#ifndef OB_OBENGINE
	class OBEngine;
#endif
#ifndef OB_FONT
	class Font;
#endif

	/**
	 * Font management abstraction
	 *
	 * @author John M. Harris, Jr.
	 * @date July 2019
	 */
	class FontManager{
		public:
			FontManager(OBEngine* eng);
			virtual ~FontManager();

			/**
			 * Returns the OBEngine instance
			 *
			 * @returns Instance of OBEngine
			 * @author John M. Harris, Jr.
			 */
		    OBEngine* getEngine();

			/**
			 * Returns the path to a font file, or NULL.
			 * Make sure to free() the result!
			 *
			 * @internal
			 * @returns path to font file or NULL if this family is not found
			 * @author John M. Harris, Jr.
			 */
			char* _getFontFile(std::string fontFamily);

			/**
			 * Returns a font from a given font name.
			 *
			 * @returns Font or NULL if no font is found
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<Font> getFont(std::string fontName, unsigned int fontSize);

			/**
			 * Returns a "default font" (the system's
			 * default monospace font) at size 16.
			 *
			 * @returns The default font, or NULL if not loadable
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<Font> getDefaultFont();
		private:
			OBEngine* eng;

			shared_ptr<Font> defaultFont;

			FcConfig* fc_cfg;
	};
}

#endif // HAVE_IRRLICHT && HAVE_SDL2 && HAVE_FONTCONFIG

#endif // OB_FONTMANAGER

// Local Variables:
// mode: c++
// End:
