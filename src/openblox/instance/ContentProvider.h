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

#ifndef OPENBLOX_INSTANCE_CONTENTPROVIDER_H_
#define OPENBLOX_INSTANCE_CONTENTPROVIDER_H_

#include "Instance.h"

BEGIN_INSTANCE

class ContentProvider: public Instance{
	public:
		ContentProvider();
		virtual ~ContentProvider();

		char* GetContent(QString url);
		void PutContent(QString url, QString content);
		void Preload(QString url);

		NO_REPLICATE();

		DECLARE_SERVICE(ContentProvider);

		static int lua_GetContent(lua_State* L);
		static int lua_PutContent(lua_State* L);
		static int lua_Preload(lua_State* L);

		static void register_lua_methods(lua_State* L);
};

END_INSTANCE

#endif
