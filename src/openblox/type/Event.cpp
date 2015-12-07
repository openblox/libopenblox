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

#include "Event.h"

#include <nasty_json.h>

#include <Instance.h>

#include <TaskScheduler.h>

BEGIN_OB_TYPE

VarWrapper::VarWrapper(){
	type = TYPE_UNKNOWN;
	wrapped = NULL;
}

VarWrapper::VarWrapper(void* anything, VarType vtype){
	type = vtype;
	wrapped = anything;
}

IntWrapper::IntWrapper(int val){
	this->val = val;
}

VarWrapper::VarWrapper(int var){
	type = TYPE_INT;
	wrapped = reinterpret_cast<void*>(new IntWrapper(var));
}

DoubleWrapper::DoubleWrapper(double val){
	this->val = val;
}

VarWrapper::VarWrapper(double var){
	type = TYPE_DOUBLE;
	wrapped = reinterpret_cast<void*>(new DoubleWrapper(var));
}

FloatWrapper::FloatWrapper(float val){
	this->val = val;
}

VarWrapper::VarWrapper(float var){
	type = TYPE_FLOAT;
	wrapped = reinterpret_cast<void*>(new FloatWrapper(var));
}

LongWrapper::LongWrapper(long val){
	this->val = val;
}

VarWrapper::VarWrapper(long var){
	type = TYPE_LONG;
	wrapped = reinterpret_cast<void*>(new LongWrapper(var));
}

UnsignedLongWrapper::UnsignedLongWrapper(unsigned long val){
	this->val = val;
}

VarWrapper::VarWrapper(unsigned long var){
	type = TYPE_UNSIGNED_LONG;
	wrapped = reinterpret_cast<void*>(new UnsignedLongWrapper(var));
}

BoolWrapper::BoolWrapper(bool val){
	this->val = val;
}

StringWrapper::StringWrapper(QString val){
	this->val = val;
}

VarWrapper::VarWrapper(bool var){
	type = TYPE_BOOL;
	wrapped =  reinterpret_cast<void*>(new BoolWrapper(var));
}

VarWrapper::VarWrapper(QString var){
	type = TYPE_STRING;
	wrapped = reinterpret_cast<void*>(new StringWrapper(var));
}

VarWrapper::VarWrapper(OpenBlox::Instance::Instance* var){
	type = TYPE_INSTANCE;
	wrapped = var;
}

VarWrapper::VarWrapper(OpenBlox::Enum::LuaEnumItem* var){
	type = TYPE_ENUMITEM;
	wrapped = var;
}

VarWrapper::VarWrapper(OpenBlox::Enum::LuaEnum* var){
	type = TYPE_ENUM;
	wrapped = var;
}

VarWrapper::~VarWrapper(){}

//End nasty type wrappers.

#define lua_evt_con_name "luaL_Type_EventConnection"

STATIC_GAME_INIT(EventConnection){
	lua_State* L = OpenBlox::OBEngine::getInstance()->getLuaState();

	luaL_newmetatable(L, lua_evt_con_name);

	lua_pushstring(L, "__metatable");
	lua_pushstring(L, "This metatable is locked");
	lua_rawset(L, -3);

	lua_pushstring(L, "__methods");
	lua_newtable(L);
	luaL_Reg methods[]{
		{"disconnect", lua_disconnect},
		{NULL, NULL}
	};
	luaL_setfuncs(L, methods, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__properties");
	lua_newtable(L);
	luaL_Reg props[]{
		{"connected", lua_getConnected},
		{NULL, NULL}
	};
	luaL_setfuncs(L, props, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, lua_toString);
	lua_rawset(L, -3);

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, lua_index);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, lua_newindex);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

EventConnection::EventConnection(Event* evt, int ref, int refL){
	this->evt = evt;
	this->ref = ref;
	this->refL = refL;

	connected = true;
}

EventConnection::~EventConnection(){}

/**
 * Wraps this EventConnection for Lua.
 * @param lua_State* L The Lua state to wrap this EventConnection for.
 * @returns int Number of returned values.
 * @author John M. Harris, Jr.
 */
int EventConnection::wrap_lua(lua_State* L){
	EventConnection** evt = (EventConnection**)lua_newuserdata(L, sizeof(*this));
	*evt = this;

	luaL_getmetatable(L, lua_evt_con_name);
	lua_setmetatable(L, -2);

	return 1;
}

/**
 * Disconnects this EventConnection from the Event that it belongs to.
 * @param lua_State* Lua State
 * @author John M. Harris, Jr.
 */
void EventConnection::disconnect(lua_State* L){
	if(connected){
		luaL_unref(L, LUA_REGISTRYINDEX, ref);
		luaL_unref(L, LUA_REGISTRYINDEX, refL);

		evt->disconnect(ref);
		connected = false;
	}
}

/**
 * Handles the EventConnection::disconnect method for Lua.
 * @param lua_State* Lua State
 * @returns int 0
 * @author John M. Harris, Jr.
 */
int EventConnection::lua_disconnect(lua_State* L){
	EventConnection* con = checkEventConnection(L, 1);
	con->disconnect(L);
	return 0;
}

/**
 * Handles attempts to get the "connected" property of this EventConnection.
 * @param lua_State* Lua State
 * @returns int 1, a bool value on the lua stack.
 * @author John M. Harris, Jr.
 */
int EventConnection::lua_getConnected(lua_State* L){
	EventConnection* con = checkEventConnection(L, 1);
	if(con != NULL){
		bool conned = con->connected;
		if(conned){
			conned = con->evt->isConnected(con->ref);
		}
		lua_pushboolean(L, conned);
		return 1;
	}
	return 0;
}

/**
 * Handles tostring calls on this EventConnection from Lua.
 * @param lua_State* Lua State
 * @returns int 1, a string value on the lua stack.
 * @author John M. Harris, Jr.
 */
int EventConnection::lua_toString(lua_State* L){
	lua_pushstring(L, "Connection");
	return 1;
}

/**
 * Handles attempts to get properties or methods of this EventConnection.
 * @param lua_State* Lua State
 * @returns int 1 if there is a corresponding value, otherwise errors. (No return)
 * @author DigiTechs
 * @author John M. Harris, Jr.
 */
int EventConnection::lua_index(lua_State* L){
	EventConnection* con = checkEventConnection(L, 1);
	if(con != NULL){
		const char* name = luaL_checkstring(L, 2);

		lua_getmetatable(L, 1);
		lua_getfield(L, -1, "__properties");
		lua_getfield(L, -1, name);
		if(lua_iscfunction(L, -1)){
			lua_remove(L, -2);
			lua_remove(L, -2);

			lua_pushvalue(L, 1);
			lua_call(L, 1, 1);
			return 1;
		}else{
			lua_pop(L, 2);
			//Check methods
			lua_getfield(L, -1, "__methods");//-2
			lua_getfield(L, -1, name);//-1
			if(lua_iscfunction(L, -1)){
				lua_remove(L, -2);
				lua_remove(L, -3);

				return 1;
			}else{
				lua_pop(L, 3);
				return luaL_error(L, "attempt to index '%s' (a nil value)", name);
			}
		}
	}
	return 0;
}

/**
 * Handles attempts to set properties of this EventConnection.
 * @param lua_State* Lua State
 * @returns int 0 or error (Does not return if an error occurs)
 * @author DigiTechs
 * @author John M. Harris, Jr.
 */
int EventConnection::lua_newindex(lua_State* L){
	const char* name = luaL_checkstring(L, 2);
	return luaL_error(L, "%s cannot be assigned to", name);
}

/**
 * Checks that the value at the specified index on the Lua state is an EventConnection, and if so returns it.
 * @param lua_State* Lua State
 * @param int Index
 * @author John M. Harris, Jr.
 * @author DigiTechs
 */
EventConnection* checkEventConnection(lua_State* L, int n){
	return *(EventConnection**)luaL_checkudata(L, n, lua_evt_con_name);
}

//End EventConnection

#define lua_evt_name "luaL_Type_Event"

STATIC_GAME_INIT(Event){
	lua_State* L = OpenBlox::OBEngine::getInstance()->getLuaState();

	luaL_newmetatable(L, lua_evt_name);

	lua_pushstring(L, "__metatable");
	lua_pushstring(L, "This metatable is locked");
	lua_rawset(L, -3);

	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, lua_toString);
	lua_rawset(L, -3);

	lua_pushstring(L, "__index");
	lua_newtable(L);

	luaL_Reg methods[]{
		{"connect", lua_connect},
		{"wait", lua_wait},
		{NULL, NULL}
	};
	luaL_setfuncs(L, methods, 0);

	lua_rawset(L, -3);

	lua_pop(L, 1);
}

Event::Event(QString LuaEventName){
	this->eventName = LuaEventName;

	shouldBlockLogService = false;

	connections = std::vector<EvtCon>();
	waiting = std::vector<lua_State*>();
	chandlers = std::vector<std::function<void(std::vector<VarWrapper>)>>();
}

Event::~Event(){}

void Event::Connect(std::function<void(std::vector<VarWrapper>)> hldr){
	chandlers.push_back(hldr);
}

/**
 * Changes shouldBlockLogService to true, this is used to keep LogService events from firing themselves.
 * @author John M. Harris, Jr.
 */
void Event::blockLogService(){
	shouldBlockLogService = true;
}

/**
 * Disconnects all connections.
 * @author John M. Harris, Jr.
 */
void Event::disconnectAll(){
	connections.clear();
}

/**
 * Disconnects a connection by ref.
 * @param int ref
 * @author John M. Harris, Jr.
 */
void Event::disconnect(int ref){
	std::vector<int>::size_type min1 = -1;
	std::vector<int>::size_type to_remove = min1;
	for(std::vector<int>::size_type i = 0; i != connections.size(); i++){
		if(connections[i].ref == ref){
			to_remove = i;
			break;
		}
	}
	if(to_remove != min1){
		connections.erase(connections.begin() + (to_remove - 1));
	}
}

/**
 * Returns whether or not the connection with matching ref is still connected.
 * @param int ref
 * @returns bool true if
 * @author John M. Harris, Jr.
 */
bool Event::isConnected(int ref){
	for(std::vector<int>::size_type i = 0; i != connections.size(); i++){
		if(connections[i].ref == ref){
			return true;
		}
	}
	return false;
}

/**
 * Wraps this Event for Lua.
 * @param lua_State* L The Lua state to wrap this EventConnection for.
 * @returns int Number of returned values.
 * @author John M. Harris, Jr.
 */
int Event::wrap_lua(lua_State* L){
	Event** evt = (Event**)lua_newuserdata(L, sizeof(*this));
	*evt = this;

	luaL_getmetatable(L, lua_evt_name);
	lua_setmetatable(L, -2);

	return 1;
}

/**
 * Converts arguments in the supplied vector to Lua types.
 * @param lua_State* Lua state
 * @param std::vector<VarWrapper> argList
 * @author John M. Harris, Jr.
 */
int Event::pushWrappersToLua(lua_State* L, std::vector<VarWrapper> argList){
	int nargs = 0;
	for(size_t i = 0; i < argList.size(); i++){
		VarWrapper wrap = argList[i];
		switch(wrap.type){
			case TYPE_INT: {
				nargs++;
				lua_pushinteger(L, reinterpret_cast<IntWrapper*>(wrap.wrapped)->val);
				break;
			}
			case TYPE_DOUBLE: {
				nargs++;
				lua_pushnumber(L, reinterpret_cast<DoubleWrapper*>(wrap.wrapped)->val);
				break;
			}
			case TYPE_FLOAT: {
				nargs++;
				lua_pushnumber(L, reinterpret_cast<FloatWrapper*>(wrap.wrapped)->val);
				break;
			}
			case TYPE_LONG: {
				nargs++;
				lua_pushnumber(L, reinterpret_cast<LongWrapper*>(wrap.wrapped)->val);
				break;
			}
			case TYPE_UNSIGNED_LONG: {
				nargs++;
				lua_pushnumber(L, reinterpret_cast<UnsignedLongWrapper*>(wrap.wrapped)->val);
				break;
			}
			case TYPE_BOOL: {
				nargs++;
				lua_pushboolean(L, reinterpret_cast<BoolWrapper*>(wrap.wrapped)->val);
				break;
			}
			case TYPE_STRING: {
				nargs++;
				lua_pushstring(L, reinterpret_cast<StringWrapper*>(wrap.wrapped)->val.toStdString().c_str());
				break;
			}
			case TYPE_JSON: {
				nargs++;
				lua_State* newState = lua_newthread(L);//+1 -0
				int ret = luaL_dostring(newState, json_script_src);
				if(ret == 0){
					lua_getglobal(newState, "Decode");
					const char* theJSON = reinterpret_cast<StringWrapper*>(wrap.wrapped)->val.toStdString().c_str();
					WLOG(QString(theJSON));
					lua_pushstring(newState, theJSON);
					ret = lua_pcall(newState, 1, 1, 0);
					if(ret == 0){
						lua_pop(L, 1);//+0 -1
						lua_xmove(newState, L, 1);
						continue;
					}else{
						WLOG(QString(lua_tostring(newState, -1)));
						WLOG("No 2nd ret");
					}
				}else{
					WLOG("No 1st ret");
				}
				lua_pop(L, 1);//+0 -1
				lua_pushnil(L);
				break;
			}
			case TYPE_INSTANCE: {
				nargs++;
				reinterpret_cast<OpenBlox::Instance::Instance*>(wrap.wrapped)->wrap_lua(L);
				break;
			}
			case TYPE_ENUMITEM: {
				nargs++;
				reinterpret_cast<OpenBlox::Enum::LuaEnumItem*>(wrap.wrapped)->wrap_lua(L);
				break;
			}
			case TYPE_ENUM: {
				nargs++;
				reinterpret_cast<OpenBlox::Enum::LuaEnum*>(wrap.wrapped)->wrap_lua(L);
				break;
			}
			default: {
				nargs++;
				lua_pushnil(L);
			}
		}
	}
	return nargs;
}

/**
 * Fires this event with the arguments specified in the argList at a later time. Pushes it onto the TaskScheduler's queue.
 * @params std::vector<VarWrapper>* argList
 * @author John M. Harris, Jr.
 */
void Event::FireLater(std::vector<VarWrapper>* argList){
	if(!waiting.empty()){
		while(!waiting.empty()){
			try{
				lua_State* L = waiting.at(waiting.size() - 1);
				if(L != NULL){
					OpenBlox::TaskScheduler::getInstance()->wakeState(L, argList, shouldBlockLogService);
				}
				waiting.pop_back();
			}catch(const std::out_of_range& e){}
		}
	}
	//TODO: Delay this
	for(std::vector<void(*)(std::vector<VarWrapper>)>::size_type i = 0; i != chandlers.size(); i++){
		std::function<void(std::vector<VarWrapper>)> m_handl = chandlers[i];
		m_handl(*argList);
	}
	for(std::vector<int>::size_type i = 0; i != connections.size(); i++){
		EvtCon con = connections[i];
		lua_State* L = con.L;
		int ref = con.ref;

		OpenBlox::TaskScheduler::getInstance()->addEventHelper(L, ref, argList, shouldBlockLogService);
	}
}

/**
 * Fires this event with the arguments specified in the argList.
 * @params std::vector<VarWrapper>* argList
 * @author John M. Harris, Jr.
 */
