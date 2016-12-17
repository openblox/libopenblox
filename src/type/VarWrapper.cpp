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

#include "type/VarWrapper.h"

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

		VarWrapper::VarWrapper(Instance::Instance* var){
			type = TYPE_INSTANCE;
			wrapped = var;
		}

		VarWrapper::VarWrapper(Type* var){
			type = TYPE_TYPE;
			wrapped = var;
		}

		VarWrapper::~VarWrapper(){}
	}
}
