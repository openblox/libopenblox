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

#include "instance/Instance.h"

#include "type/Color3.h"

#ifndef OB_INST_CONTENTPROVIDER
#define OB_INST_CONTENTPROVIDER

namespace OB{
	namespace Instance{
		/**
		 * Lighting provides access to lighting related options as well as
		 * ambient colors and similar properties.
		 *
		 * @author John M. Harris, Jr.
		 */
		class ContentProvider: public Instance{
			public:
			    ContentProvider();
				virtual ~ContentProvider();

				void Preload(std::string url);
				void Load(std::string url);
			    char* GetAsset(std::string url);
				
				DECLARE_LUA_METHOD(Preload);
				DECLARE_LUA_METHOD(Load);
				DECLARE_LUA_METHOD(GetAsset);
				DECLARE_LUA_METHOD(getRequestQueueSize);

				static void register_lua_methods(lua_State* L);
				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);

				DECLARE_CLASS(ContentProvider);
		};
	}
}

#endif // OB_INST_CONTENTPROVIDER


// Local Variables:
// mode: c++
// End:
