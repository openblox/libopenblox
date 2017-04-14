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

#include "instance/BasePart.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS_WCLONE(BasePart, PVInstance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    BasePart::BasePart(){
			Name = ClassName;

		    Anchored = true;
		    Color = make_shared<Type::Color3>(163/255, 162/255, 165/255);
		    CanCollide = false;
		    Locked = false;
		    Transparency = 0;
		}

		BasePart::~BasePart(){}

		void BasePart::setAnchored(bool anchored){
			if(anchored != Anchored){
				Anchored = anchored;

				propertyChanged("Anchored");
			}
		}
		
		bool BasePart::getAnchored(){
			return Anchored;
		}
				
		void BasePart::setColor(shared_ptr<Type::Color3> color){
			if(color == NULL){
				shared_ptr<Type::Color3> col3 = make_shared<Type::Color3>();
				if(!col3->equals(Color)){
					Color = col3;
					
					propertyChanged("Color");
				}
			}else{
				if(!color->equals(Color)){
					Color = color;

					propertyChanged("Color");
				}
			}
		}
		
		shared_ptr<Type::Color3> BasePart::getColor(){
			return Color;
		}

		void BasePart::setCanCollide(bool cancollide){
			if(cancollide != CanCollide){
				CanCollide = cancollide;

				propertyChanged("CanCollide");
			}
		}
		
		bool BasePart::getCanCollide(){
			return CanCollide;
		}

		void BasePart::setLocked(bool locked){
			if(locked != Locked){
				Locked = locked;

				propertyChanged("Locked");
			}
		}
		
		bool BasePart::getLocked(){
			return Locked;
		}

		void BasePart::setTransparency(double transparency){
			if(transparency != Transparency){
				Transparency = transparency;
				
				propertyChanged("Transparency");
			}
		}
		
		double BasePart::getTransparency(){
			return Transparency;
		}

		int BasePart::lua_setAnchored(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
					bool newV = lua_toboolean(L, 2);
					instBP->setAnchored(newV);
				}
			}
			return 0;
		}

		int BasePart::lua_getAnchored(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
					lua_pushboolean(L, instBP->getAnchored());
					return 1;
				}
			}
			lua_pushnil(L);
			return 1;
		}

		int BasePart::lua_setColor(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
				    shared_ptr<Type::Color3> col3 = Type::checkColor3(L, 2);
					if(col3){
						instBP->setColor(col3);
					}else{
						if(lua_isnil(L, 2)){
							instBP->setColor(NULL);
						}else{
							return luaL_error(L, "bad argument #1 to '?' (Color3 expected, got %s)", lua_typename(L, 2));
						}
					}
				}
			}
			return 0;
		}

		int BasePart::lua_getColor(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
				    shared_ptr<Type::Color3> col3 = instBP->getColor();
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

		int BasePart::lua_setCanCollide(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
					bool newV = lua_toboolean(L, 2);
					instBP->setCanCollide(newV);
				}
			}
			return 0;
		}

		int BasePart::lua_getCanCollide(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
					lua_pushboolean(L, instBP->getCanCollide());
					return 1;
				}
			}
			lua_pushnil(L);
			return 1;
		}

		int BasePart::lua_setLocked(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
					bool newV = lua_toboolean(L, 2);
					instBP->setLocked(newV);
				}
			}
			return 0;
		}

		int BasePart::lua_getLocked(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
					lua_pushboolean(L, instBP->getLocked());
					return 1;
				}
			}
			lua_pushnil(L);
			return 1;
		}

		int BasePart::lua_setTransparency(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
				    double newV = luaL_checknumber(L, 2);
					instBP->setTransparency(newV);
				}
			}
			return 0;
		}

		int BasePart::lua_getTransparency(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1);
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
					lua_pushnumber(L, instBP->getTransparency());
					return 1;
				}
			}
			lua_pushnil(L);
			return 1;
		}

		void BasePart::register_lua_property_setters(lua_State* L){
			PVInstance::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"Anchored", lua_setAnchored},
				{"Color", lua_setColor},
				{"CanCollide", lua_setCanCollide},
				{"Locked", lua_setLocked},
				{"Transparency", lua_setTransparency},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void BasePart::register_lua_property_getters(lua_State* L){
			PVInstance::register_lua_property_getters(L);
			
			luaL_Reg properties[] = {
				{"Anchored", lua_getAnchored},
				{"Color", lua_getColor},
				{"CanCollide", lua_getCanCollide},
				{"Locked", lua_getLocked},
				{"Transparency", lua_getTransparency},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
