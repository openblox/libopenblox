/*
 * Copyright 2016 John M. Harris, Jr.
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

#include "ClassFactory.h"
#include <Instance.h>

namespace OpenBlox{
	ClassFactory* ClassFactory::inst = NULL;

	ClassFactory::ClassFactory(){
		if(inst != NULL){
			throw new OBException(OB_TRANSLATE("ClassFactory", "Only one instance of ClassFactory can be created."));
		}
		inst = this;
	}

	ClassFactory::~ClassFactory(){}

	/**
	 * Returns the ClassFactory instance.
	 * @return ClassFactory*, NULL if there is no instance yet.
	 * @author John M. Harris, Jr.
	 */
	ClassFactory* ClassFactory::getInstance(){
		return inst;
	}

	/*
	 * Registers an Instance class with this ClassFactory.
	 * @param QString className
	 * @param ClassMaker*, cannot be NULL.
	 * @author John M. Harris, Jr.
	 */
	void ClassFactory::addClass(QString className, ClassMaker* const classMaker){
		if(!classMaker){
			throw new OBException(OB_TRANSLATE("ClassFactory", "classMaker cannot be NULL."));
		}
		lookupTable[className] = classMaker;
	}

	/**
	 * Checks to see if an Instance of the specified className can be created.
	 * @param QString className
	 * @returns bool isCreatable
	 * @author John M. Harris, Jr.
	 */
	bool ClassFactory::canCreate(QString className){
		ClassMaker* maker = lookupTable[className];
		if(maker){
			return maker->isInstantiatable();
		}
		return false;
	}

	/**
	 * Attempts to find the parent class name of an Instance class.
	 * @param QString Parent className
	 * @return QString, NULL if there is no parent class. (This should ideally only be the case with Instance itself)
	 * @author John M. Harris, Jr.
	 */
	QString ClassFactory::getParentClassName(QString className){
		if(lookupTable[className]){
			return lookupTable[className]->getParentClassName();
		}
		return NULL;
	}

	/**
	 * Attempts to create an Instance with the supplied className.
	 * @param QString className
	 * @return OpenBlox::Instance::Instance*, NULL if unable to create instance.
	 * @author John M. Harris, Jr.
	 */
	OpenBlox::Instance::Instance* ClassFactory::create(QString className){
		if(lookupTable[className]){
			ClassMaker* maker = lookupTable[className];
			if(maker->isInstantiatable()){
				return maker->getInstance();
			}
		}

		return NULL;
	}

	/**
	 * Attempts to create an Instance as a service with the supplied className, passes isDataModel to the ClassMaker.
	 * @param QString className
	 * @param bool isDataModel
	 * @return OpenBlox::Instance::Instance*, NULL if unable to create instance.
	 * @author John M. Harris, Jr.
	 */
	OpenBlox::Instance::Instance* ClassFactory::createService(QString className, bool isDataModel){
		if(lookupTable[className]){
			ClassMaker* maker = lookupTable[className];
			if(maker){
				if(maker->isService(isDataModel)){
					return maker->getInstance();
				}
			}
		}

		return NULL;
	}

	/**
	 * Attempts to create an Instance with the supplied className.
	 * @param QString className
	 * @return OpenBlox::Instance::Instance*, NULL if unable to create instance.
	 * @author John M. Harris, Jr.
	 */
	OpenBlox::Instance::Instance* ClassFactory::createReplicate(QString className){
		if(lookupTable[className]){
			ClassMaker* maker = lookupTable[className];
			if(maker){
				return maker->getInstance();
			}
		}

		return NULL;
	}

	/**
	 * Returns a vector containing all registered classNames.
	 * @return std::vector<QString>
	 * @author John M. Harris, Jr.
	 */
	std::vector<QString> ClassFactory::getRegisteredMetatables(){
		std::vector<QString> keys;

		for(std::map<QString, ClassMaker*>::iterator it = lookupTable.begin(); it != lookupTable.end(); ++it){
			keys.push_back(it->first);
		}

		return keys;
	}

	/**
	 * Used to tell if an Instance inherits from the supplied className.
	 * @param OpenBlox::Instance::Instance* obj
	 * @param QString className
	 * @return bool, true if the Instance is of className and false if it is not of className or there is no class registered with the supplied className.
	 */
	bool ClassFactory::isA(const OpenBlox::Instance::Instance* obj, QString className){
		if(lookupTable[className]){
			ClassMaker* maker = lookupTable[className];
			return maker->isA(obj);
		}
		return false;
	}
}
