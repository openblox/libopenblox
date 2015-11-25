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

#include "DataModel.h"

BEGIN_INSTANCE
	DEFINE_CLASS(DataModel, false, false, Instance);

	DataModel::DataModel() : ServiceProvider(){
		ParentLocked = true;

		Loaded = new OpenBlox::Type::Event("Loaded");
		isloaded = false;

		title = "OpenBlox";
	}

	DataModel::~DataModel(){
		delete Loaded;
	}

	DEFINE_SERVICE(DataModel);

	Instance* DataModel::GetService(QString className){
		Instance* foundService = FindService(className);
		if(foundService != NULL){
			return foundService;
		}
		Instance* newGuy = OpenBlox::ClassFactory::getInstance()->createService(className, true);
		if(newGuy){
			newGuy->setParent(this, false);
			newGuy->parentLock();
		}
		return newGuy;
	}

	/**
	 * Sets the title of the game's window.
	 * @param QString title
	 * @author John M. Harris, Jr.
	 */
	void DataModel::SetTitle(QString title){
		this->title = title;

		#ifndef OB_NO_GRAPHICS
		{
			OBEngine* eng = OpenBlox::OBEngine::getInstance();
			if(eng){
				Ogre::RenderWindow* rw = eng->getRenderWindow();
				if(rw){

				}
			}
		}
		#endif
	}

	/**
	 * Gets the title of the game's window.
	 * @returns QString title
	 * @author John M. Harris, Jr.
	 */
	QString DataModel::GetTitle(){
		return title;
	}

	/**
	 * Returns true if we are connected to a server and replication has finished. This always returns true on servers.
	 * @returns bool
	 * @author John M. Harris, Jr.
	 */
	bool DataModel::IsLoaded(){
		return true;//TODO: STUB
	}

	/**
	 * Called by NetworkClient when we're done loading all instances from the server.
	 * @author John M. Harris, Jr.
	 */
	void DataModel::loadComplete(){
		isloaded = true;
		Loaded->Fire();
	}

	/**
	 * Gets the current message of the DataModel.
	 * @returns QString message
	 * @author John M. Harris, Jr.
	 */
	QString DataModel::GetMessage(){
		return message;
	}

	/**
	 * Sets the current message of the DataModel.
	 * @param QString message
	 * @author John M. Harris, Jr.
	 */
	void DataModel::SetMessage(QString msg){
		message = msg;
	}

	/**
	 * Clears the current message of the DataModel.
	 * @author John M. Harris, Jr.
	 */
	void DataModel::ClearMessage(){
		message = QString::null;
	}

	/**
	 * Shuts down the DataModel. This cleans up network services and handles resource cleanup.
	 * @author John M. Harris, Jr.
	 */
	void DataModel::Shutdown(){
		/*
		Instance* nsInst = FindService("NetworkServer");
		if(nsInst != NULL){
			if(NetworkServer* ns = dynamic_cast<NetworkServer*>(nsInst)){
				ns->Stop();
			}
		}

		Instance* ncInst = FindService("NetworkClient");
		if(ncInst != NULL){
			if(NetworkClient* nc = dynamic_cast<NetworkClient*>(ncInst)){
				nc->Disconnect();
			}
		}
		*/

		//TODO: Call OnClose callback, before calling the shutdown hook, which actually does the work.

		OBEngine* eng = OpenBlox::OBEngine::getInstance();
		if(eng){ //Should be true... Certainly here.. But it's always nice to prevent segfaults.....
			void(*down_hook)() = eng->getShutdownHook();
			if(down_hook){
				down_hook();
				//If we're still around here, die.
				exit(EXIT_SUCCESS);
			}else{
				exit(EXIT_SUCCESS);
			}
		}
	}

	/**
	 * Gets the current FPS.
	 * @returns double FPS, -1 if an error occurs, 0 if we don't have a RenderWindow.
	 * @author John M. Harris, Jr.
	 */
	double DataModel::GetFPS(){
		double fps = -1;

		OBEngine* eng = OpenBlox::OBEngine::getInstance();
		if(eng){
			#ifndef OB_NO_GRAPHICS
			Ogre::RenderWindow* rw = eng->getRenderWindow();
			if(rw){
				fps = rw->getLastFPS();
			}
			#else
			fps = 0;
			#endif
		}

		return fps;
	}

	int DataModel::lua_Shutdown(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
			dm->Shutdown();
			return lua_yield(L, 0);//This just keeps it from returning.
		}
		return luaL_error(L, COLONERR, "Shutdown");
	}

	int DataModel::lua_SetTitle(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
			dm->SetTitle(QString(luaL_checkstring(L, 2)));
			return 0;
		}
		return luaL_error(L, COLONERR, "SetTitle");
	}

	int DataModel::lua_GetTitle(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
			lua_pushstring(L, dm->title.toStdString().c_str());
			return 1;
		}
		return luaL_error(L, COLONERR, "GetTitle");
	}

	int DataModel::lua_IsLoaded(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
			lua_pushboolean(L, dm->IsLoaded());
			return 1;
		}
		return luaL_error(L, COLONERR, "IsLoaded");
	}

	int DataModel::lua_GetMessage(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
			lua_pushstring(L, dm->GetMessage().toStdString().c_str());
			return 1;
		}
		return luaL_error(L, COLONERR, "GetMessage");
	}

	int DataModel::lua_SetMessage(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
			dm->SetMessage(QString(luaL_checkstring(L, 2)));
			return 0;
		}
		return luaL_error(L, COLONERR, "SetMessage");
	}

	int DataModel::lua_ClearMessage(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
			dm->ClearMessage();
			return 0;
		}
		return luaL_error(L, COLONERR, "ClearMessage");
	}

	int DataModel::lua_GetFPS(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
			lua_pushnumber(L, dm->GetFPS());
			return 1;
		}
		return luaL_error(L, COLONERR, "GetFPS");
	}

	void DataModel::register_lua_property_setters(lua_State* L){
		Instance::register_lua_property_setters(L);
	}

	void DataModel::register_lua_property_getters(lua_State* L){
		Instance::register_lua_property_getters(L);
	}

	void DataModel::register_lua_methods(lua_State* L){
		ServiceProvider::register_lua_methods(L);

		luaL_Reg methods[]{
			{"SetTitle", lua_SetTitle},
			{"GetTitle", lua_GetTitle},
			{"IsLoaded", lua_IsLoaded},
			{"GetMessage", lua_GetMessage},
			{"SetMessage", lua_SetMessage},
			{"ClearMessage", lua_ClearMessage},
			{"GetFPS", lua_GetFPS},
			{"Shutdown", lua_Shutdown},
			{NULL,NULL}
		};
		luaL_setfuncs(L, methods, 0);
	}
END_INSTANCE
