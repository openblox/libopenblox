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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox.	 If not, see <https://www.gnu.org/licenses/>.
 */

#include "instance/LayerCollector.h"

#ifndef OB_INST_SCREENGUI
#define OB_INST_SCREENGUI

namespace OB{
	namespace Instance{
		class ScreenGui: public LayerCollector{
			public:
			    ScreenGui(OBEngine* eng);
				virtual ~ScreenGui();

				virtual shared_ptr<Type::Vector2> getAbsolutePosition();
				virtual shared_ptr<Type::Vector2> getAbsoluteSize();

				virtual bool isEnabled();
				virtual void setEnabled(bool enabled);
				virtual int getDisplayOrder();
				virtual void setDisplayOrder(int displayOrder);

				virtual void render();

				#if HAVE_ENET
				virtual void replicateProperties(shared_ptr<NetworkReplicator> peer);
				#endif

				virtual std::map<std::string, _PropertyInfo> getProperties();
				virtual shared_ptr<Type::VarWrapper> getProperty(std::string prop);
				virtual void setProperty(std::string prop, shared_ptr<Type::VarWrapper> val);

				DECLARE_LUA_METHOD(getEnabled);
				DECLARE_LUA_METHOD(setEnabled);
				DECLARE_LUA_METHOD(getDisplayOrder);
				DECLARE_LUA_METHOD(setDisplayOrder);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				
				DECLARE_CLASS(ScreenGui);

				bool Enabled;
				int DisplayOrder;
		};
	}
}

#endif // OB_INST_SCREENGUI


// Local Variables:
// mode: c++
// End:
