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

#include "ClassFactory.h"

#include "OBException.h"

namespace OB{
	std::map<std::string, ClassMetadata*> ClassFactory::metadataTable;

	std::vector<std::string> ClassFactory::getRegisteredClasses(){
		std::vector<std::string> regged;

		for(std::map<std::string, ClassMetadata*>::iterator it = metadataTable.begin(); it != metadataTable.end(); ++it){
			regged.push_back(it->first);
		}

		return regged;
	}

	void ClassFactory::addClass(std::string className, ClassMetadata* newClassMetadata){
		if(!newClassMetadata){
			throw new OBException("newClassMetadata cannot be NULL.");
		}
		metadataTable[className] = newClassMetadata;
	}

	bool ClassFactory::canCreate(std::string className){
		ClassMetadata* classMd = metadataTable[className];
		if(classMd){
			return classMd->isInstantiatable();
		}
		return false;
	}

	std::string ClassFactory::getParentClassName(std::string className){
		ClassMetadata* classMd = metadataTable[className];
		if(classMd){
			return classMd->getParentClassName();
		}
		return "";
	}

	OB::Instance::Instance* ClassFactory::create(std::string className){
		ClassMetadata* classMd = metadataTable[className];
		if(classMd){
			if(classMd->isInstantiatable()){
				return classMd->newInstance();
			}
		}
		return NULL;
	}

	OB::Instance::Instance* ClassFactory::createService(std::string className, bool isDataModel){
		ClassMetadata* classMd = metadataTable[className];
		if(classMd){
			if(classMd->isService(isDataModel)){
				return classMd->newInstance();
			}
		}
		return NULL;
	}

	OB::Instance::Instance* ClassFactory::createReplicate(std::string className){
		ClassMetadata* classMd = metadataTable[className];
		if(classMd){
			return classMd->newInstance();
		}
		return NULL;
	}

	bool ClassFactory::isA(OB::Instance::Instance* obj, std::string className){
		ClassMetadata* classMd = metadataTable[className];
		if(classMd){
			return classMd->isA(obj);
		}
		return false;
	}
}
