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

#include "type/UDim.h"

#include "oblibconfig.h"

#ifndef OB_TYPE_UDIM2
#define OB_TYPE_UDIM2

namespace OB{
	namespace Type{
		class UDim2: public Type{
		public:
			UDim2();
			UDim2(double xScale, double xOffset, double yScale, double yOffset);
			UDim2(std::string str);
			virtual ~UDim2();

			shared_ptr<UDim> getX();
			shared_ptr<UDim> getY();

			shared_ptr<UDim2> add(shared_ptr<UDim2> v);
			shared_ptr<UDim2> sub(shared_ptr<UDim2> v);
			virtual bool equals(shared_ptr<Type> other);

			virtual std::string toString();

			static int lua_getX(lua_State* L);
			static int lua_getY(lua_State* L);

			static int lua_eq(lua_State* L);
			static int lua_add(lua_State* L);
			static int lua_sub(lua_State* L);

			static void register_lua_metamethods(lua_State* L);
			static void register_lua_methods(lua_State* L);
			static void register_lua_property_setters(lua_State* L);
			static void register_lua_property_getters(lua_State* L);

			DECLARE_TYPE();

			shared_ptr<UDim> x;
			shared_ptr<UDim> y;

		};

		shared_ptr<UDim2> checkUDim2(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);
	}
}

#endif // OB_TYPE_UDIM2


// Local Variables:
// mode: c++
// End:
