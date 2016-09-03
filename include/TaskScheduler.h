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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox.	 If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef OB_TASKSCHEDULER
#define OB_TASKSCHEDULER

namespace OB{
	/**
	 * This class is used to schedule tasks to be run on the task
	 * thread, mostly tasks involving Lua states.
	 *
	 * @author John M. Harris, Jr.
	 */
	class TaskScheduler{
		public:
		    TaskScheduler();
			virtual ~TaskScheduler();

			/**
			 * Runs the task scheduler one time, handles any pending
			 * tasks and then returns. This operation can add more
			 * tasks to the queue.
			 *
			 * @author John M. Harris, Jr.
			 */
			void tick();
	};
}

#endif // OB_TASKSCHEDULER

// Local Variables:
// mode: c++
// End:
