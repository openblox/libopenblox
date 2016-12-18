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

#include "lua/OBLua.h"

#include "lua/OBLua_OBBase.h"
#include "lua/OBLua_OBOS.h"

#include "obtype.h"

#include <cstdlib>

#include <map>
#include <iostream>

#include "OBEngine.h"
#include "utility.h"
#include "TaskScheduler.h"
#include "instance/Instance.h"

namespace OB{
	namespace Lua{
		void* l_alloc(void* ud, void* ptr, size_t osize, size_t nsize){
			(void)ud;
			(void)osize;

			if(nsize == 0){
				free(ptr);
				return NULL;
			}else{
				return realloc(ptr, nsize);
			}
		}
		
		//Stores information about Lua states used by OpenBlox, for example the 'script' value.
		static std::map<lua_State*, struct OBLState*> lStates;
		
		lua_State* initGlobal(){
			//Don't put anything on the global state, its one purpose
			//is to be the parent of coroutines.
		    lua_State* L = lua_newstate(l_alloc, NULL);

			struct OBLState* LState = new struct OBLState;
			LState->L = L;
			LState->ref = -1;
			LState->numChildStates = 0;
			LState->parent = NULL;
			LState->initUseOver = false;
			
			return L;
		}

		lua_State* initThread(lua_State* gL){
			lua_State* L = lua_newthread(gL);

			struct OBLState* LState = new struct OBLState;
			LState->L = L;
			LState->ref = luaL_ref(gL, LUA_REGISTRYINDEX);
			LState->numChildStates = 0;
			LState->parent = NULL;
			LState->initUseOver = false;

			lStates[L] = LState;

			//Load altered standard lib
			luaL_requiref(L, "_G", luaopen_obbase, 1);//OB version of Lua's base lib
			luaL_requiref(L, LUA_COLIBNAME, luaopen_coroutine, 1);
			luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);
			luaL_requiref(L, LUA_OSLIBNAME, luaopen_obos, 1);//OB version of Lua's os lib
			luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
			luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 1);
			luaL_requiref(L, LUA_UTF8LIBNAME, luaopen_utf8, 1);

			lua_pop(L, 7);

			luaL_Reg mainlib[] = {
				{"print", lua_print},
				{"warn", lua_warn},
				{"wait", lua_wait},
					
				{NULL, NULL}
			};

			lua_pushglobaltable(L);
			luaL_setfuncs(L, mainlib, 0);
			lua_pop(L, 1);

			lua_newtable(L);
			luaL_Reg instancelib[] = {
				{"new", lua_newInstance},
				{"listClasses", lua_listInstanceClasses},
				{NULL, NULL}
			};
			luaL_setfuncs(L, instancelib, 0);
			lua_setglobal(L, "Instance");

			OBEngine* eng = OBEngine::getInstance();
		    shared_ptr<Instance::DataModel> dm = eng->getDataModel();
			int gm = dm->wrap_lua(L);
			lua_pushvalue(L, -gm);
			lua_setglobal(L, "game");

			lua_pushvalue(L, -gm);
			lua_setglobal(L, "Game");

			lua_pop(L, 1);
			
			return L;
		}

		lua_State* initCoroutine(lua_State* pL){
			//Unlike "initThread", we don't load in our altered standard library.
			//We want this coroutine to use the environment of the parent state.

			lua_State* L = lua_newthread(pL);
			
			struct OBLState* LState = new struct OBLState;
			LState->L = L;
			LState->ref = luaL_ref(pL, LUA_REGISTRYINDEX);
			LState->numChildStates = 0;
			LState->initUseOver = false;

			if(lStates.count(pL)){
				struct OBLState* oL = lStates[pL];
				if(oL){
					oL->numChildStates = oL->numChildStates + 1;
				}
				
				LState->parent = oL;
			}

			lStates[L] = LState;
			
			return L;
		}

		void close_state(lua_State* L){
			if(lStates.count(L)){
		    	struct OBLState* oL = lStates[L];
				if(oL->numChildStates > 0){
					oL->initUseOver = true;
					//We aren't gonna kill it while it has kids!
					return;
				}

				OBEngine* eng = OBEngine::getInstance();
			    lua_State* gL = eng->getGlobalLuaState();

				if(oL->ref != -1){
					luaL_unref(gL, LUA_REGISTRYINDEX, oL->ref);
					oL->ref = -1;
				}

				std::map<lua_State*, struct OBLState*>::iterator it = lStates.find(L);
				lStates.erase(it);

				if(oL->parent){
					struct OBLState* poL = oL->parent;
					poL->numChildStates = poL->numChildStates - 1;
					if(poL->initUseOver && poL->numChildStates <= 0){
						close_state(poL->L);
					}
				}

				delete oL;
				lua_close(L);
			}else{
				lua_close(L);
			}
		}
		
		std::string handle_errors(lua_State* L){
			std::string lerr = std::string(lua_tostring(L, -1));

			lua_pop(L, 1);//Pop the error off the stack like it never happened.
			
			return lerr;
		}

		//Lua functions and misc
		
		int lua_print(lua_State* L){
			std::string output = "";

			int n = lua_gettop(L);
			int i;

			lua_getglobal(L, "tostring");
			for(i=1; i <= n; i++){
				const char* s;
				lua_pushvalue(L, -1);
				lua_pushvalue(L, i);
				lua_call(L, 1, 1);

				s = lua_tostring(L, -1);
				lua_pop(L, 1);
				
				if(s == NULL){
					return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
				}

				if(i > 1){
					output = output + "\t";
				}
				output = output + std::string(s);
			}

			//TODO: Pass to LogService
		    puts(output.c_str());
			
			return 0;
		}

		int lua_warn(lua_State* L){
			std::string output = "";

			int n = lua_gettop(L);
			int i;

			lua_getglobal(L, "tostring");
			for(i=1; i <= n; i++){
				const char* s;
				lua_pushvalue(L, -1);
				lua_pushvalue(L, i);
				lua_call(L, 1, 1);

				s = lua_tostring(L, -1);
				lua_pop(L, 1);
				
				if(s == NULL){
					return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("warn"));
				}

				if(i > 1){
					output = output + "\t";
				}
				output = output + std::string(s);
			}

			//TODO: Pass to LogService
		    puts(output.c_str());
			
			return 0;
		}

		//Wakes up a Lua coroutine after a wait
		int _ob_lua_wake_wait(void* metad, ob_int64 start){
			ob_int64 curTime = currentTimeMillis();

			lua_State* L = (lua_State*)metad;
			lua_pushnumber(L, (curTime - start) / 1000.0);
			lua_pushnumber(L, (curTime - 0) / 1000.0);

			int ret = lua_resume(L, NULL, 2);

			if(ret != LUA_OK && ret != LUA_YIELD){
				std::string lerr = Lua::handle_errors(L);
				std::cerr << "A Lua error occurred:" << std::endl;
				std::cerr << lerr << std::endl;

			    close_state(L);

				return 0;
			}

			if(ret == LUA_OK){
			    close_state(L);
			}
			
			return 0;
		}

		int lua_wait(lua_State* L){
		    double waitTime = 1/60;
			if(!lua_isnoneornil(L, 1)){
				waitTime = luaL_checknumber(L, 1);
			}

			OBEngine* eng = OBEngine::getInstance();
		    shared_ptr<TaskScheduler> tasks = eng->getTaskScheduler();

			ob_int64 curTime = currentTimeMillis();
			ob_int64 at = curTime + (int)(waitTime * 1000);

			tasks->enqueue(_ob_lua_wake_wait, L, at);

			return lua_yield(L, 0);
		}

		int lua_newInstance(lua_State* L){
			std::string className = std::string(luaL_checkstring(L, 1));
			Instance::Instance* par = Instance::Instance::checkInstance(L, 2);
			
		    Instance::Instance* newGuy = ClassFactory::create(className);
			if(newGuy != NULL){
				if(par != NULL){
					try{
						newGuy->setParent(par, true);
					}catch(std::exception & ex){
						return luaL_error(L, ex.what());
					}
				}
				return newGuy->wrap_lua(L);
			}
			lua_pushnil(L);
			return 1;
		}

		int lua_listInstanceClasses(lua_State* L){
			lua_newtable(L);

			std::vector<std::string> reggedClasses = ClassFactory::getRegisteredClasses();
			for(std::vector<std::string>::size_type i = 0; i != reggedClasses.size(); i++){
				int lidx = i + 1;
				lua_pushstring(L, reggedClasses[i].c_str());
				lua_rawseti(L, -2, lidx);
			}

			return 1;
		}
	}
}
