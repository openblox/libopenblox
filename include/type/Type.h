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

#include <string>

#include "lua/OBLua.h"
#include "obtype.h"

#include "mem.h"

#include <vector>

#ifndef OB_TYPE_TYPE
#define OB_TYPE_TYPE

typedef void (*luaRegisterFunc)(lua_State* L);

#define COLONERR "Expected ':' not '.' calling member function %s"

#define DECLARE_TYPE() \
	virtual std::string getLuaClassName(); \
	static void _ob_init(); \
protected: \
	static std::string TypeName; \
	static std::string LuaTypeName

#define DEFINE_TYPE(Type_Name) \
   	std::string Type_Name::TypeName = #Type_Name; \
	std::string Type_Name::LuaTypeName = "luaL_Type_" #Type_Name; \
	std::string Type_Name::getLuaClassName(){ \
		return LuaTypeName; \
	} \
	void Type_Name::_ob_init()

#define DECLARE_LUA_METHOD(MethodName) \
	static int lua_##MethodName(lua_State* L)

namespace OB{
	namespace Type{
		/**
		 * Type is the base class of all descriptor objects and other
		 * objects outside the DataModel.
		 *
		 * @author John M. Harris, Jr.
		 */
		class Type: public std::enable_shared_from_this<Type>{
			public:
				Type();
				virtual ~Type();

				/**
				 * Convenience method to register this class as a
				 * Type class.
				 *
				 * @author John M. Harris, Jr.
				 */
				static void registerLuaType(std::string typeName, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters);

				/**
				 * Returns the stringified version of this Type.
				 *
				 * @returns Stringified version of type
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual std::string toString();

			    /**
				 * Handles attempts to set properties of this Type.
				 *
				 * @param L Lua State
				 *
				 * @returns 0 or error (Does not return if an
				 * error occurs)
				 *
				 * @author DigiTechs
				 * @author John M. Harris, Jr.
				 */
				static int lua_index(lua_State* L);

				/**
				 * Handles attempts to get properties, methods or
				 * events of this Type.
				 *
				 * @param L Lua State
				 *
				 * @returns 1 if there is a corresponding value,
				 * otherwise errors. (No return)
				 *
				 * @author DigiTechs
				 * @author John M. Harris, Jr.
				 */
				static int lua_newindex(lua_State* L);

			    /**
				 * Handles equality tests from Lua.
				 *
				 * @param L Lua State
				 *
				 * @returns 1, a bool value on the lua stack.
				 * (This value is true if the two Instances are
				 * the same)
				 *
				 * @author John M. Harris, Jr.
				 */
				static int lua_eq(lua_State* L);

				static int lua_gc(lua_State* L);

				/**
				 * Handles tostring calls on this Type from Lua.
				 *
				 * @param L Lua State
				 *
				 * @returns 1, a string value on the lua stack. (The
				 * result of Type::toString)
				 *
				 * @author John M. Harris, Jr.
				 */
				static int lua_toString(lua_State* L);

				int wrap_lua(lua_State* L);

				/**
				 * Lua Metamethods for the Type class.
				 *
				 * @param L Lua State
				 *
				 * @author John M. Harris, Jr.
				 * @author DigiTechs
				 */
				static void register_lua_metamethods(lua_State* L);

				/**
				 * Lua methods for the Type class.
				 *
				 * @param L Lua State
				 *
				 * @author John M. Harris, Jr.
				 * @author DigiTechs
				 */
				static void register_lua_methods(lua_State* L);

				/**
				 * Lua property setters for the Type class.
				 *
				 * @param L Lua State
				 *
				 * @author John M. Harris, Jr.
				 * @author DigiTechs
				 */
				static void register_lua_property_setters(lua_State* L);

				/**
				 * Lua property getters for the Type class.
				 *
				 * @param L Lua State
				 *
				 * @author John M. Harris, Jr.
				 * @author DigiTechs
				 */
				static void register_lua_property_getters(lua_State* L);

				static shared_ptr<Type> checkType(lua_State* L, int index);

				DECLARE_TYPE();

				static std::vector<std::string> typeList;
		};
	}
}

#endif // OB_TYPE_TYPE


// Local Variables:
// mode: c++
// End:
