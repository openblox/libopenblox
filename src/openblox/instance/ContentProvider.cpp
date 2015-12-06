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

#include "ContentProvider.h"

#include <AssetLocator.h>

BEGIN_INSTANCE
	DEFINE_CLASS(ContentProvider, false, isDataModel, Instance);

	ContentProvider::ContentProvider() : Instance(){}

	ContentProvider::~ContentProvider(){}

	DEFINE_SERVICE(ContentProvider);

	char* ContentProvider::GetContent(QString url){
		OpenBlox::AssetLocator* al = OpenBlox::AssetLocator::getInstance();
		if(!al){
			return NULL;
		}
		return al->getAsset(url)->data;
	}

	void ContentProvider::PutContent(QString url, QString content){
		OpenBlox::AssetLocator* al = OpenBlox::AssetLocator::getInstance();
		if(!al){
			return;
		}
		al->putAsset(url, content);
	}

	void ContentProvider::Preload(QString url){
		OpenBlox::AssetLocator* al = OpenBlox::AssetLocator::getInstance();
		if(!al){
			return;
		}
		al->getAsset(url);
	}

	int ContentProvider::lua_GetContent(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(ContentProvider* cp = dynamic_cast<ContentProvider*>(inst)){
			QString url = QString(luaL_checkstring(L, 2));

			char* cont = cp->GetContent(url);

			lua_pushstring(L, cont);

			return 1;
		}
		return luaL_error(L, COLONERR, "GetContent");
	}

	int ContentProvider::lua_PutContent(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(ContentProvider* cp = dynamic_cast<ContentProvider*>(inst)){
			QString url = QString(luaL_checkstring(L, 2));
			QString content = QString(luaL_checkstring(L, 3));

			cp->PutContent(url, content);

			return 0;
		}
		return luaL_error(L, COLONERR, "PutContent");
	}

	int ContentProvider::lua_Preload(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(ContentProvider* cp = dynamic_cast<ContentProvider*>(inst)){
			QString url = QString(luaL_checkstring(L, 2));

			cp->Preload(url);

			return 0;
		}
		return luaL_error(L, COLONERR, "Preload");
	}

	void ContentProvider::register_lua_methods(lua_State* L){
		Instance::register_lua_methods(L);

		luaL_Reg methods[]{
			{"Preload", lua_Preload},
			{"GetContent", lua_GetContent},
			{"PutContent", lua_PutContent},
			{NULL, NULL}
		};
		luaL_setfuncs(L, methods, 0);
	}

END_INSTANCE
