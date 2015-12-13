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

#include "Camera.h"

BEGIN_INSTANCE
	DEFINE_NOINSTCLASS(Camera, false, Instance);

	Camera::Camera(Ogre::Camera* realCam) : Instance(){
		Name = ClassName;

		this->realCam = realCam;
	}

	Camera::~Camera(){}

	Instance* Camera::cloneImpl(Instance* newGuy){
		Instance::cloneImpl(newGuy);
		return newGuy;
	}

	int Camera::lua_getViewport(lua_State* L){
		Instance* inst = Instance::checkInstance(L, 1);
		if(Camera* cam = dynamic_cast<Camera*>(inst)){
			return (new OpenBlox::Type::Viewport(cam->realCam->getViewport()))->wrap_lua(L);
		}
		return 0;
	}

	void Camera::register_lua_property_getters(lua_State* L){
		Instance::register_lua_property_getters(L);

		luaL_Reg props[]{
			{"Viewport", lua_getViewport},
			{NULL, NULL}
		};
		luaL_setfuncs(L, props, 0);
	}
END_INSTANCE
