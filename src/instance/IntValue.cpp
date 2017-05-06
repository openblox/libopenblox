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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox.	 If not, see <https://www.gnu.org/licenses/>.
 */

#include "instance/IntValue.h"

#include "OBEngine.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(IntValue, true, false, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    IntValue::IntValue(){
			Name = ClassName;

			Value = 0;
		}

	    IntValue::~IntValue(){}

	    int IntValue::getValue(){
			return Value;
		}

		void IntValue::setValue(int value){
		    if(Value != value){
				Value = value;

				propertyChanged("Value");
			}
		}

		shared_ptr<Instance> IntValue::cloneImpl(){
			shared_ptr<IntValue> iv = make_shared<IntValue>();
			iv->Archivable = Archivable;
			iv->Name = Name;
			iv->ParentLocked = ParentLocked;

			iv->Value = Value;
			
			return iv;
		}

		int IntValue::lua_setValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<IntValue> instIV = dynamic_pointer_cast<IntValue>(inst);
				if(instIV){
				    int newV = luaL_checkinteger(L, 2);
					instIV->setValue(newV);
				}
			}
			
			return 0;
		}

		int IntValue::lua_getValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<IntValue> instIV = dynamic_pointer_cast<IntValue>(inst);
				if(instIV){
					lua_pushinteger(L, instIV->getValue());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

	    void IntValue::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"Value", lua_setValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void IntValue::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);
			
			luaL_Reg properties[] = {
				{"Value", lua_getValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
