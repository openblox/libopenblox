/*
 * Copyright (C) 2016-2017 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "type/Event.h"

#include "type/EventConnection.h"

#include "OBEngine.h"
#include "TaskScheduler.h"
#include "instance/LogService.h"

#include <algorithm>
#include <iostream>

namespace OB{
	namespace Type{
		DEFINE_TYPE(Event){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}

		Event::Event(std::string name, bool canFireFromLua, bool blockLogService){
			this->name = name;
			this->canFireFromLua = canFireFromLua;
			this->blockLogService = blockLogService;
		}

		Event::~Event(){}

		shared_ptr<EventConnection> Event::Connect(void (*fnc)(std::vector<shared_ptr<VarWrapper>>, void*), void* ud){
			shared_ptr<EventConnection> evtCon = make_shared<EventConnection>(dynamic_pointer_cast<Event>(std::enable_shared_from_this<Type>::shared_from_this()), ud, fnc);
			conns.push_back(evtCon);

			return evtCon;
		}

		void Event::disconnectAll(){
			conns.clear();
		}

		void Event::disconnect(shared_ptr<EventConnection> conn){
		    if(conns.empty()){
				return;
			}

			auto res = std::find(conns.begin(), conns.end(), conn);

			if(res != conns.end()){
				conns.erase(res);
			}
		}

		bool Event::isConnected(shared_ptr<EventConnection> conn){
			return !conns.empty() && std::find(conns.begin(), conns.end(), conn) != conns.end();
		}

		bool Event::doesBlockLogService(){
			return blockLogService;
		}

		struct evt_vconn_t{
			shared_ptr<EventConnection> evtCon;
			std::vector<shared_ptr<VarWrapper>> args;
		};
		
		int evt_do_fire_connection(void* vconn, ob_int64 startTime){
			struct evt_vconn_t* conn = (struct evt_vconn_t*)vconn;
			
			conn->evtCon->fire(conn->args);

			delete conn;

			return 0;
		}
		
		void Event::Fire(OBEngine* eng, std::vector<shared_ptr<VarWrapper>> argList){
			if(!conns.empty()){
				shared_ptr<TaskScheduler> tasks = eng->getTaskScheduler();
				
			    for(std::vector<shared_ptr<EventConnection>>::size_type i = 0; i != conns.size(); i++){					
				    /* We push these tasks onto the TaskScheduler so that
					   they can call Disconnect without messing up this loop. */
					struct evt_vconn_t* conn = new struct evt_vconn_t;
					conn->evtCon = conns[i];
					conn->args = argList;
					
					tasks->enqueue(evt_do_fire_connection, conn, 0);
				}
			}
		}
		
		void Event::Fire(OBEngine* eng){
			Fire(eng, std::vector<shared_ptr<VarWrapper>>());
		}

	    std::string Event::toString(){
			return "Event: " + name;
		}

		int Event::lua_fire(lua_State* L){
			shared_ptr<Event> evt = checkEvent(L, 1, false);
			if(!evt){
				return luaL_error(L, COLONERR, "Fire");
			}

			std::vector<shared_ptr<VarWrapper>> fireArgs;
				
			int nargs = lua_gettop(L);
			if(nargs > 1){
				for(int i = 2; i <= nargs; i++){
					int ltype = lua_type(L, i);

					switch(ltype){
						case LUA_TNIL: {
							fireArgs.push_back(make_shared<VarWrapper>((void*)NULL, TYPE_NULL));
							break;
						}
						case LUA_TNUMBER: {
							fireArgs.push_back(make_shared<VarWrapper>(lua_tonumber(L, i)));
							break;
						}
						case LUA_TBOOLEAN: {
							fireArgs.push_back(make_shared<VarWrapper>(lua_toboolean(L, i)));
							break;
						}
						case LUA_TSTRING: {
							const char* tmpStr = lua_tostring(L, i);
							fireArgs.push_back(make_shared<VarWrapper>(std::string(tmpStr)));
							break;
						}
						case LUA_TUSERDATA: {
							shared_ptr<Type> tl = checkType(L, i);
							if(tl){
								fireArgs.push_back(make_shared<VarWrapper>(tl));
								break;
							}

							shared_ptr<Instance::Instance> ti = Instance::Instance::checkInstance(L, i);
							if(ti){
								fireArgs.push_back(make_shared<VarWrapper>(ti));
								break;
							}
						}
						case LUA_TTABLE:
						case LUA_TFUNCTION:
						case LUA_TTHREAD:
						case LUA_TLIGHTUSERDATA: {
							lua_pushvalue(L, i);
							int ref = luaL_ref(L, LUA_REGISTRYINDEX);
							fireArgs.push_back(make_shared<VarWrapper>(L, ref));
							break;
						}
					}
				}
			}
			    
			evt->Fire(Lua::getEngine(L), fireArgs);
			return 0;
		}

		struct evt_lua_connection_ud_t{
		    int fncRef;
			lua_State* thread;
			bool blockedLogService;
		};
		
		void evt_lua_connection_fnc(std::vector<shared_ptr<VarWrapper>> args, void* ud){
		    struct evt_lua_connection_ud_t* eud = (struct evt_lua_connection_ud_t*)ud;

			lua_State* L = Lua::initCoroutine(eud->thread);
			
			lua_rawgeti(L, LUA_REGISTRYINDEX, eud->fncRef);

			for(std::vector<shared_ptr<VarWrapper>>::size_type i = 0; i != args.size(); i++){
				args[i]->wrap_lua(L);
			}

			shared_ptr<Instance::LogService> ls;

			if(eud->blockedLogService){
				OBEngine* eng = Lua::getEngine(L);
				if(eng){
					shared_ptr<Instance::DataModel> dm = eng->getDataModel();
					if(dm){
						ls = dm->getLogService();
						if(ls){
							ls->block();
						}
					}
				}
			}
			
			int ret = lua_resume(L, NULL, args.size());

			if(ls){
				ls->unblock();
			}
			
			if(ret != LUA_OK && ret != LUA_YIELD){
				std::string lerr = Lua::handle_errors(L);
				std::cerr << lerr << std::endl;

				Lua::close_state(L);
			}

			if(ret == LUA_OK){
				Lua::close_state(L);
			}
		}
		
		int Event::lua_connect(lua_State* L){
			shared_ptr<Event> evt = checkEvent(L, 1, false);
			if(!evt){
				return luaL_error(L, COLONERR, "Connect");
			}

			luaL_checktype(L, 2, LUA_TFUNCTION);
			lua_pushvalue(L, 2);

			//Pop the function into the Lua registry so it won't be GC'd
			int r = luaL_ref(L, LUA_REGISTRYINDEX);

			struct evt_lua_connection_ud_t* eud = new struct evt_lua_connection_ud_t;
			eud->fncRef = r;
			eud->blockedLogService = evt->doesBlockLogService();

			lua_State* newL = Lua::initCoroutine(L);
			eud->thread = newL;

			shared_ptr<EventConnection> evtCon = evt->Connect(evt_lua_connection_fnc, eud);
			return evtCon->wrap_lua(L);
		}
		
		int Event::lua_wait(lua_State* L){
			shared_ptr<Event> evt = checkEvent(L, 1, false);
			if(!evt){
				return luaL_error(L, COLONERR, "Wait");
			}
			//TODO:
			return 0;
		}

		void Event::register_lua_methods(lua_State* L){
			luaL_Reg methods[] = {
				{"Fire", lua_fire},
				{"Connect", lua_connect},
				{"Wait", lua_wait},
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}

	    shared_ptr<Event> checkEvent(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}
			
			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_Event");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<Event>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}
			
			if(errIfNot){
				luaO_typeerror(L, index, "Event");
			}
			return NULL;
		}
	}
}
