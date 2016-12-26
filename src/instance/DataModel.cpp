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
#include "instance/Lighting.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(DataModel, false, false, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, DataModel::register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    DataModel::DataModel(){
			Name = "game";
		}

	    DataModel::~DataModel(){}

		void DataModel::Shutdown(int statusCode){
		    OBEngine* eng = OBEngine::getInstance();
			eng->setExitCode(statusCode);
			eng->shutdown();
		}
		
		void DataModel::initServices(){
			lighting = make_shared<Lighting>();
			lighting->setParent(std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this(), false);
			lighting->ParentLocked = true;
		}

		shared_ptr<Lighting> DataModel::getLighting(){
			return lighting;
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

		int DataModel::lua_Shutdown(lua_State* L){
		    shared_ptr<Instance> inst = checkInstance(L, 1);
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
