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

#include "instance/Instance.h"

#ifndef OB_INST_TASKSCHEDULER
#define OB_INST_TASKSCHEDULER

namespace OB{
	namespace Instance{
		/**
		 * The TaskScheduler instance class provides access to stats
		 * from the internal TaskScheduler.
		 *
		 * @author John M. Harris, Jr.
		 */
		class TaskScheduler: public Instance{
			public:
			    TaskScheduler(OBEngine* eng);
				virtual ~TaskScheduler();

			    int getNumSleepingJobs();
				int getNumWaitingJobs();

				#if HAVE_PUGIXML
				virtual std::string serializedID();
				#endif

				virtual std::map<std::string, _PropertyInfo> getProperties();
				virtual shared_ptr<Type::VarWrapper> getProperty(std::string prop);

				DECLARE_LUA_METHOD(getNumSleepingJobs);
				DECLARE_LUA_METHOD(getNumWaitingJobs);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				
				DECLARE_CLASS(TaskScheduler);
		};
	}
}

#endif // OB_INST_TASKSCHEDULER


// Local Variables:
// mode: c++
// End:
