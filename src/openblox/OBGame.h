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
 * @file OBGame.h
 * @author John M. Harris, Jr.
 * @date July 2015
 *
 * This file is defines the OBGame singleton.
 */

#include "OpenBlox.h"

#include <QtCore>

#ifndef OPENBLOX_OBGAME_H_
#define OPENBLOX_OBGAME_H_

namespace OpenBlox{
	class OBGame{
		public:
			OBGame();
			virtual ~OBGame();

			static OBGame* getInstance();

			bool tick();

			void resized();

			//Input Injection Methods
			void mousePress(Uint8 btn, QPoint pos);
			void mouseRelease(Uint8 btn, QPoint pos);
			void mouseMove(QPoint pos);
			void mouseWheel(int delta, QPoint pos);

			void processEvent(SDL_Event event);
		private:
			lua_State* mainLuaState;

			static OBGame* inst;
	};
}

#endif
