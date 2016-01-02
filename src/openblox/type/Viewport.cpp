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

#include "Viewport.h"

#include "Color3.h"

BEGIN_OB_TYPE

STATIC_GAME_INIT(Viewport){
	OpenBlox::OBEngine* eng = OpenBlox::OBEngine::getInstance();
	if(!eng){
		return;
	}
	lua_State* L = eng->getLuaState();

	luaL_newmetatable(L, lua_viewport_name);

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
	luaL_Reg propsget[]{
			{"BackgroundColor", lua_getBackgroundColor},
			{"Width", lua_getWidth},
			{"Height", lua_getHeight},
			{"Left", lua_getLeft},
			{"Top", lua_getTop},
			{"ActualWidth", lua_getActualWidth},
			{"ActualHeight", lua_getActualHeight},
			{"ActualLeft", lua_getActualLeft},
			{"ActualTop", lua_getActualTop},
			{"ShadowsEnabled", lua_getShadowsEnabled},
			{"SkiesEnabled", lua_getSkiesEnabled},
			{"OrientationMode", lua_getOrientationMode},
		{NULL, NULL}
	};
	luaL_setfuncs(L, propsget, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__properties_set");
	lua_newtable(L);
	luaL_Reg propsset[]{
			{"BackgroundColor", lua_setBackgroundColor},
			{"ShadowsEnabled", lua_setShadowsEnabled},
			{"SkiesEnabled", lua_setSkiesEnabled},
			{"OrientationMode", lua_setOrientationMode},
		{NULL, NULL}
	};
	luaL_setfuncs(L, propsset, 0);
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

Viewport::Viewport(Ogre::Viewport* realVP){
	this->realVP = realVP;
}

Viewport::~Viewport(){}

bool Viewport::equals(Viewport* other){
	if(other){
		return realVP == other->realVP;
	}
	return false;
}

Viewport* Viewport::clone(){
	return new Viewport(realVP);
}

int Viewport::lua_index(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
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

int Viewport::lua_newindex(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		const char* name = luaL_checkstring(L, 2);

		lua_getmetatable(L, 1);
		lua_getfield(L, -1, "__properties_set");
		lua_getfield(L, -1, name);
		if(lua_iscfunction(L, -1)){
			lua_remove(L, -2);
			lua_remove(L, -2);

			lua_pushvalue(L, 1);
			lua_pushvalue(L, 3);
			lua_call(L, 2, 0);

			return 0;
		}else{
			lua_pop(L, 3);
		}
		return luaL_error(L, "%s cannot be assigned to", name);
	}
	return 0;
}

int Viewport::lua_toString(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	std::string ret = "";
	ret += "Viewport (" + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaViewport->realVP->getActualWidth())).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaViewport->realVP->getActualHeight())).str() + ")";
	lua_pushstring(L, ret.c_str());
	return 1;
}

int Viewport::lua_getBackgroundColor(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		Ogre::ColourValue bgCol = LuaViewport->realVP->getBackgroundColour();
		return (new Color3(bgCol.r, bgCol.g, bgCol.b))->wrap_lua(L);
	}
	return 0;
}

int Viewport::lua_setBackgroundColor(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		Color3* bgCol = checkColor3(L, 2);
		if(bgCol){
			LuaViewport->realVP->setBackgroundColour(Ogre::ColourValue(bgCol->r, bgCol->g, bgCol->b));
		}else{
			LuaViewport->realVP->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
		}
	}
	return 0;
}

int Viewport::lua_getWidth(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		lua_pushnumber(L, LuaViewport->realVP->getWidth());
		return 1;
	}
	return 0;
}

int Viewport::lua_getHeight(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		lua_pushnumber(L, LuaViewport->realVP->getHeight());
		return 1;
	}
	return 0;
}

int Viewport::lua_getLeft(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		lua_pushnumber(L, LuaViewport->realVP->getLeft());
		return 1;
	}
	return 0;
}

int Viewport::lua_getTop(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		lua_pushnumber(L, LuaViewport->realVP->getTop());
		return 1;
	}
	return 0;
}

int Viewport::lua_getActualWidth(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		lua_pushinteger(L, LuaViewport->realVP->getActualWidth());
		return 1;
	}
	return 0;
}

int Viewport::lua_getActualHeight(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		lua_pushinteger(L, LuaViewport->realVP->getActualHeight());
		return 1;
	}
	return 0;
}

int Viewport::lua_getActualLeft(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		lua_pushinteger(L, LuaViewport->realVP->getActualLeft());
		return 1;
	}
	return 0;
}

int Viewport::lua_getActualTop(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		lua_pushinteger(L, LuaViewport->realVP->getActualTop());
		return 1;
	}
	return 0;
}

int Viewport::lua_getShadowsEnabled(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		lua_pushboolean(L, LuaViewport->realVP->getShadowsEnabled());
		return 1;
	}
	return 0;
}

int Viewport::lua_getSkiesEnabled(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		lua_pushboolean(L, LuaViewport->realVP->getSkiesEnabled());
		return 1;
	}
	return 0;
}

int Viewport::lua_setShadowsEnabled(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		//Following ROBLOX's ways....
		bool newVal = false;
		if(lua_isboolean(L, 2)){
			newVal = lua_toboolean(L, 2);
		}else if(!lua_isnoneornil(L, 2)){
			newVal = true;
		}
		if(LuaViewport->realVP->getShadowsEnabled() != newVal){
			LuaViewport->realVP->setShadowsEnabled(newVal);
		}
	}
	return 0;
}

int Viewport::lua_setSkiesEnabled(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		//Following ROBLOX's ways....
		bool newVal = false;
		if(lua_isboolean(L, 2)){
			newVal = lua_toboolean(L, 2);
		}else if(!lua_isnoneornil(L, 2)){
			newVal = true;
		}
		if(LuaViewport->realVP->getSkiesEnabled() != newVal){
			LuaViewport->realVP->setSkiesEnabled(newVal);
		}
	}
	return 0;
}

int Viewport::lua_getOrientationMode(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		int orient = LuaViewport->realVP->getOrientationMode();
		switch(orient){
			case 0: {
				return OpenBlox::Enum::LuaOrientationMode->getEnumItem((int)OpenBlox::Enum::OrientationMode::Portrait)->wrap_lua(L);
				break;
			}
			case 1: {
				return OpenBlox::Enum::LuaOrientationMode->getEnumItem((int)OpenBlox::Enum::OrientationMode::LandscapeRight)->wrap_lua(L);
				break;
			}
			case 2: {
				return OpenBlox::Enum::LuaOrientationMode->getEnumItem((int)OpenBlox::Enum::OrientationMode::Degree_180)->wrap_lua(L);
				break;
			}
			case 3: {
				return OpenBlox::Enum::LuaOrientationMode->getEnumItem((int)OpenBlox::Enum::OrientationMode::LandscapeLeft)->wrap_lua(L);
				break;
			}
			default: {
				return 0;
			}
		}
	}
	return 0;
}

int Viewport::lua_setOrientationMode(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(LuaViewport){
		OpenBlox::Enum::LuaEnumItem* val = OpenBlox::Enum::checkEnumItem(L, 2, OpenBlox::Enum::LuaOrientationMode);
		if(!val){
			return 0;
		}
		OpenBlox::Enum::OrientationMode oMode = (OpenBlox::Enum::OrientationMode)val->value;
		switch(oMode){
			case OpenBlox::Enum::OrientationMode::Portrait: {
				LuaViewport->realVP->setOrientationMode(Ogre::OrientationMode::OR_PORTRAIT);
				break;
			}
			case OpenBlox::Enum::OrientationMode::LandscapeRight: {
				LuaViewport->realVP->setOrientationMode(Ogre::OrientationMode::OR_LANDSCAPERIGHT);
				break;
			}
			case OpenBlox::Enum::OrientationMode::Degree_180: {
				LuaViewport->realVP->setOrientationMode(Ogre::OrientationMode::OR_DEGREE_180);
				break;
			}
			case OpenBlox::Enum::OrientationMode::LandscapeLeft: {
				LuaViewport->realVP->setOrientationMode(Ogre::OrientationMode::OR_LANDSCAPELEFT);
				break;
			}
			default: {
				break;
			}
		}
	}
	return 0;
}

int Viewport::lua_eq(lua_State* L){
	Viewport* LuaViewport = checkViewport(L, 1);
	if(lua_isuserdata(L, 2)){
		Viewport* OtherViewport = checkViewport(L, 2);
		lua_pushboolean(L, LuaViewport->equals(OtherViewport));
	}else{
		lua_pushboolean(L, false);
	}
	return 1;
}

int Viewport::wrap_lua(lua_State* L){
	Viewport** LuaViewport = (Viewport**)lua_newuserdata(L, sizeof(*this));
	*LuaViewport = this;

	luaL_getmetatable(L, lua_viewport_name);
	lua_setmetatable(L, -2);
	return 1;
}

Viewport* checkViewport(lua_State* L, int n){
	return *(Viewport**)luaL_checkudata(L, n, lua_viewport_name);
}

END_OB_TYPE
