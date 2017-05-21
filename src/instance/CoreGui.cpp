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

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#endif

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(CoreGui, false, isDataModel, BasePlayerGui){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    CoreGui::CoreGui(OBEngine* eng) : BasePlayerGui(eng){
			Name = ClassName;
			
			Enabled = true;
		}

	    CoreGui::~CoreGui(){}

		shared_ptr<Instance> CoreGui::cloneImpl(){
			return NULL;
		}

		bool CoreGui::isEnabled(){
			return Enabled;
		}
		
		void CoreGui::setEnabled(bool enabled){
			if(Enabled != enabled){
				Enabled = enabled;

			    REPLICATE_PROPERTY_CHANGE(Enabled);
				propertyChanged("Enabled");
			}
		}

		#if HAVE_ENET
		void CoreGui::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
			
			peer->sendSetPropertyPacket(netId, "Enabled", make_shared<Type::VarWrapper>(Enabled));
		}
		#endif

		std::map<std::string, _PropertyInfo> CoreGui::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Enabled"] = {"bool", false, true, true};

			return propMap;
		}

		void CoreGui::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Enabled"){
			    setEnabled(val->asBool());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> CoreGui::getProperty(std::string prop){
			if(prop == "Enabled"){
				return make_shared<Type::VarWrapper>(isEnabled());
			}
			
			return Instance::getProperty(prop);
		}

		#if HAVE_PUGIXML
	    std::string CoreGui::serializedID(){
			shared_ptr<OBSerializer> serializer = eng->getSerializer();
			serializer->SetID(shared_from_this(), "CoreGui");
			
			return Instance::serializedID();
		}
		#endif
		
		int CoreGui::lua_getEnabled(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<CoreGui> instCG = dynamic_pointer_cast<CoreGui>(inst);
				if(instCG){
					lua_pushboolean(L, instCG->isEnabled());
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int CoreGui::lua_setEnabled(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<CoreGui> instCG = dynamic_pointer_cast<CoreGui>(inst);
				if(instCG){
					bool newV = lua_toboolean(L, 2);
					instCG->setEnabled(newV);
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
