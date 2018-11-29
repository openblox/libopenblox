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

#ifndef OB_OBRENDERUTILS
#define OB_OBRENDERUTILS

#include "oblibconfig.h"

#include "obtype.h"
#include "mem.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>

namespace OB{
#ifndef OB_OBENGINE
	class OBEngine;
#endif

	/**
	 * Rendering related utilities and complete hacks
	 *
	 * @author John M. Harris, Jr.
	 * @date November 2018
	 */
	class OBRenderUtils{
		public:
			OBRenderUtils(OBEngine* eng);
			virtual ~OBRenderUtils();

			/**
			 * Returns the OBEngine instance
			 *
			 * @returns Instance of OBEngine
			 * @author John M. Harris, Jr.
			 */
		    OBEngine* getEngine();

			/**
			 * Prepares the rendering pipeline for 2D rendering.
			 *
			 * @author John M. Harris, Jr.
			 */
			void prepare2DMode();

			/**
			 * Ends a 2D context
			 *
			 * @author John M. Harris, Jr.
			 */
			void end2DMode();

			/**
			 * Save an image of the last frame to a file.
			 *
			 * @param file File to write to
			 * @author John M. Harris, Jr.
			 */
			bool saveScreenshot(std::string file);

			/**
			 * Returns the currently active Irrlicht device, if any.
			 *
			 * @returns Irrlicht device
			 * @author John M. Harris, Jr.
			 */
			irr::IrrlichtDevice* getIrrlichtDevice();

		private:
			OBEngine* eng;

			bool cached2DMode;

			irr::IrrlichtDevice* irrDev;
			irr::video::IVideoDriver* irrDriv;
			irr::scene::ISceneManager* irrSceneMgr;
	};
}

#endif // HAVE_IRRLICHT

#endif // OB_OBRENDERUTILS

// Local Variables:
// mode: c++
// End:
