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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "instance/Instance.h"

#ifndef OB_INST_RUNSERVICE
#define OB_INST_RUNSERVICE

namespace OB{
	namespace Instance{
		/**
		 * Lighting provides access to the logic and
		 * rendering loop.
		 *
		 * @author John M. Harris, Jr.
		 */
		class RunService: public Instance{
			public:
				RunService(OBEngine* eng);
				virtual ~RunService();

				bool IsRunMode();
				bool IsRunning();
				bool IsClient();
				bool IsServer();
				bool IsStudio();

				void Pause();
				void Run();
				void Stop();

				void setIsStudio(bool isStudio);

				virtual void tick();

#if HAVE_PUGIXML
				virtual std::string fixedSerializedID();
#endif

				static void register_lua_events(lua_State* L);

				DECLARE_CLASS(RunService);

				bool wasRunning;
				bool running;
				bool isStudio;

				shared_ptr<Type::Event> Stepped;
		};
	}
}

#endif // OB_INST_RUNSERVICE


// Local Variables:
// mode: c++
// End:
