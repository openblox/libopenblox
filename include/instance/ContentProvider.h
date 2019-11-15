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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "instance/Instance.h"

#ifndef OB_INST_CONTENTPROVIDER
#define OB_INST_CONTENTPROVIDER

namespace OB{
	namespace Instance{
		/**
		 * The ContentProvider class provides scripts access to
		 * internal information about assets loaded and grants the
		 * ability to load assets from scripts.
		 *
		 * @author John M. Harris, Jr.
		 */
		class ContentProvider: public Instance{
			public:
				ContentProvider(OBEngine* eng);
				virtual ~ContentProvider();

				shared_ptr<Type::Event> GetAssetLoaded();
				shared_ptr<Type::Event> GetAssetLoadFailed();

				void Preload(std::string url);
				void Load(std::string url);
				char* GetAsset(std::string url);

				virtual std::string fixedSerializedID();

				DECLARE_LUA_METHOD(Preload);
				DECLARE_LUA_METHOD(Load);
				DECLARE_LUA_METHOD(GetAsset);
				DECLARE_LUA_METHOD(getRequestQueueSize);

				static void register_lua_methods(lua_State* L);
				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				static void register_lua_events(lua_State* L);

				DECLARE_CLASS(ContentProvider);

				shared_ptr<Type::Event> AssetLoaded;
				shared_ptr<Type::Event> AssetLoadFailed;
		};
	}
}

#endif // OB_INST_CONTENTPROVIDER


// Local Variables:
// mode: c++
// End:
