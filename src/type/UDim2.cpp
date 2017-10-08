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

#include "type/UDim2.h"

#include "instance/Instance.h"
#include "utility.h"

#include <sstream>

#include <math.h>

namespace OB{
	namespace Type{
		DEFINE_TYPE(UDim2){
			registerLuaType(eng, LuaTypeName, TypeName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters);
		}

		UDim2::UDim2(){
			x = make_shared<UDim>();
			y = make_shared<UDim>();
		}

		UDim2::UDim2(double xScale, double xOffset, double yScale, double yOffset){
			x = make_shared<UDim>(xScale, xOffset);
			y = make_shared<UDim>(yScale, yOffset);
		}

		UDim2::UDim2(std::string str){
			std::vector<std::string> fields = split(str);
			if(fields.size() == 4){
				std::string xScaleStr = trim(fields[0]);
				std::string xOffsetStr = trim(fields[1]);
				std::string yScaleStr = trim(fields[2]);
				std::string yOffsetStr = trim(fields[3]);

				double xScale = 0;
				double xOffset = 0;
				double yScale = 0;
				double yOffset = 0;

				if(xScaleStr.length() > 0){
					xScale = atof(xScaleStr.c_str());
				}

				if(xOffsetStr.length() > 0){
					xOffset = atof(xOffsetStr.c_str());
				}

				if(yScaleStr.length() > 0){
					yScale = atof(yScaleStr.c_str());
				}

				if(yOffsetStr.length() > 0){
					yOffset = atof(yOffsetStr.c_str());
				}
			}else{
				x = make_shared<UDim>();
				y = make_shared<UDim>();
			}
		}

		UDim2::~UDim2(){}

		shared_ptr<UDim> UDim2::getX(){
			return x;
		}

		shared_ptr<UDim> UDim2::getY(){
			return y;
		}

		shared_ptr<UDim2> UDim2::add(shared_ptr<UDim2> u){
			if(!u){
				return make_shared<UDim2>(x->getScale(), x->getOffset(), y->getScale(), y->getOffset());
			}
			shared_ptr<UDim> uX = u->getX();
			shared_ptr<UDim> uY = u->getY();
			return make_shared<UDim2>(x->getScale() + uX->getScale(), x->getOffset() + uX->getOffset(), y->getScale() + uY->getScale(), y->getOffset() + uY->getOffset());
		}

		shared_ptr<UDim2> UDim2::sub(shared_ptr<UDim2> u){
			if(!u){
				return make_shared<UDim2>(x->getScale(), x->getOffset(), y->getScale(), y->getOffset());
			}
			shared_ptr<UDim> uX = u->getX();
			shared_ptr<UDim> uY = u->getY();
			return make_shared<UDim2>(x->getScale() - uX->getScale(), x->getOffset() - uX->getOffset(), y->getScale() - uY->getScale(), y->getOffset() - uY->getOffset());
		}

		bool UDim2::equals(shared_ptr<Type> other){
			shared_ptr<UDim2> co = dynamic_pointer_cast<UDim2>(other);
			if(!co){
				return false;
			}

			return co->x->equals(x) && co->y->equals(y);
		}

		std::string UDim2::toString(){
			return x->toString() + ", " + y->toString();
		}

		int UDim2::lua_getX(lua_State* L){
			shared_ptr<UDim2> LuaUDim2 = checkUDim2(L, 1, false);
			if(!LuaUDim2){
				return 0;
			}

			return LuaUDim2->getX()->wrap_lua(L);
		}

		int UDim2::lua_getY(lua_State* L){
			shared_ptr<UDim2> LuaUDim2 = checkUDim2(L, 1, false);
			if(!LuaUDim2){
				return 0;
			}

			return LuaUDim2->getY()->wrap_lua(L);
		}

		int UDim2::lua_eq(lua_State* L){
			shared_ptr<UDim2> LuaUDim2 = checkUDim2(L, 1, false);

			if(LuaUDim2){
				shared_ptr<UDim2> OtherUDim2 = checkUDim2(L, 2, false);
				lua_pushboolean(L, LuaUDim2->equals(OtherUDim2));
			}

			lua_pushboolean(L, false);
			return 1;
		}

		int UDim2::lua_add(lua_State* L){
			shared_ptr<UDim2> LuaUDim2 = checkUDim2(L, 1, false);
			if(!LuaUDim2){
				return 0;
			}

			shared_ptr<UDim2> OtherUDim2 = checkUDim2(L, 2, true, false);
			return LuaUDim2->add(OtherUDim2)->wrap_lua(L);
		}

		int UDim2::lua_sub(lua_State* L){
			shared_ptr<UDim2> LuaUDim2 = checkUDim2(L, 1, false);
			if(!LuaUDim2){
				return 0;
			}

			shared_ptr<UDim2> OtherUDim2 = checkUDim2(L, 2, true, false);
			return LuaUDim2->sub(OtherUDim2)->wrap_lua(L);
		}

		void UDim2::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Type::lua_toString},
				{"__eq", lua_eq},
				{"__add", lua_add},
				{"__sub", lua_sub},
				{"__gc", Type::lua_gc},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void UDim2::register_lua_methods(lua_State* L){
			luaL_Reg methods[] = {
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}

		void UDim2::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"X", Instance::Instance::lua_readOnlyProperty},
				{"Y", Instance::Instance::lua_readOnlyProperty},
				{"x", Instance::Instance::lua_readOnlyProperty},
				{"y", Instance::Instance::lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void UDim2::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[] = {
				{"X", lua_getX},
				{"Y", lua_getY},
				{"x", lua_getX},
				{"y", lua_getY},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		shared_ptr<UDim2> checkUDim2(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}

			if(lua_isuserdata(L, index)){
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					luaL_getmetatable(L, "luaL_Type_UDim2");
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return dynamic_pointer_cast<UDim2>(*static_cast<shared_ptr<Type>*>(udata));
					}
					lua_pop(L, 1);
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "UDim2");
			}
			return NULL;
		}
	}
}
