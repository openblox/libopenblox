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

#include "ob_lua.h"

#include <OpenBlox.h>

#include <AssetLocator.h>
#include <TaskScheduler.h>

#include <Instance.h>

#include "obbaselib.h"
#include "oboslib.h"

#include <OBGame.h>

#include <DataModel.h>
//#include <ModuleScript.h>

#include <curl/curl.h>

namespace ob_lua{
	void* l_alloc(void *ud, void *ptr, size_t osize, size_t nsize){
		(void)ud;
		(void)osize;

		if(nsize == 0){
			free(ptr);
			return NULL;
		}else{
			return realloc(ptr, nsize);
		}
	}

	/**
	 * Map used to store information on Lua states used by OpenBlox.
	 */
	static QMap<lua_State*, LuaState*> lua_states;

	LuaState* getStateInfo(lua_State* L){
		if(lua_states.contains(L)){
			return lua_states[L];
		}else{
			return NULL;
		}
	}

	void killState(lua_State* L){
		if(LuaState* LS = getStateInfo(L)){
			if(OpenBlox::OBEngine* engine = OpenBlox::OBEngine::getInstance()){
				lua_State* gL = engine->getLuaState();

				luaL_unref(gL, LUA_REGISTRYINDEX, LS->ref);

				lua_states.remove(L);

				delete LS;
				LS = NULL;
			}
		}
	}

	LuaState* newthread(lua_State* gL){
		lua_State* L = lua_newthread(gL);

		LuaState* LState = new LuaState;
		LState->L = L;
		LState->ref = luaL_ref(gL, LUA_REGISTRYINDEX);

		return LState;
	}

	/**
	 * Map used to store external Lua library URLs.
	 */
	static std::map<std::string, std::string> libMap;

	/**
	 * Used to initialize the global Lua state.
	 * @returns lua_State* Initialized Lua state.
	 * @author John M. Harris, Jr.
	 */
	lua_State* init(){
		libMap = std::map<std::string, std::string>();
		libMap["RbxUtility"] = "https://raw.githubusercontent.com/RobloxLabs/internal-code/master/library-scripts/RbxUtility-60595411.lua";
		libMap["RbxGear"] = "https://raw.githubusercontent.com/RobloxLabs/internal-code/master/library-scripts/RbxGear-45374389.lua";
		libMap["RbxStamper"] = "https://raw.githubusercontent.com/RobloxLabs/internal-code/master/library-scripts/RbxStamper-73157242.lua";
		libMap["RbxGui"] = "https://raw.githubusercontent.com/RobloxLabs/internal-code/master/library-scripts/RbxGui-45284430.lua";

		return lua_newstate(l_alloc, NULL);
	}

