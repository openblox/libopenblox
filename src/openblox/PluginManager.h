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

#ifndef OPENBLOX_PLUGINMANAGER_H_
#define OPENBLOX_PLUGINMANAGER_H_

#include "OpenBlox.h"

#include "IOBPlugin.h"

namespace OpenBlox{
	/**
	 * This class is a convenience for internal use, and plugins loaded by the host program (e.g. the official client)
	 * @author John M. Harris, Jr.
	 */
	class Plugin{
		public:
			Plugin(char* fileName, IOBPlugin* plug);
			virtual ~Plugin();

			char* getFileName();
			char* getName();
			char* getAuthor();
			char* getVersion();

			bool isValid();
			IOBPlugin* getAbstractPlugin();

			bool init();
			bool initAfterGame();
			void shutdown();
		private:
			char* fileName;
			IOBPlugin* plug;

	};

	struct HookInfo{
		QString ns;
		QString hook_name;
		IOBPlugin* plug;
		OB_PLUGIN_HOOK_FUNC* func;
	};

	/**
	 * Used to load plugins at runtime, based on the needs of whatever we're running.
	 * @author John M. Harris, Jr.
	 */
	class PluginManager{
		public:
			PluginManager();
			virtual ~PluginManager();

			static PluginManager* getInstance();

			std::vector<Plugin*> getPlugins();

		private:
			std::vector<Plugin*> plugins;
			std::vector<HookInfo*> evt_hooks;

			static PluginManager* inst;
	};
}

#endif
