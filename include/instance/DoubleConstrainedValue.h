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

#include "instance/Instance.h"

#ifndef OB_INST_DOUBLECONSTRAINEDVALUE
#define OB_INST_DOUBLECONSTRAINEDVALUE

namespace OB{
	namespace Instance{
		/**
		 * DoubleConstrainedValue instances are used to store a
		 * number value between an upper and lower boundary,
		 * inclusive.
		 *
		 * @author John M. Harris, Jr.
		 */
		class DoubleConstrainedValue: public Instance{
		public:
			DoubleConstrainedValue(OBEngine* eng);
			virtual ~DoubleConstrainedValue();

			double getValue();
			void setValue(double value);

			double getMinValue();
			void setMinValue(double minValue);

			double getMaxValue();
			void setMaxValue(double maxValue);

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
			DECLARE_LUA_METHOD(setMinValue);
			DECLARE_LUA_METHOD(getMinValue);
			DECLARE_LUA_METHOD(setMaxValue);
			DECLARE_LUA_METHOD(getMaxValue);

			static void register_lua_property_getters(lua_State* L);
			static void register_lua_property_setters(lua_State* L);

			DECLARE_CLASS(DoubleConstrainedValue);

			double MaxValue;
			double MinValue;
			double Value;
		};
	}
}

#endif // OB_INST_DOUBLECONSTRAINEDVALUE


// Local Variables:
// mode: c++
// End:
