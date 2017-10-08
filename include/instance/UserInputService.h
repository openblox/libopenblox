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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "instance/Instance.h"

#ifndef OB_INST_USERINPUTSERVICE
#define OB_INST_USERINPUTSERVICE

#include "type/Enum.h"

namespace OB{
	namespace Instance{
		/**
		 * UserInputService is used to handle input.
		 *
		 * @author John M. Harris, Jr.
		 */
		class UserInputService: public Instance{
		public:
			UserInputService(OBEngine* eng);
			virtual ~UserInputService();

#if HAVE_PUGIXML
			virtual std::string serializedID();
#endif

			shared_ptr<Type::Event> getGamepadConnected();

			static void register_lua_events(lua_State* L);

			DECLARE_CLASS(UserInputService);

			shared_ptr<Type::Event> GamepadConnected;
			shared_ptr<Type::Event> GamepadDisconnected;
		};
	}
}

#endif // OB_INST_USERINPUTSERVICE


// Local Variables:
// mode: c++
// End:
