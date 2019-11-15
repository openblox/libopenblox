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

#include "instance/LogService.h"

#include "utility.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(LogService, false, isDataModel, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		LogService::LogService(OBEngine* eng) : Instance(eng){
			Name = ClassName;
			netId = OB_NETID_NOT_REPLICATED;

			Archivable = false;

			// These pointers are initialized to keep postLog going as quickly as possible. No lookup should be done there.
			MessageOutput = Enum::LuaMessageType->getEnumItem((int)Enum::MessageType::MessageOutput);
			MessageInfo = Enum::LuaMessageType->getEnumItem((int)Enum::MessageType::MessageInfo);
			MessageWarning = Enum::LuaMessageType->getEnumItem((int)Enum::MessageType::MessageWarning);
			MessageError = Enum::LuaMessageType->getEnumItem((int)Enum::MessageType::MessageError);

			MessageOut = make_shared<Type::Event>("MessageOut");

			blocked = false;
		}

		LogService::~LogService(){}

		std::string LogService::fixedSerializedID(){
			return "LogService";
		}

		shared_ptr<Instance> LogService::cloneImpl(){
			return NULL;
		}

		void LogService::postLog(std::string message, Enum::MessageType messageType){
			if(blocked){
				return;
			}

			shared_ptr<Type::LuaEnumItem> val;
			switch(messageType){
				case Enum::MessageType::MessageOutput: {
					val = MessageOutput;
					break;
				}
				case Enum::MessageType::MessageInfo: {
					val = MessageInfo;
					break;
				}
				case Enum::MessageType::MessageWarning: {
					val = MessageWarning;
					break;
				}
				case Enum::MessageType::MessageError: {
					val = MessageError;
					break;
				}
				default: {
					return;
				}
			}

			if(!val){
				return;
			}

			std::vector<shared_ptr<Type::VarWrapper>> args = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(message), make_shared<Type::VarWrapper>(val)});
			MessageOut->Fire(eng, args);
		}

		void LogService::block(){
			blocked = true;
		}

		void LogService::unblock(){
			blocked = false;
		}

		shared_ptr<Type::Event> LogService::getMessageOut(){
			return MessageOut;
		}

		void LogService::register_lua_events(lua_State* L){
			Instance::register_lua_events(L);

			luaL_Reg events[] = {
				{"MessageOut", WRAP_EVT(LogService, MessageOut)},
				{NULL, NULL}
			};
			luaL_setfuncs(L, events, 0);
		}
	}
}
