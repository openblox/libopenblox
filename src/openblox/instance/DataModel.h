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

#ifndef OPENBLOX_INSTANCE_DATAMODEL_H_
#define OPENBLOX_INSTANCE_DATAMODEL_H_

#include "ServiceProvider.h"

BEGIN_INSTANCE

class DataModel: public ServiceProvider{
	public:
		DataModel();
		virtual ~DataModel();

		virtual Instance* GetService(QString className);

		void SetTitle(QString title);
		QString GetTitle();

		bool IsLoaded();

		void loadComplete();

		QString GetMessage();
		void SetMessage(QString msg);
		void ClearMessage();

		void Shutdown();

		double GetFPS();

		DECLARE_SERVICE(DataModel);

		static int lua_Shutdown(lua_State* L);

		static int lua_SetTitle(lua_State* L);
		static int lua_GetTitle(lua_State* L);

		static int lua_IsLoaded(lua_State* L);

		static int lua_GetMessage(lua_State* L);
		static int lua_SetMessage(lua_State* L);
		static int lua_ClearMessage(lua_State* L);

		static int lua_GetFPS(lua_State* L);

		static void register_lua_methods(lua_State* L);
		static void register_lua_property_getters(lua_State* L);
		static void register_lua_property_setters(lua_State* L);
	private:
		bool isloaded;
		QString message;
		QString title;
		OpenBlox::Type::Event* Loaded;
};

END_INSTANCE

#endif
