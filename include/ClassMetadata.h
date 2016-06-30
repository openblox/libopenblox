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
#include <vector>

#include "instance/Instance.h"

#ifndef OB_CLASSMETADATA
#define OB_CLASSMETADATA

namespace OB{
	class ClassMetadata{
		public:
			/**
			 * "Constructor" for this virtual class.
			 *
			 * @param std::string Class name
			 * @param std::string Parent class name
			 * @param Is instantiatable
			 * @author John M. Harris, Jr.
			 */
		    void init(std::string className, std::string parentClassName, bool isInstable);
			
			/**
			 * Used by the ClassFactory to create new instances of the
			 * class described by this ClassMetadata instance.
			 *
			 * @returns OB::Instance::Instance* or NULL if this is
			 * abstract.
			 * @author John M. Harris, Jr.
			 */
			virtual OB::Instance::Instance* newInstance() = 0;

			/**
			 * Used to tell if the supplied OB::Instance::Instance* is
			 * of the class described by this ClassMetadata instance.
			 *
			 * @param OB::Instance::Instance* obj
			 * @returns bool isInstance
			 * @author John M. Harris, Jr.
			 */
			virtual bool isA(OB::Instance::Instance* obj) = 0;

			/**
			 * Used to tell if the class described by this
			 * ClassMetadata can be created. If this returns false,
			 * newInstance should return NULL.
			 */
			bool isInstantiatable();

			/**
			 * Returns true if the class described is a service,
			 * passes isDataModel for DataModel-only services.
			 *
			 * @param bool isDataModel
			 * @returns bool isService
			 * @author John M. Harris, Jr.
			 */
			virtual bool isService(bool isDataModel) = 0;

			/**
			 * Returns the name of the class this ClassMetadata
			 * describes.
			 *
			 * @returns std::string Class name
			 * @author John M. Harris, Jr.
			 */
			std::string getClassName();

			/**
			 * Returns the parent class name of the class this
			 * ClassMetadata describes. For Instance itself,
			 * this is "", that should not be true of any
			 * other Instance, or "bad things" will happen.
			 *
			 * @returns std::string Parent class name
			 * @author John M. Harris, Jr.
			 */
			std::string getParentClassName();
		private:
			std::string className;
			std::string parentClassName;
			bool isInstable;
	};
}

#endif // OB_CLASSMETADATA

// Local Variables:
// mode: c++
// End: