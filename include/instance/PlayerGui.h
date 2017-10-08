/*
 * Copyright (C) 2016 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "instance/BasePlayerGui.h"

#ifndef OB_INST_PLAYERGUI
#define OB_INST_PLAYERGUI

namespace OB{
	namespace Instance{
		/**
		 * PlayerGui is a non-persistent container for UI elements to
		 * be shown to the end user. Usually the contents of
		 * StarterGui are copied here when the Player object is
		 * initialized.
		 *
		 * @author John M. Harris, Jr.
		 */
		class PlayerGui: public BasePlayerGui{
		public:
			PlayerGui(OBEngine* eng);
			virtual ~PlayerGui();

			DECLARE_CLASS(PlayerGui);
		};
	}
}

#endif // OB_INST_


// Local Variables:
// mode: c++
// End:
