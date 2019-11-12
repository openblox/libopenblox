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

#include "instance/GuiBase2d.h"

#include "obtype.h"

#include "type/Color3.h"
#include "type/UDim2.h"

#ifndef OB_INST_GUIOBJECT
#define OB_INST_GUIOBJECT

namespace OB{
	namespace Instance{
		class GuiObject: public GuiBase2d{
			public:
				GuiObject(OBEngine* eng);
				virtual ~GuiObject();

				virtual std::vector<shared_ptr<GuiBase2d>> getRenderableChildren();
				virtual bool containsPoint(shared_ptr<Type::Vector2> p);
				virtual bool handleClick(shared_ptr<Type::Vector2> p);

				virtual shared_ptr<Type::Vector2> getAbsolutePosition();
				virtual shared_ptr<Type::Vector2> getAbsoluteSize();
				virtual double getAbsoluteRotation();
				virtual struct _ob_rect getAbsoluteClippingArea();

				virtual void setupClipping();
				virtual void undoClipping();

				virtual void renderInside();
				virtual void render();

				virtual bool isActive();
				virtual void setActive(bool active);
				virtual shared_ptr<Type::Color3> getBackgroundColor3();
				virtual void setBackgroundColor3(shared_ptr<Type::Color3> backgroundColor3);
				virtual double getBackgroundTransparency();
				virtual void setBackgroundTransparency(double backgroundTransparency);
				virtual shared_ptr<Type::Color3> getBorderColor3();
				virtual void setBorderColor3(shared_ptr<Type::Color3> borderColor3);
				virtual int getBorderSizePixel();
				virtual void setBorderSizePixel(int borderSizePixel);
				virtual bool getClipsDescendants();
				virtual void setClipsDescendants(bool clipsDescendants);
				virtual shared_ptr<Type::UDim2> getPosition();
				virtual void setPosition(shared_ptr<Type::UDim2> position);
				virtual shared_ptr<Type::UDim2> getSize();
				virtual void setSize(shared_ptr<Type::UDim2> size);
				virtual bool isVisible();
				virtual void setVisible(bool visible);
				virtual int getZIndex();
				virtual void setZIndex(int zIndex);
				virtual double getRotation();
				virtual void setRotation(double rotation);


#if HAVE_ENET
				virtual void replicateProperties(shared_ptr<NetworkReplicator> peer);
#endif

				virtual std::map<std::string, _PropertyInfo> getProperties();
				virtual shared_ptr<Type::VarWrapper> getProperty(std::string prop);
				virtual void setProperty(std::string prop, shared_ptr<Type::VarWrapper> val);

				int calculateBackgroundAlpha();

				DECLARE_LUA_METHOD(getAbsoluteRotation);
				DECLARE_LUA_METHOD(getActive);
				DECLARE_LUA_METHOD(setActive);
				DECLARE_LUA_METHOD(getBackgroundColor3);
				DECLARE_LUA_METHOD(setBackgroundColor3);
				DECLARE_LUA_METHOD(getBackgroundTransparency);
				DECLARE_LUA_METHOD(setBackgroundTransparency);
				DECLARE_LUA_METHOD(getBorderColor3);
				DECLARE_LUA_METHOD(setBorderColor3);
				DECLARE_LUA_METHOD(getBorderSizePixel);
				DECLARE_LUA_METHOD(setBorderSizePixel);
				DECLARE_LUA_METHOD(getClipsDescendants);
				DECLARE_LUA_METHOD(setClipsDescendants);
				DECLARE_LUA_METHOD(getPosition);
				DECLARE_LUA_METHOD(setPosition);
				DECLARE_LUA_METHOD(getSize);
				DECLARE_LUA_METHOD(setSize);
				DECLARE_LUA_METHOD(getVisible);
				DECLARE_LUA_METHOD(setVisible);
				DECLARE_LUA_METHOD(getZIndex);
				DECLARE_LUA_METHOD(setZIndex);
				DECLARE_LUA_METHOD(getRotation);
				DECLARE_LUA_METHOD(setRotation);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);

				DECLARE_CLASS(GuiObject);

				bool Active;
				shared_ptr<Type::Color3> BackgroundColor3;
				double BackgroundTransparency;
				shared_ptr<Type::Color3> BorderColor3;
				int BorderSizePixel;
				bool ClipsDescendants;
				shared_ptr<Type::UDim2> Position;
				shared_ptr<Type::UDim2> Size;
				bool Visible;
				int ZIndex;
				double Rotation;
		};
	}
}

#endif // OB_INST_GUIOBJECT


// Local Variables:
// mode: c++
// End:
