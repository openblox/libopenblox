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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "instance/Part.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"
#include "instance/Lighting.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Part, true, false, BasePart){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Part::Part(OBEngine* eng) : BasePart(eng){
			Name = ClassName;

			Size = make_shared<Type::Vector3>(1, 1, 1);
		}

		Part::~Part(){}

		shared_ptr<Instance> Part::cloneImpl(){
			shared_ptr<Part> p = make_shared<Part>(eng);
			p->Archivable = Archivable;
			p->Name = Name;
			p->ParentLocked = ParentLocked;

			p->Anchored = Anchored;
			p->Color = Color;
			p->CanCollide = CanCollide;
			p->Locked = Locked;
			p->Transparency = Transparency;

			p->Size = Size;

			return p;
		}

		void Part::setSize(shared_ptr<Type::Vector3> size){
			if(size == NULL){
				shared_ptr<Type::Vector3> vec3 = make_shared<Type::Vector3>(0, 0, 0);
				if(!vec3->equals(Size)){
					Size = vec3;

					updateSize();
					REPLICATE_PROPERTY_CHANGE(Size);
					propertyChanged("Size");
				}
			}else{
				if(!size->equals(Size)){
					Size = size;

					updateSize();
					REPLICATE_PROPERTY_CHANGE(Size);
					propertyChanged("Size");
				}
			}
		}

		shared_ptr<Type::Vector3> Part::getSize(){
			return Size;
		}

		void Part::updateSize(){
#if HAVE_IRRLICHT
			if(irrNode){
				shared_ptr<Type::Vector3> size = getSize();

				if(size){
					if(irrNode){
						irrNode->setScale(size->toIrrlichtVector3df());
					}
				}
			}
#endif
		}

		void Part::updateColor(){
#if HAVE_IRRLICHT
			if(irrNode){
				irr::scene::IMeshSceneNode* mnode = (irr::scene::IMeshSceneNode*)irrNode;
				irr::scene::IMesh* tMesh = mnode->getMesh();

				shared_ptr<Type::Color3> col3 = getColor();
				irr::video::SColor irrCol = col3->toIrrlichtSColor(255);

				irr::video::SMaterial& thisMat = irrNode->getMaterial(0);
				thisMat.EmissiveColor = irrCol;
				thisMat.DiffuseColor.set(0, 0, 0, 0);
				thisMat.AmbientColor.set(0, 0, 0, 0);
				thisMat.ColorMaterial = irr::video::ECM_NONE;
			}
#endif
		}

#if HAVE_ENET
		void Part::replicateProperties(shared_ptr<NetworkReplicator> peer){
			BasePart::replicateProperties(peer);

			peer->sendSetPropertyPacket(netId, "Size", make_shared<Type::VarWrapper>(Size));
		}
#endif

#if HAVE_IRRLICHT
		void Part::newIrrlichtNode(){
			irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
			if(irrDev){
				irr::scene::ISceneManager* smgr = irrDev->getSceneManager();
				if(smgr){
					irrNode = smgr->addCubeSceneNode(1);

					if(irrNode){
						irr::scene::IMeshSceneNode* mirrNode = (irr::scene::IMeshSceneNode*)irrNode;
						mirrNode->setMaterialFlag(irr::video::EMF_LIGHTING, true);

						shared_ptr<DataModel> dm = eng->getDataModel();
						if (dm) {
							shared_ptr<Lighting>lighting = dm->getLighting();
							if (lighting) {
								mirrNode->setMaterialFlag(irr::video::EMF_FOG_ENABLE, lighting->isFogEnabled());
							}
						}

						updateColor();
						updatePosition();
						updateRotation();
						updateSize();
					}
				}
			}
		}
#endif

		std::map<std::string, _PropertyInfo> Part::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = BasePart::getProperties();
			propMap["Size"] = {"Vector3", false, true, true};

			return propMap;
		}

		void Part::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			if(prop == "Size"){
				setSize(val->asVector3());
				return;
			}

			BasePart::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> Part::getProperty(std::string prop){
			if(prop == "Size"){
				return make_shared<Type::VarWrapper>(getSize());
			}

			return BasePart::getProperty(prop);
		}

		int Part::lua_setSize(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Part> instP = dynamic_pointer_cast<Part>(inst);
				if(instP){
					shared_ptr<Type::Vector3> vec3 = Type::checkVector3(L, 2, true, true);
					instP->setSize(vec3);
				}
			}

			return 0;
		}

		int Part::lua_getSize(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Part> instP = dynamic_pointer_cast<Part>(inst);
				if(instP){
					shared_ptr<Type::Vector3> vec3 = instP->getSize();
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

		void Part::register_lua_property_setters(lua_State* L){
			BasePart::register_lua_property_setters(L);

			luaL_Reg properties[] = {
				{"Size", lua_setSize},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Part::register_lua_property_getters(lua_State* L){
			BasePart::register_lua_property_getters(L);

			luaL_Reg properties[] = {
				{"Size", lua_getSize},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