	/**
	 * Used to initialize a new Lua state, this is a coroutine of the global state.
	 * @returns LuaState* Initialized Lua state.
	 * @author John M. Harris, Jr.
	 */
	LuaState* newState(){
		OpenBlox::OBEngine* engine = OpenBlox::OBEngine::getInstance();
		if(!engine){
			return NULL;
		}

		OpenBlox::OBGame* game = engine->getGame();
		if(!game){
			return NULL;
		}

		lua_State* gL = engine->getLuaState();

		lua_State* L = lua_newthread(gL);

		LuaState* LState = new LuaState;
		LState->L = L;
		LState->ref = luaL_ref(gL, LUA_REGISTRYINDEX);

		lua_states[L] = LState;

		luaL_requiref(L, "_G", luaopen_obbase, 1);//OB version of Lua's base lib
		luaL_requiref(L, LUA_COLIBNAME, luaopen_coroutine, 1);
		luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);
		luaL_requiref(L, LUA_OSLIBNAME, luaopen_obos, 1);//OB version of Lua's os lib
		luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
		luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 1);
		luaL_requiref(L, LUA_UTF8LIBNAME, luaopen_utf8, 1);

		lua_pop(L, 7);

		luaL_Reg mainlib[]{
			//{"require", lua_require},

			//{"alert", lua_alert},
			{"print", lua_print},
			{"warn", lua_warn},

			{"wait", lua_wait},
			{"Wait", lua_wait},
			{"delay", lua_delay},
			{"Delay", lua_delay},
			{"spawn", lua_spawn},
			{"Spawn", lua_spawn},

			{"ElapsedTime", lua_elapsedTime},
			{"elapsedTime", lua_elapsedTime},
			{"tick", lua_tick},

			{"LoadLibrary", lua_LoadLibrary},

			{NULL, NULL}
		};

		lua_pushglobaltable(L);
		luaL_setfuncs(L, mainlib, 0);
		lua_pop(L, 1);

		lua_newtable(L);
		luaL_Reg instancelib[]{
			{"new", lua_newInstance},
			{NULL, NULL}
		};
		luaL_setfuncs(L, instancelib, 0);
		lua_setglobal(L, "Instance");

		OpenBlox::Instance::DataModel* dm = game->getDataModel();
		int gm = dm->wrap_lua(L);
		lua_pushvalue(L, -gm);
		lua_setglobal(L, "game");

		lua_pushvalue(L, -gm);
		lua_setglobal(L, "Game");

		lua_pop(L, 1);

		return LState;
	}

	/*
	int lua_require(lua_State *L){
		if(lua_isstring(L, 1)){
			//Load module from Asset URI
		}else{
			OpenBlox::Instance::Instance* moduleScript = OpenBlox::Instance::Instance::checkInstance(L, 1);
			if(moduleScript){
				if(ob_instance::ModuleScript* ms = dynamic_cast<ob_instance::ModuleScript*>(moduleScript)){
					if(ms->ref != -1){
						lua_rawgeti(L, LUA_REGISTRYINDEX, ms->ref);//+1
						return 1;
					}

					OpenBlox::OBGame* game = OpenBlox::OBGame::getInstance();
					if(game){
						LuaState* LS_mod = game->newLuaState();
						lua_State* L_mod = LS_mod->L;

						int scr = ms->wrap_lua(L_mod);//+1

						lua_pushvalue(L_mod, -scr);//+1
						lua_setglobal(L_mod, "Script");//-1

						lua_pushvalue(L_mod, -scr);//+1
						lua_setglobal(L_mod, "script");//-1

						lua_pop(L_mod, 1);//-1

						QString src = ms->Source;

						int s = luaL_loadbuffer(L_mod, src.toStdString().c_str(), src.size(), QString("@" + ms->GetFullName()).toStdString().c_str());
						if(s == 0){
							s = lua_pcall(L_mod, 0, 1, 0);
							if(s == 0){
								ms->ref = luaL_ref(L_mod, LUA_REGISTRYINDEX);

								ob_lua::killState(L_mod);

								lua_rawgeti(L, LUA_REGISTRYINDEX, ms->ref);
								return 1;
							}else{
								ob_lua::killState(L_mod);
								return luaL_error(L, "Error loading module. (%s)", lua_tostring(L_mod, -1));
							}
						}else{
							ob_lua::killState(L_mod);
							return luaL_error(L, "Error loading module. (%s)", lua_tostring(L_mod, -1));
						}

						ob_lua::killState(L_mod);
						return luaL_error(L, "Error loading module");
					}else{
						return luaL_error(L, "Error loading module.");
					}
				}
			}
			return luaL_error(L, "bad argument #1 to 'require' (ModuleScript expected, got %s)", lua_typename(L, 1));
		}
		return luaL_error(L, "Error loading module");
	}
	*/

	/**
	 * Redefined print function, based on Lua's baselib print function.
	 * @params lua_State* L The Lua state.
	 * @returns int The number of returned variables.
	 */
	int lua_print(lua_State* L){
		QString output = "";

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
			output = output + s;
		}

		OpenBlox::OBEngine* inst = OpenBlox::OBEngine::getInstance();
		if(inst != NULL){
			inst->print(output);
		}

		return 0;
	}

	/**
	 * Basic warn function, based on Lua_print.
	 * @params lua_State* L The Lua state.
	 * @returns int The number of returned variables.
	 */
	int lua_warn(lua_State* L){
		QString output = "";

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
			output = output + s;
		}

		OpenBlox::OBEngine* inst = OpenBlox::OBEngine::getInstance();
		if(inst != NULL){
			inst->warn(output);
		}

		return 0;
	}

	/**
	 * Used to yield the current coroutine for a certain amount of time.
	 * @params lua_State* L The Lua state.
	 * @returns int The number of returned variables.
	 */
	int lua_wait(lua_State* L){
		double waitTime = 1/60;
		if(!lua_isnoneornil(L, 1)){
			waitTime = luaL_checknumber(L, 1);
		}
		return OpenBlox::TaskScheduler::getInstance()->wait(L, waitTime);
	}

	/**
	 * Used to run a Lua function in another coroutine after a certain amount of time.
	 * @params lua_State* L The Lua state.
	 * @returns int The number of returned variables.
	 */
	int lua_delay(lua_State* L){
		if(lua_isnumber(L, 2)){
			double secs = lua_tonumber(L, 2);
			return OpenBlox::TaskScheduler::getInstance()->delay(L, 1, (long)(secs*1000));
		}else{
			double secs = luaL_checknumber(L, 1);
			return OpenBlox::TaskScheduler::getInstance()->delay(L, 2, (long)(secs*1000));
		}
	}

	/**
	 * Used to run a Lua function in another coroutine as soon as possible.
	 * @params lua_State* L The Lua state.
	 * @returns int The number of returned variables.
	 */
	int lua_spawn(lua_State* L){
		return OpenBlox::TaskScheduler::getInstance()->delay(L, 1, 0);
	}

	/**
	 * Returns the elapsed time of the program.
	 * @params lua_State* L The Lua state.
	 * @returns int The number of returned variables.
	 */
	int lua_elapsedTime(lua_State* L){
		double elapsedSecs = (QDateTime::currentMSecsSinceEpoch() - OpenBlox::OBEngine::getInstance()->getStartTime()) / 1000.0;
		lua_pushnumber(L, elapsedSecs);
		return 1;
	}

	/**
	 * Returns the current UNIX time in seconds.
	 * @params lua_State* L The Lua state.
	 * @returns int The number of returned variables.
	 */
	int lua_tick(lua_State* L){
		double unixSecs = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
		lua_pushnumber(L, unixSecs);
		return 1;
	}

	/**
	 * Used to load an external Lua library by name.
	 * @params lua_State* L The Lua state.
	 * @returns int The number of returned variables.
	 */
	int lua_LoadLibrary(lua_State* L){
		std::string libName = std::string(luaL_checkstring(L, 1));
		std::map<std::string, std::string>::iterator it = libMap.find(libName);
		if(it != libMap.end()){
			struct OpenBlox::asset_response body;
			body.size = 0;
			body.data = new char[4096];
			if(body.data == NULL){
				LOGE("[LoadLibrary] Failed to allocate memory.");
				return luaL_error(L, "Error loading library %s", libName.c_str());
			}

			body.data[0] = '\0';

			CURL* curl;
			CURLcode res;

			curl = curl_easy_init();
			if(curl){
				curl_easy_setopt(curl, CURLOPT_URL, it->second.c_str());

				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);

				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OpenBlox::AssetLocator::write_data);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);

				res = curl_easy_perform(curl);
				if(res != CURLE_OK){
					LOGE("[LoadLibrary] cURL Error: %s", curl_easy_strerror(res));
					return luaL_error(L, "Error loading library %s", libName.c_str());
				}

				curl_easy_cleanup(curl);

				int s = luaL_loadbuffer(L, body.data, strlen(body.data), libName.c_str());
				if(s == 0){
					s = lua_pcall(L, 0, 1, 0);
					if(s == 0){
						return 1;
					}else{
						//TODO: Handle errors, maybe?
					}
				}else{
					return luaL_error(L, "Error loading library %s", libName.c_str());
				}
			}
		}
		return luaL_error(L, "Unknown library %s", libName.c_str());
	}

	int lua_newInstance(lua_State* L){
		QString className = QString(luaL_checkstring(L, 1));
		OpenBlox::Instance::Instance* par = OpenBlox::Instance::Instance::checkInstance(L, 2);
		if(OpenBlox::ClassFactory::getInstance() != NULL){
			OpenBlox::Instance::Instance* newGuy = OpenBlox::ClassFactory::getInstance()->create(className);
			if(newGuy != NULL){
				if(par != NULL){
					try{
						newGuy->setParent(par, true);
					}catch(std::exception & ex){
						return luaL_error(L, ex.what());
					}
				}
				return newGuy->wrap_lua(L);
			}else{
				//LOGI("newGuy is NULL");
			}
		}else{
			LOGI("ClassFactory is NULL!");
		}
		lua_pushnil(L);
		return 1;
	}

	void handle_lua_errors(lua_State* L){
		QString output(lua_tostring(L, -1));

		OpenBlox::OBEngine::getInstance()->print_error(output);

		lua_pop(L, 1);
	}
}
