/*
 * Copyright (C) 2016 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "instance/Instance.h"

#ifndef OB_INST_SERVICEPROVIDER
#define OB_INST_SERVICEPROVIDER

namespace OB{
	namespace Instance{
		/**
		 * ServiceProvider provides methods for objects like DataModel
		 * that provide access to service classes like Lighting.
		 *
		 * @author John M. Harris, Jr.
		 */
		class ServiceProvider: public Instance{
			public:
				ServiceProvider(OBEngine* eng);
				virtual ~ServiceProvider();

				/**
				 * Pretty much the same as FindFirstChildOfClass, but made
				 * specifically for services. Doesn't check recursively.
				 *
				 * @param className Class name to look for
				 * @returns The service requested or NULL
				 * @author John M. Harris, Jr.
				 */
				virtual shared_ptr<Instance> FindService(std::string className);

				/**
				 * Calls FindService, and attempts to create the
				 * service if it is not found.
				 *
				 * @param className Class name to look for
				 * @returns The service requested, or NULL if it cannot be created.
				 * @author John M. Harris, Jr.
				 */
				virtual shared_ptr<Instance> GetService(std::string className);

				DECLARE_LUA_METHOD(FindService);
				DECLARE_LUA_METHOD(GetService);

				DECLARE_CLASS(ServiceProvider);

				static void register_lua_methods(lua_State* L);
		};
	}
}

#endif // OB_INST_SERVICEPROVIDER


// Local Variables:
// mode: c++
// End:
