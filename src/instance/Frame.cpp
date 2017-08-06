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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox.	 If not, see <https://www.gnu.org/licenses/>.
 */

#include "instance/Frame.h"

#include "type/Color3.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Frame, true, false, GuiObject){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    Frame::Frame(OBEngine* eng) : GuiObject(eng){
			Name = ClassName;
		}

	    Frame::~Frame(){}

		shared_ptr<Instance> Frame::cloneImpl(){
			return NULL;
		}

		void Frame::render(){
			if(Visible){
				if(irr::IrrlichtDevice* irrDev = getEngine()->getIrrlichtDevice()){
					if(irr::video::IVideoDriver* irrDriv = irrDev->getVideoDriver()){
						shared_ptr<Type::Vector2> pos = getAbsolutePosition();
						shared_ptr<Type::Vector2> siz = getAbsoluteSize()->add(pos);
						
						irrDriv->draw2DRectangle(BackgroundColor3->toIrrlichtSColor(calculateBackgroundAlpha()), irr::core::rect<irr::s32>(pos->getX(), pos->getY(), siz->getX(), siz->getY()));
					}
				}
			}
		}
	}
}
