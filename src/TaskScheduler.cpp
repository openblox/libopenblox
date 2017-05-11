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

#include "TaskScheduler.h"

#include <algorithm>

#include "OBException.h"
#include "utility.h"

namespace OB{
	TaskScheduler::TaskScheduler(){
	    SortsTasks = true;

		numWaiting = 0;
	}

	TaskScheduler::~TaskScheduler(){}

	bool operator==(const _ob_waiting_task& t1, const _ob_waiting_task& t2){
		return &t1 == &t2;
	}

	bool TaskScheduler::GetSortsTasks(){
		return SortsTasks;
	}

	void TaskScheduler::SetSortsTasks(bool sortsTasks){
		SortsTasks = sortsTasks;
	}

	int TaskScheduler::GetNumSleepingJobs(){
		return tasks.size();
	}

	int TaskScheduler::GetNumWaitingJobs(){
		return numWaiting;
	}

	void TaskScheduler::tick(){
		if(!tasks.empty()){
			/* This vector contains tasks that returned response code
			 * 1. These are pushed back onto the tasks vector after
			 * other tasks have been handled, or until a task returns
			 * an area code that marks the end of task handling for
			 * this tick.
			 */
			std::vector<_ob_waiting_task> tmpPopped;
			
			bool stopProcTasks = false;

			std::vector<_ob_waiting_task> runThisTick;

			ob_int64 curTime = currentTimeMillis();
			while(!tasks.empty()){
				_ob_waiting_task t = tasks.back();
				
				if(t.at < curTime){
					runThisTick.push_back(t);
					tasks.pop_back();
				}else{
				    break;
				}
			}

			numWaiting = runThisTick.size();
			
			while(!runThisTick.empty() && !stopProcTasks){
				_ob_waiting_task t = runThisTick.back();
				runThisTick.pop_back();
				numWaiting--;
				
				int retCode = t.task_fnc(t.metad, t.start);
				
				switch(retCode){
					case 1: {
						tmpPopped.push_back(t);
						break;
					}
					case 2: {
						stopProcTasks = true;
						break;
					}
					case 3: {
						tmpPopped.push_back(t);
						stopProcTasks = true;
						break;
					}
				}
			}

			if(!runThisTick.empty()){
				while(!runThisTick.empty()){
					tasks.push_back(runThisTick.back());
					runThisTick.pop_back();
				}
				if(SortsTasks){
					sortTasks();
				}
			}

			if(!tmpPopped.empty()){
				while(!tmpPopped.empty()){
					tasks.push_back(tmpPopped.back());
					tmpPopped.pop_back();
				}
				if(SortsTasks){
					sortTasks();
				}
			}
		}
	}

	void TaskScheduler::enqueue(ob_task_fnc fnc, void* metad, ob_int64 at){
		ob_int64 curTime = currentTimeMillis();
		
		_ob_waiting_task t;
		t.start = curTime;
		t.at = at;
		t.metad = metad;
		t.task_fnc = fnc;
		
		tasks.push_back(t);

		if(SortsTasks){
			sortTasks();
		}
	}

	bool __ob_taskscheduler_sort_cmp(const _ob_waiting_task& t1, const _ob_waiting_task& t2){
		return t1.at > t2.at;
	}

	void TaskScheduler::sortTasks(){
		std::sort(tasks.begin(), tasks.end(), __ob_taskscheduler_sort_cmp);
	}
}
