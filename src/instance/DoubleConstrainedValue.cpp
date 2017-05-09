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

#include "instance/DoubleConstrainedValue.h"

#include "OBEngine.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(DoubleConstrainedValue, true, false, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    DoubleConstrainedValue::DoubleConstrainedValue(){
			Name = ClassName;

			MaxValue = 0;
			MinValue = 0;
			Value = 0;
		}

	    DoubleConstrainedValue::~DoubleConstrainedValue(){}

	    double DoubleConstrainedValue::getValue(){
			return Value;
		}

		void DoubleConstrainedValue::setValue(double value){
		    if(Value != value){
				Value = value;

				REPLICATE_PROPERTY_CHANGE(Value);
				propertyChanged("Value");
			}
		}

	    double DoubleConstrainedValue::getMaxValue(){
			return MaxValue;
		}

		void DoubleConstrainedValue::setMaxValue(double maxValue){
		    if(MaxValue != maxValue){
				MaxValue = maxValue;

				REPLICATE_PROPERTY_CHANGE(MaxValue);
				propertyChanged("MaxValue");
			}
		}

	    double DoubleConstrainedValue::getMinValue(){
			return MinValue;
		}

		void DoubleConstrainedValue::setMinValue(double minValue){
		    if(MinValue != minValue){
				MinValue = minValue;

				REPLICATE_PROPERTY_CHANGE(MinValue);
				propertyChanged("MinValue");
			}
		}

		shared_ptr<Instance> DoubleConstrainedValue::cloneImpl(){
			shared_ptr<DoubleConstrainedValue> dcv = make_shared<DoubleConstrainedValue>();
			dcv->Archivable = Archivable;
			dcv->Name = Name;
			dcv->ParentLocked = ParentLocked;

			dcv->MinValue = MinValue;
			dcv->MaxValue = MaxValue;
			dcv->Value = Value;
			
			return dcv;
		}

		void DoubleConstrainedValue::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
			
			peer->sendSetPropertyPacket(netId, "Value", make_shared<Type::VarWrapper>(Value));
			peer->sendSetPropertyPacket(netId, "MinValue", make_shared<Type::VarWrapper>(Value));
			peer->sendSetPropertyPacket(netId, "MaxValue", make_shared<Type::VarWrapper>(Value));
		}

		std::map<std::string, _PropertyInfo> DoubleConstrainedValue::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Value"] = {"double", false, true};
			propMap["MinValue"] = {"double", false, true};
			propMap["MaxValue"] = {"double", false, true};

			return propMap;
		}

		void DoubleConstrainedValue::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Value"){
				setValue(val->asDouble());
				return;
			}
			if(prop == "MinValue"){
				setMinValue(val->asDouble());
				return;
			}
			if(prop == "MaxValue"){
				setMaxValue(val->asDouble());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> DoubleConstrainedValue::getProperty(std::string prop){
			if(prop == "Value"){
				return make_shared<Type::VarWrapper>(getValue());
			}
			if(prop == "MinValue"){
				return make_shared<Type::VarWrapper>(getMinValue());
			}
			if(prop == "MaxValue"){
				return make_shared<Type::VarWrapper>(getMaxValue());
			}
			
			return Instance::getProperty(prop);
		}

		int DoubleConstrainedValue::lua_setMinValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<DoubleConstrainedValue> instDCV = dynamic_pointer_cast<DoubleConstrainedValue>(inst);
				if(instDCV){
				    double newV = luaL_checknumber(L, 2);
					instDCV->setMinValue(newV);
				}
			}
			
			return 0;
		}

		int DoubleConstrainedValue::lua_getMinValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<DoubleConstrainedValue> instDCV = dynamic_pointer_cast<DoubleConstrainedValue>(inst);
				if(instDCV){
					lua_pushinteger(L, instDCV->getMinValue());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int DoubleConstrainedValue::lua_setMaxValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<DoubleConstrainedValue> instDCV = dynamic_pointer_cast<DoubleConstrainedValue>(inst);
				if(instDCV){
				    double newV = luaL_checknumber(L, 2);
					instDCV->setMaxValue(newV);
				}
			}
			
			return 0;
		}

		int DoubleConstrainedValue::lua_getMaxValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<DoubleConstrainedValue> instDCV = dynamic_pointer_cast<DoubleConstrainedValue>(inst);
				if(instDCV){
					lua_pushnumber(L, instDCV->getMaxValue());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int DoubleConstrainedValue::lua_setValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<DoubleConstrainedValue> instDCV = dynamic_pointer_cast<DoubleConstrainedValue>(inst);
				if(instDCV){
				    double newV = luaL_checknumber(L, 2);
					instDCV->setValue(newV);
				}
			}
			
			return 0;
		}

		int DoubleConstrainedValue::lua_getValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<DoubleConstrainedValue> instDCV = dynamic_pointer_cast<DoubleConstrainedValue>(inst);
				if(instDCV){
					lua_pushinteger(L, instDCV->getValue());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

	    void DoubleConstrainedValue::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"MinValue", lua_setMinValue},
				{"MaxValue", lua_setMaxValue},
				{"Value", lua_setValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void DoubleConstrainedValue::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);
			
			luaL_Reg properties[] = {
				{"MinValue", lua_getMinValue},
				{"MaxValue", lua_getMaxValue},
				{"Value", lua_getValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
