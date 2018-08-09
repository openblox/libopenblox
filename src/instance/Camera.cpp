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

#include "instance/Camera.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"
#include "type/Vector3.h"
#include <math.h>

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Camera, true, false, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Camera::Camera(OBEngine* eng) : Instance(eng){
			Name = ClassName;

			Archivable = false;

			fov = 70.0f;

			CFrame = make_shared<Type::CFrame>(make_shared<Type::Vector3>(0, 0, 0), make_shared<Type::Vector3>(0, 0, 0));
#if HAVE_IRRLICHT
			irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
			if (irrDev){
				irr::scene::ISceneManager* irrSceneMgr = irrDev->getSceneManager();
				if (irrSceneMgr){
					camera = irrSceneMgr->addCameraSceneNode(0, irr::core::vector3df(0, 30, -40), irr::core::vector3df(0, 5, 0));
					camera->setFOV(irr::core::degToRad(fov));
				}
			}
#endif
		}

		Camera::~Camera(){
#if HAVE_IRRLICHT
			camera->remove();
#endif
		}

		bool Camera::SaveScreenshot(std::string file){
			return getEngine()->saveScreenshot(file);
		}

		void Camera::updateCFrame(){
			//shared_ptr<Type::Vector3> pos = camera_cframe->getPosition();
			//camera->setPosition(pos->toIrrlichtVector3df());
			//camera->setRotation(irr::core::vector3d<irr::f32>(camera_cframe->getX(), camera_cframe->getY(), camera_cframe->getZ()));
		}

		void Camera::setCFrame(shared_ptr<Type::CFrame> newCFrame){
			if (!newCFrame->equals(CFrame)){
				CFrame = newCFrame;
				updateCFrame();

				REPLICATE_PROPERTY_CHANGE(CFrame);
				propertyChanged("CFrame");
			}
		}

		shared_ptr<Type::CFrame> Camera::getCFrame(){
			return CFrame;
		}

		void Camera::updateFieldOfView(){
			if (fov < 1.00f){
				eng->getLogger()->log("FieldOfView set out of range, should be between 1.00 and 120.00; setting to 1.00", OB::OLL_Warning);
				fov = 1.00f;
			}
			if (fov > 120.00f){
				eng->getLogger()->log("FieldOfView set out of range, should be between 1.00 and 120.00; setting to 120.00", OB::OLL_Warning);
				fov = 120.00f;
			}

#if HAVE_IRRLICHT
			float fovInRadians = irr::core::degToRad(fov);
			camera->setFOV(fovInRadians);
#endif
		}

		void Camera::setFieldOfView(float newFieldOfView){
			if (newFieldOfView != fov){
				fov = newFieldOfView;

				updateFieldOfView();

				REPLICATE_PROPERTY_CHANGE(fov);
				propertyChanged("FieldOfView");
			}
		}

		float Camera::getFieldOfView(){
			return fov;
		}

		shared_ptr<Instance> Camera::cloneImpl(){
			return NULL;
		}

#if HAVE_ENET
		void Camera::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);

			peer->sendSetPropertyPacket(netId, "FieldOfView", make_shared<Type::VarWrapper>(fov));
			peer->sendSetPropertyPacket(netId, "CFrame", make_shared<Type::VarWrapper>(CFrame));
		}
#endif

		std::map<std::string, _PropertyInfo> Camera::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["FieldOfView"] = { "float", false, true, true };
			return propMap;
		}

		void Camera::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			if (prop == "FieldOfView"){
				setFieldOfView(val->asFloat());
				return;
			}
			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> Camera::getProperty(std::string prop){
			if (prop == "FieldOfView"){
				return make_shared<Type::VarWrapper>(getFieldOfView());
			}
			return Instance::getProperty(prop);
		}

		int Camera::lua_SaveScreenshot(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Camera> instC = dynamic_pointer_cast<Camera>(inst);
				if(instC){
					std::string desired = std::string(luaL_checkstring(L, 2));
					lua_pushboolean(L, instC->SaveScreenshot(desired));
					return 1;
				}
			}

			return 0;
		}

		int Camera::lua_setCFrame(lua_State* L){
			shared_ptr<Instance>inst = checkInstance(L, 1, false);
			if (inst){
				shared_ptr<Camera> instC = dynamic_pointer_cast<Camera>(inst);
				if (instC){
					shared_ptr<Type::CFrame> desired = Type::checkCFrame(L, 2, true, true);
					instC->setCFrame(desired);
				}
			}
			return 0;
		}

		int Camera::lua_getCFrame(lua_State* L){
			shared_ptr<Instance>inst = checkInstance(L, 1, false);
			if (inst){
				shared_ptr<Camera> instC = dynamic_pointer_cast<Camera>(inst);
				if (instC){
					shared_ptr<Type::CFrame> instCF = instC->getCFrame();
					if (instCF){
						return instCF->wrap_lua(L);
					}
					else{
						lua_pushnil(L);
						return 1;
					}
				}
			}
			lua_pushnil(L);
			return 1;
		}

		int Camera::lua_setFieldOfView(lua_State* L){
			shared_ptr<Instance>inst = checkInstance(L, 1, false);
			if (inst){
				shared_ptr<Camera> instC = dynamic_pointer_cast<Camera>(inst);
				if (instC){
					double newVal = luaL_checknumber(L, 2);
					instC->setFieldOfView(newVal);
				}
			}
			return 0;
		}

		int Camera::lua_getFieldOfView(lua_State* L){
			shared_ptr<Instance>inst = checkInstance(L, 1, false);
			if (inst){
				shared_ptr<Camera> instC = dynamic_pointer_cast<Camera>(inst);
				if (instC){
					double fov = instC->getFieldOfView();
					lua_pushnumber(L, fov);
					return 1;
				}
			}
			lua_pushnil(L);
			return 1;
		}

		void Camera::register_lua_methods(lua_State* L){
			Instance::register_lua_methods(L);

			luaL_Reg methods[] = {
				{"SaveScreenshot", lua_SaveScreenshot},
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}

		void Camera::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);

			luaL_Reg properties[] = {
				{"CFrame", lua_setCFrame},
				{"FieldOfView", lua_setFieldOfView},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Camera::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);

			luaL_Reg properties[] = {
				{"CFrame", lua_getCFrame},
				{"FieldOfView", lua_getFieldOfView},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
