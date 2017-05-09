/*
 * Copyright (C) 2016-2017 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "type/VarWrapper.h"

#include "instance/Instance.h"

#include "type/Vector3.h"
#include "type/Vector2.h"
#include "type/Color3.h"

namespace OB{
	namespace Type{
		VarWrapper::VarWrapper(){
			wrapped = NULL;
			type = TYPE_UNKNOWN;
		}
		
		VarWrapper::VarWrapper(void* anything, VarType vtype){
			wrapped = anything;
			type = vtype;
		}

	    OB_PRIMATIVE_WRAPPER_IMPL(Int, int, INT);
		OB_PRIMATIVE_WRAPPER_IMPL(Double, double, DOUBLE);
		OB_PRIMATIVE_WRAPPER_IMPL(Float, float, FLOAT);
		OB_PRIMATIVE_WRAPPER_IMPL(Long, long, LONG);
		OB_PRIMATIVE_WRAPPER_IMPL(UnsignedLong, unsigned long, UNSIGNED_LONG);
		OB_PRIMATIVE_WRAPPER_IMPL(Bool, bool, BOOL);
		OB_PRIMATIVE_WRAPPER_IMPL(String, std::string, STRING);

		LuaReferencedWrapper::LuaReferencedWrapper(lua_State* L, int ref){
			this->ref = ref;
			this->L = L;
		}

		LuaReferencedWrapper::~LuaReferencedWrapper(){
		    luaL_unref(L, LUA_REGISTRYINDEX, ref);
		}

		VarWrapper::VarWrapper(shared_ptr<Instance::Instance> var){
			type = TYPE_INSTANCE;
			wrapped = malloc(sizeof(shared_ptr<Instance::Instance>));
			if(!wrapped){
				std::bad_alloc exception;
				throw exception;
			}
			new(wrapped) shared_ptr<Instance::Instance>(var);
		}

		VarWrapper::VarWrapper(shared_ptr<Type> var){
			type = TYPE_TYPE;
			wrapped = malloc(sizeof(shared_ptr<Type>));
			if(!wrapped){
				std::bad_alloc exception;
				throw exception;
			}
			new(wrapped) shared_ptr<Type>(var);
		}

		VarWrapper::VarWrapper(lua_State* L, int ref){
			type = TYPE_LUA_OBJECT;
			wrapped = (void*)new LuaReferencedWrapper(L, ref);
		}

		VarWrapper::~VarWrapper(){
			switch(type){
				case TYPE_INT: {
				    IntWrapper* iw = static_cast<IntWrapper*>(wrapped);
					delete iw;
					break;
				}
				case TYPE_DOUBLE: {
				    DoubleWrapper* dw = static_cast<DoubleWrapper*>(wrapped);
					delete dw;
					break;
				}
				case TYPE_FLOAT: {
				    FloatWrapper* fw = static_cast<FloatWrapper*>(wrapped);
					delete fw;
					break;
				}
				case TYPE_LONG: {
				    LongWrapper* lw = static_cast<LongWrapper*>(wrapped);
					delete lw;
					break;
				}
				case TYPE_UNSIGNED_LONG: {
					UnsignedLongWrapper* ulw = static_cast<UnsignedLongWrapper*>(wrapped);
					delete ulw;
					break;
				}
				case TYPE_BOOL: {
				    BoolWrapper* bw = static_cast<BoolWrapper*>(wrapped);
					delete bw;
					break;
				}
				case TYPE_STRING: {
				    StringWrapper* sw = static_cast<StringWrapper*>(wrapped);
					delete sw;
					break;
				}
				case TYPE_INSTANCE: {
					(*static_cast<shared_ptr<Instance::Instance>*>(wrapped)).reset();
					delete static_cast<shared_ptr<Instance::Instance>*>(wrapped);
					break;
				}
				case TYPE_TYPE: {
					(*static_cast<shared_ptr<Type>*>(wrapped)).reset();
				    delete static_cast<shared_ptr<Type>*>(wrapped);
					break;
				}
				case TYPE_LUA_OBJECT: {
					LuaReferencedWrapper* lrw = static_cast<LuaReferencedWrapper*>(wrapped);
					delete lrw;
					break;
				}
				case TYPE_UNKNOWN: {
					//This might end very badly!
				    free(wrapped);
					break;
				}
			}
			wrapped = NULL;
		}

		int VarWrapper::asInt(){
			if(type == TYPE_INT){
				return static_cast<IntWrapper*>(wrapped)->val;
			}
			return -1;
		}
		
		double VarWrapper::asDouble(){
			if(type == TYPE_DOUBLE){
				return static_cast<DoubleWrapper*>(wrapped)->val;
			}
			return -1;
		}
		
		float VarWrapper::asFloat(){
			if(type == TYPE_FLOAT){
				return static_cast<FloatWrapper*>(wrapped)->val;
			}
			return -1;
		}
		
		long VarWrapper::asLong(){
			if(type == TYPE_LONG){
				return static_cast<LongWrapper*>(wrapped)->val;
			}
			return -1;
		}
		
		unsigned long VarWrapper::asULong(){
			if(type == TYPE_UNSIGNED_LONG){
				return static_cast<UnsignedLongWrapper*>(wrapped)->val;
			}
			return 0;
		}
		
		bool VarWrapper::asBool(){
			if(type == TYPE_BOOL){
				return static_cast<BoolWrapper*>(wrapped)->val;
			}
			return false;
		}

		std::string VarWrapper::asString(){
			if(type == TYPE_STRING){
				return static_cast<StringWrapper*>(wrapped)->val;
			}
			return "";
		}

		shared_ptr<Instance::Instance> VarWrapper::asInstance(){
			if(type == TYPE_INSTANCE){
				shared_ptr<Instance::Instance> inst = *static_cast<shared_ptr<Instance::Instance>*>(wrapped);
			    return inst;
			}
			return NULL;
		}

		shared_ptr<Type> VarWrapper::asType(){
			if(type == TYPE_TYPE){
				shared_ptr<Type> tp = *static_cast<shared_ptr<Type>*>(wrapped);
				return tp;
			}
			return NULL;
		}

		shared_ptr<Vector3> VarWrapper::asVector3(){
			if(type == TYPE_TYPE){
				shared_ptr<Type> tp = *static_cast<shared_ptr<Type>*>(wrapped);
				return dynamic_pointer_cast<Vector3>(tp);
			}
			return NULL;
		}

		shared_ptr<Vector2> VarWrapper::asVector2(){
			if(type == TYPE_TYPE){
				shared_ptr<Type> tp = *static_cast<shared_ptr<Type>*>(wrapped);
				return dynamic_pointer_cast<Vector2>(tp);
			}
			return NULL;
		}

		shared_ptr<Color3> VarWrapper::asColor3(){
			if(type == TYPE_TYPE){
				shared_ptr<Type> tp = *static_cast<shared_ptr<Type>*>(wrapped);
				return dynamic_pointer_cast<Color3>(tp);
			}
			return NULL;
		}

		bool VarWrapper::valueEquals(shared_ptr<VarWrapper> other){
			if(!other || other->type != type){
				return false;
			}
			switch(type){
				case TYPE_INT: {
				    return static_cast<IntWrapper*>(wrapped)->val == static_cast<IntWrapper*>(other->wrapped)->val;
				}
				case TYPE_DOUBLE: {
					return static_cast<DoubleWrapper*>(wrapped)->val == static_cast<DoubleWrapper*>(other->wrapped)->val;
				}
				case TYPE_FLOAT: {
					return static_cast<FloatWrapper*>(wrapped)->val == static_cast<FloatWrapper*>(other->wrapped)->val;
					break;
				}
				case TYPE_LONG: {
					return static_cast<LongWrapper*>(wrapped)->val == static_cast<LongWrapper*>(other->wrapped)->val;
				}
				case TYPE_UNSIGNED_LONG: {
					return static_cast<UnsignedLongWrapper*>(wrapped)->val == static_cast<UnsignedLongWrapper*>(other->wrapped)->val;
				}
				case TYPE_BOOL: {
					return static_cast<BoolWrapper*>(wrapped)->val == static_cast<BoolWrapper*>(other->wrapped)->val;
				}
				case TYPE_STRING: {
					return static_cast<StringWrapper*>(wrapped)->val == static_cast<StringWrapper*>(other->wrapped)->val;
				}
				case TYPE_INSTANCE: {
				    shared_ptr<Instance::Instance> inst = *static_cast<shared_ptr<Instance::Instance>*>(wrapped);
					shared_ptr<Instance::Instance> oinst = *static_cast<shared_ptr<Instance::Instance>*>(other->wrapped);
				    return oinst == inst;
				}
				case TYPE_TYPE: {
					shared_ptr<Type> tp = *static_cast<shared_ptr<Type>*>(wrapped);
					shared_ptr<Type> otp = *static_cast<shared_ptr<Type>*>(other->wrapped);

				    if(tp){
						return tp->equals(otp);
					}else{
						return tp == otp;
					}
				}
				case TYPE_LUA_OBJECT: {
					return false;//This shouldn't really be seeing any equality tests..
				}
				case TYPE_NULL:
				case TYPE_UNKNOWN: {
				    return true;
				}
			}
			return false;
		}

		void VarWrapper::wrap_lua(lua_State* L){
			switch(type){
				case TYPE_INT: {
					lua_pushinteger(L, static_cast<IntWrapper*>(wrapped)->val);
					break;
				}
				case TYPE_DOUBLE: {
					lua_pushnumber(L, static_cast<DoubleWrapper*>(wrapped)->val);
					break;
				}
				case TYPE_FLOAT: {
					lua_pushnumber(L, static_cast<FloatWrapper*>(wrapped)->val);
					break;
				}
				case TYPE_LONG: {
					lua_pushnumber(L, static_cast<LongWrapper*>(wrapped)->val);
					break;
				}
				case TYPE_UNSIGNED_LONG: {
					lua_pushnumber(L, static_cast<UnsignedLongWrapper*>(wrapped)->val);
					break;
				}
				case TYPE_BOOL: {
					lua_pushboolean(L, static_cast<BoolWrapper*>(wrapped)->val);
					break;
				}
				case TYPE_STRING: {
					lua_pushstring(L, static_cast<StringWrapper*>(wrapped)->val.c_str());
					break;
				}
				case TYPE_INSTANCE: {
					shared_ptr<Instance::Instance> inst = *static_cast<shared_ptr<Instance::Instance>*>(wrapped);
					inst->wrap_lua(L);
					break;
				}
				case TYPE_TYPE: {
					shared_ptr<Type> tp = *static_cast<shared_ptr<Type>*>(wrapped);
					tp->wrap_lua(L);
					break;
				}
				case TYPE_LUA_OBJECT: {
					LuaReferencedWrapper* lrw = static_cast<LuaReferencedWrapper*>(wrapped);
				    lua_rawgeti(L, LUA_REGISTRYINDEX, lrw->ref);
					break;
				}
				case TYPE_NULL:
				case TYPE_UNKNOWN: {
					lua_pushnil(L);
					break;
				}
			}
		}
	}
}
