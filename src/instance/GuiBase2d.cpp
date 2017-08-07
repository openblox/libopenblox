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

#include "instance/GuiBase2d.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS_WCLONE(GuiBase2d, GuiBase){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    GuiBase2d::GuiBase2d(OBEngine* eng) : GuiBase(eng){
			Name = ClassName;
		}

	    GuiBase2d::~GuiBase2d(){}

		std::vector<shared_ptr<GuiBase2d>> GuiBase2d::getRenderableChildren(){
		    std::vector<shared_ptr<GuiBase2d>> renderableKids;

			std::vector<shared_ptr<Instance>> kids = GetChildren();
			
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != kids.size(); i++){
				shared_ptr<Instance> kid = kids[i];
				if(shared_ptr<GuiBase2d> kgb2 = dynamic_pointer_cast<GuiBase2d>(kid)){
				    renderableKids.push_back(kgb2);
				}
			}

			return renderableKids;
		}

		bool GuiBase2d::containsPoint(shared_ptr<Type::Vector2> p){
			return false;
		}

		bool GuiBase2d::handleClick(shared_ptr<Type::Vector2> p){
			std::vector<shared_ptr<Instance>> kids = GetChildren();
			
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != kids.size(); i++){
				shared_ptr<Instance> kid = kids[i];
				if(shared_ptr<GuiBase2d> kgb2 = dynamic_pointer_cast<GuiBase2d>(kid)){
				    if(kgb2->handleClick(p)){
						return true;
					}
				}
			}

			return false;
		}

		void GuiBase2d::render(){
			std::vector<shared_ptr<GuiBase2d>> renderableKids = getRenderableChildren();

			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != renderableKids.size(); i++){
				shared_ptr<GuiBase2d> kid = renderableKids[i];
				if(kid){
					kid->render();
				}
			}
		}

		shared_ptr<Type::Vector2> GuiBase2d::getAbsolutePosition(){
			return make_shared<Type::Vector2>();
		}

		shared_ptr<Type::Vector2> GuiBase2d::getAbsoluteSize(){
			return make_shared<Type::Vector2>();
		}

		int GuiBase2d::lua_getAbsolutePosition(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiBase2d> instGB2 = dynamic_pointer_cast<GuiBase2d>(inst);
				if(instGB2){
				    return instGB2->getAbsolutePosition()->wrap_lua(L);
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int GuiBase2d::lua_getAbsoluteSize(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiBase2d> instGB2 = dynamic_pointer_cast<GuiBase2d>(inst);
				if(instGB2){
				    return instGB2->getAbsoluteSize()->wrap_lua(L);
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		std::map<std::string, _PropertyInfo> GuiBase2d::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["AbsolutePosition"] = {"Vector2", true, true, false};
			propMap["AbsoluteSize"] = {"Vector2", true, true, false};

			return propMap;
		}

	    shared_ptr<Type::VarWrapper> GuiBase2d::getProperty(std::string prop){
			if(prop == "AbsolutePosition"){
				return make_shared<Type::VarWrapper>(getAbsolutePosition());
			}
			if(prop == "AbsoluteSize"){
				return make_shared<Type::VarWrapper>(getAbsoluteSize());
			}
			
			return Instance::getProperty(prop);
		}

		void GuiBase2d::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"AbsolutePosition", Instance::lua_readOnlyProperty},
				{"AbsoluteSize", Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void GuiBase2d::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);
			
		    luaL_Reg properties[] = {
				{"AbsolutePosition", lua_getAbsolutePosition},
				{"AbsoluteSize", lua_getAbsoluteSize},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
