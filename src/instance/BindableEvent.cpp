/*
 * Copyright (C) 2017 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "instance/BindableEvent.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(BindableEvent, true, false, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		BindableEvent::BindableEvent(OBEngine* eng) : Instance(eng){
			Name = ClassName;

			Event = make_shared<Type::Event>("Event");
		}

		BindableEvent::~BindableEvent(){}

		shared_ptr<Instance> BindableEvent::cloneImpl(){
			shared_ptr<BindableEvent> be = make_shared<BindableEvent>(eng);
			be->Archivable = Archivable;
			be->Name = Name;
			be->ParentLocked = ParentLocked;

			return be;
		}

		void BindableEvent::register_lua_events(lua_State* L){
			Instance::register_lua_events(L);

			luaL_Reg events[] = {
				{"Event", WRAP_EVT(BindableEvent, Event)},
				{NULL, NULL}
			};
			luaL_setfuncs(L, events, 0);
		}
	}
}
