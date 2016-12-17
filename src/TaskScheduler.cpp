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

#include <cstdio>

namespace OB{
	TaskScheduler::TaskScheduler(){
	}

	TaskScheduler::~TaskScheduler(){}

	bool operator==(const _ob_waiting_task& t1, const _ob_waiting_task& t2){
		return &t1 == &t2;
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
			
			while(!tasks.empty() && !stopProcTasks){
				ob_int64 curTime = currentTimeMillis();

				_ob_waiting_task t = tasks.back();				

				if(t.at < curTime){
					tasks.pop_back();
					
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
				}else{
				    stopProcTasks = true;
				}
			}

			if(!tmpPopped.empty()){
				while(!tmpPopped.empty()){
					tasks.push_back(tmpPopped.back());
					tmpPopped.pop_back();
				}
				sortTasks();
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
		
		sortTasks();
	}

	bool __ob_taskscheduler_sort_cmp(const _ob_waiting_task& t1, const _ob_waiting_task& t2){
		return t1.at > t2.at;
	}

	void TaskScheduler::sortTasks(){
		std::sort(tasks.begin(), tasks.end(), __ob_taskscheduler_sort_cmp);
	}
}