void Event::Fire(std::vector<VarWrapper>* argList){
	if(!waiting.empty()){
		while(!waiting.empty()){
			try{
				lua_State* L = waiting.at(waiting.size() - 1);
				if(L != NULL){
					OpenBlox::TaskScheduler::getInstance()->wakeState(L, argList, shouldBlockLogService);
				}
				waiting.pop_back();
			}catch(const std::out_of_range& e){}
		}
	}
	for(std::vector<void(*)(std::vector<VarWrapper>)>::size_type i = 0; i != chandlers.size(); i++){
		std::function<void(std::vector<VarWrapper>)> m_handl = chandlers[i];
		m_handl(*argList);
	}
	for(std::vector<int>::size_type i = 0; i != connections.size(); i++){
		EvtCon con = connections[i];
		lua_State* L = ob_lua::newthread(con.L)->L;
		int ref = con.ref;

		if(shouldBlockLogService){
			OpenBlox::TaskScheduler::getInstance()->shouldSuppressLogService(true);
		}

		lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

		int nargs = pushWrappersToLua(L, *argList);

		int ret = lua_resume(L, NULL, nargs);
		if(ret != 0 && ret != LUA_YIELD){
			ob_lua::handle_lua_errors(L);
		}
		if(ret == LUA_OK){
			ob_lua::killState(L);
		}

		if(shouldBlockLogService){
			OpenBlox::TaskScheduler::getInstance()->shouldSuppressLogService(false);
		}
	}
}

/**
 * Fires this event with no arguments.
 * @author John M. Harris, Jr.
 */
void Event::Fire(){
	Fire(new std::vector<VarWrapper>());
}

/**
 * Fires this event with the arguments specified in the argList.
 * @params std::initializer_list<VarWrapper> argList
 * @author John M. Harris, Jr.
 */
void Event::Fire(std::initializer_list<VarWrapper> argList){
	Fire(new std::vector<VarWrapper>(argList));
}

/**
 * Allows callbacks to be connected to this Event from Lua.
 * @param lua_State* Lua State
 * @returns int 1, a userdata value on the lua stack representing the EventConnection.
 * @author John M. Harris, Jr.
 */
int Event::lua_connect(lua_State* L){
	Event* evt = checkEvent(L, 1);

	luaL_checktype(L, 2, LUA_TFUNCTION);
	lua_pushvalue(L, 2);

	int r = luaL_ref(L, LUA_REGISTRYINDEX);

	EvtCon econ = EvtCon();
	econ.L = ob_lua::newthread(L)->L;
	econ.ref = r;
	int refL = luaL_ref(L, LUA_REGISTRYINDEX);
	econ.refL = refL;

	evt->connections.push_back(econ);

	EventConnection* con = new EventConnection(evt, r, refL);
	return con->wrap_lua(L);
}


/**
 * Yields the Lua coutine until this event fires.
 * @param lua_State* Lua State
 * @returns int Number of returned values.
 * @author John M. Harris, Jr.
 */
int Event::lua_wait(lua_State* L){
	Event* evt = checkEvent(L, 1);
	evt->waiting.push_back(L);
	return lua_yield(L, 0);
}

/**
 * Handles tostring calls on this Event from Lua.
 * @param lua_State* Lua State
 * @returns int 1, a string value on the lua stack.
 * @author John M. Harris, Jr.
 */
int Event::lua_toString(lua_State* L){
	Event* LuaEvent = checkEvent(L, 1);
	QString ret = "Signal "+ LuaEvent->eventName;
	lua_pushstring(L, ret.toStdString().c_str());
	return 1;
}

/**
 * Checks that the value at the specified index on the Lua state is an Event, and if so returns it.
 * @param lua_State* Lua State
 * @param int Index
 * @author John M. Harris, Jr.
 * @author DigiTechs
 */
Event* checkEvent(lua_State* L, int n){
	return *(Event**)luaL_checkudata(L, n, lua_evt_name);
}

END_OB_TYPE
