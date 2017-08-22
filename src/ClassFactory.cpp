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

#include "OBEngine.h"

#include "OBException.h"

//Alllllll Instances
#include "instance/Instance.h"
#include "instance/LuaSourceContainer.h"
#include "instance/BaseScript.h"
#include "instance/Script.h"
#include "instance/Camera.h"
#include "instance/PVInstance.h"
#include "instance/Model.h"
#include "instance/BindableEvent.h"
#include "instance/ServiceProvider.h"
#include "instance/Lighting.h"
#include "instance/ContentProvider.h"
#include "instance/BasePlayerGui.h"
#include "instance/CoreGui.h"
#include "instance/StarterGui.h"
#include "instance/RunService.h"
#include "instance/TaskScheduler.h"
#include "instance/ReplicatedStorage.h"
#include "instance/ReplicatedFirst.h"
#include "instance/PlayerGui.h"
#include "instance/Workspace.h"
#include "instance/LogService.h"
#include "instance/DataModel.h"
#include "instance/Humanoid.h"
#include "instance/Folder.h"
#include "instance/BasePart.h"
#include "instance/MeshPart.h"
#include "instance/Part.h"
#include "instance/BoolValue.h"
#include "instance/DoubleConstrainedValue.h"
#include "instance/Color3Value.h"
#include "instance/IntConstrainedValue.h"
#include "instance/IntValue.h"
#include "instance/NumberValue.h"
#include "instance/ObjectValue.h"
#include "instance/NetworkReplicator.h"
#include "instance/ClientReplicator.h"
#include "instance/ServerReplicator.h"
#include "instance/NetworkPeer.h"
#include "instance/NetworkServer.h"
#include "instance/NetworkClient.h"
#include "instance/GuiBase.h"
#include "instance/GuiBase2d.h"
#include "instance/LayerCollector.h"
#include "instance/ScreenGui.h"
#include "instance/GuiObject.h"
#include "instance/Frame.h"
#include "instance/GuiLabel.h"
#include "instance/ImageLabel.h"
#include "instance/UserInputService.h"
#include "instance/Player.h"
#include "instance/Players.h"

namespace OB{
	std::map<std::string, ClassMetadata*> ClassFactory::metadataTable;
	bool ClassFactory::isInitialized = false;

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
		metadataTable.insert(std::make_pair(className, newClassMetadata));
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

	shared_ptr<Instance::Instance> ClassFactory::create(std::string className, OBEngine* eng){
		ClassMetadata* classMd = metadataTable[className];
		if(classMd){
			if(classMd->isInstantiatable()){
				return classMd->newInstance(eng);
			}
		}
		return NULL;
	}

	shared_ptr<Instance::Instance> ClassFactory::createService(std::string className, bool isDataModel, OBEngine* eng){
		ClassMetadata* classMd = metadataTable[className];
		if(classMd){
			if(classMd->isService(isDataModel)){
				return classMd->newInstance(eng);
			}
		}
		return NULL;
	}

    shared_ptr<Instance::Instance> ClassFactory::createReplicate(std::string className, OBEngine* eng){
		ClassMetadata* classMd = metadataTable[className];
		if(classMd){
			return classMd->newInstance(eng);
		}
		return NULL;
	}

	bool ClassFactory::isA(shared_ptr<Instance::Instance> obj, std::string className){
		ClassMetadata* classMd = metadataTable[className];
		if(classMd){
			return classMd->isA(obj);
		}
		return false;
	}

	void ClassFactory::initClasses(OBEngine* eng){
		for(std::map<std::string, ClassMetadata*>::iterator it = metadataTable.begin(); it != metadataTable.end(); ++it){
			it->second->getInitFunc()(eng);
		}
	}

    void ClassFactory::registerCoreClasses(){
		if(isInitialized){
			return;
		}
		isInitialized = true;
		
		Instance::Instance::registerClass();
		Instance::LuaSourceContainer::registerClass();
		Instance::BaseScript::registerClass();
		Instance::Script::registerClass();
		Instance::Camera::registerClass();
		Instance::PVInstance::registerClass();
		Instance::Model::registerClass();
	    Instance::BindableEvent::registerClass();
		Instance::ServiceProvider::registerClass();
		Instance::Lighting::registerClass();
		Instance::ContentProvider::registerClass();
		Instance::BasePlayerGui::registerClass();
		Instance::CoreGui::registerClass();
		Instance::StarterGui::registerClass();
		Instance::RunService::registerClass();
		Instance::TaskScheduler::registerClass();
		Instance::ReplicatedStorage::registerClass();
		Instance::ReplicatedFirst::registerClass();
		Instance::PlayerGui::registerClass();
		Instance::Workspace::registerClass();
		Instance::LogService::registerClass();
		Instance::DataModel::registerClass();
		Instance::Humanoid::registerClass();
		Instance::Folder::registerClass();
		Instance::BasePart::registerClass();
		Instance::MeshPart::registerClass();
		Instance::Part::registerClass();
		Instance::BoolValue::registerClass();
		Instance::DoubleConstrainedValue::registerClass();
		Instance::Color3Value::registerClass();
		Instance::IntConstrainedValue::registerClass();
		Instance::IntValue::registerClass();
		Instance::NumberValue::registerClass();
		Instance::ObjectValue::registerClass();
		Instance::GuiBase::registerClass();
		Instance::GuiBase2d::registerClass();
		Instance::LayerCollector::registerClass();
		Instance::ScreenGui::registerClass();
		Instance::GuiObject::registerClass();
		Instance::Frame::registerClass();
		Instance::GuiLabel::registerClass();
		Instance::ImageLabel::registerClass();
		Instance::UserInputService::registerClass();
		Instance::Players::registerClass();
		Instance::Player::registerClass();

		#if HAVE_ENET
		Instance::NetworkReplicator::registerClass();
		Instance::ClientReplicator::registerClass();
		Instance::ServerReplicator::registerClass();
		Instance::NetworkPeer::registerClass();
		Instance::NetworkServer::registerClass();
		Instance::NetworkClient::registerClass();
		#endif
	}
}
