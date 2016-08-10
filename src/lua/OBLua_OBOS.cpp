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

#include "lua/OBLua_OBOS.h"

//From here own out is heavily based on loslib.c…

#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utility.h"

#if !defined(LUA_STRFTIMEOPTIONS)	/* { */
/*
 ** list of valid conversion specifiers for the 'strftime' function
 */

#if defined(LUA_USE_C89)
#define LUA_STRFTIMEOPTIONS	{ "aAbBcdHIjmMpSUwWxXyYz%", "" }
#else  /* C99 specification */
#define LUA_STRFTIMEOPTIONS \
	{ "aAbBcCdDeFgGhHIjmMnprRStTuUVwWxXyYzZ%", "", \
	  "E", "cCxXyY",  \
	  "O", "deHImMSuUVwWy" }
#endif

#endif					/* } */

#if !defined(l_time_t)		/* { */
/*
 ** type to represent time_t in Lua
 */
#define l_timet			lua_Integer
#define l_pushtime(L,t)		lua_pushinteger(L,(lua_Integer)(t))
#define l_checktime(L,a)	((time_t)luaL_checkinteger(L,a))

#endif				/* } */

#if !defined(lua_tmpnam)	/* { */
/*
 ** By default, Lua uses tmpnam except when POSIX is available, where it
 ** uses mkstemp.
 */

#if defined(LUA_USE_POSIX)	/* { */

#include <unistd.h>

#define LUA_TMPNAMBUFSIZE	32

#if !defined(LUA_TMPNAMTEMPLATE)
#define LUA_TMPNAMTEMPLATE	"/tmp/lua_XXXXXX"
#endif

#define lua_tmpnam(b,e) { \
        strcpy(b, LUA_TMPNAMTEMPLATE); \
        e = mkstemp(b); \
        if (e != -1) close(e); \
        e = (e == -1); }

#else				/* }{ */

/* ISO C definitions */
#define LUA_TMPNAMBUFSIZE	L_tmpnam
#define lua_tmpnam(b,e)		{ e = (tmpnam(b) == NULL); }

#endif				/* } */

#endif				/* } */

#if !defined(l_gmtime)		/* { */
/*
 ** By default, Lua uses gmtime/localtime, except when POSIX is available,
 ** where it uses gmtime_r/localtime_r
 */

#if defined(LUA_USE_POSIX)	/* { */

#define l_gmtime(t,r)		gmtime_r(t,r)
#define l_localtime(t,r)	localtime_r(t,r)

#else				/* }{ */

/* ISO C definitions */
#define l_gmtime(t,r)		((void)(r)->tm_sec, gmtime(t))
#define l_localtime(t,r)  	((void)(r)->tm_sec, localtime(t))

#endif				/* } */

#endif				/* } */

namespace OB{
	namespace Lua{
		static int os_getenv(lua_State* L){
			lua_pushstring(L, getenv(luaL_checkstring(L, 1))); /* if NULL push nil */
			return 1;
		}

		static int os_clock(lua_State* L){
			lua_pushnumber(L, ((lua_Number)clock()) / (lua_Number)CLOCKS_PER_SEC);
			return 1;
		}

		/*
		** {======================================================
		** Time/Date operations
		** { year=%Y, month=%m, day=%d, hour=%H, min=%M, sec=%S,
		**   wday=%w+1, yday=%j, isdst=? }
		** =======================================================
		*/

		static void setfield(lua_State* L, const char* key, int value){
			lua_pushinteger(L, value);
			lua_setfield(L, -2, key);
		}

		static void setboolfield(lua_State* L, const char* key, int value){
			if(value < 0) /* undefined? */
				return; /* does not set field */
			lua_pushboolean(L, value);
			lua_setfield(L, -2, key);
		}

		static int getboolfield(lua_State* L, const char* key){
			int res;
			res = (lua_getfield(L, -1, key) == LUA_TNIL) ? -1 : lua_toboolean(L, -1);
			lua_pop(L, 1);
			return res;
		}

		static int getfield(lua_State* L, const char* key, int d){
			int res, isnum;
			lua_getfield(L, -1, key);
			res = (int)lua_tointegerx(L, -1, &isnum);
			if(!isnum){
				if(d < 0){
					return luaL_error(L, "field '%s' missing in date table", key);
				}
				res = d;
			}
			lua_pop(L, 1);
			return res;
		}

		static const char* checkoption(lua_State* L, const char* conv, char* buff){
			static const char * const options[] = LUA_STRFTIMEOPTIONS;
			unsigned int i;
			for(i = 0; i < sizeof(options) / sizeof(options[0]); i += 2){
				if(*conv != '\0' && strchr(options[i], *conv) != NULL){
					buff[1] = *conv;
					if(*options[i + 1] == '\0'){ /* one-char conversion specifier? */
						buff[2] = '\0'; /* end buffer */
						return conv + 1;
					}else if(*(conv + 1) != '\0'&&
							 strchr(options[i + 1], *(conv + 1)) != NULL){
						buff[2] = *(conv + 1); /* valid two-char conversion specifier */
						buff[3] = '\0'; /* end buffer */
						return conv + 2;
					}
				}
			}
			luaL_argerror(L, 1,
						  lua_pushfstring(L, "invalid conversion specifier '%%%s'", conv));
			return conv; /* to avoid warnings */
		}

		static int os_date(lua_State* L){
			const char* s = luaL_optstring(L, 1, "%c");
			time_t t = luaL_opt(L, l_checktime, 2, time(NULL));
			struct tm tmr, *stm;
			if(*s == '!'){ /* UTC? */
				stm = l_gmtime(&t, &tmr);
				s++; /* skip '!' */
			}else
				stm = l_localtime(&t, &tmr);
			if(stm == NULL) /* invalid date? */
				lua_pushnil(L);
			else if(strcmp(s, "*t") == 0){
				lua_createtable(L, 0, 9); /* 9 = number of fields */
				setfield(L, "sec", stm->tm_sec);
				setfield(L, "min", stm->tm_min);
				setfield(L, "hour", stm->tm_hour);
				setfield(L, "day", stm->tm_mday);
				setfield(L, "month", stm->tm_mon + 1);
				setfield(L, "year", stm->tm_year + 1900);
				setfield(L, "wday", stm->tm_wday + 1);
				setfield(L, "yday", stm->tm_yday + 1);
				setboolfield(L, "isdst", stm->tm_isdst);
			}else{
				char cc[4];
				luaL_Buffer b;
				cc[0] = '%';
				luaL_buffinit(L, &b);
				while(*s){
					if(*s != '%') /* no conversion specifier? */
						luaL_addchar(&b, *s++);
					else{
						size_t reslen;
						char buff[200]; /* should be big enough for any conversion result */
						s = checkoption(L, s + 1, cc);
						reslen = strftime(buff, sizeof(buff), cc, stm);
						luaL_addlstring(&b, buff, reslen);
					}
				}
				luaL_pushresult(&b);
			}
			return 1;
		}

		static int os_time(lua_State* L){
			time_t t;
			if(lua_isnoneornil(L, 1)){ /* called without args? */
				lua_pushinteger(L, (int)(currentTimeMillis() / 1000));
				return 1;
			}else{
				struct tm ts;
				luaL_checktype(L, 1, LUA_TTABLE);
				lua_settop(L, 1); /* make sure table is at the top */
				ts.tm_sec = getfield(L, "sec", 0);
				ts.tm_min = getfield(L, "min", 0);
				ts.tm_hour = getfield(L, "hour", 12);
				ts.tm_mday = getfield(L, "day", -1);
				ts.tm_mon = getfield(L, "month", -1) - 1;
				ts.tm_year = getfield(L, "year", -1) - 1900;
				ts.tm_isdst = getboolfield(L, "isdst");
				t = mktime(&ts);
			}
			if(t != (time_t)(l_timet)t){
				luaL_error(L, "time result cannot be represented in this Lua installation");
			}else if(t == (time_t)(-1)){
				lua_pushnil(L);
			}else{
				l_pushtime(L, t);
			}
			return 1;
		}

		static int os_difftime(lua_State* L){
			double res = difftime((l_checktime(L, 1)), (l_checktime(L, 2)));
			lua_pushnumber(L, (lua_Number)res);
			return 1;
		}

		static const luaL_Reg syslib[] = {
			{"clock", os_clock},
			{"date", os_date},
			{"difftime", os_difftime},
			{"getenv", os_getenv},
			{"time", os_time},
			{NULL, NULL}
		};

		int luaopen_obos(lua_State* L){
			luaL_newlib(L, syslib);
			return 1;
		}
	}
}
