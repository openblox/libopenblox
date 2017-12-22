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

#ifndef OB_PLUGINMANAGER
#define OB_PLUGINMANAGER

#include "Plugin.h"

#include "obtype.h"
#include "mem.h"

#include <pthread.h>

#include <string>
#include <map>
#include <vector>

#include "oblibconfig.h"

namespace OB{
    class OBEngine;

	/**
	 * The PluginManager class is the proper way to add functionality
	 * to OpenBlox. Many OpenBlox core events can be "hooked" into
	 * using this class.
	 *
	 * @author John M. Harris, Jr.
	 */
    class PluginManager{
		public:
		    PluginManager(OBEngine* eng);
			virtual ~PluginManager();

			/**
			 * Returns all currently loaded plugins.
			 *
			 * @author John M. Harris, Jr.
			 */
			std::map<std::string, shared_ptr<Plugin>> getPlugins();

			/**
			 * Loads a plugin, if there is no plugin with the provided
			 * name already loaded.
			 *
			 * @returns true on success, false if the plugin could not be loaded.
			 * @author John M. Harris, Jr.
			 */
		    bool loadPlugin(std::string name, shared_ptr<Plugin> plugin);

			/**
			 * Unloads the plugin specified by the first parameter.
			 * If there is no plugin with that name, this function is
			 * no-op.
			 *
			 * @author John M. Harris, Jr.
			 */
			void unloadPlugin(std::string name);

			/**
			 * @internal
			 */
			void tick();

		private:
			std::map<std::string, shared_ptr<Plugin>> plugins;

			OBEngine* eng;
    };
}

#endif // OB_PLUGINMANAGER

// Local Variables:
// mode: c++
// End:
