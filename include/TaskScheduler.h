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

#include <vector>

#include "obtype.h"

#ifndef OB_TASKSCHEDULER
#define OB_TASKSCHEDULER

namespace OB{
	class OBEngine;

	/**
	 * This typedef describes the type of function accepted by the
	 * TaskScheduler. When queuing a task, you specify a function of
	 * this type, and any type to be used as metadata. That type is
	 * cast to a void*, which will be passed as the first parameter of
	 * this function on invocation. The second parameter, startTime,
	 * is the time, in milliseconds, that the task was queued. This is
	 * used mainly by certain Lua functions, such as `wait` and
	 * `delay`.
	 *
	 * The return code of this function is very important, as each
	 * return code signals a different handling in the TaskScheduler.
	 * If you don't know what you're doing, just return 0 and you'll
	 * be fine. Other return codes are described here:
	 *
	 * 0 - Signals success, removes this task from the scheduler and
	 * the TaskScheduler continues processing tasks.
	 *
	 * 1 - Signals that this task either did not complete its' job or
	 * has not run yet, and that it should be left on the scheduler.
	 * The scheduler continues processing further tasks, but this task
	 * is left to be processed later.
	 *
	 * 2 - Signals success, same as a response code 0, except the
	 * TaskScheduler should stop processing tasks for this tick.
	 *
	 * 3 - Signals that the task did not complete its' job, same as
	 * a response code 1, except the TaskScheduler should stop
	 * processing tasks for this tick.
	 */
	typedef int (*ob_task_fnc)(void* metad, ob_int64 startTime);

	/**
	 * This struct holds the internal data of a waiting task. This
	 * struct is not guaranteed to be the same between versions of OB,
	 * and should not be used in any code outside of libopenblox,
	 * including, but not limited to, the OpenBlox client, server and
	 * Studio software. I would go so far as to suggest that this
	 * should not be used outside of the TaskScheduler class itself,
	 * but it is exposed here for external use, if it is necessary.
	 */
	struct _ob_waiting_task{
	public:
		ob_int64 at;
		ob_int64 start;
		void* metad;
		ob_task_fnc task_fnc;
		bool getsPaused;
		bool dmBound;
	};

	/**
	 * This class is used to schedule tasks to be run on the task
	 * thread, mostly tasks involving Lua states.
	 *
	 * When GetSortsTasks is false, the TaskScheduler will act as a
	 * FIFO queue.
	 *
	 * @author John M. Harris, Jr.
	 */
	class TaskScheduler{
	public:
		TaskScheduler(OBEngine* eng);
		virtual ~TaskScheduler();

		/*
		 * Returns whether or not this TaskScheduler sorts tasks.
		 *
		 * If true, tasks enqueued will be sorted by the time
		 * they're meant to run at.
		 *
		 * @returns bool, true if tasks are to be sorted
		 * @author John M. Harris, Jr.
		 */
		bool GetSortsTasks();

		/*
		 * Sets whether or not this TaskScheduler sorts tasks.
		 *
		 * @param sortsTasks Whether or not tasks are to be sorted
		 * @sa TaskScheduler::GetSortsTasks
		 * @author John M. Harris, Jr.
		 */
		void SetSortsTasks(bool sortsTasks);

		/**
		 * Returns the number of sleeping jobs.
		 *
		 * Any job that is currently waiting to run, but will not
		 * be run on this tick is considered to be "sleeping".
		 *
		 * @returns int, number of sleeping jobs
		 */
		int GetNumSleepingJobs();

		/**
		 * Returns the number of waiting jobs.
		 *
		 * Any job that is currently waiting to run, and will be
		 * run by the end of this tick is "waiting".
		 *
		 * Some jobs can make the TaskScheduler skip remaining
		 * waiting jobs, so don't rely on this for anything
		 * important.
		 *
		 * @returns int, number of waiting jobs
		 */
		int GetNumWaitingJobs();

		/**
		 * Runs the task scheduler one time, handles any pending
		 * tasks and then returns. This operation can add more
		 * tasks to the queue.
		 *
		 * @author John M. Harris, Jr.
		 */
		void tick();

		/**
		 * Removes DM bound tasks from the queue.
		 *
		 * @author John M. Harris, Jr.
		 */
		void removeDMBound();

		/**
		 * Queues a task to be run at a given time.
		 *
		 * @param fnc Function to queue
		 * @param metad Metadata to be passed to the function.
		 * @param at When this task needs to run.
		 * @param getsPaused Whether or not this task gets paused with RunService.
		 * @param dmBound If a task is 'DM bound', it is removed from the TaskScheduler when RunService::Stop is called.
		 *
		 * @author John M. Harris, Jr.
		 */
		void enqueue(ob_task_fnc fnc, void* metad, ob_int64 at, bool getsPaused, bool dmBound);
	private:
		std::vector<_ob_waiting_task> tasks;

		OBEngine* eng;

		bool SortsTasks;

		/**
		 * Internal method used to sort tasks.
		 *
		 * @author John M. Harris, Jr.
		 */
		void sortTasks();

		int numWaiting;
	};
}

#endif // OB_TASKSCHEDULER

// Local Variables:
// mode: c++
// End:
