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
			Position = make_shared<Type::Vector3>(0, 0, 0);
		    Rotation = make_shared<Type::Vector3>(0, 0, 0);
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

					updateColor();
					propertyChanged("Color");
				}
			}else{
				if(!color->equals(Color)){
					Color = color;

					updateColor();
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

		void BasePart::setPosition(shared_ptr<Type::Vector3> position){
			if(position == NULL){
				shared_ptr<Type::Vector3> vec3 = make_shared<Type::Vector3>(0, 0, 0);
				if(!vec3->equals(Position)){
					Position = vec3;

					updatePosition();
					propertyChanged("Position");
				}
			}else{
				if(!position->equals(Position)){
					Position = position;

					updatePosition();
					propertyChanged("Position");
				}
			}
		}
		
		shared_ptr<Type::Vector3> BasePart::getPosition(){
			return Position;
		}

		void BasePart::setRotation(shared_ptr<Type::Vector3> rotation){
			if(rotation == NULL){
				shared_ptr<Type::Vector3> vec3 = make_shared<Type::Vector3>(0, 0, 0);
				if(!vec3->equals(Rotation)){
					Rotation = vec3;

					updateRotation();
					propertyChanged("Rotation");
				}
			}else{
				if(!rotation->equals(Rotation)){
					Rotation = rotation;

					updateRotation();
					propertyChanged("Rotation");
				}
			}
		}
		
		shared_ptr<Type::Vector3> BasePart::getRotation(){
			return Rotation;
		}

		void BasePart::updateColor(){}

		void BasePart::updatePosition(){
			#if HAVE_IRRLICHT
			if(irrNode){
				irrNode->setPosition(getPosition()->toIrrlichtVector3df());
			}
			#endif
		}

		void BasePart::updateRotation(){
			#if HAVE_IRRLICHT
			if(irrNode){
				irrNode->setRotation(getRotation()->toIrrlichtVector3df());
			}
			#endif
		}

		int BasePart::lua_setAnchored(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
				    shared_ptr<Type::Color3> col3 = Type::checkColor3(L, 2, true, true);
					instBP->setColor(col3);
				}
			}
			
			return 0;
		}

		int BasePart::lua_getColor(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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

		int BasePart::lua_setPosition(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
				    shared_ptr<Type::Vector3> vec3 = Type::checkVector3(L, 2, true, true);
					instBP->setPosition(vec3);
				}
			}
			
			return 0;
		}

		int BasePart::lua_getPosition(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
				    shared_ptr<Type::Vector3> vec3 = instBP->getPosition();
					if(vec3){
						return vec3->wrap_lua(L);
					}else{
						lua_pushnil(L);
						return 1;
					}
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int BasePart::lua_setRotation(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
				    shared_ptr<Type::Vector3> vec3 = Type::checkVector3(L, 2, true, true);
					instBP->setRotation(vec3);
				}
			}
			
			return 0;
		}

		int BasePart::lua_getRotation(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<BasePart> instBP = dynamic_pointer_cast<BasePart>(inst);
				if(instBP){
				    shared_ptr<Type::Vector3> vec3 = instBP->getRotation();
					if(vec3){
						return vec3->wrap_lua(L);
					}else{
						lua_pushnil(L);
						return 1;
					}
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
				{"Position", lua_setPosition},
				{"Rotation", lua_setRotation},
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
				{"Position", lua_getPosition},
				{"Rotation", lua_getRotation},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
