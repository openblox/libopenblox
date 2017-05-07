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

#include "instance/ContentProvider.h"

#include "OBEngine.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(ContentProvider, false, isDataModel, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    ContentProvider::ContentProvider(){
			Name = ClassName;
			netId = OB_NETID_NOT_REPLICATED;

			AssetLoaded = make_shared<Type::Event>("AssetLoaded");
			AssetLoadFailed = make_shared<Type::Event>("AssetLoadFailed");
		}

	    ContentProvider::~ContentProvider(){}

		shared_ptr<Instance> ContentProvider::cloneImpl(){
			return NULL;
		}

		shared_ptr<Type::Event> ContentProvider::GetAssetLoaded(){
			return AssetLoaded;
		}

		shared_ptr<Type::Event> ContentProvider::GetAssetLoadFailed(){
			return AssetLoadFailed;
		}

	    void ContentProvider::Preload(std::string url){
		    OBEngine* eng = OBEngine::getInstance();
			shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();

			assetLoc->loadAsset(url);
		}

		void ContentProvider::Load(std::string url){
		    OBEngine* eng = OBEngine::getInstance();
			shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();

			assetLoc->loadAssetSync(url);
		}

		char* ContentProvider::GetAsset(std::string url){
			OBEngine* eng = OBEngine::getInstance();
			shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();

			shared_ptr<AssetResponse> resp = assetLoc->getAsset(url);
			if(resp){
			    char* dat = resp->getData();
				int siz = resp->getSize();
				if(dat && siz > 0){
					return dat;
				}
			}
			
		    return NULL;
		}

		int ContentProvider::lua_Preload(lua_State* L){
		    shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(shared_ptr<ContentProvider> cp = dynamic_pointer_cast<ContentProvider>(inst)){
				std::string urlStr = std::string(luaL_checkstring(L, 2));
				
			    cp->Preload(urlStr);
				return 0;
			}
			
			return luaL_error(L, COLONERR, "Preload");
		}

		int ContentProvider::lua_Load(lua_State* L){
		    shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(shared_ptr<ContentProvider> cp = dynamic_pointer_cast<ContentProvider>(inst)){
				std::string urlStr = std::string(luaL_checkstring(L, 2));
				
			    cp->Load(urlStr);
				return 0;
			}
			
			return luaL_error(L, COLONERR, "Load");
		}

		int ContentProvider::lua_GetAsset(lua_State* L){
		    shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(shared_ptr<ContentProvider> cp = dynamic_pointer_cast<ContentProvider>(inst)){
				std::string urlStr = std::string(luaL_checkstring(L, 2));
				
			    char* strAsset = cp->GetAsset(urlStr);
				if(strAsset == NULL){
					lua_pushnil(L);
					return 1;
				}
				
				lua_pushstring(L, strAsset);
				return 1;
			}
			
			return luaL_error(L, COLONERR, "GetAsset");
		}

		int ContentProvider::lua_getRequestQueueSize(lua_State* L){
		    shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(shared_ptr<ContentProvider> cp = dynamic_pointer_cast<ContentProvider>(inst)){
			    OBEngine* eng = OBEngine::getInstance();
				shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();

				lua_pushinteger(L, assetLoc->getRequestQueueSize());
				return 1;
			}
			
			lua_pushnil(L);
			return 1;
		}

		void ContentProvider::register_lua_methods(lua_State* L){
		    Instance::register_lua_methods(L);

			luaL_Reg methods[] = {
				{"Preload", lua_Preload},
				{"Load", lua_Load},
				{"GetAsset", lua_GetAsset},
			    {NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}
		
		void ContentProvider::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"RequestQueueSize", Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void ContentProvider::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);
			
			luaL_Reg properties[] = {
				{"RequestQueueSize", lua_getRequestQueueSize},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void ContentProvider::register_lua_events(lua_State* L){
			Instance::register_lua_events(L);
			
			luaL_Reg events[] = {
				{"AssetLoaded", WRAP_EVT(ContentProvider, AssetLoaded)},
				{"AssetLoadFailed", WRAP_EVT(ContentProvider, AssetLoadFailed)},
				{NULL, NULL}
			};
			luaL_setfuncs(L, events, 0);
		}
	}
}
