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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
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
#include "instance/LogService.h"

#include "type/Color3.h"
#include "type/Vector3.h"
#include "type/Vector2.h"
#include "type/CFrame.h"
#include "type/UDim.h"
#include "type/UDim2.h"

#include "type/Enum.h"

namespace OB{
	namespace Lua{
		OBLState* globalOBLState = NULL;

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

		// Stores information about Lua states used by OpenBlox, for example the 'script' value.
		static std::map<lua_State*, struct OBLState*> lStates;

		lua_State* initGlobal(OBEngine* eng){
			// Don't put anything on the global state, its one purpose
			// is to be the parent of coroutines.
			lua_State* L = lua_newstate(l_alloc, NULL);

			struct OBLState* LState = new struct OBLState;
			LState->L = L;
			LState->ref = -1;
			LState->numChildStates = 0;
			LState->parent = NULL;
			LState->initUseOver = false;
			LState->eng = eng;
			LState->getsPaused = false;
			LState->dmBound = false;

			lStates[L] = LState;

			return L;
		}

		OBEngine* getEngine(lua_State* L){
			struct OBLState* LState = lStates[L];
			if(LState){
				return LState->eng;
			}
			return NULL;
		}

		lua_State* initThread(lua_State* gL){
			lua_State* L = lua_newthread(gL);

			struct OBLState* LState = new struct OBLState;
			LState->L = L;
			LState->ref = luaL_ref(gL, LUA_REGISTRYINDEX);
			LState->numChildStates = 0;
			LState->parent = NULL;
			LState->initUseOver = false;
			LState->eng = getEngine(gL);
			LState->getsPaused = true;
			LState->dmBound = true;

			lStates[L] = LState;

			// Load altered standard lib
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
				{"Wait", lua_wait},
				{"delay", lua_delay},
				{"Delay", lua_delay},
				{"spawn", lua_spawn},
				{"Spawn", lua_spawn},
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

			lua_newtable(L);
			luaL_Reg color3lib[] = {
				{"new", lua_newColor3},
				{"fromRGB", lua_Color3FromRGB},
				{NULL, NULL}
			};
			luaL_setfuncs(L, color3lib, 0);
			lua_setglobal(L, "Color3");

			lua_newtable(L);
			luaL_Reg vector3lib[] = {
				{"new", lua_newVector3},
				{NULL, NULL}
			};
			luaL_setfuncs(L, vector3lib, 0);
			lua_setglobal(L, "Vector3");

			lua_newtable(L);
			luaL_Reg vector2lib[] = {
				{"new", lua_newVector2},
				{NULL, NULL}
			};
			luaL_setfuncs(L, vector2lib, 0);
			lua_setglobal(L, "Vector2");

			lua_newtable(L);
			luaL_Reg cframelib[] = {
				{"new", lua_newCFrame},
				{NULL, NULL}
			};
			luaL_setfuncs(L, cframelib, 0);
			lua_setglobal(L, "CFrame");

			lua_newtable(L);
			luaL_Reg udimlib[] = {
				{"new", lua_newUDim},
				{NULL, NULL}
			};
			luaL_setfuncs(L, udimlib, 0);
			lua_setglobal(L, "UDim");

			lua_newtable(L);
			luaL_Reg udim2lib[] = {
				{"new", lua_newUDim2},
				{NULL, NULL}
			};
			luaL_setfuncs(L, udim2lib, 0);
			lua_setglobal(L, "UDim2");

			Enum::registerLuaEnums(L);

			OBEngine* eng = getEngine(L);
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
			// Unlike "initThread", we don't load in our altered standard library.
			// We want this coroutine to use the environment of the parent state.

			lua_State* L = lua_newthread(pL);

			struct OBLState* LState = new struct OBLState;
			LState->L = L;
			LState->ref = luaL_ref(pL, LUA_REGISTRYINDEX);
			LState->numChildStates = 0;
			LState->initUseOver = false;
			LState->eng = getEngine(pL);
			LState->getsPaused = true;
			LState->dmBound = true;

			if(lStates.count(pL)){
				struct OBLState* oL = lStates[pL];
				if(oL){
					oL->numChildStates = oL->numChildStates + 1;
				}

				LState->parent = oL;
				LState->getsPaused = oL->getsPaused;
				LState->dmBound = oL->dmBound;
			}

			lStates[L] = LState;

			return L;
		}

		void close_state(lua_State* L){
			if(lStates.count(L)){
				struct OBLState* oL = lStates[L];
				if(oL->numChildStates > 0){
					oL->initUseOver = true;
					// We aren't gonna kill it while it has kids!
					return;
				}

				OBEngine* eng = getEngine(L);
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
				//lua_close(L);
			}/*else{
			   lua_close(L);
			   }*/
		}

		bool getsPaused(lua_State* L){
			if(lStates.count(L)){
				struct OBLState* LState = lStates[L];
				return LState->getsPaused;
			}
			return false;
		}

		void setGetsPaused(lua_State* L, bool getsPaused){
			if(lStates.count(L)){
				struct OBLState* LState = lStates[L];
				LState->getsPaused = getsPaused;
			}
		}

		bool isDMBound(lua_State* L){
			if(lStates.count(L)){
				struct OBLState* LState = lStates[L];
				return LState->dmBound;
			}
			return false;
		}

		void setDMBound(lua_State* L, bool dmBound){
			if(lStates.count(L)){
				struct OBLState* LState = lStates[L];
				LState->dmBound = dmBound;
			}
		}

		std::string handle_errors(lua_State* L){
			std::string lerr = std::string(lua_tostring(L, -1));

			OBEngine* eng = getEngine(L);
			shared_ptr<OBLogger> logger = eng->getLogger();
			logger->log(lerr, OLL_Error);

			lua_pop(L, 1);// Pop the error off the stack like it never happened.

			return lerr;
		}

		// Lua functions and misc

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

			OBEngine* eng = getEngine(L);
			shared_ptr<OBLogger> logger = eng->getLogger();
			logger->log(output, OLL_None);

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

			OBEngine* eng = getEngine(L);
			shared_ptr<OBLogger> logger = eng->getLogger();
			logger->log(output, OLL_Warning);

			return 0;
		}

		// Wakes up a Lua coroutine after a wait
		int _ob_lua_wake_wait(void* metad, ob_uint64 start){
			ob_uint64 curTime = currentTimeMillis();

			lua_State* L = (lua_State*)metad;
			lua_pushnumber(L, (curTime - start) / 1000.0);
			lua_pushnumber(L, curTime / 1000.0);

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

			struct OBLState* LState = lStates[L];
			OBEngine* eng = LState->eng;
			shared_ptr<TaskScheduler> tasks = eng->getTaskScheduler();

			ob_uint64 curTime = currentTimeMillis();
			ob_uint64 at = curTime + (ob_uint64)(waitTime * 1000);

			tasks->enqueue(_ob_lua_wake_wait, L, at, LState->getsPaused, LState->dmBound);

			return lua_yield(L, 0);
		}

