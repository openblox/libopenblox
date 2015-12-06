#ifndef OPENBLOX_ENUM_LUAENUMITEM_H_
#define OPENBLOX_ENUM_LUAENUMITEM_H_

#include <OpenBlox.h>

#define lua_enumitem_name "luaL_EnumItem"

namespace OpenBlox{ namespace Enum{
	/**
	 * Lua representation of enumeration values.
	 * @author John M. Harris, Jr.
	 * @author DigiTechs
	 */
	class LuaEnumItem{
		public:
			LuaEnumItem(QString type, QString name, int value);
			virtual ~LuaEnumItem();

			QString type;
			QString name;
			int value;

			template<class T> T toEnum();

			int wrap_lua(lua_State* L);

			static int lua_index(lua_State* L);
			static int lua_toString(lua_State* L);

			DECLARE_STATIC_INIT(LuaEnumItem);
	};

	class LuaEnum;

	LuaEnumItem* checkEnumItem(lua_State* L, int idx, LuaEnum* enum_type);
	LuaEnumItem* checkAnyEnumItem(lua_State* L, int idx);
}}

#endif
