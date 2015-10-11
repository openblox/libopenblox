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

#ifndef OPENBLOX_CLASSFACTORY_H_
#define OPENBLOX_CLASSFACTORY_H_

#include "OpenBlox.h"

namespace OpenBlox{
	namespace Instance{
		class Instance;//Forward declaration
	}

	/**
	 * Structure used to describe a class, used by ClassFactory to create new instances of the class described.
	 * @author John M. Harris, Jr.
	 */
	struct ClassMaker{
		/**
		 * Used by the ClassFactory to create new instances. Abstract classes should return NULL.
		 * @returns ob_instance:Instance* or NULL if abstract.
		 * @author John M. Harris, Jr.
		 */
		virtual OpenBlox::Instance::Instance* getInstance() const = 0;
		/**
		 * Used to tell if the supplied ob_Instance::Instance extends the class this ClassMaker describes.
		 * @param ob_instance::Instance* obj
		 * @returns bool isInstance
		 * @author John M. Harris, Jr.
		 */
		virtual bool isA(const OpenBlox::Instance::Instance* obj) = 0;
		/**
		 * Used to tell if this class can be created. If this returns false, getInstance should return NULL.
		 * @returns bool isInstantiatable
		 * @author John M. Harris, Jr.
		 */
		virtual bool isInstantiatable() = 0;
		/**
		 * Returns true if the class described by this ClassMaker is a service, passes isDataModel for DataModel-only services.
		 * @param bool isDataModel
		 * @returns bool isService
		 * @author John M. Harris, Jr.
		 */
		virtual bool isService(bool isDataModel) = 0;
		/**
		 * Returns the parent class name of the class this ClassMaker describes.
		 * @returns QString Parent className
		 * @author John M. Harris, Jr.
		 */
		virtual QString getParentClassName() = 0;
		virtual ~ClassMaker(){}
	};

	/**
	 * The "factory" used to create Instance objects from Lua (Or other language bindings?)
	 * @author John M. Harris, Jr.
	 */
	class ClassFactory{
		public:
			ClassFactory();
			virtual ~ClassFactory();

			static ClassFactory* getInstance();

			std::vector<QString> getRegisteredMetatables();
			void addClass(QString className, ClassMaker* const newClassMaker);
			bool canCreate(QString className);
			QString getParentClassName(QString className);
			OpenBlox::Instance::Instance* create(QString className);
			OpenBlox::Instance::Instance* createService(QString className, bool isDataModel);
			OpenBlox::Instance::Instance* createReplicate(QString className);
			bool isA(const OpenBlox::Instance::Instance* obj, QString className);

		private:
			std::map<QString, ClassMaker*> lookupTable;

			static ClassFactory* inst;
	};
}
#endif
