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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "instance/Lighting.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

#include "instance/Sky.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#endif

#include <cfloat>

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Lighting, false, isDataModel, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Lighting::Lighting(OBEngine* eng) : Instance(eng){
			Name = ClassName;
			netId = OB_NETID_LIGHTING;

		    Sky = NULL;
			SkyTransparent = false;

			FogEnabled = false;
			FogStart = 0;
			FogEnd = 0;

			//set the fogend to a very large value to prevent the fog from showing
#if HAVE_IRRLICHT
			irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
			if (irrDev){
				irr::video::IVideoDriver* driver = irrDev->getVideoDriver();
				if (driver){
					driver->setFog(irr::video::SColor(255, 0, 0, 0), irr::video::EFT_FOG_LINEAR, 0, 0xFFFFFFFF, 0, true, false);
				}
			}
#endif
		}

		Lighting::~Lighting(){}

#if HAVE_PUGIXML
		std::string Lighting::serializedID(){
			shared_ptr<OBSerializer> serializer = eng->getSerializer();
			serializer->SetID(shared_from_this(), getClassName());

			return Instance::serializedID();
		}
#endif

		shared_ptr<Instance> Lighting::cloneImpl(){
			return NULL;
		}

		void Lighting::removeChild(shared_ptr<Instance> kid){
			Instance::removeChild(kid);

			if(kid){
				if(kid == Sky){
					setSky(NULL);
				}
			}
		}

	    void Lighting::preRender(){
			if(Sky){
				Sky->preRender();
			}
		}

		void Lighting::render(){
			if(Sky){
				Sky->render();
			}
		}

		shared_ptr<Instance> Lighting::getSky(){
			return Sky;
		}

		void Lighting::setSky(shared_ptr<Instance> sky){
			shared_ptr<OB::Instance::Sky> newSky;

			if(sky){
				if(shared_ptr<OB::Instance::Sky> sI = dynamic_pointer_cast<OB::Instance::Sky>(sky)){
					newSky = sI;
				}else{
					throw new OBException("The property Sky must be an instance of the Sky class.");
				}

				shared_ptr<Instance> skyPar = sky->getParent();
				if(!skyPar || skyPar->GetNetworkID() != OB_NETID_LIGHTING){
					throw new OBException("The property Sky must be a direct descendant of Lighting.");
				}
			}

			if(Sky != sky){
				if(Sky){
					if(shared_ptr<OB::Instance::Sky> sI = dynamic_pointer_cast<OB::Instance::Sky>(Sky)){
						sI->deactivateSky();
					}
				}

			    Sky = sky;

				if(Sky){
				    newSky->activateSky();
				}

				REPLICATE_PROPERTY_CHANGE(Sky);
				propertyChanged("Sky");
			}
		}

		shared_ptr<Type::Color3> Lighting::getSkyColor(){
			return SkyColor;
		}

		void Lighting::setSkyColor(shared_ptr<Type::Color3> skyColor){
			if(skyColor == NULL){
				shared_ptr<Type::Color3> col3 = make_shared<Type::Color3>();
				if(!col3->equals(SkyColor)){
					SkyColor = col3;

					REPLICATE_PROPERTY_CHANGE(SkyColor);
					propertyChanged("SkyColor");
				}
			}else{
				if(!skyColor->equals(SkyColor)){
					SkyColor = skyColor;

					REPLICATE_PROPERTY_CHANGE(SkyColor);
					propertyChanged("SkyColor");
				}
			}
		}

		bool Lighting::isSkyTransparent(){
			return SkyTransparent;
		}

		void Lighting::setSkyTransparent(bool skyTransparent){
			if(skyTransparent != SkyTransparent){
				SkyTransparent = skyTransparent;

				REPLICATE_PROPERTY_CHANGE(SkyTransparent);
				propertyChanged("SkyTransparent");
			}
		}

		void Lighting::updateFog(){
#if HAVE_IRRLICHT
			irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
			if(irrDev == NULL){
				return;
			}
			irr::video::IVideoDriver* driver = irrDev->getVideoDriver();
			if(driver == NULL){
				return;
			}

			if(FogEnabled){
				irr::video::SColor irrFogCol;
				if(FogColor != NULL){
					irrFogCol = FogColor->toIrrlichtSColor();
				}else{
					irrFogCol = irr::video::SColor(255, 0, 0, 0);
				}

				driver->setFog(irrFogCol, irr::video::EFT_FOG_LINEAR, FogStart, FogEnd, 0, true, false);
			}else{
				driver->setFog(irr::video::SColor(255, 0, 0, 0), irr::video::EFT_FOG_LINEAR, 0, 0xFFFFFFFF, 0, true, false);
			}
#endif
		}

		bool Lighting::isFogEnabled(){
			return FogEnabled;
		}

		void Lighting::setFogEnabled(bool fogEnabled){
			if(FogEnabled != fogEnabled){
				FogEnabled = fogEnabled;

				REPLICATE_PROPERTY_CHANGE(FogEnabled);
				propertyChanged("FogEnabled");

				updateFog();
			}
		}

		shared_ptr<Type::Color3> Lighting::getFogColor(){
			return FogColor;
		}

		void Lighting::setFogColor(shared_ptr<Type::Color3> fogColor){
			if(fogColor == NULL){
				shared_ptr<Type::Color3> col3 = make_shared<Type::Color3>();
				if(!col3->equals(FogColor)){
					FogColor = col3;

					REPLICATE_PROPERTY_CHANGE(FogColor);
					propertyChanged("FogColor");

					updateFog();
				}
			}else{
				if(!fogColor->equals(FogColor)){
					FogColor = fogColor;

					REPLICATE_PROPERTY_CHANGE(FogColor);
					propertyChanged("FogColor");

					updateFog();
				}
			}
		}

		float Lighting::getFogStart(){
			return FogStart;
		}

		void Lighting::setFogStart(float fogStart){
			if(fogStart != FogStart){
				FogStart = fogStart;

				REPLICATE_PROPERTY_CHANGE(FogStart);
				propertyChanged("FogStart");

				updateFog();
			}
		}

		float Lighting::getFogEnd(){
			return FogEnd;
		}

		void Lighting::setFogEnd(float fogEnd){
			if(fogEnd != FogEnd){
				FogEnd = fogEnd;

				REPLICATE_PROPERTY_CHANGE(FogEnd);
				propertyChanged("FogEnd");

				updateFog();
			}
		}

#if HAVE_ENET
		void Lighting::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);

			peer->sendSetPropertyPacket(netId, "Sky", make_shared<Type::VarWrapper>(Sky));
			peer->sendSetPropertyPacket(netId, "SkyColor", make_shared<Type::VarWrapper>(SkyColor));
			peer->sendSetPropertyPacket(netId, "SkyTransparent", make_shared<Type::VarWrapper>(SkyTransparent));
			peer->sendSetPropertyPacket(netId, "FogEnabled", make_shared<Type::VarWrapper>(FogEnabled));
			peer->sendSetPropertyPacket(netId, "FogColor", make_shared<Type::VarWrapper>(FogColor));
			peer->sendSetPropertyPacket(netId, "FogStart", make_shared<Type::VarWrapper>(FogStart));
			peer->sendSetPropertyPacket(netId, "FogEnd", make_shared<Type::VarWrapper>(FogEnd));
		}
#endif

		std::map<std::string, _PropertyInfo> Lighting::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Sky"] = {"Instance", false, true, true};
			propMap["SkyColor"] = {"Color3", false, true, true};
			propMap["SkyTransparent"] = {"bool", false, true, true};
			propMap["FogEnabled"] = {"bool", false, true, true};
			propMap["FogColor"] = {"Color3", false, true, true};
			propMap["FogStart"] = {"float", false, true, true};
			propMap["FogEnd"] = {"float", false, true, true};

			return propMap;
		}

		void Lighting::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			if(prop == "Sky"){
				shared_ptr<Instance> skyInst = val->asInstance();
				if(!skyInst){
					setSky(NULL);
				}else{
				    setSky(skyInst);
				}
				return;
			}
			if(prop == "SkyTransparent"){
				setSkyTransparent(val->asBool());
				return;
			}
			if(prop == "SkyColor"){
				setSkyColor(val->asColor3());
				return;
			}
			if(prop == "FogEnabled"){
				setFogEnabled(val->asBool());
				return;
			}
			if(prop == "FogColor"){
				setFogColor(val->asColor3());
				return;
			}
			if(prop == "FogStart"){
				setFogStart(val->asFloat());
				return;
			}
			if(prop == "FogEnd"){
				setFogEnd(val->asFloat());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> Lighting::getProperty(std::string prop){
			if(prop == "Sky"){
				return make_shared<Type::VarWrapper>(getSky());
			}
			if(prop == "SkyColor"){
				return make_shared<Type::VarWrapper>(getSkyColor());
			}
			if(prop == "SkyTransparent"){
				return make_shared<Type::VarWrapper>(isSkyTransparent());
			}
			if(prop == "FogEnabled"){
				return make_shared<Type::VarWrapper>(isFogEnabled());
			}
			if(prop == "FogColor"){
				return make_shared<Type::VarWrapper>(getFogColor());
			}
			if(prop == "FogStart"){
				return make_shared<Type::VarWrapper>(getFogStart());
			}
			if(prop == "FogEnd"){
				return make_shared<Type::VarWrapper>(getFogEnd());
			}

			return Instance::getProperty(prop);
		}

		int Lighting::lua_getSky(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					shared_ptr<Instance> sky = instL->getSky();
					if(sky){
						return sky->wrap_lua(L);
					}else{
						lua_pushnil(L);
						return 1;
					}
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Lighting::lua_setSky(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					shared_ptr<Instance> skyInst = checkInstance(L, 2, false);

					if(skyInst == NULL){
						instL->setSky(NULL);
					}else{
						try{
							instL->setSky(skyInst);
						}catch(OBException* ex){
							return luaL_error(L, ex->getMessage().c_str());
						}
					}
				}
			}

			return 0;
		}

		int Lighting::lua_getSkyColor(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					shared_ptr<Type::Color3> col3 = instL->getSkyColor();
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					shared_ptr<Type::Color3> col3 = Type::checkColor3(L, 2, true, true);
					instL->setSkyColor(col3);
				}
			}

			return 0;
		}

		int Lighting::lua_getSkyTransparent(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					lua_pushboolean(L, instL->isSkyTransparent());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Lighting::lua_setSkyTransparent(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					bool newV = lua_toboolean(L, 2);
					instL->setSkyTransparent(newV);
				}
			}

			return 0;
		}

		int Lighting::lua_getFogEnabled(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					lua_pushboolean(L, instL->isFogEnabled());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Lighting::lua_setFogEnabled(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					bool newV = lua_toboolean(L, 2);
					instL->setFogEnabled(newV);
				}
			}

			return 0;
		}

		int Lighting::lua_getFogColor(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					shared_ptr<Type::Color3> col3 = instL->getFogColor();
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

		int Lighting::lua_setFogColor(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					shared_ptr<Type::Color3> col3 = Type::checkColor3(L, 2, true, true);
					instL->setFogColor(col3);
				}
			}

			return 0;
		}

		int Lighting::lua_getFogStart(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					lua_pushnumber(L, instL->getFogStart());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Lighting::lua_setFogStart(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					float newV = luaL_checknumber(L, 2);
					instL->setFogStart(newV);
				}
			}

			return 0;
		}

		int Lighting::lua_getFogEnd(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					lua_pushnumber(L, instL->getFogEnd());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Lighting::lua_setFogEnd(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Lighting> instL = dynamic_pointer_cast<Lighting>(inst);
				if(instL){
					float newV = luaL_checknumber(L, 2);
					instL->setFogEnd(newV);
				}
			}

			return 0;
		}

		void Lighting::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);

			luaL_Reg properties[] = {
			    {"Sky", lua_setSky},
				{"SkyColor", lua_setSkyColor},
				{"SkyTransparent", lua_setSkyTransparent},
				{"FogEnabled", lua_setFogEnabled},
				{"FogColor", lua_setFogColor},
				{"FogStart", lua_setFogStart},
				{"FogEnd", lua_setFogEnd},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Lighting::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);

			luaL_Reg properties[] = {
				{"Sky", lua_getSky},
				{"SkyColor", lua_getSkyColor},
				{"SkyTransparent", lua_getSkyTransparent},
				{"FogEnabled", lua_getFogEnabled},
				{"FogColor", lua_getFogColor},
				{"FogStart", lua_getFogStart},
				{"FogEnd", lua_getFogEnd},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
