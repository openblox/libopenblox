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

#include "instance/PVInstance.h"

#include "oblibconfig.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#endif

#include "type/Color3.h"
#include "type/Vector3.h"

#ifndef OB_INST_BASEPART
#define OB_INST_BASEPART

namespace OB{
	namespace Instance{
		/**
		 * BasePart is the base class of all physical objects in
		 * OpenBlox.
		 *
		 * @author John M. Harris, Jr.
		 */
		class BasePart: public PVInstance{
			public:
			    BasePart();
				virtual ~BasePart();

			    void setAnchored(bool anchored);
				bool getAnchored();
				
				void setColor(shared_ptr<Type::Color3> color);
				shared_ptr<Type::Color3> getColor();

				void setCanCollide(bool cancollide);
				bool getCanCollide();

				void setLocked(bool locked);
				bool getLocked();

				void setTransparency(double transparency);
				double getTransparency();

				void setPosition(shared_ptr<Type::Vector3> position);
				shared_ptr<Type::Vector3> getPosition();

				void setRotation(shared_ptr<Type::Vector3> rotation);
				shared_ptr<Type::Vector3> getRotation();

				virtual void updateColor();
				virtual void updatePosition();
				virtual void updateRotation();

				#if HAVE_ENET
				/**
				 * Replicates properties of this Instance.
				 * 
				 * @param peer Peer
				 * @author John M. Harris, Jr.
				 */
				virtual void replicateProperties(shared_ptr<NetworkReplicator> peer);
				#endif

				virtual std::map<std::string, std::string> getProperties();
				virtual shared_ptr<Type::VarWrapper> getProperty(std::string prop);
				virtual void setProperty(std::string prop, shared_ptr<Type::VarWrapper> val);

				DECLARE_LUA_METHOD(setAnchored);
				DECLARE_LUA_METHOD(getAnchored);
				DECLARE_LUA_METHOD(setColor);
				DECLARE_LUA_METHOD(getColor);
				DECLARE_LUA_METHOD(setCanCollide);
				DECLARE_LUA_METHOD(getCanCollide);
				DECLARE_LUA_METHOD(setLocked);
				DECLARE_LUA_METHOD(getLocked);
				DECLARE_LUA_METHOD(setTransparency);
				DECLARE_LUA_METHOD(getTransparency);
				DECLARE_LUA_METHOD(setPosition);
				DECLARE_LUA_METHOD(getPosition);
				DECLARE_LUA_METHOD(setRotation);
				DECLARE_LUA_METHOD(getRotation);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);

				DECLARE_CLASS(BasePart);

				bool Anchored;
				shared_ptr<Type::Color3> Color;
				bool CanCollide;
				bool Locked;
				double Transparency;
				shared_ptr<Type::Vector3> Position;
				shared_ptr<Type::Vector3> Rotation;
		};
	}
}

#endif // OB_INST_BASEPART


// Local Variables:
// mode: c++
// End:
