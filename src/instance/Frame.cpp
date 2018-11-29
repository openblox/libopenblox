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

#include "instance/Frame.h"

#include "type/Color3.h"

#include "OBRenderUtils.h"

#if HAVE_IRRLICHT
#include "GL/gl.h"
#endif

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
			shared_ptr<Frame> clone = make_shared<Frame>(eng);
			clone->Archivable = Archivable;
			clone->Name = Name;
			clone->ParentLocked = ParentLocked;

			clone->Active = Active;
			clone->BackgroundColor3 = BackgroundColor3;
			clone->BackgroundTransparency = BackgroundTransparency;
			clone->BorderColor3 = BorderColor3;
			clone->BorderSizePixel = BorderSizePixel;
			clone->ClipsDescendants = ClipsDescendants;
			clone->Position = Position;
			clone->Size = Size;
			clone->ZIndex = ZIndex;

			return clone;
		}

		void Frame::render(){
#if HAVE_IRRLICHT
			if(Visible){
				if(irr::IrrlichtDevice* irrDev = getEngine()->getIrrlichtDevice()){
					if(irr::video::IVideoDriver* irrDriv = irrDev->getVideoDriver()){
						shared_ptr<OBRenderUtils> renderUtils = getEngine()->getRenderUtils();
						if(renderUtils){
							renderUtils->prepare2DMode();

							shared_ptr<Type::Color3> bgColor = BackgroundColor3;
							double bgTrans = BackgroundTransparency;
							shared_ptr<Type::Color3> borderColor = BorderColor3;
							int borderSize = BorderSizePixel;
							shared_ptr<Type::Vector2> pos = getAbsolutePosition();
							shared_ptr<Type::Vector2> siz = getAbsoluteSize()->add(pos);

							glColor4d(bgColor->getR(), bgColor->getG(), bgColor->getB(), 1 - bgTrans);

							glRectd(pos->getX(), pos->getY(), siz->getX(), siz->getY());

							glColor4d(borderColor->getR(), borderColor->getG(), borderColor->getB(), 1 - bgTrans);
							//Border Top
							glRectd(pos->getX() - borderSize, pos->getY() - borderSize, siz->getX() + borderSize, pos->getY());
							//Border Left
							glRectd(pos->getX() - borderSize, pos->getY() - borderSize, pos->getX(), siz->getY() + borderSize);
							//Border Right
							glRectd(siz->getX(), pos->getY() - borderSize, siz->getX() + borderSize, siz->getY() + borderSize);
							//Border Bottom
							glRectd(pos->getX() - borderSize, siz->getY(), siz->getX() + borderSize, siz->getY() + borderSize);

						    renderUtils->end2DMode();

							GuiObject::render();
						}
					}
				}
			}
#endif
		}
	}
}
