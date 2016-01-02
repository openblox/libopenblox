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

#include "BoolValue.h"

BEGIN_INSTANCE
	DEFINE_CLASS(BoolValue, true, false, Instance);

	BoolValue::BoolValue() : Instance(){
		Name = ClassName;
		Value = false;
	}

	BoolValue::~BoolValue(){}

	Instance* BoolValue::cloneImpl(Instance* newGuy){
		if(BoolValue* bvNew = dynamic_cast<BoolValue*>(newGuy)){
			bvNew->Value = Value;
		}
		Instance::cloneImpl(newGuy);
		return newGuy;
	}

	int BoolValue::lua_getValue(lua_State* L){
		Instance* inst = Instance::checkInstance(L, 1);
		if(BoolValue* bv = dynamic_cast<BoolValue*>(inst)){
			lua_pushboolean(L, bv->Value);
			return 1;
		}
		return 0;
	}

	int BoolValue::lua_setValue(lua_State* L){
		Instance* inst = Instance::checkInstance(L, 1);
		if(BoolValue* bv = dynamic_cast<BoolValue*>(inst)){
			bool newV = lua_toboolean(L, 2);
			if(newV != bv->Value){
				bv->Value = newV;

				REPLICATE_PROPERTY_CHANGE(bv, "Value", newV);

				propertyChanged("Value", bv);
			}
		}
		return 0;
	}

	void BoolValue::register_lua_property_getters(lua_State* L){
		Instance::register_lua_property_getters(L);

		luaL_Reg props[]{
			{"Value", lua_getValue},
			{NULL, NULL}
		};
		luaL_setfuncs(L, props, 0);
	}

	void BoolValue::register_lua_property_setters(lua_State* L){
		Instance::register_lua_property_setters(L);

		luaL_Reg props[]{
			{"Value", lua_setValue},
			{NULL, NULL}
		};
		luaL_setfuncs(L, props, 0);
	}
END_INSTANCE
