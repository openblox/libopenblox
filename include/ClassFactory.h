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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox.	 If not, see <https://www.gnu.org/licenses/>.
 */

#include <string>

#ifndef OB_CLASSFACTORY
#define OB_CLASSFACTORY

namespace OB{
	/**
	 * The "factory" used to create Instance classes by name.
	 *
	 * @author John M. Harris, Jr.
	 */
	class ClassFactory{
		public:
			/**
			 * Constructor for the ClassFactory singleton.
			 *
			 * @author John M. Harris, Jr.
			 */
			ClassFactory();

			virtual ~ClassFactory();

			/**
			 * Returns the global instance of the ClassFactory
			 * singleton.
			 *
			 * @returns ClassFactory* global instance
			 * @author John M. Harris, Jr.
			 */
			static ClassFactory* getInstance();
	};
}

#endif // OB_CLASSFACTORY

// Local Variables:
// mode: c++
// End:
