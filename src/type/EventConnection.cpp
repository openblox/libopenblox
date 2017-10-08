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

#include "type/Event.h"

#include "type/EventConnection.h"

#include "instance/Instance.h"

#include <cstdlib>

namespace OB{
	namespace Type{
		DEFINE_TYPE(EventConnection){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}

		EventConnection::EventConnection(shared_ptr<Event> evt, std::function<void(std::vector<shared_ptr<VarWrapper>>)> fnc){
			this->evt = evt;
			this->fnc = fnc;
			ud = NULL;
		}

		EventConnection::EventConnection(shared_ptr<Event> evt, std::function<void(std::vector<shared_ptr<VarWrapper>>)> fnc, void* ud){
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
				evt->disconnect(dynamic_pointer_cast<EventConnection>(std::enable_shared_from_this<Type>::shared_from_this()));
				evt = NULL;
			}
		}

		bool EventConnection::isConnected(){
			if(evt){
				return evt->isConnected(dynamic_pointer_cast<EventConnection>(std::enable_shared_from_this<Type>::shared_from_this()));
			}else{
				return false;
			}
		}

		void EventConnection::fire(std::vector<shared_ptr<VarWrapper>> args){
			fnc(args);
		}

		std::string EventConnection::toString(){
			return "EventConnection";
		}

		int EventConnection::lua_disconnect(lua_State* L){
			shared_ptr<EventConnection> evtCon = checkEventConnection(L, 1, false);
			if(!evtCon){
				return luaL_error(L, COLONERR, "Disconnect");
			}

			evtCon->Disconnect();
			return 0;
		}

		int EventConnection::lua_isConnected(lua_State* L){
			shared_ptr<EventConnection> evtCon = checkEventConnection(L, 1, true);

			if(evtCon){
				lua_pushboolean(L, evtCon->isConnected());
				return 1;
			}

			return 0;
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

		shared_ptr<EventConnection> checkEventConnection(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}

			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_EventConnection");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<EventConnection>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "EventConnection");
			}
			return NULL;
		}
	}
}
