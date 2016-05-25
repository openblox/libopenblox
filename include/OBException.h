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

/**
 * @file OBException.h
 * @author John M. Harris, Jr.
 * @date May 2016
 *
 * This file defines OBException, which is the generic exception class
 * of the OpenBlox game engine.
 */

#include <string>

#ifndef OB_OBEXCEPTION
#define OB_OBEXCEPTION

namespace OB{
	/**
	 * The general purpose exception class of the OpenBlox game engine.

	 * @author John M. Harris, Jr.
	 */
	class OBException{
		public:
			OBException();
			OBException(std::string string);
			virtual ~OBException();

			std::string getMessage();
			std::string toString();

			void raise();

			operator std::string() const{
				return what;
			}

		private:
			std::string what;
	};
}

#endif // OB_OBEXCEPTION

