#include "Enums.h"

namespace OpenBlox{ namespace Enum{
	DENUM(HttpContentType,
		"ApplicationJson",
		"ApplicationXml",
		"ApplicationUrlEncoded",
		"TextPlain",
		"TextXml"
	);

	void registerLuaEnums(lua_State* L){
		lua_newtable(L);

		for(std::map<QString, LuaEnum*>::iterator it = LuaEnum::enums->begin(); it != LuaEnum::enums->end(); ++it){
			LuaEnum* en = it->second;

			lua_pushstring(L, en->type.toStdString().c_str());
			en->wrap_lua(L);
			lua_rawset(L, -3);
		}

		lua_setglobal(L, "Enum");
	}
}}
