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

#ifndef OB_OBINPUTEVENTRECIVER
#define OB_OBINPUTEVENTRECIVER

#include "oblibconfig.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>

namespace OB{
#ifndef OB_OBENGINE
	class OBEngine;
#endif

	/**
	 * This class handles basic input from Irrlicht.
	 *
	 * @author John M. Harris, Jr.
	 * @date November 2018
	 */
	class OBInputEventReceiver: public irr::IEventReceiver{
		public:
			OBInputEventReceiver(OBEngine* eng);
			virtual ~OBInputEventReceiver();

			virtual bool OnEvent(const irr::SEvent& evt);

			OBEngine* getEngine();

		private:
			OBEngine* eng;
	};
}
#endif

#endif // OB_OBINPUTEVENTRECIVER

// Local Variables:
// mode: c++
// End:
