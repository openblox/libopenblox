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

#include "instance/Model.h"

#include "type/Vector3.h"

#include "oblibconfig.h"

#if HAVE_BULLET
#include <btBulletDynamicsCommon.h>
#endif

#ifndef OB_INST_WORKSPACE
#define OB_INST_WORKSPACE

namespace OB{
	namespace Instance{
		/**
		 * Workspace contains all physics objects that will be
		 * rendered by the engine.
		 *
		 * @author John M. Harris, Jr.
		 */
		class Workspace: public Model{
			public:
			    Workspace();
				virtual ~Workspace();

				double getDistributedGameTime();

			    shared_ptr<Type::Vector3> getGravity();
				void setGravity(shared_ptr<Type::Vector3> gravity);
				
				double getFallenPartsDestroyHeight();
				void setFallenPartsDestroyHeight(double fpdh);

				bool getDestroyFallenParts();
				void setDestroyFallenParts(bool dfp);

				DECLARE_LUA_METHOD(getDistributedGameTime);
				DECLARE_LUA_METHOD(getGravity);
				DECLARE_LUA_METHOD(setGravity);
				DECLARE_LUA_METHOD(getFallenPartsDestroyHeight);
				DECLARE_LUA_METHOD(setFallenPartsDestroyHeight);
				DECLARE_LUA_METHOD(getDestroyFallenParts);
				DECLARE_LUA_METHOD(setDestroyFallenParts);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);

				DECLARE_CLASS(Workspace);

				void updateGravity();

				#if HAVE_BULLET
				btBroadphaseInterface* broadphase;
				btDefaultCollisionConfiguration* collisionConfiguration;
				btSequentialImpulseConstraintSolver* solver;
				btDiscreteDynamicsWorld* dynamicsWorld;
				#endif

			    shared_ptr<Type::Vector3> Gravity;
				double FallenPartsDestroyHeight;
				bool DestroyFallenParts;
		};
	}
}

#endif // OB_INST_WORKSPACE


// Local Variables:
// mode: c++
// End:
