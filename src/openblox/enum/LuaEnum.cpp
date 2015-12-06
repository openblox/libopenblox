#include "LuaEnum.h"

namespace OpenBlox{ namespace Enum{
	/**
	 * Holds a reference to all Lua enums by name.
	 * @author DigiTechs
	 */
	std::map<QString, LuaEnum*>* LuaEnum::enums = NULL;

	STATIC_INIT(LuaEnum){
		lua_State* L = OpenBlox::OBEngine::getInstance()->getLuaState();

		luaL_newmetatable(L, lua_enum_name);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, LuaEnum::lua_index);
		lua_rawset(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, LuaEnum::lua_toString);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	LuaEnum::LuaEnum(QString type, ...){
		this->type = type;

		enumValues = std::map<QString, LuaEnumItem*>();
		va_list args;
		va_start(args, type);
		int i = 0;
		while(true){
			const char* arg = va_arg(args, const char*);
			if(arg == NULL){
				break;
			}
			enumValues.insert(std::pair<QString, LuaEnumItem*>(QString(arg), new LuaEnumItem(type, arg, i)));
			i++;
		}
		va_end(args);

		if(enums == NULL){
			enums = new std::map<QString, LuaEnum*>();
		}

		enums->insert(std::pair<QString, LuaEnum*>(type, this));
	}

	LuaEnum::~LuaEnum(){}

	/**
	 * Gets an EnumItem by value.
	 * @returns LuaEnumItem* Pointer to this EnumItem's Lua representation.
	 * @author DigiTechs
	 */
	LuaEnumItem* LuaEnum::getEnumItem(int value){
		for(std::map<QString, LuaEnumItem*>::iterator it = enumValues.begin(); it != enumValues.end(); ++it){
			if(it->second->value == value){
				return it->second;
			}
		}
		return NULL;
	}

	/**
	 * Wraps this LuaEnum for Lua.
	 * @param lua_State* L The Lua state to wrap this LuaEnum for.
	 * @returns int Number of returned values.
	 * @author DigiTechs
	 */
	int LuaEnum::wrap_lua(lua_State* L){
		LuaEnum** enm = (LuaEnum**)lua_newuserdata(L, sizeof(*this));
		*enm = this;

		luaL_getmetatable(L, lua_enum_name);
		lua_setmetatable(L, -2);

		return 1;
	}

	/**
	 * Returns properties of this type.
	 * @param lua_State* L The Lua state indexing this LuaEnum.
	 * @returns int Number of returned values.
	 * @author DigiTechs
	 * @author John M. Harris, Jr.
	 */
	int LuaEnum::lua_index(lua_State* L){
		LuaEnum* con = checkEnum(L, 1);
		if(con != NULL){
			QString propname = QString(luaL_checkstring(L, 2));
			if(propname == "GetEnumItems"){
				lua_pushcfunction(L, lua_getEnumItems);
				return 1;
			}else{
				LuaEnumItem* enm = con->enumValues[propname];
				if(enm != NULL){
					return enm->wrap_lua(L);
				}
			}
		}
		return 0;
	}

	/**
	 * Returns a string representation of this LuaEnum.
	 * @param lua_State* L The Lua state calling tostring.
	 * @returns int Number of returned values.
	 * @author DigiTechs
	 * @author John M. Harris, Jr.
	 */
	int LuaEnum::lua_toString(lua_State* L){
		LuaEnum* itm = checkEnum(L, 1);
		QString name = "Enum.";
		name += itm->type;
		lua_pushstring(L, name.toStdString().c_str());
		return 1;
	}

	/**
	 * Returns all EnumItems of this Enum, in their Lua wrappers.
	 * @param lua_State* L The Lua state calling GetEnumItems.
	 * @returns int Number of returned values, 1 (a table).
	 * @author John M. Harris, Jr.
	 */
	int LuaEnum::lua_getEnumItems(lua_State* L){
		LuaEnum* itm = checkEnum(L, 1);
		if(itm != NULL){
			lua_newtable(L);

			int i = 1;

			for(std::map<QString, LuaEnumItem*>::iterator it = itm->enumValues.begin(); it != itm->enumValues.end(); ++it){
				it->second->wrap_lua(L);
				lua_rawseti(L, -2, i);
				i++;
			}

			return 1;
		}
		return 0;
	}

	/**
	 * Checks that the object at this index on the Lua stack is a LuaEnum.
	 * @param lua_State* L The Lua state.
	 * @param int n Index on the stack.
	 * @returns LuaEnum* LuaEnum from index on stack or errors on the Lua side.
	 * @author John M. Harris, Jr.
	 */
	LuaEnum* checkEnum(lua_State* L, int n){
		return *(LuaEnum**)luaL_checkudata(L, n, lua_enum_name);
	}
}}
