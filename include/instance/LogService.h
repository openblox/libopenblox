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

#ifndef OB_INST_LOGSERVICE
#define OB_INST_LOGSERVICE

#include "type/Enum.h"

namespace OB{
	namespace Instance{
		/**
		 * LogService provides access to game logs, including
		 * print, warn and error calls from Lua.
		 *
		 * @author John M. Harris, Jr.
		 */
		class LogService: public Instance{
			public:
				LogService(OBEngine* eng);
				virtual ~LogService();

				virtual std::string fixedSerializedID();

				void postLog(std::string message, Enum::MessageType messageType);

				void block();
				void unblock();

				shared_ptr<Type::Event> getMessageOut();

				static void register_lua_events(lua_State* L);

				DECLARE_CLASS(LogService);

				bool blocked;
				shared_ptr<Type::Event> MessageOut;

				shared_ptr<Type::LuaEnumItem> MessageOutput;
				shared_ptr<Type::LuaEnumItem> MessageInfo;
				shared_ptr<Type::LuaEnumItem> MessageWarning;
				shared_ptr<Type::LuaEnumItem> MessageError;
		};
	}
}

#endif // OB_INST_LOGSERVICE


// Local Variables:
// mode: c++
// End:
