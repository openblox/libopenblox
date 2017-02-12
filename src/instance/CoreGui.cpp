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

#include "instance/CoreGui.h"

#include "OBEngine.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#endif

namespace OB{
	namespace Instance{
		DEFINE_CLASS(CoreGui, false, isDataModel, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    CoreGui::CoreGui(){
			Name = ClassName;
			
			Enabled = true;
		}

	    CoreGui::~CoreGui(){}

		shared_ptr<Instance> CoreGui::cloneImpl(){
			return NULL;
		}

		bool CoreGui::IsEnabled(){
			return Enabled;
		}
		
		void CoreGui::SetEnabled(bool enabled){
			if(Enabled != enabled){
				Enabled = enabled;

				//TODO: Update?
			}
		}
		
		int CoreGui::lua_getEnabled(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<CoreGui> instCG = dynamic_pointer_cast<CoreGui>(inst);
				if(instCG){
					lua_pushboolean(L, instCG->IsEnabled());
				}
			}
			lua_pushnil(L);
			return 1;
		}

		int CoreGui::lua_setEnabled(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<CoreGui> instCG = dynamic_pointer_cast<CoreGui>(inst);
				if(instCG){
					bool newV = lua_toboolean(L, 2);
					instCG->SetEnabled(newV);
				}
			}
			return 0;
		}

		void CoreGui::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"Enabled", lua_setEnabled},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void CoreGui::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);
			
			luaL_Reg properties[] = {
				{"Enabled", lua_getEnabled},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
