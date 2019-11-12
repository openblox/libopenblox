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

#include "OBRenderUtils.h"

#include "OBEngine.h"

#if HAVE_IRRLICHT

#if HAVE_SDL2
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
#endif

namespace OB{
	int OBRenderUtils::stencilMaskDepth = 0;

	OBRenderUtils::OBRenderUtils(OBEngine* eng){
		this->eng = eng;
		irrDev = eng->getIrrlichtDevice();
		irrDriv = irrDev->getVideoDriver();
		irrSceneMgr = irrDev->getSceneManager();

		cached2DMode = false;
	}

    OBRenderUtils::~OBRenderUtils(){}

	OBEngine* OBRenderUtils::getEngine(){
		return eng;
	}

	void OBRenderUtils::prepare2DMode(){
		if(!cached2DMode && eng->doesRendering() && irrDriv){
			cached2DMode = true;

			// Dirty hack, maybe just set a material in the future?
			irrDriv->drawPixel(0, 0, irr::video::SColor(0, 255, 255, 255));
		}
	}

	void OBRenderUtils::end2DMode(){
	    cached2DMode = false;
	}

	bool OBRenderUtils::saveScreenshot(std::string file){
		if(eng->doesRendering()){
			if(irrDriv){
				irr::video::IImage* img = irrDriv->createScreenShot();
				if(img){
					return irrDriv->writeImageToFile(img,  irr::io::path(file.c_str()));
				}
			}
		}
		return false;
	}

	// STATIC FUNCTIONS

#if HAVE_SDL2
	void OBRenderUtils::pushScreenCoordinateMatrix(){
		glPushAttrib(GL_TRANSFORM_BIT);
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(viewport[0], viewport[2], viewport[1], viewport[3], -1, 1);

		glPopAttrib();
	}

	void OBRenderUtils::popProjectionMatrix(){
		glPushAttrib(GL_TRANSFORM_BIT);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();
	}
#endif
}

#endif
