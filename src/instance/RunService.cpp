/*
 * Copyright (C) 2017 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "instance/RunService.h"

#include "instance/DataModel.h"
#include "TaskScheduler.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(RunService, false, isDataModel, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		RunService::RunService(OBEngine* eng) : Instance(eng){
			Name = ClassName;
			netId = OB_NETID_NOT_REPLICATED;

			Archivable = false;

			wasRunning = false;
			running = false;
			isStudio = false;

			Stepped = make_shared<Type::Event>("Stepped");
		}

		RunService::~RunService(){}

		shared_ptr<Instance> RunService::cloneImpl(){
			return NULL;
		}

		bool RunService::IsRunMode(){
			return IsStudio() && IsRunning();
		}

		bool RunService::IsRunning(){
			return running;
		}

		bool RunService::IsClient(){
			return !IsServer();
		}

		bool RunService::IsServer(){
			shared_ptr<DataModel> dm = eng->getDataModel();
			return dm->FindService("NetworkServer") != NULL;
		}

		bool RunService::IsStudio(){
			return isStudio;
		}

		void RunService::Pause(){
			running = false;
		}

		void RunService::Run(){
			if(!running){
				running = true;
				if(!wasRunning){
					wasRunning = true;
					// TODO: wake scripts in DataModel
				}
			}
		}

		void RunService::Stop(){
			running = false;
			wasRunning = false;
			getEngine()->getTaskScheduler()->removeDMBound();
			// TODO: Disconnect events
		}

		void RunService::setIsStudio(bool isStudio){
			this->isStudio = isStudio;
		}

		void RunService::tick(){
			Stepped->Fire(eng);

			tickChildren();
		}

#if HAVE_PUGIXML
		std::string RunService::serializedID(){
			shared_ptr<OBSerializer> serializer = eng->getSerializer();
			serializer->SetID(shared_from_this(), getClassName());

			return Instance::serializedID();
		}
#endif

		void RunService::register_lua_events(lua_State* L){
			Instance::register_lua_events(L);

			luaL_Reg events[] = {
				{"Stepped", WRAP_EVT(RunService, Stepped)},
				{NULL, NULL}
			};
			luaL_setfuncs(L, events, 0);
		}
	}
}
