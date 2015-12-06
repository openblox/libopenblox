#ifndef OPENBLOX_ENUM_ENUMS_H_
#define OPENBLOX_ENUM_ENUMS_H_

#include "LuaEnum.h"

namespace OpenBlox{ namespace Enum{
	#define LENUM(name, ...) \
	extern LuaEnum* Lua##name; \
	enum class name{__VA_ARGS__, __COUNT}

	#define DENUM(name, ...) \
		LuaEnum* Lua##name = new LuaEnum(#name, \
			__VA_ARGS__, \
			NULL \
	); \

	void registerLuaEnums(lua_State* L);

	//Start Enum definitions

	LENUM(HttpContentType,
		ApplicationJson,
		ApplicationXml,
		ApplicationUrlEncoded,
		TextPlain,
		TextXml
	);
}}

#endif
