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

#include "TaskScheduler.h"

#include "type/Event.h"

namespace OpenBlox{
	TaskScheduler* TaskScheduler::inst = NULL;

	TaskScheduler::TaskScheduler(){
		if(inst != NULL){
			throw new OBException("Only one instance of TaskScheduler can be created.");
		}
		inst = this;

		blockLogServ = false;
	}

	TaskScheduler::~TaskScheduler(){}

	/**
	 * Returns the TaskScheduler instance.
	 * @return TaskScheduler*, NULL if there is no instance yet.
	 * @author John M. Harris, Jr.
	 */
	TaskScheduler* TaskScheduler::getInstance(){
		return inst;
	}

	/**
	 * Runs the task scheduler. Should not yield for longer than a few milliseconds.
	 * @author John M. Harris, Jr.
	 * @author DigiTechs
	 */
	void TaskScheduler::tick(){
		if(!ctasks.empty()){
			while(!ctasks.empty()){
				std::function<void()> func = ctasks.back();
				func();

				ctasks.pop_back();
			}
		}
		if(!etasks.empty()){
			std::vector<EventHelper> eetasks;
			while(!etasks.empty()){
				EventHelper helper = etasks.back();
				if(helper.isWaking){
					lua_State* L = helper.L;

					int nargs = Type::Event::pushWrappersToLua(L, *helper.argList);

					int ret = lua_resume(L, NULL, nargs);
					if(ret != 0 && ret != LUA_YIELD){
						ob_lua::handle_lua_errors(L);
					}
					if(ret == 0){
						ob_lua::killState(L);
					}
				}else{
					if(helper.shouldBlockLogService){
						blockLogServ = true;
					}
					lua_State* L = helper.L;

					//If the state is already running something, just wait to fire this later.
					if(lua_status(L) == LUA_YIELD){
						eetasks.push_back(helper);
						etasks.pop_back();
						continue;
					}

					lua_rawgeti(L, LUA_REGISTRYINDEX, helper.ref);

					int nargs = Type::Event::pushWrappersToLua(L, *helper.argList);

					int ret = lua_resume(L, NULL, nargs);
					if(ret != 0 && ret != LUA_YIELD){
						ob_lua::handle_lua_errors(L);
					}
					if(ret == 0){
						ob_lua::killState(L);
					}

					if(helper.shouldBlockLogService){
						blockLogServ = false;
					}
				}

				etasks.pop_back();
			}
			if(!eetasks.empty()){
				for(std::vector<EventHelper>::size_type i = 0; i != eetasks.size(); i++){
					etasks.push_back(eetasks[i]);
				}
			}
		}
		if(!tasks.empty()){
			while(!tasks.empty()){
				qint64 curTime = QDateTime::currentMSecsSinceEpoch();

				waiting_task t = tasks.back();

				if(t.at < curTime){
					if(t.func != NULL){
						lua_State* L = t.L;
						int numResults = t.func(L);
						if(numResults > 0){
							int ret = lua_resume(L, NULL, numResults);
							if(ret != 0){
								ob_lua::handle_lua_errors(L);
							}
							if(ret == 0){
								ob_lua::killState(L);
							}

							tasks.pop_back();
						}
					}else{
						lua_State* L = t.L;

						if(t.ref != -1){
							//lua_resume(L, 0);
							lua_rawgeti(L, LUA_REGISTRYINDEX, t.ref);

							lua_pushnumber(L, (curTime - t.start) / 1000.0);
							lua_pushnumber(L, (curTime - t.start) / 1000.0);

							int ret = lua_pcall(L, 2, LUA_MULTRET, 0);
							if(ret != 0){
								ob_lua::handle_lua_errors(L);
							}
							if(ret == 0){
								ob_lua::killState(L);
							}
							luaL_unref(L, LUA_REGISTRYINDEX, t.ref);
						}else{
							lua_pushnumber(L, (curTime - t.start) / 1000.0);
							lua_pushnumber(L, (curTime - OBEngine::getInstance()->getStartTime()) / 1000.0);
							int ret = lua_resume(L, NULL, 2);
							if(ret != 0 && ret != LUA_YIELD){
								ob_lua::handle_lua_errors(L);
							}
							if(ret == 0){
								ob_lua::killState(L);
							}
						}
						tasks.pop_back();
					}
				}else{
					break;
				}
			}
		}
	}

	/**
	 * Returns whether or not LogService should be suppressed at this time.
	 * @returns bool Should suppress events
	 * @author John M. Harris, Jr.
	 */
	bool TaskScheduler::shouldSuppressLogService(){
		return blockLogServ;
	}

	/**
	 * Used to suppress LogService from outside the TaskScheduler.
	 * @param bool Should suppress events
	 * @author John M. Harris, Jr.
	 */
	void TaskScheduler::shouldSuppressLogService(bool should){
		blockLogServ = should;
	}

	/**
	 * Adds a delayed task to the task scheduler.
	 * @param lua_State* L The current lua state.
	 * @param int idx The index of the lua function to run on the new coroutine.
	 * @param qint64 millis Wait time before running this task in milliseconds.
	 * @returns int 0 (no variables returned to lua)
	 * @author John M. Harris, Jr.
	 * @author DigiTechs
	 */
	int TaskScheduler::delay(lua_State* L, int idx, qint64 millis){
		if(!lua_isfunction(L, idx) || lua_iscfunction(L, idx)){
			return luaL_argerror(L, idx, "Lua function expected");
		}
		ob_lua::LuaState* NLS = ob_lua::newthread(L);
		lua_State* NL = NLS->L;
		lua_pushvalue(L, idx);
		lua_xmove(L, NL, 1);

		waiting_task t = waiting_task();
		t.L = NL;
		qint64 curTime = QDateTime::currentMSecsSinceEpoch();
		t.at = curTime + millis;
		t.start = curTime;
		//t.ref = luaL_ref(L, LUA_REGISTRYINDEX);
		t.ref = -1;
		t.func = NULL;

		enqueue_task(t);

		return 0;
	}

	/**
	 * Adds a waiting task and yields the current lua coroutine.
	 * @param lua_State* L The current lua state.
	 * @param double secs Wait time before running this task in seconds.
	 * @author John M. Harris, Jr.
	 * @author DigiTechs
	 */
	int TaskScheduler::wait(lua_State* L, double secs){
		waiting_task t = waiting_task();
		t.L = L;
		qint64 curTime = QDateTime::currentMSecsSinceEpoch();
		t.at = curTime + (int)(secs * 1000);
		t.start = curTime;
		t.ref = -1;
		t.func = NULL;

		enqueue_task(t);

		return lua_yield(L, 0);
	}

	/**
	 * Undocumented.
	 * @param lua_State* L The current lua state.
	 * @author John M. Harris, Jr.
	 * @author DigiTechs
	 */
	int TaskScheduler::waitFor(lua_State* L, int(*func)(lua_State*)){
		waiting_task t = waiting_task();
		t.at = QDateTime::currentMSecsSinceEpoch();
		t.ref = -1;
		t.func = func;

		enqueue_task(t);

		return lua_yield(L, 0);
	}

	/**
	 * Resumes a yielded Lua coroutine.
	 * @param lua_State* L The current lua state.
	 * @param int Number of arguments to resume with
	 * @author John M. Harris, Jr.
	 */
	void TaskScheduler::wakeState(lua_State* L, std::vector<Type::VarWrapper>* argList, bool shouldBlockLogService){
		EventHelper helper;
		helper.isWaking = true;
		helper.shouldBlockLogService = shouldBlockLogService;
		helper.L = L;
		helper.ref = -1;
		helper.argList = argList;

		etasks.push_back(helper);
	}

	/**
	 * Adds a C-sided task to be called by the TaskScheduler.
	 * @param void(*func)() Function to call from the TaskScheduler.
	 * @author John M. Harris, Jr.
	 */
	void TaskScheduler::addTask(std::function<void()> func){
		ctasks.push_back(func);
	}

	/**
	 * Adds an event helper with the specified values.
	 * @param lua_State* Lua state
	 * @param int ref
	 * @param std::vector<ob_type::VarWrapper> argList
	 * @author John M. Harris, Jr.
	 */
	void TaskScheduler::addEventHelper(lua_State* L, int ref, std::vector<Type::VarWrapper>* argList, bool shouldBlockLogService){
		EventHelper helper;
		helper.isWaking = false;
		helper.shouldBlockLogService = shouldBlockLogService;
		helper.L = L;
		helper.ref = ref;
		helper.argList = argList;

		etasks.push_back(helper);
	}

	/**
	 * Adds a task to the queue.
	 * @param waiting_task task The task to enqueue.
	 * @author John M. Harris, Jr.
	 * @author DigiTechs
	 */
	void TaskScheduler::enqueue_task(waiting_task task){
		tasks.push_back(task);
		std::sort(tasks.begin(), tasks.end(), [](const waiting_task& struct1, const waiting_task& struct2)->bool{
			return struct1.at > struct2.at;
		});
	}
}
