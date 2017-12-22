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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "instance/BaseValue.h"

#include "type/Color3.h"

#ifndef OB_INST_COLOR3VALUE
#define OB_INST_COLOR3VALUE

namespace OB{
	namespace Instance{
		/**
		 * Color3Value instances are used to store a color value.
		 *
		 * @author John M. Harris, Jr.
		 */
		class Color3Value: public BaseValue{
			public:
				Color3Value(OBEngine* eng);
				virtual ~Color3Value();

				shared_ptr<Type::Color3> getValue();
				void setValue(shared_ptr<Type::Color3> value);

#if HAVE_ENET
				/**
				 * Replicates properties of this Instance.
				 *
				 * @param peer Peer
				 * @author John M. Harris, Jr.
				 */
				virtual void replicateProperties(shared_ptr<NetworkReplicator> peer);
#endif

				virtual std::map<std::string, _PropertyInfo> getProperties();
				virtual shared_ptr<Type::VarWrapper> getProperty(std::string prop);
				virtual void setProperty(std::string prop, shared_ptr<Type::VarWrapper> val);

				DECLARE_LUA_METHOD(setValue);
				DECLARE_LUA_METHOD(getValue);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);

				DECLARE_CLASS(Color3Value);

				shared_ptr<Type::Color3> Value;
		};
	}
}

#endif // OB_INST_COLOR3VALUE


// Local Variables:
// mode: c++
// End:
