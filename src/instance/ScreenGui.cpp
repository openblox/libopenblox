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

#include "instance/ScreenGui.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(ScreenGui, true, false, LayerCollector){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    ScreenGui::ScreenGui(OBEngine* eng) : LayerCollector(eng){
			Name = ClassName;

			Enabled = true;
			DisplayOrder = 0;
		}

	    ScreenGui::~ScreenGui(){}

		shared_ptr<Instance> ScreenGui::cloneImpl(){
			return NULL;
		}

		void ScreenGui::render(){
			if(Enabled){
				GuiBase2d::render();
			}
		}

		shared_ptr<Type::Vector2> ScreenGui::getAbsolutePosition(){
			if(Parent){
				if(shared_ptr<GuiBase2d> pgo = dynamic_pointer_cast<GuiBase2d>(Parent)){
					return pgo->getAbsolutePosition();
				}
			}
			return make_shared<Type::Vector2>(0, 0);
		}

		shared_ptr<Type::Vector2> ScreenGui::getAbsoluteSize(){
			if(Parent){
				if(shared_ptr<GuiBase2d> pgo = dynamic_pointer_cast<GuiBase2d>(Parent)){
					return pgo->getAbsoluteSize();
				}
			}
			return make_shared<Type::Vector2>(0, 0);
		}

		bool ScreenGui::isEnabled(){
			return Enabled;
		}

		void ScreenGui::setEnabled(bool enabled){
			if(Enabled != enabled){
			    Enabled = enabled;

				REPLICATE_PROPERTY_CHANGE(Enabled);
				propertyChanged("Enabled");
			}
		}

		int ScreenGui::getDisplayOrder(){
			return DisplayOrder;
		}
		
		void ScreenGui::setDisplayOrder(int displayOrder){
			if(DisplayOrder != displayOrder){
			    DisplayOrder = displayOrder;

				REPLICATE_PROPERTY_CHANGE(DisplayOrder);
				propertyChanged("DisplayOrder");
			}
		}

		#if HAVE_ENET
		void ScreenGui::replicateProperties(shared_ptr<NetworkReplicator> peer){
		    GuiBase2d::replicateProperties(peer);
			
		    peer->sendSetPropertyPacket(netId, "Enabled", make_shared<Type::VarWrapper>(Enabled));
			peer->sendSetPropertyPacket(netId, "DisplayOrder", make_shared<Type::VarWrapper>(DisplayOrder));
		}
		#endif

		std::map<std::string, _PropertyInfo> ScreenGui::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = GuiBase2d::getProperties();
			propMap["Enabled"] = {"bool", false, true, true};
			propMap["DisplayOrder"] = {"int", false, true, true};

			return propMap;
		}

	    shared_ptr<Type::VarWrapper> ScreenGui::getProperty(std::string prop){
			if(prop == "Enabled"){
				return make_shared<Type::VarWrapper>(isEnabled());
			}
			if(prop == "DisplayOrder"){
				return make_shared<Type::VarWrapper>(getDisplayOrder());
			}

			return GuiBase2d::getProperty(prop);
		}

		void ScreenGui::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Enabled"){
			    setEnabled(val->asBool());
				return;
			}
			if(prop == "DisplayOrder"){
			    setDisplayOrder(val->asInt());
				return;
			}

			Instance::setProperty(prop, val);
		}

		int ScreenGui::lua_getEnabled(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<ScreenGui> instSG = dynamic_pointer_cast<ScreenGui>(inst);
				if(instSG){
					lua_pushboolean(L, instSG->isEnabled());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int ScreenGui::lua_setEnabled(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<ScreenGui> instSG = dynamic_pointer_cast<ScreenGui>(inst);
				if(instSG){
					bool newV = lua_toboolean(L, 2);
					instSG->setEnabled(newV);
				}
			}
			
			return 0;
		}

		int ScreenGui::lua_getDisplayOrder(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<ScreenGui> instSG = dynamic_pointer_cast<ScreenGui>(inst);
				if(instSG){
					lua_pushinteger(L, instSG->getDisplayOrder());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int ScreenGui::lua_setDisplayOrder(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<ScreenGui> instSG = dynamic_pointer_cast<ScreenGui>(inst);
				if(instSG){
					int newV = luaL_checkinteger(L, 2);
					instSG->setDisplayOrder(newV);
				}
			}
			
			return 0;
		}

		void ScreenGui::register_lua_property_setters(lua_State* L){
		    GuiBase2d::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"Enabled", lua_setEnabled},
				{"DisplayOrder", lua_setDisplayOrder},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void ScreenGui::register_lua_property_getters(lua_State* L){
		    GuiBase2d::register_lua_property_getters(L);
			
		    luaL_Reg properties[] = {
				{"Enabled", lua_getEnabled},
				{"DisplayOrder", lua_getDisplayOrder},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
