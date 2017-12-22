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

#include "PluginManager.h"

namespace OB{
	PluginManager::PluginManager(OBEngine* eng){
		this->eng = eng;
	}

	PluginManager::~PluginManager(){}

	std::map<std::string, shared_ptr<Plugin>> PluginManager::getPlugins(){
		return plugins;
	}

	bool PluginManager::loadPlugin(std::string name, shared_ptr<Plugin> plugins){
		// TODO: Implement
		return false;
	}

	void PluginManager::unloadPlugin(std::string name){}

	void PluginManager::tick(){
		// TODO: Call tick of plugins with a tick hook
	}
}
