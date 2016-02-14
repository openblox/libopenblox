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

#include "PluginManager.h"

namespace OpenBlox{
	Plugin::Plugin(char* fileName, IOBPlugin* plug){
		this->fileName = fileName;
		this->plug = plug;
	}

	Plugin::~Plugin(){
		if(fileName){
			delete[] fileName;
		}
		if(plug){
			delete plug;
		}
	}

	char* Plugin::getFileName(){
		return fileName;
	}

	char* Plugin::getName(){
		if(isValid()){
			return plug->getName();
		}
		return NULL;
	}

	char* Plugin::getAuthor(){
		if(isValid()){
			return plug->getAuthor();
		}
		return NULL;
	}

	char* Plugin::getVersion(){
		if(isValid()){
			return plug->getVersion();
		}
		return NULL;
	}

	bool Plugin::isValid(){
		return plug != NULL;
	}

	IOBPlugin* Plugin::getAbstractPlugin(){
		return plug;
	}

	bool Plugin::init(){
		if(isValid()){
			return plug->init();
		}
		return false;
	}

	bool Plugin::initAfterGame(){
		if(isValid()){
			return plug->init();
		}
		return false;
	}

	void Plugin::shutdown(){
		if(isValid()){
			plug->shutdown();
		}
	}

	PluginManager* PluginManager::inst = NULL;

	PluginManager::PluginManager(){
		if(inst != NULL){
			throw new OBException(OB_TRANSLATE("PluginManager", "Only one instance of PluginManager can be created."));
		}
		inst = this;

		plugins = std::vector<Plugin*>();
		evt_hooks = std::vector<HookInfo*>();
	}

	PluginManager::~PluginManager(){}

	/**
	 * Returns the PluginManager instance.
	 * @return PluginManager*, NULL if there is no instance yet.
	 * @author John M. Harris, Jr.
	 */
	PluginManager* PluginManager::getInstance(){
		return inst;
	}
}
