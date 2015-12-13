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

#include "Lighting.h"

BEGIN_INSTANCE
	DEFINE_CLASS(Lighting, false, isDataModel, Instance);

	Lighting::Lighting() : Instance(){
		Name = ClassName;
		Ambient = new OpenBlox::Type::Color3(0, 0, 0);
		FogColor = new OpenBlox::Type::Color3(0, 0, 0);
		FogStart = 0;
		FogEnd = 0;

		OpenBlox::OBEngine* eng = OpenBlox::OBEngine::getInstance();
		if(eng){
			Ogre::SceneManager* sceneMgr = eng->getSceneManager();
			if(sceneMgr){
				Ogre::ColourValue ambCol = sceneMgr->getAmbientLight();
				delete Ambient;
				Ambient = new OpenBlox::Type::Color3(ambCol.r, ambCol.g, ambCol.b);

				Ogre::ColourValue fogCol = sceneMgr->getFogColour();
				delete FogColor;
				FogColor = new OpenBlox::Type::Color3(fogCol.r, fogCol.g, fogCol.b);

				FogStart = sceneMgr->getFogStart();
				FogEnd = sceneMgr->getFogEnd();
			}
		}
	}

	Lighting::~Lighting(){}

	DEFINE_SERVICE(Lighting);

	int Lighting::lua_getAmbient(lua_State* L){
		Instance* inst = Instance::checkInstance(L, 1);
		if(Lighting* li = dynamic_cast<Lighting*>(inst)){
			return li->Ambient->wrap_lua(L);
		}
		return 0;
	}

	int Lighting::lua_setAmbient(lua_State* L){
		Instance* inst = Instance::checkInstance(L, 1);
		if(Lighting* li = dynamic_cast<Lighting*>(inst)){
			OpenBlox::Type::Color3* newV = OpenBlox::Type::checkColor3(L, 2);
			if(newV == NULL){
				newV = new OpenBlox::Type::Color3(0, 0, 0);
			}
			if(!li->Ambient->equals(newV)){
				li->Ambient = newV;

				OpenBlox::OBEngine* eng = OpenBlox::OBEngine::getInstance();
				if(eng){
					Ogre::SceneManager* sceneMgr = eng->getSceneManager();
					if(sceneMgr){
						sceneMgr->setAmbientLight(Ogre::ColourValue(newV->r, newV->g, newV->b));
					}
				}

				REPLICATE_PROPERTY_CHANGE(li, "Ambient", newV);

				propertyChanged("Ambient", li);
			}
		}
		return 0;
	}

	void Lighting::register_lua_property_getters(lua_State* L){
		Instance::register_lua_property_getters(L);

		luaL_Reg props[]{
			{"Ambient", lua_getAmbient},
			{NULL, NULL}
		};
		luaL_setfuncs(L, props, 0);
	}

	void Lighting::register_lua_property_setters(lua_State* L){
		Instance::register_lua_property_setters(L);

		luaL_Reg props[]{
			{"Ambient", lua_setAmbient},
			{NULL, NULL}
		};
		luaL_setfuncs(L, props, 0);
	}
END_INSTANCE
