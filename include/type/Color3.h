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

#include "type/Type.h"

#include "oblibconfig.h"

#if HAVE_IRRLICHT
#include <irrlicht/SColor.h>
#endif

#ifndef OB_TYPE_COLOR3
#define OB_TYPE_COLOR3

namespace OB{
	namespace Type{
		class Color3: public Type{
		public:
			Color3();
			Color3(double r, double g, double b);
			Color3(int r, int g, int b);
			Color3(std::string str);
			virtual ~Color3();

			double getR();
			double getG();
			double getB();

			int getRi();
			int getGi();
			int getBi();

#if HAVE_IRRLICHT

			irr::video::SColor toIrrlichtSColor(unsigned int alpha = 255);

#endif

			virtual bool equals(shared_ptr<Type> other);

			virtual std::string toString();

			static int lua_getR(lua_State* L);
			static int lua_getG(lua_State* L);
			static int lua_getB(lua_State* L);

			static int lua_eq(lua_State* L);

			static void register_lua_metamethods(lua_State* L);
			static void register_lua_property_setters(lua_State* L);
			static void register_lua_property_getters(lua_State* L);

			DECLARE_TYPE();

			double r;
			double g;
			double b;

		};

		shared_ptr<Color3> checkColor3(lua_State* L, int n, bool errIfNot = true, bool allowNil = true);
	}
}

#endif // OB_TYPE_COLOR3


// Local Variables:
// mode: c++
// End:
