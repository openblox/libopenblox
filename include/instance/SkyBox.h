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

#include "instance/Sky.h"

#ifndef OB_INST_SKYBOX
#define OB_INST_SKYBOX

namespace OB{
	namespace Instance{
		class SkyBox: public Sky{
			public:
				SkyBox(OBEngine* eng);
				virtual ~SkyBox();

				virtual void setTop(std::string top);
				std::string getTop();

				virtual void setBottom(std::string bottom);
				std::string getBottom();

				virtual void setLeft(std::string top);
				std::string getLeft();

				virtual void setRight(std::string top);
				std::string getRight();

				virtual void setFront(std::string top);
				std::string getFront();

				virtual void setBack(std::string top);
				std::string getBack();

				virtual void activateSky();
				virtual void deactivateSky();

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

				DECLARE_LUA_METHOD(setTop);
				DECLARE_LUA_METHOD(getTop);

				DECLARE_LUA_METHOD(setBottom);
				DECLARE_LUA_METHOD(getBottom);

				DECLARE_LUA_METHOD(setLeft);
				DECLARE_LUA_METHOD(getLeft);

				DECLARE_LUA_METHOD(setRight);
				DECLARE_LUA_METHOD(getRight);

				DECLARE_LUA_METHOD(setFront);
				DECLARE_LUA_METHOD(getFront);

				DECLARE_LUA_METHOD(setBack);
				DECLARE_LUA_METHOD(getBack);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);

				DECLARE_CLASS(SkyBox);

				std::string Top;
				std::string Bottom;
				std::string Left;
				std::string Right;
				std::string Front;
				std::string Back;

				void updateSkyBoxTextures();
				void updateSkyBox();

				virtual bool assetLoaded(std::string res);

#if HAVE_IRRLICHT

				irr::video::ITexture* top_tex;
				irr::video::ITexture* bottom_tex;
				irr::video::ITexture* left_tex;
				irr::video::ITexture* right_tex;
				irr::video::ITexture* front_tex;
				irr::video::ITexture* back_tex;

				bool top_loading;
				bool bottom_loading;
				bool left_loading;
				bool right_loading;
				bool front_loading;
				bool back_loading;

				irr::scene::ISceneNode* irrNode;

#endif
		};
	}
}

#endif // OB_INST_SKYBOX


// Local Variables:
// mode: c++
// End:
