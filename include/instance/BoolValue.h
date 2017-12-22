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

#ifndef OB_INST_BOOLVALUE
#define OB_INST_BOOLVALUE

namespace OB{
	namespace Instance{
		/**
		 * BoolValue instances are used to store a boolean value.
		 *
		 * @author John M. Harris, Jr.
		 */
		class BoolValue: public BaseValue{
			public:
				BoolValue(OBEngine* eng);
				virtual ~BoolValue();

				bool getValue();
				void setValue(bool value);

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

				DECLARE_CLASS(BoolValue);

				bool Value;
		};
	}
}

#endif // OB_INST_BOOLVALUE


// Local Variables:
// mode: c++
// End:
