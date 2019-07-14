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

#include "FontManager.h"

#include "Font.h"

#if HAVE_IRRLICHT && HAVE_SDL2 && HAVE_FONTCONFIG

#include "OBException.h"

#include <string.h>

#include <fontconfig/fontconfig.h>

namespace OB{
	FontManager::FontManager(OBEngine* eng){
		this->eng = eng;

		defaultFont = NULL;

	    fc_cfg = FcInitLoadConfigAndFonts();
		if(!fc_cfg){
			throw new OBException("Failed to initialize fontconfig.");
		}
		FcConfigSetRescanInterval(fc_cfg, 0);
	}

	FontManager::~FontManager(){}

    OBEngine* FontManager::getEngine(){
		return eng;
	}

	// MAKE SURE THE RESULT IS `free()`d!
    char* FontManager::_getFontFile(std::string fontName){
		FcPattern* pat = FcNameParse((const FcChar8*)(fontName.c_str()));
		FcConfigSubstitute(fc_cfg, pat, FcMatchPattern);
		FcDefaultSubstitute(pat);

		char* file = NULL;

		FcResult result = FcResultNoMatch;
		FcPattern* font = FcFontMatch(fc_cfg, pat, &result);
		if(font){
			FcChar8* fontFile = NULL;

			if(FcPatternGetString(font, FC_FILE, 0, &fontFile) == FcResultMatch){
				file = strdup((char*)fontFile);
			}

			FcPatternDestroy(font);
		}

		FcPatternDestroy(pat);

	    return file;
	}

	shared_ptr<Font> FontManager::getFont(std::string fontName, unsigned int fontSize){
		char* fontFile = _getFontFile(fontName);
		if(fontFile){
		    shared_ptr<Font> fFont = make_shared<Font>(fontFile, fontSize);

			free(fontFile);

			return fFont;
		}
		return NULL;
	}

	shared_ptr<Font> FontManager::getDefaultFont(){
		if(!defaultFont){
			defaultFont = getFont("monospace", 16);
		}
		return defaultFont;
	}
}

#endif
