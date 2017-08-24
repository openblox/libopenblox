/*
 * Copyright (C) 2017 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "OBEngine.h"
#include "config.h"

#include "type/Color3.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#include "GL/gl.h"
#endif

#ifndef OB_RENDERUTILS
#define OB_RENDERUTILS

#if HAVE_IRRLICHT
namespace OB{
	class RenderUtils{
		public:
			static irr::video::IVideoDriver* getDriver(OBEngine* eng);
			static void prepare2D(OBEngine* eng);
	};
}
#endif

#endif // OB_RENDERUTILS

// Local Variables:
// mode: c++
// End:
