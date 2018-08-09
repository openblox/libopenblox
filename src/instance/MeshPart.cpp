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

#include "instance/MeshPart.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"
#include "instance/Lighting.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(MeshPart, true, false, BasePart){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		MeshPart::MeshPart(OBEngine* eng) : BasePart(eng){
			Name = ClassName;

			Mesh = "";
		}

		MeshPart::~MeshPart(){}

		shared_ptr<Instance> MeshPart::cloneImpl(){
			shared_ptr<MeshPart> mp = make_shared<MeshPart>(eng);
			mp->Archivable = Archivable;
			mp->Name = Name;
			mp->ParentLocked = ParentLocked;

			mp->Anchored = Anchored;
			mp->Color = Color;
			mp->CanCollide = CanCollide;
			mp->Locked = Locked;
			mp->Transparency = Transparency;

			mp->Mesh = Mesh;
			mp->updateMesh();
			shared_ptr<Instance> parInst = Parent;
			if(parInst){
				if(shared_ptr<PVInstance> oInst = dynamic_pointer_cast<PVInstance>(parInst)){
					irr::scene::ISceneNode* parIrrNode = oInst->getIrrNode();
					if(parIrrNode){
						parIrrNode->addChild(mp->irrNode);
					}
				}
			}

			return mp;
		}

		void MeshPart::setMesh(std::string mesh){
			if(mesh != Mesh){
				Mesh = mesh;

				if(!Mesh.empty()){
					shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
					if(assetLoc){
						if(assetLoc->hasAsset(Mesh)){
							updateMesh();
							shared_ptr<Instance> parInst = Parent;
							if(parInst){
								if(shared_ptr<PVInstance> oInst = dynamic_pointer_cast<PVInstance>(parInst)){
									irr::scene::ISceneNode* parIrrNode = oInst->getIrrNode();
									if(parIrrNode){
										parIrrNode->addChild(irrNode);
									}
								}
							}
						}else{
							shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
							assetLoc->addWaitingInstance(sharedThis);
							assetLoc->loadAsset(Mesh);
						}
					}
				}

				REPLICATE_PROPERTY_CHANGE(Mesh);
				propertyChanged("Mesh");
			}
		}

		std::string MeshPart::getMesh(){
			return Mesh;
		}

		void MeshPart::updateMesh(){
#if HAVE_IRRLICHT
			shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
			if(assetLoc){
				shared_ptr<AssetResponse> resp = assetLoc->getAsset(Mesh);
				if(resp){
					irr::io::IReadFile* irf = resp->toIReadFile();
					if(irf){
						irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
						if(irrDev){
							irr::scene::ISceneManager* smgr = irrDev->getSceneManager();
							if(smgr){
								irr::scene::IAnimatedMesh* realMesh = smgr->getMesh(irf);
								if(realMesh){
									irr::scene::ISceneNode* oldNode = irrNode;
									if(oldNode){
										oldNode->remove();
									}

									irrNode = smgr->addMeshSceneNode(realMesh);
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
									}
								}
							}
						}
					}
				}
			}
#endif
		}

		void MeshPart::updateColor(){
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

		bool MeshPart::assetLoaded(std::string res){
			if(Mesh.empty()){
				// It's entirely possible that the Mesh property has
				// been set to "" since we started waiting.
				return true;
			}
			if(res == Mesh){
				updateMesh();
				shared_ptr<Instance> parInst = Parent;
				if(parInst){
					if(shared_ptr<PVInstance> oInst = dynamic_pointer_cast<PVInstance>(parInst)){
						irr::scene::ISceneNode* parIrrNode = oInst->getIrrNode();
						if(parIrrNode){
							parIrrNode->addChild(irrNode);
						}
					}
				}
				return true;
			}

			return false;
		}

#if HAVE_ENET
		void MeshPart::replicateProperties(shared_ptr<NetworkReplicator> peer){
			BasePart::replicateProperties(peer);

			peer->sendSetPropertyPacket(netId, "Mesh", make_shared<Type::VarWrapper>(Mesh));
		}
#endif

#if HAVE_IRRLICHT
		void MeshPart::newIrrlichtNode(){
			updateMesh();
		}
#endif

		std::map<std::string, _PropertyInfo> MeshPart::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = BasePart::getProperties();
			propMap["Mesh"] = {"string", false, true, true};

			return propMap;
		}

		void MeshPart::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			if(prop == "Mesh"){
				setMesh(val->asString());
				return;
			}

			BasePart::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> MeshPart::getProperty(std::string prop){
			if(prop == "Mesh"){
				return make_shared<Type::VarWrapper>(getMesh());
			}

			return BasePart::getProperty(prop);
		}

		int MeshPart::lua_setMesh(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<MeshPart> instMP = dynamic_pointer_cast<MeshPart>(inst);
				if(instMP){
					std::string newV = std::string(luaL_checkstring(L, 2));
					instMP->setMesh(newV);
				}
			}

			return 0;
		}

		int MeshPart::lua_getMesh(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<MeshPart> instMP = dynamic_pointer_cast<MeshPart>(inst);
				if(instMP){
					lua_pushstring(L, instMP->getMesh().c_str());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		void MeshPart::register_lua_property_setters(lua_State* L){
			BasePart::register_lua_property_setters(L);

			luaL_Reg properties[] = {
				{"Mesh", lua_setMesh},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void MeshPart::register_lua_property_getters(lua_State* L){
			BasePart::register_lua_property_getters(L);

			luaL_Reg properties[] = {
				{"Mesh", lua_getMesh},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
