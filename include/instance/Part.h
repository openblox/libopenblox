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

#include "instance/BasePart.h"

#include "oblibconfig.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#endif

#ifndef OB_INST_PART
#define OB_INST_PART

namespace OB{
	namespace Instance{
		/**
		 * Part allows creation of primitive 3D cubes.
		 *
		 * @author John M. Harris, Jr.
		 */
		class Part: public BasePart{
			public:
			    Part();
				virtual ~Part();

				shared_ptr<Type::Vector3> getSize();
				void setSize(shared_ptr<Type::Vector3> size);

				virtual void updateSize();
				virtual void updateColor();

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

				DECLARE_LUA_METHOD(setSize);
				DECLARE_LUA_METHOD(getSize);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);

				DECLARE_CLASS(Part);

				shared_ptr<Type::Vector3> Size;
		};
	}
}

#endif // OB_INST_PART


// Local Variables:
// mode: c++
// End:
