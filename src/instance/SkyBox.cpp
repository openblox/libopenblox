/*
 * Copyright (C) 2018 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "instance/SkyBox.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#endif

namespace OB{
	namespace Instance{
		DEFINE_CLASS(SkyBox, true, false, Sky){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		SkyBox::SkyBox(OBEngine* eng) : Sky(eng){
			Name = ClassName;

			skyActive = false;

			Top = "";
			Bottom = "";
			Left = "";
			Right = "";
			Front = "";
			Back = "";

#if HAVE_IRRLICHT
			skybox_needs_updating = false;

			irrNode = NULL;

			top_tex = NULL;
			bottom_tex = NULL;
			left_tex = NULL;
			right_tex = NULL;
			front_tex = NULL;
			back_tex = NULL;

			top_loading = false;
			bottom_loading = false;
			left_loading = false;
			right_loading = false;
			front_loading = false;
			back_loading = false;
#endif
		}

	    SkyBox::~SkyBox(){}

		shared_ptr<Instance> SkyBox::cloneImpl(){
		    shared_ptr<SkyBox> sb = make_shared<SkyBox>(eng);
			sb->Archivable = Archivable;
			sb->Name = Name;
			sb->ParentLocked = ParentLocked;

			sb->Top = Top;
			sb->Bottom = Bottom;
			sb->Left = Left;
			sb->Right = Right;
			sb->Front = Front;
			sb->Back = Back;

			sb->skybox_needs_updating = true;

			return sb;
		}

		void SkyBox::setTop(std::string top){
			if(top != Top){
				Top = top;

#if HAVE_IRRLICHT
				if(!Top.empty()){
					shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
					if(assetLoc){
						if(assetLoc->hasAsset(Top)){
							top_tex = NULL;
							top_loading = false;

						    skybox_needs_updating = true;
							updateSkyBox();
						}else{
							top_tex = NULL;

							top_loading = true;

							shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
							assetLoc->addWaitingInstance(sharedThis);
							assetLoc->loadAsset(Top);
						}
					}
				}else{
					top_tex = NULL;

					updateSkyBox();
				}
#endif

				REPLICATE_PROPERTY_CHANGE(Top);
				propertyChanged("Top");
			}
		}

		std::string SkyBox::getTop(){
			return Top;
		}

		void SkyBox::setBottom(std::string bottom){
			if(bottom != Bottom){
				Bottom = bottom;

#if HAVE_IRRLICHT
				if(!Bottom.empty()){
					shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
					if(assetLoc){
						if(assetLoc->hasAsset(Bottom)){
						    bottom_tex = NULL;
							bottom_loading = false;

						    skybox_needs_updating = true;
							updateSkyBox();
						}else{
						    bottom_tex = NULL;

							bottom_loading = true;

							shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
							assetLoc->addWaitingInstance(sharedThis);
							assetLoc->loadAsset(Bottom);
						}
					}
				}else{
				    bottom_tex = NULL;

					updateSkyBox();
				}
#endif

				REPLICATE_PROPERTY_CHANGE(Bottom);
				propertyChanged("Bottom");
			}
		}

		std::string SkyBox::getBottom(){
			return Bottom;
		}

		void SkyBox::setLeft(std::string left){
			if(left != Left){
				Left = left;

#if HAVE_IRRLICHT
				if(!Left.empty()){
					shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
					if(assetLoc){
						if(assetLoc->hasAsset(Left)){
						    left_tex = NULL;
							left_loading = false;

						    skybox_needs_updating = true;
							updateSkyBox();
						}else{
							left_tex = NULL;

							left_loading = true;

							shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
							assetLoc->addWaitingInstance(sharedThis);
							assetLoc->loadAsset(Left);
						}
					}
				}else{
				    left_tex = NULL;

					updateSkyBox();
				}
#endif

				REPLICATE_PROPERTY_CHANGE(Left);
				propertyChanged("Left");
			}
		}

		std::string SkyBox::getLeft(){
			return Left;
		}

	    void SkyBox::setRight(std::string right){
			if(right != Right){
				Right = right;

#if HAVE_IRRLICHT
				if(!Right.empty()){
					shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
					if(assetLoc){
						if(assetLoc->hasAsset(Right)){
						    right_tex = NULL;
							right_loading = false;

						    skybox_needs_updating = true;
							updateSkyBox();
						}else{
							right_tex = NULL;

							right_loading = true;

							shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
							assetLoc->addWaitingInstance(sharedThis);
							assetLoc->loadAsset(Right);
						}
					}
				}else{
				    right_tex = NULL;

					updateSkyBox();
				}
#endif

				REPLICATE_PROPERTY_CHANGE(Right);
				propertyChanged("Right");
			}
		}

		std::string SkyBox::getRight(){
			return Right;
		}

	    void SkyBox::setFront(std::string front){
			if(front != Front){
				Front = front;

#if HAVE_IRRLICHT
				if(!Front.empty()){
					shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
					if(assetLoc){
						if(assetLoc->hasAsset(Front)){
						    front_tex = NULL;
							front_loading = false;

						    skybox_needs_updating = true;
							updateSkyBox();
						}else{
							front_tex = NULL;

							front_loading = true;

							shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
							assetLoc->addWaitingInstance(sharedThis);
							assetLoc->loadAsset(Front);
						}
					}
				}else{
				    front_tex = NULL;

					updateSkyBox();
				}
#endif

				REPLICATE_PROPERTY_CHANGE(Front);
				propertyChanged("Front");
			}
		}

		std::string SkyBox::getFront(){
			return Front;
		}

		void SkyBox::setBack(std::string back){
			if(back != Back){
			    Back = back;

#if HAVE_IRRLICHT
				if(!Back.empty()){
					shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
					if(assetLoc){
						if(assetLoc->hasAsset(Back)){
						    back_tex = NULL;
							back_loading = false;

						    skybox_needs_updating = true;
							updateSkyBox();
						}else{
							back_tex = NULL;

							back_loading = true;

							shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
							assetLoc->addWaitingInstance(sharedThis);
							assetLoc->loadAsset(Back);
						}
					}
				}else{
				    back_tex = NULL;

					updateSkyBox();
				}
#endif

				REPLICATE_PROPERTY_CHANGE(Back);
				propertyChanged("Back");
			}
		}

		std::string SkyBox::getBack(){
			return Back;
		}

		void SkyBox::activateSky(){
			Sky::activateSky();

#if HAVE_IRRLICHT
			updateSkyBox();
#endif
		}

		void SkyBox::deactivateSky(){
			Sky::deactivateSky();

#if HAVE_IRRLICHT
			updateSkyBox();
#endif
		}

		void SkyBox::preRender(){
#if HAVE_IRRLICHT
			if(skybox_needs_updating){
				skybox_needs_updating = false;

				bool didLoadTexture = false;

				shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
				if(assetLoc){
					irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
					if(irrDev){
						irr::video::IVideoDriver* videoDriver = irrDev->getVideoDriver();
						if(videoDriver){
							if(!top_tex){
								if(!Top.empty()){
									shared_ptr<AssetResponse> resp = assetLoc->getAsset(Top);
									if(resp){
										irr::io::IReadFile* irf = resp->toIReadFile();
										if(irf){
											top_tex = videoDriver->getTexture(irf);
											didLoadTexture = true;
										}
									}
								}
							}

							if(!bottom_tex){
								if(!Bottom.empty()){
									shared_ptr<AssetResponse> resp = assetLoc->getAsset(Bottom);
									if(resp){
										irr::io::IReadFile* irf = resp->toIReadFile();
										if(irf){
											bottom_tex = videoDriver->getTexture(irf);
											didLoadTexture = true;
										}
									}
								}
							}

							if(!left_tex){
								if(!Left.empty()){
									shared_ptr<AssetResponse> resp = assetLoc->getAsset(Left);
									if(resp){
										irr::io::IReadFile* irf = resp->toIReadFile();
										if(irf){
											left_tex = videoDriver->getTexture(irf);
											didLoadTexture = true;
										}
									}
								}
							}

							if(!right_tex){
								if(!Right.empty()){
									shared_ptr<AssetResponse> resp = assetLoc->getAsset(Right);
									if(resp){
										irr::io::IReadFile* irf = resp->toIReadFile();
										if(irf){
											right_tex = videoDriver->getTexture(irf);
											didLoadTexture = true;
										}
									}
								}
							}

							if(!front_tex){
								if(!Front.empty()){
									shared_ptr<AssetResponse> resp = assetLoc->getAsset(Front);
									if(resp){
										irr::io::IReadFile* irf = resp->toIReadFile();
										if(irf){
											front_tex = videoDriver->getTexture(irf);
											didLoadTexture = true;
										}
									}
								}
							}

							if(!back_tex){
								if(!Back.empty()){
									shared_ptr<AssetResponse> resp = assetLoc->getAsset(Back);
									if(resp){
										irr::io::IReadFile* irf = resp->toIReadFile();
										if(irf){
											back_tex = videoDriver->getTexture(irf);
											didLoadTexture = true;
										}
									}
								}
							}
						}
					}
				}

				if(didLoadTexture){
					updateSkyBox();
				}
			}
			#endif
		}

#if HAVE_IRRLICHT
		void SkyBox::updateSkyBox(){
			if(skyActive){
				if(irrNode){
					irrNode->remove();
					irrNode = NULL;
				}

			    irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
				if(irrDev){
					irr::scene::ISceneManager* smgr = irrDev->getSceneManager();
					if(smgr){
						irrNode = smgr->addSkyBoxSceneNode(top_tex, bottom_tex, left_tex, right_tex, front_tex, back_tex);
					}
				}
			}else{
				if(irrNode){
					irrNode->remove();
					irrNode = NULL;
				}
			}
		}

		bool SkyBox::assetLoaded(std::string res){
			bool assetsChanged = false;

			if(top_loading && res == Top){
				top_tex = NULL;
				top_loading = false;

				assetsChanged = true;
			}
			if(bottom_loading && res == Bottom){
			    bottom_tex = NULL;
			    bottom_loading = false;

				assetsChanged = true;
			}
			if(left_loading && res == Left){
			    left_tex = NULL;
			    left_loading = false;

				assetsChanged = true;
			}
			if(right_loading && res == Right){
			    right_tex = NULL;
			    right_loading = false;

				assetsChanged = true;
			}
			if(front_loading && res == Front){
			    front_tex = NULL;
				front_loading = false;

				assetsChanged = true;
			}
			if(back_loading && res == Back){
				back_tex = NULL;
			    back_loading = false;

				assetsChanged = true;
			}


			if(assetsChanged){
			    skybox_needs_updating = true;
				updateSkyBox();
			}

			return (!top_loading && !bottom_loading && !left_loading && !right_loading && !front_loading && !back_loading);
		}
#endif

#if HAVE_ENET
		void SkyBox::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);

			peer->sendSetPropertyPacket(netId, "Top", make_shared<Type::VarWrapper>(Top));
			peer->sendSetPropertyPacket(netId, "Bottom", make_shared<Type::VarWrapper>(Bottom));
			peer->sendSetPropertyPacket(netId, "Left", make_shared<Type::VarWrapper>(Left));
			peer->sendSetPropertyPacket(netId, "Right", make_shared<Type::VarWrapper>(Right));
			peer->sendSetPropertyPacket(netId, "Front", make_shared<Type::VarWrapper>(Front));
			peer->sendSetPropertyPacket(netId, "Back", make_shared<Type::VarWrapper>(Back));
		}
#endif

		std::map<std::string, _PropertyInfo> SkyBox::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Top"] = {"string", false, true, true};
			propMap["Bottom"] = {"string", false, true, true};
			propMap["Left"] = {"string", false, true, true};
			propMap["Right"] = {"string", false, true, true};
			propMap["Front"] = {"string", false, true, true};
			propMap["Back"] = {"string", false, true, true};

			return propMap;
		}

		void SkyBox::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			if(prop == "Top"){
				setTop(val->asString());
				return;
			}
			if(prop == "Bottom"){
				setBottom(val->asString());
				return;
			}
			if(prop == "Left"){
				setLeft(val->asString());
				return;
			}
			if(prop == "Right"){
				setRight(val->asString());
				return;
			}
			if(prop == "Front"){
				setFront(val->asString());
				return;
			}
			if(prop == "Back"){
				setBack(val->asString());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> SkyBox::getProperty(std::string prop){
			if(prop == "Top"){
				return make_shared<Type::VarWrapper>(getTop());
			}
			if(prop == "Bottom"){
				return make_shared<Type::VarWrapper>(getBottom());
			}
			if(prop == "Left"){
				return make_shared<Type::VarWrapper>(getLeft());
			}
			if(prop == "Right"){
				return make_shared<Type::VarWrapper>(getRight());
			}
			if(prop == "Front"){
				return make_shared<Type::VarWrapper>(getFront());
			}
			if(prop == "Back"){
				return make_shared<Type::VarWrapper>(getBack());
			}

			return Instance::getProperty(prop);
		}

		int SkyBox::lua_setTop(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					std::string newV = std::string(luaL_checkstring(L, 2));
					instSB->setTop(newV);
				}
			}

			return 0;
		}

		int SkyBox::lua_getTop(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					lua_pushstring(L, instSB->getTop().c_str());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int SkyBox::lua_setBottom(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					std::string newV = std::string(luaL_checkstring(L, 2));
					instSB->setBottom(newV);
				}
			}

			return 0;
		}

		int SkyBox::lua_getBottom(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					lua_pushstring(L, instSB->getBottom().c_str());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int SkyBox::lua_setLeft(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					std::string newV = std::string(luaL_checkstring(L, 2));
					instSB->setLeft(newV);
				}
			}

			return 0;
		}

		int SkyBox::lua_getLeft(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					lua_pushstring(L, instSB->getLeft().c_str());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int SkyBox::lua_setRight(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					std::string newV = std::string(luaL_checkstring(L, 2));
					instSB->setRight(newV);
				}
			}

			return 0;
		}

		int SkyBox::lua_getRight(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					lua_pushstring(L, instSB->getRight().c_str());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int SkyBox::lua_setFront(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					std::string newV = std::string(luaL_checkstring(L, 2));
					instSB->setFront(newV);
				}
			}

			return 0;
		}

		int SkyBox::lua_getFront(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					lua_pushstring(L, instSB->getFront().c_str());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int SkyBox::lua_setBack(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					std::string newV = std::string(luaL_checkstring(L, 2));
					instSB->setBack(newV);
				}
			}

			return 0;
		}

		int SkyBox::lua_getBack(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyBox> instSB = dynamic_pointer_cast<SkyBox>(inst);
				if(instSB){
					lua_pushstring(L, instSB->getBack().c_str());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		void SkyBox::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);

			luaL_Reg properties[] = {
                {"Top", lua_setTop},
				{"Bottom", lua_setBottom},
				{"Left", lua_setLeft},
				{"Right", lua_setRight},
				{"Front", lua_setFront},
				{"Back", lua_setBack},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void SkyBox::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);

			luaL_Reg properties[] = {
                {"Top", lua_getTop},
				{"Bottom", lua_getBottom},
				{"Left", lua_getLeft},
				{"Right", lua_getRight},
				{"Front", lua_getFront},
				{"Back", lua_getBack},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
