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
	}
}
