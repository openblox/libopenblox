#ifndef OPENBLOX_ENUM_LUAENUM_H_
#define OPENBLOX_ENUM_LUAENUM_H_

#include "LuaEnumItem.h"

#define lua_enum_name "luaL_Enum"

namespace OpenBlox{ namespace Enum{
	/**
	 * Lua representation of enumeration classes.
	 * @author John M. Harris, Jr.
	 * @author DigiTechs
	 */
	class LuaEnum{
		public:
			LuaEnum(QString type, ...);
			virtual ~LuaEnum();

			std::map<QString, LuaEnumItem*> enumValues;
			QString type;
			LuaEnumItem* getEnumItem(int value);

			int wrap_lua(lua_State* L);

			static int lua_index(lua_State* L);
			static int lua_toString(lua_State* L);
			static int lua_getEnumItems(lua_State* L);

			static std::map<QString, LuaEnum*>* enums;

			DECLARE_STATIC_INIT(LuaEnum);
	};

	LuaEnum* checkEnum(lua_State* L, int idx);
}}

#endif
