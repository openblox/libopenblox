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

#ifndef OB_PLUGIN
#define OB_PLUGIN

namespace OB{
    class OBEngine;

	/**
	 * All OpenBlox plugins inherit from this class.
	 *
	 * @author John M. Harris, Jr.
	 */
    class Plugin{
		public:
			/**
			 * Called by the PluginManager when this plugin is loaded.
			 *
			 * @author John M. Harris, Jr.
			 */
			virtual void plugin_init(OBEngine* eng) = 0;

			/**
			 * Called by the PluginManager before unloading this
			 * plugin.
			 *
			 * @author John M. Harris, Jr.
			 */
			virtual void plugin_deinit() = 0;
    };
}

#endif // OB_PLUGIN

// Local Variables:
// mode: c++
// End:
