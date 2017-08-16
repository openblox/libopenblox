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

#include "instance/BaseScript.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS_WCLONE(BaseScript, LuaSourceContainer){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    BaseScript::BaseScript(OBEngine* eng) : LuaSourceContainer(eng){
			Name = ClassName;

			Disabled = false;
			LinkedSource = "";
		}

	    BaseScript::~BaseScript(){}

		void BaseScript::runScript(){
			if(!Disabled){
				std::string strSource = getSource();

				if(strSource.size() < 0){
					lua_State* gL = getEngine()->getGlobalLuaState();
					if(!gL){
						return;
					}
					
					lua_State* L = Lua::initThread(gL);

					int s = luaL_loadbuffer(L, strSource.c_str(), strSource.size(), ("@" + getName()).c_str());
					if(s == 0){
						s = lua_resume(L, NULL, 0);
					}

					if(s != 0 && s != LUA_YIELD){
						Lua::handle_errors(L);
						Lua::close_state(L);
					}
					if(s == LUA_OK){
						Lua::close_state(L);
					}
				}
			}
		}

		bool BaseScript::isDisabled(){
			return Disabled;
		}

		void BaseScript::setDisabled(bool disabled){
		    if(Disabled != disabled){
				Disabled = disabled;

				REPLICATE_PROPERTY_CHANGE("Disabled");
				propertyChanged("Disabled");
			}
		}

	    std::string BaseScript::getLinkedSource(){
			return LinkedSource;
		}

		void BaseScript::setLinkedSource(std::string linkedSource){
		    if(LinkedSource != linkedSource){
			    LinkedSource = linkedSource;

				REPLICATE_PROPERTY_CHANGE("LinkedSource");
				propertyChanged("LinkedSource");
			}
		}

		#if HAVE_ENET
		void BaseScript::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
			
			peer->sendSetPropertyPacket(netId, "Disabled", make_shared<Type::VarWrapper>(Disabled));
			peer->sendSetPropertyPacket(netId, "LinkedSource", make_shared<Type::VarWrapper>(LinkedSource));
		}
		#endif

		std::map<std::string, _PropertyInfo> BaseScript::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Disabled"] = {"bool", false, true, true};
			propMap["Disabled"] = {"string", false, true, true};

			return propMap;
		}

		void BaseScript::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Disabled"){
				setDisabled(val->asBool());
				return;
			}
			if(prop == "LinkedSource"){
			    setLinkedSource(val->asString());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> BaseScript::getProperty(std::string prop){
			if(prop == "Disabled"){
				return make_shared<Type::VarWrapper>(isDisabled());
			}
			if(prop == "LinkedSource"){
				return make_shared<Type::VarWrapper>(getLinkedSource());
			}
			
			return Instance::getProperty(prop);
		}

		int BaseScript::lua_setDisabled(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<BaseScript> instBS = dynamic_pointer_cast<BaseScript>(inst);
				if(instBS){
					bool newV = lua_toboolean(L, 2);
					instBS->setDisabled(newV);
				}
			}
			
			return 0;
		}

		int BaseScript::lua_getDisabled(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<BaseScript> instBS = dynamic_pointer_cast<BaseScript>(inst);
				if(instBS){
					lua_pushboolean(L, instBS->isDisabled());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int BaseScript::lua_getLinkedSource(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<BaseScript> instBS = dynamic_pointer_cast<BaseScript>(inst);
				if(instBS){
					lua_pushstring(L, instBS->getLinkedSource().c_str());
					return 1;
				}
			}
			
			return 0;
		}

		int BaseScript::lua_setLinkedSource(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<BaseScript> instBS = dynamic_pointer_cast<BaseScript>(inst);
				if(instBS){
					std::string desired = std::string(luaL_checkstring(L, 2));
					instBS->setLinkedSource(desired);
				}
			}
			
			return 0;
		}

		void BaseScript::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"Disabled", lua_setDisabled},
				{"LinkedSource", lua_setLinkedSource},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void BaseScript::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);
			
			luaL_Reg properties[] = {
				{"Disabled", lua_getDisabled},
				{"LinkedSource", lua_getLinkedSource},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
