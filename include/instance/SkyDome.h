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

#ifndef OB_INST_SKYDOME
#define OB_INST_SKYDOME

namespace OB{
	namespace Instance{
		class SkyDome: public Sky{
			public:
				SkyDome(OBEngine* eng);
				virtual ~SkyDome();

				virtual void setDome(std::string dome);
				std::string getDome();

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

				DECLARE_LUA_METHOD(setDome);
				DECLARE_LUA_METHOD(getDome);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);

				DECLARE_CLASS(SkyDome);

				std::string Dome;

				void updateSkyDome();

				virtual bool assetLoaded(std::string res);

#if HAVE_IRRLICHT

				irr::video::ITexture* dome_tex;

				irr::scene::ISceneNode* irrNode;

#endif
		};
	}
}

#endif // OB_INST_SKYDOME


// Local Variables:
// mode: c++
// End:
