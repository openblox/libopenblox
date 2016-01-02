/*
 * Copyright 2016 John M. Harris, Jr.
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

#ifndef OPENBLOX_INSTANCE_NUMBERVALUE_H_
#define OPENBLOX_INSTANCE_NUMBERVALUE_H_

#include "Instance.h"

BEGIN_INSTANCE

class NumberValue: public Instance{
	public:
		NumberValue();
		virtual ~NumberValue();

		double Value;

		DECLARE_CLASS(NumberValue);

		static int lua_getValue(lua_State* L);
		static int lua_setValue(lua_State* L);

		static void register_lua_property_getters(lua_State* L);
		static void register_lua_property_setters(lua_State* L);
};

END_INSTANCE

#endif
