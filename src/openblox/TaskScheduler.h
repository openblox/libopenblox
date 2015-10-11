/*
 * Copyright 2015 John M. Harris, Jr.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenBlox.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPENBLOX_TASKSCHEDULER_H_
#define OPENBLOX_TASKSCHEDULER_H_

#include "OpenBlox.h"

//C++11
#include <vector>
#include <functional>

namespace OpenBlox{
	namespace Type{
		class VarWrapper;//Forward declaration
	}

	struct waiting_task{
		qint64 at;
		qint64 start;
		int ref;
		lua_State* L;
		int(*func)(lua_State*);
	};

	struct EventHelper{
		lua_State* L;
		int ref;
		bool isWaking;
		bool shouldBlockLogService;
		std::vector<Type::VarWrapper>* argList;
	};

	/**
	 * General purpose task scheduler. We want to be able to send lua tasks to this as well as C functions, or even bound C++ methods.
	 * @author John M. Harris, Jr.
	 */
	class TaskScheduler{
		public:
			TaskScheduler();
			virtual ~TaskScheduler();

			static TaskScheduler* getInstance();

			void tick();

			bool shouldSuppressLogService();
			void shouldSuppressLogService(bool should);

			int delay(lua_State* L, int idx, qint64 millis);
			int wait(lua_State* L, double secs);
			int waitFor(lua_State* L, int(*func)(lua_State*));
			void wakeState(lua_State* L, std::vector<Type::VarWrapper>* argList, bool shouldBlockLogService);
			void addTask(std::function<void()> func);
			void addEventHelper(lua_State* L, int ref, std::vector<Type::VarWrapper>* argList, bool shouldBlockLogService);

			void enqueue_task(waiting_task task);

		private:
			std::vector<waiting_task> tasks;
			std::vector<std::function<void()>> ctasks;
			std::vector<EventHelper> etasks;
			QMutex mutex;

			bool blockLogServ;

			static TaskScheduler* inst;
	};
}

#endif
