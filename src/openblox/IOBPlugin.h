/*
 * Copyright 2016 John M. Harris, Jr.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenBlox.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPENBLOX_IOBPLUGIN_H_
#define OPENBLOX_IOBPLUGIN_H_

#include <vector>

namespace OpenBlox{
	class IOBPlugin;

	typedef IOBPlugin* (*OB_PLUGIN_INIT_FUNC)();

	/**
	 * This class is the base of all plugins, both internal and external. You can force
	 * a plugin to be loaded at runtime, by calling PluginManager::addPlugin() with a new Plugin
	 * with a fileName of NULL and IOBPlugin set to an instance of this class.
	 * @author John M. Harris, Jr.
	 */
	class IOBPlugin{
		public:
			IOBPlugin();
			virtual ~IOBPlugin();

			virtual char* getName() = 0;
			virtual char* getAuthor() = 0;
			virtual char* getVersion() = 0;

			virtual bool init() = 0;
			virtual void shutdown() = 0;
	};

	//"Normal" return value, nothing special
	#define OB_PLUGIN_HOOK_OK 0
	//Cancels this event (only works for certain event hooks)
	#define OB_PLUGIN_HOOK_CANCEL
	//Do not propagate this hook past this plugin
	#define OB_PLUGIN_HOOK_CONSUME 2
	typedef int (*OB_PLUGIN_HOOK_FUNC)(char* pluginNamespace, char* hookName, void* ud);
}

#endif
