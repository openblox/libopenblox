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

#ifndef OB_INST_INTCONSTRAINEDVALUE
#define OB_INST_INTCONSTRAINEDVALUE

namespace OB{
	namespace Instance{
		/**
		 * IntConstrainedValue instances are used to store an
		 * integer value between an upper and lower boundary,
		 * inclusive.
		 *
		 * @author John M. Harris, Jr.
		 */
		class IntConstrainedValue: public Instance{
			public:
			    IntConstrainedValue();
				virtual ~IntConstrainedValue();

			    int getValue();
				void setValue(int value);

			    int getMinValue();
				void setMinValue(int minValue);

				int getMaxValue();
				void setMaxValue(int maxValue);

				DECLARE_LUA_METHOD(setValue);
				DECLARE_LUA_METHOD(getValue);
				DECLARE_LUA_METHOD(setMinValue);
				DECLARE_LUA_METHOD(getMinValue);
				DECLARE_LUA_METHOD(setMaxValue);
				DECLARE_LUA_METHOD(getMaxValue);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);

				DECLARE_CLASS(IntConstrainedValue);

				int MaxValue;
			    int MinValue;
			    int Value;
		};
	}
}

#endif // OB_INST_INTCONSTRAINEDVALUE


// Local Variables:
// mode: c++
// End:
