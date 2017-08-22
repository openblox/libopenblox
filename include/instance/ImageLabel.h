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

#include "instance/GuiLabel.h"

#ifndef OB_INST_IMAGELABEL
#define OB_INST_IMAGELABEL

namespace OB{
	namespace Instance{
		class ImageLabel: public GuiLabel{
			public:
			    ImageLabel(OBEngine* eng);
				virtual ~ImageLabel();

				void updateImage();

				void setImage(std::string image);
				std::string getImage();

				void setImageColor3(shared_ptr<Type::Color3> imageColor3);
				shared_ptr<Type::Color3> getImageColor3();

			    void setImageTransparency(double imageTransparency);
				double getImageTransparency();

				bool isLoaded();

				virtual bool assetLoaded(std::string res);

				virtual void render();

				#if HAVE_ENET
				/**
				 * Replicates properties of this Instance.
				 * 
				 * @param peer Peer
				 * @author John M. Harris, Jr.
				 */
				virtual void replicateProperties(shared_ptr<NetworkReplicator> peer);
				#endif

				virtual std::map<std::string, _PropertyInfo> getProperties();
				virtual shared_ptr<Type::VarWrapper> getProperty(std::string prop);
				virtual void setProperty(std::string prop, shared_ptr<Type::VarWrapper> val);

				DECLARE_LUA_METHOD(setImage);
				DECLARE_LUA_METHOD(getImage);
				DECLARE_LUA_METHOD(setImageColor3);
				DECLARE_LUA_METHOD(getImageColor3);
				DECLARE_LUA_METHOD(setImageTransparency);
				DECLARE_LUA_METHOD(getImageTransparency);
				DECLARE_LUA_METHOD(getLoaded);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				
				DECLARE_CLASS(ImageLabel);

				#if HAVE_IRRLICHT
				irr::video::ITexture* img;
				#endif

				std::string Image;
				shared_ptr<Type::Color3> ImageColor3;
				double ImageTransparency;
				
		};
	}
}

#endif // OB_INST_IMAGELABEL


// Local Variables:
// mode: c++
// End:
