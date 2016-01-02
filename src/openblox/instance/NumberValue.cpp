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

#include "NumberValue.h"

BEGIN_INSTANCE
	DEFINE_CLASS(NumberValue, true, false, Instance);

	NumberValue::NumberValue() : Instance(){
		Name = ClassName;
		Value = 0;
	}

	NumberValue::~NumberValue(){}

	Instance* NumberValue::cloneImpl(Instance* newGuy){
		if(NumberValue* bvNew = dynamic_cast<NumberValue*>(newGuy)){
			bvNew->Value = Value;
		}
		Instance::cloneImpl(newGuy);
		return newGuy;
	}

	int NumberValue::lua_getValue(lua_State* L){
		Instance* inst = Instance::checkInstance(L, 1);
		if(NumberValue* nv = dynamic_cast<NumberValue*>(inst)){
			lua_pushnumber(L, nv->Value);
			return 1;
		}
		return 0;
	}

	int NumberValue::lua_setValue(lua_State* L){
		Instance* inst = Instance::checkInstance(L, 1);
		if(NumberValue* nv = dynamic_cast<NumberValue*>(inst)){
			double newV = lua_tonumber(L, 2);
			if(newV != nv->Value){
				nv->Value = newV;

				REPLICATE_PROPERTY_CHANGE(nv, "Value", newV);

				propertyChanged("Value", nv);
			}
		}
		return 0;
	}

	void NumberValue::register_lua_property_getters(lua_State* L){
		Instance::register_lua_property_getters(L);

		luaL_Reg props[]{
			{"Value", lua_getValue},
			{NULL, NULL}
		};
		luaL_setfuncs(L, props, 0);
	}

	void NumberValue::register_lua_property_setters(lua_State* L){
		Instance::register_lua_property_setters(L);

		luaL_Reg props[]{
			{"Value", lua_setValue},
			{NULL, NULL}
		};
		luaL_setfuncs(L, props, 0);
	}
END_INSTANCE
