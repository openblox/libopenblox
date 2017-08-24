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

#include "instance/ImageLabel.h"

#include "RenderUtils.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

#include "type/Color3.h"

#if HAVE_IRRLICHT
#include "GL/gl.h"
#endif

namespace OB{
	namespace Instance{
		DEFINE_CLASS(ImageLabel, true, false, GuiLabel){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    ImageLabel::ImageLabel(OBEngine* eng) : GuiLabel(eng){
			Name = ClassName;

			Image = "";
			ImageColor3 = make_shared<Type::Color3>(1, 1, 1);
			ImageTransparency = 1;

			#if HAVE_IRRLICHT
			img = NULL;
			#endif
		}

	    ImageLabel::~ImageLabel(){}

		shared_ptr<Instance> ImageLabel::cloneImpl(){
			return NULL;
		}

		void ImageLabel::updateImage(){
			#if HAVE_IRRLICHT
			shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
			if(assetLoc){
				shared_ptr<AssetResponse> resp = assetLoc->getAsset(Image);
				if(resp){
					irr::io::IReadFile* irf = resp->toIReadFile();
					if(irf){
						irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
						if(irrDev){
							irr::video::IVideoDriver* irrDriv = irrDev->getVideoDriver();
							if(irrDriv){
								img = irrDriv->getTexture(irf);
							}
						}
					}
				}
			}
			#endif
		}

		void ImageLabel::setImage(std::string image){
			if(image != Image){
			    Image = image;

				if(!Image.empty()){
					shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();
					if(assetLoc){
						if(assetLoc->hasAsset(Image)){
							updateImage();
						}else{
							shared_ptr<Instance> sharedThis = std::enable_shared_from_this<OB::Instance::Instance>::shared_from_this();
							assetLoc->addWaitingInstance(sharedThis);
							assetLoc->loadAsset(Image);
						}
					}
				}

				REPLICATE_PROPERTY_CHANGE(Image);
				propertyChanged("Image");
			}
		}

		std::string ImageLabel::getImage(){
			return Image;
		}

		void ImageLabel::setImageColor3(shared_ptr<Type::Color3> imageColor3){
			if(!ImageColor3->equals(imageColor3)){
				if(imageColor3){
				    ImageColor3 = imageColor3;
				}else{
				    ImageColor3 = make_shared<Type::Color3>();
				}

				REPLICATE_PROPERTY_CHANGE(ImageColor3);
				propertyChanged("ImageColor3");
			}
		}

		shared_ptr<Type::Color3> ImageLabel::getImageColor3(){
			return ImageColor3;
		}

		void ImageLabel::setImageTransparency(double imageTransparency){
			if(ImageTransparency != imageTransparency){
				ImageTransparency = imageTransparency;

				REPLICATE_PROPERTY_CHANGE(ImageTransparency);
				propertyChanged("ImageTransparency");
			}
		}

		double ImageLabel::getImageTransparency(){
			return ImageTransparency;
		}

		bool ImageLabel::isLoaded(){
			return img != NULL;
		}

		bool ImageLabel::assetLoaded(std::string res){
			if(res == Image){
				updateImage();
				return true;
			}
			
			return false;
		}

		void ImageLabel::render(){
			#if HAVE_IRRLICHT
			if(Visible){
				if(irr::IrrlichtDevice* irrDev = getEngine()->getIrrlichtDevice()){
					if(irr::video::IVideoDriver* irrDriv = irrDev->getVideoDriver()){
						RenderUtils::prepare2D(getEngine());

						shared_ptr<Type::Color3> bgColor = BackgroundColor3;
						double bgTrans = BackgroundTransparency;
						shared_ptr<Type::Color3> borderColor = BorderColor3;
						int borderSize = BorderSizePixel;
						shared_ptr<Type::Vector2> pos = getAbsolutePosition();
						shared_ptr<Type::Vector2> siz = getAbsoluteSize()->add(pos);

						glColor4d(bgColor->getR(), bgColor->getG(), bgColor->getB(), 1 - bgTrans);

						glRectd(pos->getX(), pos->getY(), siz->getX(), siz->getY());

						glColor4d(borderColor->getR(), borderColor->getB(), borderColor->getB(), 1 - bgTrans);
						//Border Top
						glRectd(pos->getX() - borderSize, pos->getY() - borderSize, siz->getX() + borderSize, pos->getY());
						//Border Left
						glRectd(pos->getX() - borderSize, pos->getY() - borderSize, pos->getX(), siz->getY() + borderSize);
						//Border Right
						glRectd(siz->getX(), pos->getY() - borderSize, siz->getX() + borderSize, siz->getY() + borderSize);
						//Border Bottom
						glRectd(pos->getX() - borderSize, siz->getY(), siz->getX() + borderSize, siz->getY() + borderSize);

						GuiObject::render();
					}
				}
			}
			#endif
		}

		#if HAVE_ENET
		void ImageLabel::replicateProperties(shared_ptr<NetworkReplicator> peer){
		    GuiObject::replicateProperties(peer);
			
			peer->sendSetPropertyPacket(netId, "Image", make_shared<Type::VarWrapper>(Image));
			peer->sendSetPropertyPacket(netId, "ImageColor3", make_shared<Type::VarWrapper>(ImageColor3));
			peer->sendSetPropertyPacket(netId, "ImageTransparency", make_shared<Type::VarWrapper>(ImageTransparency));
		}
		#endif

		std::map<std::string, _PropertyInfo> ImageLabel::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = GuiObject::getProperties();
			propMap["Image"] = {"string", false, true, true};
			propMap["ImageColor3"] = {"Color3", false, true, true};
			propMap["ImageTransparency"] = {"double", false, true, true};
			propMap["Loaded"] = {"bool", true, true, false};

			return propMap;
		}

