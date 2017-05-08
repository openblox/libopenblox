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

#include "instance/DataModel.h"

#include "OBEngine.h"

//Services we're including just to initiate them ahead of time
#include "instance/Workspace.h"
#include "instance/Lighting.h"
#include "instance/ContentProvider.h"
#include "instance/LogService.h"
#include "instance/RunService.h"
#include "instance/ReplicatedFirst.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(DataModel, false, false, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, DataModel::register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    DataModel::DataModel(){
			Name = "Game";

			netId = OB_NETID_DATAMODEL;
			netIdStartIdx = (rand() % (101 - OB_NETID_START)) + OB_NETID_START;
			netIdNextIdx = netIdStartIdx;
		}

	    DataModel::~DataModel(){}

		void DataModel::Shutdown(int statusCode){
		    OBEngine* eng = OBEngine::getInstance();
			eng->setExitCode(statusCode);
			eng->shutdown();
		}
		
		void DataModel::initServices(){
			shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();

			workspace = make_shared<Workspace>();
			workspace->setParent(sharedThis, false);
			workspace->ParentLocked = true;
			
			lighting = make_shared<Lighting>();
			lighting->setParent(sharedThis, false);
			lighting->ParentLocked = true;

			contentProvider = make_shared<ContentProvider>();
		    contentProvider->setParent(sharedThis, false);
		    contentProvider->ParentLocked = true;

			logService = make_shared<LogService>();
			logService->setParent(sharedThis, false);
			logService->ParentLocked = true;

		    runService = make_shared<RunService>();
		    runService->setParent(sharedThis, false);
		    runService->ParentLocked = true;

		    replicatedFirst = make_shared<ReplicatedFirst>();
		    replicatedFirst->setParent(sharedThis, false);
		    replicatedFirst->ParentLocked = true;
		}

		shared_ptr<Workspace> DataModel::getWorkspace(){
			return workspace;
		}

		shared_ptr<Lighting> DataModel::getLighting(){
			return lighting;
		}

		shared_ptr<ContentProvider> DataModel::getContentProvider(){
			return contentProvider;
		}

		shared_ptr<LogService> DataModel::getLogService(){
			return logService;
		}

		shared_ptr<RunService> DataModel::getRunService(){
			return runService;
		}

		shared_ptr<Instance> DataModel::cloneImpl(){
			return NULL;
		}

		shared_ptr<Instance> DataModel::GetService(std::string className){
			shared_ptr<Instance> foundService = FindService(className);
			if(foundService != NULL){
				return foundService;
			}
		    shared_ptr<Instance> newGuy = ClassFactory::createService(className, true);
			if(newGuy){
				newGuy->setParent(std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this(), false);
				newGuy->ParentLocked = true;
			}
			return newGuy;
		}

		weak_ptr<Instance> DataModel::lookupInstance(ob_uint64 netId){
			std::cout << "Net ID: " << netId << std::endl;
			if(netId >= OB_NETID_START){
				auto findIt = instMap.find(netId);
				if(findIt != instMap.end()){
					return findIt->second;
				}
			}else{
				switch(netId){
					case OB_NETID_DATAMODEL: {
						return std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
					}
					case OB_NETID_WORKSPACE: {
						return workspace;
					}
					case OB_NETID_LIGHTING: {
						return lighting;
					}
				}
			}

			return shared_ptr<Instance>(NULL);
		}

		void DataModel::putInstance(shared_ptr<Instance> inst){
			if(inst){
				ob_uint64 reqNetId = inst->GetNetworkID();
				if(reqNetId >= OB_NETID_START){
				    auto findIt = instMap.find(reqNetId);
					if(findIt != instMap.end()){
						inst->setNetworkID(OB_NETID_UNASSIGNED);
						return;
					}

					instMap[reqNetId] = weak_ptr<Instance>(inst);
				}
			}
		}

		void DataModel::dropInstance(ob_uint64 reqNetId){
			if(reqNetId > OB_NETID_START){
				auto findIt = instMap.find(reqNetId);
				if(findIt != instMap.end()){
					instMap.erase(findIt);

					// This way we aren't wasting memory unless
					// there's already an insane number of instances.
					if(netIdNextIdx > (ULONG_MAX / 2)){
						freedNetIDs.push_back(reqNetId);
					}
				}
			}
		}

		ob_uint64 DataModel::nextNetworkID(){
			if(netIdNextIdx == OB_NETID_UNASSIGNED){
				// This will probably never happen, but why not be
				// prepared?
				if(!freedNetIDs.empty()){
					ob_uint64 tmpID = freedNetIDs.back();
					freedNetIDs.pop_back();
					return tmpID;
				}
				goto nextIsUnassigned;
			}
			
		    netIdNextIdx++;

			if(netIdNextIdx >= ULONG_MAX){
				netIdNextIdx = OB_NETID_UNASSIGNED;
			}

			return netIdNextIdx;

		  nextIsUnassigned:
			std::cout << "Ran out of free network IDs." << std::endl;
			return OB_NETID_UNASSIGNED;
		}

		void DataModel::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
		}

		void DataModel::replicateChildren(shared_ptr<NetworkReplicator> peer){
			replicatedFirst->replicate(peer);
			
			std::vector<shared_ptr<Instance>> kids = GetChildren();
			
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != kids.size(); i++){
				shared_ptr<Instance> kid = kids[i];
				if(kid && kid != replicatedFirst){
					kid->replicate(peer);
				}
			}
		}

		int DataModel::lua_Shutdown(lua_State* L){
		    shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(shared_ptr<DataModel> dm = dynamic_pointer_cast<DataModel>(inst)){
				int statusCode = 0;
				if(!lua_isnoneornil(L, 2)){
					statusCode = luaL_checkinteger(L, 2);
				}

				dm->Shutdown(statusCode);
				
				return 0;
			}
			
			return luaL_error(L, COLONERR, "Shutdown");
		}

		void DataModel::register_lua_methods(lua_State* L){
		    ServiceProvider::register_lua_methods(L);

			luaL_Reg methods[] = {
				{"Shutdown", lua_Shutdown},
			    {NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}
	}
}
