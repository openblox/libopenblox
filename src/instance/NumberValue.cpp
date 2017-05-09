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

#include "instance/NumberValue.h"

#include "OBEngine.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(NumberValue, true, false, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    NumberValue::NumberValue(){
			Name = ClassName;

			Value = 0;
		}

	    NumberValue::~NumberValue(){}

	    double NumberValue::getValue(){
			return Value;
		}

		void NumberValue::setValue(double value){
		    if(Value != value){
				Value = value;

				REPLICATE_PROPERTY_CHANGE(Value);
				propertyChanged("Value");
			}
		}

		shared_ptr<Instance> NumberValue::cloneImpl(){
			shared_ptr<NumberValue> nv = make_shared<NumberValue>();
			nv->Archivable = Archivable;
			nv->Name = Name;
			nv->ParentLocked = ParentLocked;

			nv->Value = Value;
			
			return nv;
		}

		void NumberValue::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
		    
			peer->sendSetPropertyPacket(netId, "Value", make_shared<Type::VarWrapper>(Value));
		}

		std::map<std::string, std::string> NumberValue::getProperties(){
			std::map<std::string, std::string> propMap = Instance::getProperties();
			propMap["Value"] = "double";

			return propMap;
		}

		void NumberValue::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Value"){
			    setValue(val->asDouble());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> NumberValue::getProperty(std::string prop){
			if(prop == "Value"){
				return make_shared<Type::VarWrapper>(getValue());
			}
			
			return Instance::getProperty(prop);
		}

		int NumberValue::lua_setValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<NumberValue> instNV = dynamic_pointer_cast<NumberValue>(inst);
				if(instNV){
				    double newV = luaL_checknumber(L, 2);
					instNV->setValue(newV);
				}
			}
			
			return 0;
		}

		int NumberValue::lua_getValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<NumberValue> instNV = dynamic_pointer_cast<NumberValue>(inst);
				if(instNV){
					lua_pushnumber(L, instNV->getValue());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

	    void NumberValue::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"Value", lua_setValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void NumberValue::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);
			
			luaL_Reg properties[] = {
				{"Value", lua_getValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
