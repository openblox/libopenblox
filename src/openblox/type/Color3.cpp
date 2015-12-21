/*
 * Copyright 2015 John M. Harris, Jr.
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

#include "Color3.h"

BEGIN_OB_TYPE

STATIC_GAME_INIT(Color3){
	OpenBlox::OBEngine* eng = OpenBlox::OBEngine::getInstance();
	if(!eng){
		return;
	}
	lua_State* L = eng->getLuaState();

	luaL_newmetatable(L, lua_color3_name);

	lua_pushstring(L, "__metatable");
	lua_pushstring(L, OB_TRANSLATE("LuaType", "This metatable is locked"));
	lua_rawset(L, -3);

	lua_pushstring(L, "__methods");
	lua_newtable(L);
	luaL_Reg methods[]{
		{NULL, NULL}
	};
	luaL_setfuncs(L, methods, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__properties");
	lua_newtable(L);
	luaL_Reg props[]{
		{"R", lua_getR},
		{"r", lua_getR},
		{"G", lua_getG},
		{"g", lua_getG},
		{"B", lua_getB},
		{"b", lua_getB},
		{NULL, NULL}
	};
	luaL_setfuncs(L, props, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, lua_toString);
	lua_rawset(L, -3);

	lua_pushstring(L, "__eq");
	lua_pushcfunction(L, lua_eq);
	lua_rawset(L, -3);

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, lua_index);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, lua_newindex);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

Color3::Color3(){
	r = 0;
	g = 0;
	b = 0;
}

Color3::Color3(double r, double g, double b){
	this->r = r;
	this->g = g;
	this->b = b;
}

Color3::~Color3(){}

bool Color3::equals(Color3* other){
	if(other){
		return (r == other->r && g == other->g && b == other->b);
	}
	return false;
}

Color3* Color3::clone(){
	Color3* newGuy = new Color3();
	newGuy->r = r;
	newGuy->g = g;
	newGuy->b = b;

	return newGuy;
}

int Color3::lua_index(lua_State* L){
	Color3* LuaColor3 = checkColor3(L, 1);
	if(LuaColor3){
		const char* name = luaL_checkstring(L, 2);

		lua_getmetatable(L, 1);
		lua_getfield(L, -1, "__properties");
		lua_getfield(L, -1, name);
		if(lua_iscfunction(L, -1)){
			lua_remove(L, -2);
			lua_remove(L, -2);

			lua_pushvalue(L, 1);
			lua_call(L, 1, 1);
			return 1;
		}else{
			lua_pop(L, 2);
			//Check methods
			lua_getfield(L, -1, "__methods");//-2
			lua_getfield(L, -1, name);//-1
			if(lua_iscfunction(L, -1)){
				lua_remove(L, -2);
				lua_remove(L, -3);

				return 1;
			}else{
				lua_pop(L, 3);
				return luaL_error(L, "attempt to index '%s' (a nil value)", name);
			}
		}
	}
	return 0;
}

int Color3::lua_newindex(lua_State* L){
	const char* name = luaL_checkstring(L, 2);
	return luaL_error(L, "%s cannot be assigned to", name);
}

int Color3::lua_toString(lua_State* L){
	Color3* LuaColor3 = checkColor3(L, 1);
	std::string ret = "";
	ret += ((std::ostringstream&)(std::ostringstream() << std::dec << LuaColor3->r)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaColor3->g)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaColor3->b)).str();
	lua_pushstring(L, ret.c_str());
	return 1;
}

int Color3::lua_getR(lua_State* L){
	Color3* LuaColor3 = checkColor3(L, 1);
	lua_pushnumber(L, LuaColor3->r);
	return 1;
}

int Color3::lua_getG(lua_State* L){
	Color3* LuaColor3 = checkColor3(L, 1);
	lua_pushnumber(L, LuaColor3->g);
	return 1;
}

int Color3::lua_getB(lua_State* L){
	Color3* LuaColor3 = checkColor3(L, 1);
	lua_pushnumber(L, LuaColor3->b);
	return 1;
}

int Color3::lua_eq(lua_State* L){
	Color3* LuaColor3 = checkColor3(L, 1);
	if(lua_isuserdata(L, 2)){
		Color3* OtherColor3 = checkColor3(L, 2);
		lua_pushboolean(L, LuaColor3->equals(OtherColor3));
	}else{
		lua_pushboolean(L, false);
	}
	return 1;
}

int Color3::wrap_lua(lua_State* L){
	Color3** LuaColor3 = (Color3**)lua_newuserdata(L, sizeof(*this));
	*LuaColor3 = this;

	luaL_getmetatable(L, lua_color3_name);
	lua_setmetatable(L, -2);
	return 1;
}

Color3* checkColor3(lua_State* L, int n){
	return *(Color3**)luaL_checkudata(L, n, lua_color3_name);
}

END_OB_TYPE
