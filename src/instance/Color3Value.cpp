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

#include "instance/Color3Value.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Color3Value, true, false, BaseValue){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Color3Value::Color3Value(OBEngine* eng) : BaseValue(eng){
			Name = ClassName;

			Value = make_shared<Type::Color3>(0, 0, 0);
		}

		Color3Value::~Color3Value(){}

		shared_ptr<Type::Color3> Color3Value::getValue(){
			return Value;
		}

		void Color3Value::setValue(shared_ptr<Type::Color3> value){
			if(!value){
				value = make_shared<Type::Color3>();
			}
			if(!Value->equals(value)){
				Value = value;

				REPLICATE_PROPERTY_CHANGE(Value);
				propertyChanged("Value");
			}
		}

		shared_ptr<Instance> Color3Value::cloneImpl(){
			shared_ptr<Color3Value> cv = make_shared<Color3Value>(eng);
			cv->Archivable = Archivable;
			cv->Name = Name;
			cv->ParentLocked = ParentLocked;

			cv->Value = Value;

			return cv;
		}

#if HAVE_ENET
		void Color3Value::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);

			peer->sendSetPropertyPacket(netId, "Value", make_shared<Type::VarWrapper>(Value));
		}
#endif

		std::map<std::string, _PropertyInfo> Color3Value::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Value"] = {"Color3", false, true, true};

			return propMap;
		}

		void Color3Value::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			if(prop == "Value"){
				setValue(val->asColor3());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> Color3Value::getProperty(std::string prop){
			if(prop == "Value"){
				return make_shared<Type::VarWrapper>(getValue());
			}

			return Instance::getProperty(prop);
		}

		int Color3Value::lua_setValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Color3Value> instCV = dynamic_pointer_cast<Color3Value>(inst);
				if(instCV){
					shared_ptr<Type::Color3> col3 = Type::checkColor3(L, 2, true, true);
					instCV->setValue(col3);
				}
			}

			return 0;
		}

		int Color3Value::lua_getValue(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Color3Value> instCV = dynamic_pointer_cast<Color3Value>(inst);
				if(instCV){
					shared_ptr<Type::Color3> col3 = instCV->getValue();
					if(col3){
						return col3->wrap_lua(L);
					}else{
						lua_pushnil(L);
						return 1;
					}
				}
			}

			lua_pushnil(L);
			return 1;
		}

		void Color3Value::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);

			luaL_Reg properties[] = {
				{"Value", lua_setValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Color3Value::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);

			luaL_Reg properties[] = {
				{"Value", lua_getValue},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
