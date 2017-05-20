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

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

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

				REPLICATE_PROPERTY_CHANGE(Anchored);
				propertyChanged("Anchored");
			}
		}
		
		bool BasePart::getAnchored(){
			return Anchored;
		}
				
		void BasePart::setColor(shared_ptr<Type::Color3> color){
			if(!color){
				shared_ptr<Type::Color3> col3 = make_shared<Type::Color3>();
				if(!col3->equals(Color)){
					Color = col3;

					updateColor();
					REPLICATE_PROPERTY_CHANGE(Color);
					propertyChanged("Color");
				}
			}else{
				if(!color->equals(Color)){
					Color = color;

					updateColor();
					REPLICATE_PROPERTY_CHANGE(Color);
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

				REPLICATE_PROPERTY_CHANGE(CanCollide);
				propertyChanged("CanCollide");
			}
		}
		
		bool BasePart::getCanCollide(){
			return CanCollide;
		}

		void BasePart::setLocked(bool locked){
			if(locked != Locked){
				Locked = locked;

				REPLICATE_PROPERTY_CHANGE(Locked);
				propertyChanged("Locked");
			}
		}
		
		bool BasePart::getLocked(){
			return Locked;
		}

		void BasePart::setTransparency(double transparency){
			if(transparency != Transparency){
				Transparency = transparency;

				REPLICATE_PROPERTY_CHANGE(Transparency);
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
					REPLICATE_PROPERTY_CHANGE(Position);
					propertyChanged("Position");
				}
			}else{
				if(!position->equals(Position)){
					Position = position;

					updatePosition();
					REPLICATE_PROPERTY_CHANGE(Position);
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
					REPLICATE_PROPERTY_CHANGE(Rotation);
					propertyChanged("Rotation");
				}
			}else{
				if(!rotation->equals(Rotation)){
					Rotation = rotation;

					updateRotation();
					REPLICATE_PROPERTY_CHANGE(Rotation);
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

		#if HAVE_ENET
		void BasePart::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
			
			peer->sendSetPropertyPacket(netId, "Anchored", make_shared<Type::VarWrapper>(Anchored));
			peer->sendSetPropertyPacket(netId, "Color", make_shared<Type::VarWrapper>(Color));
			peer->sendSetPropertyPacket(netId, "CanCollide", make_shared<Type::VarWrapper>(CanCollide));
			peer->sendSetPropertyPacket(netId, "Locked", make_shared<Type::VarWrapper>(Locked));
			peer->sendSetPropertyPacket(netId, "Transparency", make_shared<Type::VarWrapper>(Transparency));
			peer->sendSetPropertyPacket(netId, "Position", make_shared<Type::VarWrapper>(Position));
			peer->sendSetPropertyPacket(netId, "Rotation", make_shared<Type::VarWrapper>(Rotation));
		}
		#endif

		std::map<std::string, _PropertyInfo> BasePart::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Anchored"] = {"bool", false, true, true};
			propMap["Color"] = {"Color3", false, true, true};
			propMap["CanCollide"] = {"bool", false, true, true};
			propMap["Locked"] = {"bool", false, true, true};
			propMap["Transparency"] = {"double", false, true, true};
			propMap["Position"] = {"Vector3", false, true, false};
			propMap["Rotation"] = {"Vector3", false, true, false};

			return propMap;
		}

		void BasePart::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Anchored"){
			    setAnchored(val->asBool());
				return;
			}
			if(prop == "Color"){
				setColor(val->asColor3());
				return;
			}
			if(prop == "CanCollide"){
				setCanCollide(val->asBool());
				return;
			}
			if(prop == "Locked"){
				setLocked(val->asBool());
				return;
			}
			if(prop == "Transparency"){
			    setTransparency(val->asDouble());
				return;
			}
			if(prop == "Position"){
				setPosition(val->asVector3());
				return;
			}
			if(prop == "Rotation"){
				setRotation(val->asVector3());
				return;
			}

		    Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> BasePart::getProperty(std::string prop){
			if(prop == "Name"){
				return make_shared<Type::VarWrapper>(getName());
			}
		    if(prop == "Anchored"){
			    return make_shared<Type::VarWrapper>(getAnchored());
			}
			if(prop == "Color"){
			    return make_shared<Type::VarWrapper>(getColor());
			}
			if(prop == "CanCollide"){
			    return make_shared<Type::VarWrapper>(getCanCollide());
			}
			if(prop == "Locked"){
			    return make_shared<Type::VarWrapper>(getLocked());
			}
			if(prop == "Transparency"){
			    return make_shared<Type::VarWrapper>(getTransparency());
			}
			if(prop == "Position"){
			    return make_shared<Type::VarWrapper>(getPosition());
			}
			if(prop == "Rotation"){
			    return make_shared<Type::VarWrapper>(getRotation());
			}
			
			return Instance::getProperty(prop);
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
