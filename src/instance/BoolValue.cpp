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

#include "instance/BoolValue.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(BoolValue, true, false, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    BoolValue::BoolValue(OBEngine* eng) : Instance(eng){
			Name = ClassName;

			Value = false;
		}

	    BoolValue::~BoolValue(){}

		bool BoolValue::getValue(){
			return Value;
		}

		void BoolValue::setValue(bool value){
		    if(Value != value){
				Value = value;

				REPLICATE_PROPERTY_CHANGE(Value);
				propertyChanged("Value");
			}
		}

		shared_ptr<Instance> BoolValue::cloneImpl(){
			shared_ptr<BoolValue> bv = make_shared<BoolValue>(eng);
			bv->Archivable = Archivable;
			bv->Name = Name;
			bv->ParentLocked = ParentLocked;

			bv->Value = Value;
			
			return bv;
		}

		#if HAVE_ENET
		void BoolValue::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
			
			peer->sendSetPropertyPacket(netId, "Value", make_shared<Type::VarWrapper>(Value));
		}
		#endif

		std::map<std::string, _PropertyInfo> BoolValue::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Value"] = {"bool", false, true, true};

			return propMap;
		}

		void BoolValue::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Value"){
				setValue(val->asBool());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> BoolValue::getProperty(std::string prop){
			if(prop == "Value"){
				return make_shared<Type::VarWrapper>(getValue());
			}
			
			return Instance::getProperty(prop);
		}

		int BoolValue::lua_setValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<BoolValue> instBV = dynamic_pointer_cast<BoolValue>(inst);
				if(instBV){
					bool newV = lua_toboolean(L, 2);
					instBV->setValue(newV);
				}
			}
			
			return 0;
		}

		int BoolValue::lua_getValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<BoolValue> instBV = dynamic_pointer_cast<BoolValue>(inst);
				if(instBV){
					lua_pushboolean(L, instBV->getValue());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		void BoolValue::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"Value", lua_setValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void BoolValue::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);
			
			luaL_Reg properties[] = {
				{"Value", lua_getValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
