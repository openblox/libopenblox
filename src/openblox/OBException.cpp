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

#include "OBException.h"

namespace OpenBlox{
	/**
	 * Creates an OBException with no message.
	 * @author John M. Harris, Jr.
	 */
	OBException::OBException(){}

	/**
	 * Creates an OBException with the message defined by the parameter "what".
	 * @param std::string what
	 * @author John M. Harris, Jr.
	 */
	OBException::OBException(std::string what){
		this->what = what;
	}

	OBException::~OBException(){}

	/**
	 * Returns the message of this OBException.
	 * @return std::string message
	 * @author John M. Harris, Jr.
	 */
	std::string OBException::getMessage(){
		return what;
	}

	/**
	 * Convenience function for converting this exception to a string, does the same thing as OBException::getMessage
	 * @return std::string message
	 * @author John M. Harris, Jr.
	 */
	std::string OBException::toString(){
		return what;
	}

	/**
	 * Convenience function to throw this exception.
	 * @author John M. Harris, Jr.
	 */
	void OBException::raise(){
		throw *this;
	}
}
