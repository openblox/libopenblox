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

#include "instance/Lighting.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Lighting, false, isDataModel, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    Lighting::Lighting(){
			Name = ClassName;
		}

	    Lighting::~Lighting(){}

		shared_ptr<Instance> Lighting::cloneImpl(){
			return NULL;
		}

		shared_ptr<Type::Color3> Lighting::GetSkyColor(){
			return SkyColor;
		}
		
		void Lighting::SetSkyColor(shared_ptr<Type::Color3> skyColor){
			if(skyColor == NULL){
				shared_ptr<Type::Color3> col3 = make_shared<Type::Color3>();
				if(!col3->equals(SkyColor)){
					SkyColor = col3;
				}
			}else{
				if(!skyColor->equals(SkyColor)){
					SkyColor = skyColor;
				}
			}
		}

		int Lighting::lua_getSkyColor(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
				    shared_ptr<Type::Color3> col3 = instL->GetSkyColor();
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

		int Lighting::lua_setSkyColor(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
				    shared_ptr<Type::Color3> col3 = Type::checkColor3(L, 2);
					if(col3){
						instL->SetSkyColor(col3);
					}else{
						if(lua_isnil(L, 2)){
							instL->SetSkyColor(NULL);
						}else{
							return luaL_error(L, "bad argument #1 to '?' (Color3 expected, got %s)", lua_typename(L, 2));
						}
					}
				}
			}
			lua_pushnil(L);
			return 1;
		}

		void Lighting::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"SkyColor", lua_setSkyColor},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Lighting::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);
			
			luaL_Reg properties[] = {
				{"SkyColor", lua_getSkyColor},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
