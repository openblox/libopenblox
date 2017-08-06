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

#ifndef OB_TYPE_VARWRAPPER
#define OB_TYPE_VARWRAPPER

#include "type/Type.h"

namespace OB{
	namespace Instance{
		class Instance;
	};
	
	namespace Type{
		class Vector3;
		class Vector2;
		class Color3;
		class UDim;
		class UDim2;
		
		/**
		 * Describes the type of the wrapped variable for conversion
		 * to Lua.
		 *
		 * @author John M. Harris, Jr.
		 */
	    enum VarType: size_t{
			TYPE_INT,
			TYPE_DOUBLE,
			TYPE_FLOAT,
			TYPE_LONG,
			TYPE_UNSIGNED_LONG,
			TYPE_BOOL,
			TYPE_STRING,
			TYPE_INSTANCE,
			TYPE_TYPE,
			TYPE_LUA_OBJECT,
			TYPE_NULL,
			TYPE_UNKNOWN
		};

		#define OB_PRIMATIVE_WRAPPER(ntype, type) \
		class ntype##Wrapper{ \
			public: \
				ntype##Wrapper(type val); \
				type val; \
		}

		#define OB_PRIMATIVE_WRAPPER_IMPL(ntype, typee, type_C) \
		ntype##Wrapper::ntype##Wrapper(typee val){ \
			this->val = val; \
		}\
		VarWrapper::VarWrapper(typee var){ \
			type = TYPE_##type_C; \
			wrapped = reinterpret_cast<void*>(new ntype##Wrapper(var)); \
		}

	    OB_PRIMATIVE_WRAPPER(Int, int);
		OB_PRIMATIVE_WRAPPER(Double, double);
		OB_PRIMATIVE_WRAPPER(Float, float);
		OB_PRIMATIVE_WRAPPER(Long, long);
		OB_PRIMATIVE_WRAPPER(UnsignedLong, unsigned long);
		OB_PRIMATIVE_WRAPPER(Bool, bool);
		OB_PRIMATIVE_WRAPPER(String, std::string);

		class LuaReferencedWrapper{
			public:
				LuaReferencedWrapper(lua_State* L, int ref);
				~LuaReferencedWrapper();

				lua_State* L;
				int ref;
		};

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
				VarWrapper(std::string var);
				VarWrapper(shared_ptr<Instance::Instance> var);
				VarWrapper(shared_ptr<Type> var);
				VarWrapper(lua_State* L, int ref);
				~VarWrapper();

				int asInt();
				double asDouble();
				float asFloat();
				long asLong();
				unsigned long asULong();
				bool asBool();
				std::string asString();
				shared_ptr<Instance::Instance> asInstance();
				shared_ptr<Type> asType();
				shared_ptr<Vector3> asVector3();
				shared_ptr<Vector2> asVector2();
				shared_ptr<Color3> asColor3();
				shared_ptr<UDim> asUDim();
				shared_ptr<UDim2> asUDim2();

				bool valueEquals(shared_ptr<VarWrapper> other);

				void wrap_lua(lua_State* L);

				void* wrapped;
				VarType type;
		};
	}
}

#endif // OB_TYPE_VARWRAPPER


// Local Variables:
// mode: c++
// End:
