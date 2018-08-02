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

#include "instance/SkyDome.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#endif

namespace OB{
	namespace Instance{
		DEFINE_CLASS(SkyDome, true, false, Sky){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		SkyDome::SkyDome(OBEngine* eng) : Sky(eng){
			Name = ClassName;

			skyActive = false;

		    Dome = "";

#if HAVE_IRRLICHT
			skydome_needs_updating = false;

			irrNode = NULL;

		    dome_tex = NULL;
#endif
		}

	    SkyDome::~SkyDome(){}

		shared_ptr<Instance> SkyDome::cloneImpl(){
		    shared_ptr<SkyDome> sd = make_shared<SkyDome>(eng);
			sd->Archivable = Archivable;
			sd->Name = Name;
			sd->ParentLocked = ParentLocked;

			sd->Dome = Dome;

			sd->skydome_needs_updating = true;

			return sd;
		}

		void SkyDome::setDome(std::string dome){
			if(dome != Dome){
			    Dome = dome;

#if HAVE_IRRLICHT

				if(!Dome.empty()){
					shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
					if(assetLoc){
						if(assetLoc->hasAsset(Dome)){
						    dome_tex = NULL;

						    skydome_needs_updating = true;
						}else{
							dome_tex = NULL;

							shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
							assetLoc->addWaitingInstance(sharedThis);
							assetLoc->loadAsset(Dome);
						}
					}
				}else{
				    dome_tex = NULL;

				    updateSkyDome();
				}

#endif

				REPLICATE_PROPERTY_CHANGE(Dome);
				propertyChanged("Dome");
			}
		}

		std::string SkyDome::getDome(){
			return Dome;
		}

		void SkyDome::activateSky(){
			Sky::activateSky();

#if HAVE_IRRLICHT
			updateSkyDome();
#endif
		}

		void SkyDome::deactivateSky(){
			Sky::deactivateSky();

#if HAVE_IRRLICHT
			updateSkyDome();
#endif
		}

		void SkyDome::preRender(){
#if HAVE_IRRLICHT
			if(skydome_needs_updating){
				skydome_needs_updating = false;

				shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
				if(assetLoc){
					irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
					if(irrDev){
						irr::video::IVideoDriver* videoDriver = irrDev->getVideoDriver();
						if(videoDriver){
							if(!dome_tex){
								shared_ptr<AssetResponse> resp = assetLoc->getAsset(Dome);
								if(resp){
									irr::io::IReadFile* irf = resp->toIReadFile();
									if(irf){
										dome_tex = videoDriver->getTexture(irf);

										updateSkyDome();
									}
								}
							}
						}
					}
				}
			}
#endif
		}

#if HAVE_IRRLICHT
		void SkyDome::updateSkyDome(){
			if(skyActive){
				if(irrNode){
					irrNode->remove();
					irrNode = NULL;
				}

			    irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
				if(irrDev){
					if(dome_tex){
						irr::scene::ISceneManager* smgr = irrDev->getSceneManager();
						if(smgr){
							irrNode = smgr->addSkyDomeSceneNode(dome_tex);
						}
					}
				}
			}else{
				if(irrNode){
					irrNode->remove();
					irrNode = NULL;
				}
			}
		}

		bool SkyDome::assetLoaded(std::string res){
			if(Dome.empty()){
				return true;
			}


			if(res == Dome){
				dome_tex = NULL;

			    skydome_needs_updating = true;

			    return true;
			}
		}
#endif

#if HAVE_ENET
		void SkyDome::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);

			peer->sendSetPropertyPacket(netId, "Dome", make_shared<Type::VarWrapper>(Dome));
		}
#endif

		std::map<std::string, _PropertyInfo> SkyDome::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Dome"] = {"string", false, true, true};

			return propMap;
		}

		void SkyDome::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			if(prop == "Dome"){
				setDome(val->asString());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> SkyDome::getProperty(std::string prop){
			if(prop == "Dome"){
				return make_shared<Type::VarWrapper>(getDome());
			}

			return Instance::getProperty(prop);
		}

		int SkyDome::lua_setDome(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyDome> instSD = dynamic_pointer_cast<SkyDome>(inst);
				if(instSD){
					std::string newV = std::string(luaL_checkstring(L, 2));
					instSD->setDome(newV);
				}
			}

			return 0;
		}

		int SkyDome::lua_getDome(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<SkyDome> instSD = dynamic_pointer_cast<SkyDome>(inst);
				if(instSD){
					lua_pushstring(L, instSD->getDome().c_str());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		void SkyDome::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);

			luaL_Reg properties[] = {
                {"Dome", lua_setDome},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void SkyDome::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);

			luaL_Reg properties[] = {
                {"Dome", lua_getDome},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