		// Wakes up a Lua coroutine after a delay
		int _ob_lua_wake_delay(void* metad, ob_uint64 start){
			lua_State* L = (lua_State*)metad;
			int ret = lua_resume(L, NULL, 0);

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

		int _ob_lua_processDelay(lua_State* L, double secs, int idx){
			if(!lua_isfunction(L, idx) && !lua_iscfunction(L, idx)){
				return luaL_argerror(L, idx, "Lua function expected");
			}

			lua_State* cL = initCoroutine(L);

			lua_pushvalue(L, idx);
			lua_xmove(L, cL, 1);

			struct OBLState* LState = lStates[L];
			OBEngine* eng = LState->eng;
			shared_ptr<TaskScheduler> tasks = eng->getTaskScheduler();

			ob_uint64 curTime = currentTimeMillis();
			ob_uint64 at = curTime + (ob_uint64)(secs * 1000);

			tasks->enqueue(_ob_lua_wake_delay, cL, at, LState->getsPaused, LState->dmBound);

			return 0;
		}

		int lua_delay(lua_State* L){
			if(lua_isnumber(L, 2)){
				double secs = lua_tonumber(L, 2);
				return _ob_lua_processDelay(L, secs, 1);
			}else{
				double secs = luaL_checknumber(L, 1);
				return _ob_lua_processDelay(L, secs, 2);
			}
		}

		int lua_spawn(lua_State* L){
			return _ob_lua_processDelay(L, 0, 1);
		}

		int lua_newInstance(lua_State* L){
			std::string className = std::string(luaL_checkstring(L, 1));
			shared_ptr<Instance::Instance> par = Instance::Instance::checkInstance(L, 2);

			shared_ptr<Instance::Instance> newGuy = ClassFactory::create(className, getEngine(L));
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

		int lua_newColor3(lua_State* L){
			double r = 0;
			double g = 0;
			double b = 0;

			if(!lua_isnone(L, 1) && !lua_isnone(L, 2) && !lua_isnone(L, 3)){
				r = luaL_checknumber(L, 1);
				g = luaL_checknumber(L, 2);
				b = luaL_checknumber(L, 3);
			}

			shared_ptr<Type::Color3> newGuy = make_shared<Type::Color3>(r, g, b);
			return newGuy->wrap_lua(L);
		}

		int lua_Color3FromRGB(lua_State* L) {
			double r = 0;
			double g = 0;
			double b = 0;

			if (!lua_isnone(L, 1) && !lua_isnone(L, 2) && !lua_isnone(L, 3)) {
				r = luaL_checknumber(L, 1);
				g = luaL_checknumber(L, 2);
				b = luaL_checknumber(L, 3);
			}

			shared_ptr<Type::Color3> newGuy = make_shared<Type::Color3>(r / 255, g / 255, b / 255);
			return newGuy->wrap_lua(L);
		}

		int lua_newVector3(lua_State* L){
			double x = 0;
			double y = 0;
			double z = 0;

			if(!lua_isnone(L, 1) && !lua_isnone(L, 2) && !lua_isnone(L, 3)){
				x = luaL_checknumber(L, 1);
				y = luaL_checknumber(L, 2);
				z = luaL_checknumber(L, 3);
			}

			shared_ptr<Type::Vector3> newGuy = make_shared<Type::Vector3>(x, y, z);
			return newGuy->wrap_lua(L);
		}

		int lua_newVector2(lua_State* L){
			double x = 0;
			double y = 0;

			if(!lua_isnone(L, 1) && !lua_isnone(L, 2)){
				x = luaL_checknumber(L, 1);
				y = luaL_checknumber(L, 2);
			}

			shared_ptr<Type::Vector2> newGuy = make_shared<Type::Vector2>(x, y);
			return newGuy->wrap_lua(L);
		}

		int lua_newCFrame(lua_State* L){
			int nargs = lua_gettop(L);
			if(nargs == 0){
				return make_shared<Type::CFrame>()->wrap_lua(L);
			}else if(nargs == 2){
				shared_ptr<Type::Vector3> pos = Type::checkVector3(L, 1, true, false);
				shared_ptr<Type::Vector3> lA = Type::checkVector3(L, 1, true, false);
				return make_shared<Type::CFrame>(pos, lA)->wrap_lua(L);
			}else if(nargs == 3){
				double x = luaL_checknumber(L, 1);
				double y = luaL_checknumber(L, 2);
				double z = luaL_checknumber(L, 3);
				return make_shared<Type::CFrame>(x, y, z)->wrap_lua(L);
			}else if(nargs == 7){
				double x = luaL_checknumber(L, 1);
				double y = luaL_checknumber(L, 2);
				double z = luaL_checknumber(L, 3);
				double qx = luaL_checknumber(L, 4);
				double qy = luaL_checknumber(L, 5);
				double qz = luaL_checknumber(L, 6);
				double qw = luaL_checknumber(L, 7);
				return make_shared<Type::CFrame>(x, y, z, qx, qy, qz, qw)->wrap_lua(L);
			}else if(nargs == 12){
				double x = luaL_checknumber(L, 1);
				double y = luaL_checknumber(L, 2);
				double z = luaL_checknumber(L, 3);
				double r00 = luaL_checknumber(L, 4);
				double r01 = luaL_checknumber(L, 5);
				double r02 = luaL_checknumber(L, 6);
				double r10 = luaL_checknumber(L, 7);
				double r11 = luaL_checknumber(L, 8);
				double r12 = luaL_checknumber(L, 9);
				double r20 = luaL_checknumber(L, 10);
				double r21 = luaL_checknumber(L, 11);
				double r22 = luaL_checknumber(L, 12);
				return make_shared<Type::CFrame>(x, y, z,
								 r00, r01, r02,
								 r10, r11, r12,
								 r20, r21, r22)->wrap_lua(L);
			}
			return 0;
		}

		int lua_newUDim(lua_State* L){
			double scale = 0;
			double offset = 0;

			if(!lua_isnone(L, 1) && !lua_isnone(L, 2)){
				scale = luaL_checknumber(L, 1);
				offset = luaL_checknumber(L, 2);
			}

			shared_ptr<Type::UDim> newGuy = make_shared<Type::UDim>(scale, offset);
			return newGuy->wrap_lua(L);
		}

		int lua_newUDim2(lua_State* L){
			double xScale = 0;
			double xOffset = 0;
			double yScale = 0;
			double yOffset = 0;

			if(!lua_isnone(L, 1) && !lua_isnone(L, 2)){
				xScale = luaL_checknumber(L, 1);
				xOffset = luaL_checknumber(L, 2);
				yScale = luaL_checknumber(L, 3);
				yOffset = luaL_checknumber(L, 4);
			}

			shared_ptr<Type::UDim2> newGuy = make_shared<Type::UDim2>(xScale, xOffset, yScale, yOffset);
			return newGuy->wrap_lua(L);
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
