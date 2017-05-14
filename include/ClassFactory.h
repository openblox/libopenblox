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
#include <map>

#include "ClassMetadata.h"

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
			 * Returns a vector containing all registered class names.
			 *
			 * @returns Vector containing all registered class names.
			 * @author John M. Harris, Jr.
			 */
			static std::vector<std::string> getRegisteredClasses();

			/**
			 * Adds an Instance class to the ClassFactory's metatable
			 * map. Classes must have unique names, and parent classes
			 * are to be added before their children.
			 *
			 * @param className Name of the class to add. Should contain no spaces and should be CamelCase e.g. MyInstanceName
			 * @param newClassMetadata Metadata describing the class
			 * @author John M. Harris, Jr.
			 */
			static void addClass(std::string className, ClassMetadata* newClassMetadata);

			/**
			 * Returns whether or not a class of given name can be
			 * created.
			 *
			 * @param className Name of the class to check
			 * @returns True if this class can be created (e.g. by Instance.new in Lua)
			 * @author John M. Harris, Jr.
			 */
			static bool canCreate(std::string className);

			/**
			 * Returns the name of the parent class of a given class.
			 *
			 * @param className Name of the class to check
			 * @returns Parent class name, or an empty string in the case of Instance
			 * @author John M. Harris, Jr.
			 */
			static std::string getParentClassName(std::string className);

			/**
			 * Returns a new instance of the given class name, or NULL
			 * if the class is abstract or otherwise cannot be created.
			 *
			 * This includes classes that can only be created by
			 * services. If you attempt to call this on one of those
			 * classes, it will return NULL.
			 *
			 * @param className Name of the class to be created
			 * @returns New instance or NULL
			 * @author John M. Harris, Jr.
			 */
			static shared_ptr<Instance::Instance> create(std::string className);

			/**
			 * Returns a new instance of the given class name, or NULL
			 * if the class is abstract or is not a service.
			 *
			 * If the caller is the DataModel, the second parameter,
			 * isDataModel, will be true. This allows services to be
			 * created only by the DataModel, or only by other
			 * ServiceProviders.
			 *
			 * @param className Name of the class to be created
			 * @param isDataModel True if being called from the DataModel, otherwise false.
			 * @returns New instance or NULL
			 * @author John M. Harris, Jr.
			 */
			static shared_ptr<Instance::Instance> createService(std::string className, bool isDataModel);

			/**
			 * Returns a new instance of a given class name, or NULL
			 * if the class is abstract or otherwise cannot be
			 * created.
			 *
			 * This is only used to Instance replication.
			 *
			 * @param className Name of the class to be created
			 * @returns New instance or NULL
			 * @author John M. Harris, Jr.
			 */
			static shared_ptr<Instance::Instance> createReplicate(std::string className);

			/**
			 * Returns whether or not a given object is an instance of
			 * the given class name.
			 *
			 * @param obj Instance to test against
			 * @param className Name of the class to test against
			 * @returns true if obj is an instance of className, otherwise false
			 * @author John M. Harris, Jr.
			 */
			static bool isA(shared_ptr<Instance::Instance> obj, std::string className);

			/**
			 * Initializes Lua metatables and other Instance related
			 * runtime requirements.
			 *
			 * @author John M. Harris, Jr.
			 */
			static void initClasses();

			/**
			 * Registers core classes with the ClassFactory.
			 * This is safe to call at any time. It is suggested that,
			 * if you want to register your own instance classes, you
			 * call this before the creation of OBEngine. However, 
			 * this is not a requirement, as the constructor for
			 * OBEngine calls this function, and you can register a
			 * class at any point in the execution of a program.
			 *
			 * @author John M. Harris, Jr.
			 */
			static void registerCoreClasses();
			
		private:
			static std::map<std::string, ClassMetadata*> metadataTable;
			static bool isInitialized;
	};
}

#endif // OB_CLASSFACTORY

// Local Variables:
// mode: c++
// End:
