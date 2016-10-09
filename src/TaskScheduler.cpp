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

#include "OBException.h"

namespace OB{
	TaskScheduler::TaskScheduler(){
	}

	TaskScheduler::~TaskScheduler(){}

	void TaskScheduler::tick(){
		
	}

	void TaskScheduler::enqueue(ob_task_fnc fnc, void* metad, ob_int64 at){
		ob_int64 curTime = currentTimeMillis();
		
		_ob_waiting_task t;
		t.start = curTime;
		t.at = at;
		t.metad = metad;
		t.task_fnc = fnc;

		enq(t);
	}

	void TaskScheduler::enq(_ob_waiting_task t){
		tasks.push_back(t);

		std::sort(tasks.begin(), tasks.end(), [](const _ob_waiting_task& t1, const _ob_waiting_task& t2)->bool{
			return t1.at > t2.at;
		});
	}
}
