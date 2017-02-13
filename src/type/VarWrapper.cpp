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
				case TYPE_UNKNOWN: {
					//This might end very badly!
				    free(wrapped);
					break;
				}
			}
			wrapped = NULL;
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
					break;
				}
				case TYPE_NULL: {
					lua_pushnil(L);
					break;
				}
				case TYPE_UNKNOWN: {
					lua_pushnil(L);
					break;
				}
			}
		}
	}
}
