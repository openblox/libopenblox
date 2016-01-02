/*
 * Copyright 2016 John M. Harris, Jr.
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

#ifndef OB_TYPE_EVENT_H_
#define OB_TYPE_EVENT_H_

#include <OpenBlox.h>

#include "OBType.h"

#include <enum/Enums.h>

#include "Vector2.h"
#include "Vector3.h"
#include "Color3.h"
#include "Viewport.h"

//C++11
#include <functional>
#include <initializer_list>

//Qt
#include <QtCore>

namespace OpenBlox{

namespace Instance{
	class Instance;//Forward declare Instance here
}

namespace Type{

/**
 * Describes the type of a wrapped variable for conversion to Lua.
 * @author John M. Harris, Jr.
 */
enum VarType{
	TYPE_INT,//0
	TYPE_DOUBLE,//1
	TYPE_FLOAT,//2
	TYPE_LONG,//3
	TYPE_UNSIGNED_LONG,//4
	TYPE_BOOL,//5
	TYPE_STRING,//6
	TYPE_JSON,//7
	TYPE_INSTANCE,//8
	TYPE_ENUM,//9
	TYPE_ENUMITEM,//10
	TYPE_VECTOR2,//11
	TYPE_VECTOR3,//12
	TYPE_COLOR3,//13
	TYPE_VIEWPORT,//14
	TYPE_UNKNOWN//15
};

/**
 * Holds an int, int cannot be cast to void*
 * @author John M. Harris, Jr.
 */
class IntWrapper{
	public:
		IntWrapper(int val);
		int val;
};

/**
 * Holds a double, double cannot be cast to void*
 * @author John M. Harris, Jr.
 */
class DoubleWrapper{
	public:
		DoubleWrapper(double val);
		double val;
};

/**
 * Holds a float, float cannot be cast to void*
 * @author John M. Harris, Jr.
 */
class FloatWrapper{
	public:
		FloatWrapper(float val);
		float val;
};

/**
 * Holds a long, long cannot be cast to void*
 * @author John M. Harris, Jr.
 */
class LongWrapper{
	public:
		LongWrapper(long val);
		long val;
};

/**
 * Holds an unsigned long, unsigned long cannot be cast to void*
 * @author John M. Harris, Jr.
 */
class UnsignedLongWrapper{
	public:
		UnsignedLongWrapper(unsigned long val);
		unsigned long val;
};

/**
 * Holds a bool, bool cannot be cast to void*
 * @author John M. Harris, Jr.
 */
class BoolWrapper{
	public:
		BoolWrapper(bool val);
		bool val;
};

/**
 * Holds a QString, QString shouldn't be cast to void*
 * @author John M. Harris, Jr.
 */
class StringWrapper{
	public:
		StringWrapper(QString val);
		QString val;
};

/**
 * Wraps a variable for conversion to Lua.
 * @author John M. Harris, Jr.
 */
class VarWrapper{
	public:
		VarWrapper();
		VarWrapper(void* anything, VarType vtype);
		VarWrapper(int var);
		VarWrapper(double var);
		VarWrapper(float var);
		VarWrapper(long var);
		VarWrapper(unsigned long var);
		VarWrapper(bool var);
		VarWrapper(QString var);
		VarWrapper(OpenBlox::Instance::Instance* var);
		VarWrapper(OpenBlox::Enum::LuaEnumItem* var);
		VarWrapper(OpenBlox::Enum::LuaEnum* var);
		VarWrapper(Vector2* var);
		VarWrapper(Vector3* var);
		VarWrapper(Color3* var);
		VarWrapper(Viewport* var);
		~VarWrapper();

		void* wrapped;
		VarType type;
};

class Event;

/**
 * Represents a connection to Event objects. This is used to disconnect a callback.
 * @author John M. Harris, Jr.
 */
class EventConnection{
	public:
		EventConnection(Event* evt, int ref, int refL);
		virtual ~EventConnection();

		void disconnect(lua_State* L);

		DECLARE_GAME_STATIC_INIT(EventConnection);

		int wrap_lua(lua_State* L);

		static int lua_disconnect(lua_State* L);

		static int lua_getConnected(lua_State* L);

		static int lua_toString(lua_State* L);
		static int lua_index(lua_State* L);
		static int lua_newindex(lua_State* L);
	protected:
		Event* evt;
		int ref;
		int refL;
		bool connected;
};

EventConnection* checkEventConnection(lua_State* L, int n);

/**
 * Basic event class, based on that of ROBLOX.
 * @author John M. Harris, Jr.
 */
class Event{
	public:
		struct EvtCon{
			lua_State* L;
			int ref;
			int refL;
		};

		Event(QString name);
		virtual ~Event();

		void Connect(std::function<void(std::vector<VarWrapper>)>);

		void blockLogService();

		void disconnectAll();
		void disconnect(int ref);
		bool isConnected(int ref);

		DECLARE_GAME_STATIC_INIT(Event);

		int wrap_lua(lua_State* L);

		static int pushWrappersToLua(lua_State* L, std::vector<VarWrapper> argList);

		void FireLater(std::vector<VarWrapper>* argList);
		void Fire(std::vector<VarWrapper>* argList);
		void Fire();
		void Fire(std::initializer_list<VarWrapper> argList);
	private:
		std::vector<EvtCon> connections;
		std::vector<lua_State*> waiting;
		std::vector<std::function<void(std::vector<VarWrapper>)>> chandlers;

		QString eventName;
		bool shouldBlockLogService;

		static int lua_connect(lua_State* L);
		static int lua_wait(lua_State* L);

		static int lua_toString(lua_State* L);
};

Event* checkEvent(lua_State* L, int n);

}}

#endif
