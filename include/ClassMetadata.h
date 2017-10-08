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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include <string>
#include <vector>

#include "mem.h"

#ifndef OB_CLASSMETADATA
#define OB_CLASSMETADATA

namespace OB{
	class OBEngine;

#ifndef OB_INST_INSTANCE
	namespace Instance{
		class Instance;
	}
#endif

	typedef void (*InstanceInitFnc)(OBEngine* eng);

	class ClassMetadata{
	public:
		/**
		 * Used by the ClassFactory to create new instances of the
		 * class described by this ClassMetadata instance.
		 *
		 * @returns New Instance or NULL if the class this describes is abstract.
		 * @author John M. Harris, Jr.
		 */
		virtual shared_ptr<Instance::Instance> newInstance(OBEngine* eng) = 0;

		/**
		 * Used to tell if the supplied OB::Instance::Instance* is
		 * of the class described by this ClassMetadata instance.
		 *
		 * @param obj Instance to test against this type
		 * @returns true if obj is an instance of the defined class
		 * @author John M. Harris, Jr.
		 */
		virtual bool isA(shared_ptr<Instance::Instance> obj) = 0;

		/**
		 * Used to tell if the class described by this
		 * ClassMetadata can be created.
		 *
		 * @returns true if this class can be created. Doesn't apply to services or replication. Replication bypasses this entirely.
		 */
		virtual bool isInstantiatable() = 0;

		/**
		 * Returns true if the class described is a service,
		 * passes isDataModel for DataModel-only services.
		 *
		 * @param isDataModel Whether or not this is being called from the DataModel
		 * @returns true if this is a service, otherwise false.
		 * @author John M. Harris, Jr.
		 */
		virtual bool isService(bool isDataModel) = 0;

		/**
		 * Returns the name of the class this ClassMetadata
		 * describes.
		 *
		 * @returns Name of the class described by this ClassMetadata
		 * @author John M. Harris, Jr.
		 */
		virtual std::string getClassName() = 0;

		/**
		 * Returns the parent class name of the class this
		 * ClassMetadata describes. For Instance itself,
		 * this is "", that should not be true of any
		 * other Instance, or “bad things” will happen.
		 *
		 * @returns Parent class name
		 * @author John M. Harris, Jr.
		 */
		virtual std::string getParentClassName() = 0;

		/**
		 * Used to initialize Lua metatables for classes, among
		 * other things.
		 *
		 * @author John M. Harris, Jr.
		 */
		virtual InstanceInitFnc getInitFunc() = 0;
	};
}

#endif // OB_CLASSMETADATA

// Local Variables:
// mode: c++
// End:
