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

#include "instance/BasePlayerGui.h"

#include "type/Color3.h"

#ifndef OB_INST_COREGUI
#define OB_INST_COREGUI

namespace OB{
	namespace Instance{
		/**
		 * CoreGui is a persistent container for UI elements to be
		 * shown to the end user. Unlike PlayerGui, CoreGui is shown
		 * even when there is no Player object. This makes it useful
		 * for server information output or Playerless client games.
		 *
		 * Some 2D games may choose to use CoreGui to show their
		 * content, so all default content, even when enabled, is
		 * set with a Z-Index of no higher than 1. If you set your
		 * Z-Index to be higher than 1, even if you don't disable
		 * the default content, your UI elements will be above the
		 * default elements.
		 *
		 * When Enabled is false, no elements in CoreGui will be
		 * visible. This is comparable to the Visible property of most
		 * GUI objects.
		 *
		 * @author John M. Harris, Jr.
		 */
		class CoreGui: public BasePlayerGui{
		public:
			CoreGui(OBEngine* eng);
			virtual ~CoreGui();

			bool isEnabled();
			void setEnabled(bool enabled);

			virtual shared_ptr<Type::Vector2> getAbsolutePosition();
			virtual shared_ptr<Type::Vector2> getAbsoluteSize();

			virtual void render();

#if HAVE_ENET
			/**
			 * Replicates properties of this Instance.
			 *
			 * @param peer Peer
			 * @author John M. Harris, Jr.
			 */
			virtual void replicateProperties(shared_ptr<NetworkReplicator> peer);
#endif

#if HAVE_PUGIXML
			virtual std::string serializedID();
#endif

			virtual std::map<std::string, _PropertyInfo> getProperties();
			virtual shared_ptr<Type::VarWrapper> getProperty(std::string prop);
			virtual void setProperty(std::string prop, shared_ptr<Type::VarWrapper> val);

			DECLARE_LUA_METHOD(getEnabled);
			DECLARE_LUA_METHOD(setEnabled);

			static void register_lua_property_getters(lua_State* L);
			static void register_lua_property_setters(lua_State* L);

			DECLARE_CLASS(CoreGui);

			bool Enabled;
		};
	}
}

#endif // OB_INST_COREGUI


// Local Variables:
// mode: c++
// End:
