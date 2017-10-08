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

#ifndef OB_INST_PLAYERS
#define OB_INST_PLAYERS

namespace OB{
	namespace Instance{
		/**
		 * The Players service provides an instance to represent
		 * players currently in the game. It is recommended, but not
		 * required, that a Player object is created with the
		 * CreatePlayer method of ClientReplicator during or before
		 * the authentication stage of an incoming connection.
		 *
		 * When the RobloxCompatMode property of DataModel is true,
		 * this service will automatically create a Player object for
		 * connecting players and will automatically create a local
		 * player in some situations.
		 *
		 * @sa \ref roblox-compat "Roblox Compatibility Mode"
		 * @author John M. Harris, Jr.
		 */
		class Players: public Instance{
		public:
			Players(OBEngine* eng);
			virtual ~Players();

#if HAVE_PUGIXML
			virtual std::string serializedID();
#endif

			virtual void removeChild(shared_ptr<Instance> kid);
			virtual void addChild(shared_ptr<Instance> kid);

			shared_ptr<Type::Event> getPlayerAdded();
			shared_ptr<Type::Event> getPlayerRemoving();

			static void register_lua_events(lua_State* L);

			DECLARE_CLASS(Players);

			shared_ptr<Type::Event> PlayerAdded;
			shared_ptr<Type::Event> PlayerRemoving;
		};
	}
}

#endif // OB_INST_PLAYERS


// Local Variables:
// mode: c++
// End:
