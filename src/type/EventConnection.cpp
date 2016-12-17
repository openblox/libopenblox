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

#include "type/Event.h"

#include "type/EventConnection.h"

#include "instance/Instance.h"

#include <cstdlib>

namespace OB{
	namespace Type{
		DEFINE_TYPE(EventConnection){
			registerLuaType(LuaTypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}
		
		EventConnection::EventConnection(Event* evt, void* ud, void (*fnc)(std::vector<VarWrapper>, void*)){
			this->evt = evt;
			this->ud = ud;
		    this->fnc = fnc;
		}

		EventConnection::~EventConnection(){
		    if(ud){
				free(ud);
				ud = NULL;
			}
		}

		void EventConnection::Disconnect(){
			if(evt){
				evt->disconnect(this);
				evt = NULL;
			}
		}

		bool EventConnection::isConnected(){
		    if(evt){
			    return evt->isConnected(this);
			}else{
				return false;
			}
		}

		void EventConnection::fire(std::vector<VarWrapper> args){
			fnc(args, ud);
		}

		std::string EventConnection::toString(){
			return "EventConnection";
		}

		int EventConnection::lua_disconnect(lua_State* L){
			EventConnection* evtCon = checkEventConnection(L, 1);

			if(evtCon){
				evtCon->Disconnect();
			}

		    return 0;
		}

		int EventConnection::lua_isConnected(lua_State* L){
			EventConnection* evtCon = checkEventConnection(L, 1);

			if(evtCon){
			    lua_pushboolean(L, evtCon->isConnected());
			}else{
				lua_pushnil(L);
			}
			
			return 1;
		}

		void EventConnection::register_lua_methods(lua_State* L){
			luaL_Reg methods[] = {
				{"Disconnect", lua_disconnect},
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}

		void EventConnection::register_lua_property_setters(lua_State* L){
			luaL_Reg props[] = {
				{"Connected", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, props, 0);
		}

		void EventConnection::register_lua_property_getters(lua_State* L){
			luaL_Reg props[] = {
				{"Connected", lua_isConnected},
				{NULL, NULL}
			};
			luaL_setfuncs(L, props, 0);
		}

		EventConnection* checkEventConnection(lua_State* L, int index){
			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_EventConnection");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return *(EventConnection**)udata;
					}
					lua_pop(L, 1);
				}
				return NULL;
			}
			return NULL;
		}
	}
}
