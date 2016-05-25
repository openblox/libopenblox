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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "OBEngine.h"

#include "OBException.h"
#include "utility.h"

namespace OB{
    OBEngine* OBEngine::inst = NULL;

	OBEngine::OBEngine(){
		if(inst != NULL){
			throw new OBException("Only one instance of OBEngine can exist.");
		}
		inst = this;

		initialized = false;
		startTime = currentTimeMillis();

		globalState = NULL;
	}

	OBEngine::~OBEngine(){}

	OBEngine* OBEngine::getInstance(){
		return inst;
	}

	void OBEngine::init(){
		//TODO: stub
	}

	void OBEngine::tick(){
		//TODO: stub
	}

	void OBEngine::render(){
		//TODO: stub
	}

	lua_State* OBEngine::getGlobalLuaState(){
		return globalState;
	}

	ob_int64 OBEngine::getStartTime(){
		return startTime;
	}
}