		void ImageLabel::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Image"){
			    setImage(val->asString());
				return;
			}
			if(prop == "ImageColor3"){
			    setImageColor3(val->asColor3());
				return;
			}
			if(prop == "ImageTransparency"){
			    setImageTransparency(val->asDouble());
				return;
			}

		    GuiObject::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> ImageLabel::getProperty(std::string prop){
			if(prop == "Image"){
				return make_shared<Type::VarWrapper>(getImage());
			}
			if(prop == "ImageColor3"){
				return make_shared<Type::VarWrapper>(getImage());
			}
			if(prop == "ImageTransparency"){
				return make_shared<Type::VarWrapper>(getImage());
			}
			
			return GuiObject::getProperty(prop);
		}

		int ImageLabel::lua_setImage(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<ImageLabel> instIL = dynamic_pointer_cast<ImageLabel>(inst);
				if(instIL){
					std::string newV = std::string(luaL_checkstring(L, 2));
					instIL->setImage(newV);
				}
			}
			
			return 0;
		}

		int ImageLabel::lua_getImage(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<ImageLabel> instIL = dynamic_pointer_cast<ImageLabel>(inst);
				if(instIL){
				    lua_pushstring(L, instIL->getImage().c_str());
					return 1;
				}
			}
		    
			lua_pushnil(L);
			return 1;
		}

		int ImageLabel::lua_getImageColor3(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<ImageLabel> instIL = dynamic_pointer_cast<ImageLabel>(inst);
				if(instIL){
				    shared_ptr<Type::Color3> col3 = instIL->getImageColor3();
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

		int ImageLabel::lua_setImageColor3(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<ImageLabel> instIL = dynamic_pointer_cast<ImageLabel>(inst);
				if(instIL){
					shared_ptr<Type::Color3> col3 = Type::checkColor3(L, 2, true, true);
					instIL->setImageColor3(col3);
					return 1;
				}
			}
			
			return 0;
		}

		int ImageLabel::lua_getImageTransparency(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<ImageLabel> instIL = dynamic_pointer_cast<ImageLabel>(inst);
				if(instIL){
					lua_pushnumber(L, instIL->getImageTransparency());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		int ImageLabel::lua_setImageTransparency(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<ImageLabel> instIL = dynamic_pointer_cast<ImageLabel>(inst);
				if(instIL){
					float newV = luaL_checknumber(L, 2);
					instIL->setImageTransparency(newV);
				}
			}
			
			return 0;
		}

		int ImageLabel::lua_getLoaded(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<ImageLabel> instIL = dynamic_pointer_cast<ImageLabel>(inst);
				if(instIL){
					lua_pushboolean(L, instIL->isLoaded());
					return 1;
				}
			}
			
			lua_pushnil(L);
			return 1;
		}

		void ImageLabel::register_lua_property_setters(lua_State* L){
		    GuiObject::register_lua_property_setters(L);
			
			luaL_Reg properties[] = {
				{"Image", lua_setImage},
				{"ImageColor3", lua_setImageColor3},
				{"ImageTransparency", lua_setImageTransparency},
				{"Loaded", Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void ImageLabel::register_lua_property_getters(lua_State* L){
		    GuiObject::register_lua_property_getters(L);
			
		    luaL_Reg properties[] = {
				{"Image", lua_getImage},
				{"ImageColor3", lua_getImageColor3},
				{"ImageTransparency", lua_getImageTransparency},
				{"Loaded", lua_getLoaded},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
