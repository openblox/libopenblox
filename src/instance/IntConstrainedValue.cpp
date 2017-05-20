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

#include "instance/IntConstrainedValue.h"

#include "OBEngine.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(IntConstrainedValue, true, false, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    IntConstrainedValue::IntConstrainedValue(){
			Name = ClassName;

			MaxValue = 0;
			MinValue = 0;
			Value = 0;
		}

	    IntConstrainedValue::~IntConstrainedValue(){}

	    int IntConstrainedValue::getValue(){
			return Value;
		}

		void IntConstrainedValue::setValue(int value){
		    if(Value != value){
				Value = value;

				REPLICATE_PROPERTY_CHANGE(Value);
				propertyChanged("Value");
			}
		}

	    int IntConstrainedValue::getMaxValue(){
			return MaxValue;
		}

		void IntConstrainedValue::setMaxValue(int maxValue){
		    if(MaxValue != maxValue){
				MaxValue = maxValue;

				REPLICATE_PROPERTY_CHANGE(MaxValue);
				propertyChanged("MaxValue");
			}
		}

	    int IntConstrainedValue::getMinValue(){
			return MinValue;
		}

		void IntConstrainedValue::setMinValue(int minValue){
		    if(MinValue != minValue){
				MinValue = minValue;

				REPLICATE_PROPERTY_CHANGE(MinValue);
				propertyChanged("MinValue");
			}
		}

		shared_ptr<Instance> IntConstrainedValue::cloneImpl(){
			shared_ptr<IntConstrainedValue> icv = make_shared<IntConstrainedValue>();
			icv->Archivable = Archivable;
			icv->Name = Name;
			icv->ParentLocked = ParentLocked;

			icv->MinValue = MinValue;
			icv->MaxValue = MaxValue;
			icv->Value = Value;
			
			return icv;
		}

		#if HAVE_ENET
		void IntConstrainedValue::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
			
			peer->sendSetPropertyPacket(netId, "Value", make_shared<Type::VarWrapper>(Value));
			peer->sendSetPropertyPacket(netId, "MinValue", make_shared<Type::VarWrapper>(Value));
			peer->sendSetPropertyPacket(netId, "MaxValue", make_shared<Type::VarWrapper>(Value));
		}
		#endif

		std::map<std::string, _PropertyInfo> IntConstrainedValue::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Value"] = {"int", false, true, true};
			propMap["MinValue"] = {"int", false, true, true};
			propMap["MaxValue"] = {"int", false, true, true};

			return propMap;
		}

		void IntConstrainedValue::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Value"){
				setValue(val->asInt());
				return;
			}
			if(prop == "MinValue"){
				setMinValue(val->asInt());
				return;
			}
			if(prop == "MaxValue"){
				setMaxValue(val->asInt());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> IntConstrainedValue::getProperty(std::string prop){
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

		int IntConstrainedValue::lua_setMinValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<IntConstrainedValue> instICV = dynamic_pointer_cast<IntConstrainedValue>(inst);
				if(instICV){
				    int newV = luaL_checkinteger(L, 2);
					instICV->setMinValue(newV);
				}
			}
			
			return 0;
		}

		int IntConstrainedValue::lua_getMinValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<IntConstrainedValue> instICV = dynamic_pointer_cast<IntConstrainedValue>(inst);
				if(instICV){
					lua_pushinteger(L, instICV->getMinValue());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int IntConstrainedValue::lua_setMaxValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<IntConstrainedValue> instICV = dynamic_pointer_cast<IntConstrainedValue>(inst);
				if(instICV){
				    int newV = luaL_checkinteger(L, 2);
					instICV->setMaxValue(newV);
				}
			}
			
			return 0;
		}

		int IntConstrainedValue::lua_getMaxValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<IntConstrainedValue> instICV = dynamic_pointer_cast<IntConstrainedValue>(inst);
				if(instICV){
					lua_pushinteger(L, instICV->getMaxValue());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int IntConstrainedValue::lua_setValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<IntConstrainedValue> instICV = dynamic_pointer_cast<IntConstrainedValue>(inst);
				if(instICV){
				    int newV = luaL_checkinteger(L, 2);
					instICV->setValue(newV);
				}
			}
			
			return 0;
		}

		int IntConstrainedValue::lua_getValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<IntConstrainedValue> instICV = dynamic_pointer_cast<IntConstrainedValue>(inst);
				if(instICV){
					lua_pushinteger(L, instICV->getValue());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

	    void IntConstrainedValue::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"MinValue", lua_setMinValue},
				{"MaxValue", lua_setMaxValue},
				{"Value", lua_setValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void IntConstrainedValue::register_lua_property_getters(lua_State* L){
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
