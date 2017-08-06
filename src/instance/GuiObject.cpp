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

#include "instance/GuiObject.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS_WCLONE(GuiObject, GuiBase2d){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    GuiObject::GuiObject(OBEngine* eng) : GuiBase2d(eng){
			Name = ClassName;
		}

	    GuiObject::~GuiObject(){}

		shared_ptr<Type::Vector2> GuiObject::getAbsolutePosition(){
			return make_shared<Type::Vector2>();
		}

		shared_ptr<Type::Vector2> GuiObject::getAbsoluteSize(){
			return make_shared<Type::Vector2>();
		}

		bool GuiObject::isActive(){
			return Active;
		}
		
		void GuiObject::setActive(bool active){
			if(Active != active){
				Active = active;

				REPLICATE_PROPERTY_CHANGE(Active);
				propertyChanged("Active");
			}
		}
		
		shared_ptr<Type::Color3> GuiObject::getBackgroundColor3(){
			return BackgroundColor3;
		}
		
		void GuiObject::setBackgroundColor3(shared_ptr<Type::Color3> backgroundColor3){
			if(!BackgroundColor3->equals(backgroundColor3)){
				if(backgroundColor3){
					BackgroundColor3 = backgroundColor3;
				}else{
					BackgroundColor3 = make_shared<Type::Color3>();
				}

				REPLICATE_PROPERTY_CHANGE(BackgroundColor3);
				propertyChanged("BackgroundColor3");
			}
		}
		
		double GuiObject::getBackgroundTransparency(){
			return BackgroundTransparency;
		}
		
		void GuiObject::setBackgroundTransparency(double backgroundTransparency){
			if(BackgroundTransparency != backgroundTransparency){
				BackgroundTransparency = backgroundTransparency;

				REPLICATE_PROPERTY_CHANGE(BackgroundTransparency);
				propertyChanged("BackgroundTransparency");
			}
		}
		
		shared_ptr<Type::Color3> GuiObject::getBorderColor3(){
			return BorderColor3;
		}
		
		void GuiObject::setBorderColor3(shared_ptr<Type::Color3> borderColor3){
			if(BorderColor3 != borderColor3){
				if(borderColor3){
					BorderColor3 = borderColor3;
				}else{
					BorderColor3 = make_shared<Type::Color3>();
				}

				REPLICATE_PROPERTY_CHANGE(BorderColor3);
				propertyChanged("BorderColor3");
			}
		}
		
		int GuiObject::getBorderSizePixel(){
			return BorderSizePixel;
		}
		
		void GuiObject::setBorderSizePixel(int borderSizePixel){
			if(BorderSizePixel != borderSizePixel){
				BorderSizePixel = borderSizePixel;

				REPLICATE_PROPERTY_CHANGE(BorderSizePixel);
				propertyChanged("BorderSizePixel");
			}
		}
		
		bool GuiObject::getClipsDescendants(){
			return ClipsDescendants;
		}
		
		void GuiObject::setClipsDescendants(bool clipsDescendants){
			if(ClipsDescendants != clipsDescendants){
				ClipsDescendants = clipsDescendants;

				REPLICATE_PROPERTY_CHANGE(ClipsDescendants);
				propertyChanged("ClipsDescendants");
			}
		}
		
		shared_ptr<Type::UDim2> GuiObject::getPosition(){
			return Position;
		}
		
		void GuiObject::setPosition(shared_ptr<Type::UDim2> position){
			if(!Position->equals(position)){
				if(position){
					Position = position;
				}else{
					Position = make_shared<Type::UDim2>();
				}

				REPLICATE_PROPERTY_CHANGE(Position);
				propertyChanged("Position");
			}
		}
		
		shared_ptr<Type::UDim2> GuiObject::getSize(){
			return Size;
		}
		
		void GuiObject::setSize(shared_ptr<Type::UDim2> size){
			if(!Size->equals(size)){
				if(size){
					Size = size;
				}else{
					Size = make_shared<Type::UDim2>();
				}

				REPLICATE_PROPERTY_CHANGE(Size);
				propertyChanged("Size");
			}
		}
		
		bool GuiObject::isVisible(){
			return Visible;
		}
		
		void GuiObject::setVisible(bool visible){
			if(Visible != visible){
				Visible = visible;

				REPLICATE_PROPERTY_CHANGE(Visible);
				propertyChanged("Visible");
			}
		}
		
		int GuiObject::getZIndex(){
			return ZIndex;
		}
		
		void GuiObject::setZIndex(int zIndex){
			if(ZIndex != zIndex){
				ZIndex = zIndex;

				REPLICATE_PROPERTY_CHANGE(ZIndex);
				propertyChanged("ZIndex");
			}
		}

		#if HAVE_ENET
		void GuiObject::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);
			
			peer->sendSetPropertyPacket(netId, "Active", make_shared<Type::VarWrapper>(Active));
			peer->sendSetPropertyPacket(netId, "BackgroundColor3", make_shared<Type::VarWrapper>(BackgroundColor3));
			peer->sendSetPropertyPacket(netId, "BackgroundTransparency", make_shared<Type::VarWrapper>(BackgroundTransparency));
			peer->sendSetPropertyPacket(netId, "BorderColor3", make_shared<Type::VarWrapper>(BorderColor3));
			peer->sendSetPropertyPacket(netId, "BorderSizePixel", make_shared<Type::VarWrapper>(BorderSizePixel));
			peer->sendSetPropertyPacket(netId, "ClipsDescendants", make_shared<Type::VarWrapper>(ClipsDescendants));
			peer->sendSetPropertyPacket(netId, "Position", make_shared<Type::VarWrapper>(Position));
			peer->sendSetPropertyPacket(netId, "Size", make_shared<Type::VarWrapper>(Size));
			peer->sendSetPropertyPacket(netId, "Visible", make_shared<Type::VarWrapper>(Visible));
			peer->sendSetPropertyPacket(netId, "ZIndex", make_shared<Type::VarWrapper>(ZIndex));
		}
		#endif

		std::map<std::string, _PropertyInfo> GuiObject::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = GuiBase2d::getProperties();
			propMap["Active"] = {"bool", false, true, true};
			propMap["BackgroundColor3"] = {"Color3", false, true, true};
			propMap["BackgroundTransparency"] = {"double", false, true, true};
			propMap["BorderColor3"] = {"Color3", false, true, true};
			propMap["BorderSizePixel"] = {"int", false, true, true};
			propMap["ClipsDescendants"] = {"bool", false, true, true};
			propMap["Position"] = {"UDim2", false, true, true};
			propMap["Size"] = {"UDim2", false, true, true};
			propMap["Visible"] = {"bool", false, true, true};
			propMap["ZIndex"] = {"int", false, true, true};

			return propMap;
		}

	    shared_ptr<Type::VarWrapper> GuiObject::getProperty(std::string prop){
			if(prop == "Active"){
				return make_shared<Type::VarWrapper>(isActive());
			}
			if(prop == "BackgroundColor3"){
				return make_shared<Type::VarWrapper>(getBackgroundColor3());
			}
			if(prop == "BackgroundTransparency"){
				return make_shared<Type::VarWrapper>(getBackgroundTransparency());
			}
			if(prop == "BorderColor3"){
				return make_shared<Type::VarWrapper>(getBorderColor3());
			}
			if(prop == "BorderSizePixel"){
				return make_shared<Type::VarWrapper>(getBorderSizePixel());
			}
			if(prop == "ClipsDescendants"){
				return make_shared<Type::VarWrapper>(getClipsDescendants());
			}
			if(prop == "Position"){
				return make_shared<Type::VarWrapper>(getPosition());
			}
			if(prop == "Size"){
				return make_shared<Type::VarWrapper>(getSize());
			}
			if(prop == "Visible"){
				return make_shared<Type::VarWrapper>(isVisible());
			}
			if(prop == "ZIndex"){
				return make_shared<Type::VarWrapper>(getZIndex());
			}
			
			return GuiBase2d::getProperty(prop);
		}

		void GuiObject::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Active"){
			    setActive(val->asBool());
				return;
			}
			if(prop == "BackgroundColor3"){
			    setBackgroundColor3(val->asColor3());
				return;
			}
			if(prop == "BackgroundTransparency"){
			    setBackgroundTransparency(val->asDouble());
				return;
			}
			if(prop == "BorderColor3"){
			    setBorderColor3(val->asColor3());
				return;
			}
			if(prop == "BorderSizePixel"){
			    setBorderSizePixel(val->asInt());
				return;
			}
			if(prop == "ClipsDescendants"){
			    setClipsDescendants(val->asBool());
				return;
			}
			if(prop == "Position"){
			    setPosition(val->asUDim2());
				return;
			}
			if(prop == "Size"){
			    setSize(val->asUDim2());
				return;
			}
			if(prop == "Visible"){
			    setVisible(val->asBool());
				return;
			}
			if(prop == "ZIndex"){
			    setZIndex(val->asInt());
				return;
			}

			Instance::setProperty(prop, val);
		}

		int GuiObject::lua_getActive(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					lua_pushboolean(L, instGO->isActive());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int GuiObject::lua_setActive(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					bool newV = lua_toboolean(L, 2);
					instGO->setActive(newV);
				}
			}
			
			return 0;
		}

		int GuiObject::lua_getBackgroundColor3(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
				    shared_ptr<Type::Color3> col3 = instGO->getBackgroundColor3();
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

		int GuiObject::lua_setBackgroundColor3(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
				    shared_ptr<Type::Color3> col3 = Type::checkColor3(L, 2, true, true);
					instGO->setBackgroundColor3(col3);
				}
			}
			
			return 0;
		}

		int GuiObject::lua_getBackgroundTransparency(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					lua_pushnumber(L, instGO->getBackgroundTransparency());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int GuiObject::lua_setBackgroundTransparency(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					float newV = luaL_checknumber(L, 2);
					instGO->setBackgroundTransparency(newV);
				}
			}
			
			return 0;
		}

		int GuiObject::lua_getBorderColor3(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
				    shared_ptr<Type::Color3> col3 = instGO->getBorderColor3();
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

		int GuiObject::lua_setBorderColor3(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
				    shared_ptr<Type::Color3> col3 = Type::checkColor3(L, 2, true, true);
					instGO->setBorderColor3(col3);
				}
			}
			
			return 0;
		}

		int GuiObject::lua_getBorderSizePixel(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					lua_pushinteger(L, instGO->getBorderSizePixel());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int GuiObject::lua_setBorderSizePixel(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					int newV = luaL_checkinteger(L, 2);
					instGO->setBorderSizePixel(newV);
				}
			}
			
			return 0;
		}

		int GuiObject::lua_getClipsDescendants(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					lua_pushboolean(L, instGO->getClipsDescendants());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int GuiObject::lua_setClipsDescendants(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					bool newV = lua_toboolean(L, 2);
					instGO->setClipsDescendants(newV);
				}
			}
			
			return 0;
		}

		int GuiObject::lua_getPosition(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
				    shared_ptr<Type::UDim2> udim2 = instGO->getPosition();
					if(udim2){
						return udim2->wrap_lua(L);
					}else{
						lua_pushnil(L);
						return 1;
					}
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int GuiObject::lua_setPosition(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
				    shared_ptr<Type::UDim2> udim2 = Type::checkUDim2(L, 2, true, true);
					instGO->setPosition(udim2);
				}
			}
			
			return 0;
		}

		int GuiObject::lua_getSize(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
				    shared_ptr<Type::UDim2> udim2 = instGO->getSize();
					if(udim2){
						return udim2->wrap_lua(L);
					}else{
						lua_pushnil(L);
						return 1;
					}
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int GuiObject::lua_setSize(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
				    shared_ptr<Type::UDim2> udim2 = Type::checkUDim2(L, 2, true, true);
					instGO->setSize(udim2);
				}
			}
			
			return 0;
		}

		int GuiObject::lua_getVisible(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					lua_pushboolean(L, instGO->isVisible());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int GuiObject::lua_setVisible(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					bool newV = lua_toboolean(L, 2);
					instGO->setVisible(newV);
				}
			}
			
			return 0;
		}

		int GuiObject::lua_getZIndex(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					lua_pushinteger(L, instGO->getZIndex());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int GuiObject::lua_setZIndex(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<GuiObject> instGO = dynamic_pointer_cast<GuiObject>(inst);
				if(instGO){
					int newV = luaL_checkinteger(L, 2);
					instGO->setZIndex(newV);
				}
			}
			
			return 0;
		}

		void GuiObject::register_lua_property_setters(lua_State* L){
		    GuiBase2d::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"Active", lua_setActive},
				{"BackgroundColor3", lua_setBackgroundColor3},
				{"BackgroundTransparency", lua_setBackgroundTransparency},
				{"BorderColor3", lua_setBorderColor3},
				{"BorderSizePixel", lua_setBorderSizePixel},
				{"ClipsDescendants", lua_setClipsDescendants},
				{"Position", lua_setPosition},
				{"Size", lua_setSize},
				{"Visible", lua_setVisible},
				{"ZIndex", lua_setZIndex},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void GuiObject::register_lua_property_getters(lua_State* L){
		    GuiBase2d::register_lua_property_getters(L);
			
		    luaL_Reg properties[] = {
				{"Active", lua_getActive},
				{"BackgroundColor3", lua_getBackgroundColor3},
				{"BackgroundTransparency", lua_getBackgroundTransparency},
				{"BorderColor3", lua_getBorderColor3},
				{"BorderSizePixel", lua_getBorderSizePixel},
				{"ClipsDescendants", lua_getClipsDescendants},
				{"Position", lua_getPosition},
				{"Size", lua_getSize},
				{"Visible", lua_getVisible},
				{"ZIndex", lua_getZIndex},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
