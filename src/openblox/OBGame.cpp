/*
 * Copyright 2015 John M. Harris, Jr.
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

/**
 * @file OBGame.cpp
 * @author John M. Harris, Jr.
 * @date July 2015
 *
 * This file is implements the OBGame singleton.
 */

#include "OBGame.h"

#include "AssetLocator.h"
#include "ClassFactory.h"
#include "TaskScheduler.h"

namespace OpenBlox{
	OBGame::OBGame(OBEngine* _engine){
		engine = _engine;

		new ClassFactory();
		new TaskScheduler();
		new AssetLocator();
	}

	OBGame::~OBGame(){}

	OBEngine* OBGame::_getEngine(){
		return engine;
	}
}
