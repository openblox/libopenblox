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

#include "instance/Workspace.h"

#include "utility.h"

#include "instance/Camera.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Workspace, false, isDataModel, Model){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Workspace::Workspace(OBEngine* eng) : Model(eng){
			Name = ClassName;
			netId = OB_NETID_WORKSPACE;

			Gravity = make_shared<Type::Vector3>(0, -196.2, 0);
			FallenPartsDestroyHeight = -1000;
			DestroyFallenParts = true;

#if HAVE_BULLET
			broadphase = new btDbvtBroadphase();
			collisionConfiguration = new btDefaultCollisionConfiguration();
			dispatcher = new btCollisionDispatcher(collisionConfiguration);
			solver = new btSequentialImpulseConstraintSolver();
			dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
			updateGravity();
#endif

#if HAVE_IRRLICHT
			irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
			if(irrDev){
				irr::scene::ISceneManager* sceneMgr = irrDev->getSceneManager();
				if(sceneMgr){
					irrNode = sceneMgr->addEmptySceneNode();
				}
			}
#endif
		}

		Workspace::~Workspace(){
#if HAVE_BULLET
			delete dynamicsWorld;
			delete solver;
			delete dispatcher;
			delete collisionConfiguration;
			delete broadphase;
#endif
		}

		shared_ptr<Instance> Workspace::cloneImpl(){
			return NULL;
		}

		double Workspace::getDistributedGameTime(){
			ob_uint64 startTime = eng->getStartTime();
			ob_uint64 curTime = currentTimeMillis();

			double runTime = (double)((curTime - startTime) / 1000.0);
			return runTime;
		}

		shared_ptr<Instance> Workspace::getCurrentCamera(){
			return CurrentCamera;
		}

		void Workspace::setCurrentCamera(shared_ptr<Instance> inst){
			if(shared_ptr<Camera> cInst = dynamic_pointer_cast<Camera>(inst)){
				CurrentCamera = cInst;

				if(CurrentCamera){
					CurrentCamera->setParent(std::enable_shared_from_this<Instance>::shared_from_this(), true);
				}
			}
		}

		shared_ptr<Type::Vector3> Workspace::getGravity(){
			return Gravity;
		}

		void Workspace::setGravity(shared_ptr<Type::Vector3> gravity){
			if(gravity == NULL){
				shared_ptr<Type::Vector3> vec3 = make_shared<Type::Vector3>(0, 0, 0);
				if(!vec3->equals(Gravity)){
					Gravity = vec3;

					updateGravity();
					REPLICATE_PROPERTY_CHANGE(Gravity);
					propertyChanged("Gravity");
				}
			}else{
				if(!gravity->equals(Gravity)){
					Gravity = gravity;

					updateGravity();
					REPLICATE_PROPERTY_CHANGE(Gravity);
					propertyChanged("Gravity");
				}
			}
		}

		double Workspace::getFallenPartsDestroyHeight(){
			return FallenPartsDestroyHeight;
		}

		void Workspace::setFallenPartsDestroyHeight(double fpdh){
			if(FallenPartsDestroyHeight != fpdh){
				FallenPartsDestroyHeight = fpdh;

				REPLICATE_PROPERTY_CHANGE(FallenPartsDestroyHeight);
				propertyChanged("FallenPartsDestroyHeight");
			}
		}

		bool Workspace::getDestroyFallenParts(){
			return DestroyFallenParts;
		}

		void Workspace::setDestroyFallenParts(bool dfp){
			if(DestroyFallenParts != dfp){
				DestroyFallenParts = dfp;

				REPLICATE_PROPERTY_CHANGE(DestroyFallenParts);
				propertyChanged("DestroyFallenParts");
			}
		}

		void Workspace::updateGravity(){
#if HAVE_BULLET
			dynamicsWorld->setGravity(getGravity()->toBulletVector3());
#endif
		}

#if HAVE_ENET
		void Workspace::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);

			peer->sendSetPropertyPacket(netId, "Gravity", make_shared<Type::VarWrapper>(Gravity));
			peer->sendSetPropertyPacket(netId, "FallenPartsDestroyHeight", make_shared<Type::VarWrapper>(FallenPartsDestroyHeight));
			peer->sendSetPropertyPacket(netId, "DestroyFallenParts", make_shared<Type::VarWrapper>(DestroyFallenParts));
		}
#endif

#if HAVE_PUGIXML
		std::string Workspace::serializedID(){
			shared_ptr<OBSerializer> serializer = eng->getSerializer();
			serializer->SetID(shared_from_this(), getClassName());

			return Instance::serializedID();
		}
#endif

		std::map<std::string, _PropertyInfo> Workspace::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["CurrentCamera"] = {"Instance", false, true, false};
			propMap["Gravity"] = {"Vector3", false, true, true};
			propMap["FallenPartsDestroyHeight"] = {"double", false, true, true};
			propMap["DestroyFallenParts"] = {"bool", false, true, true};

			return propMap;
		}

		void Workspace::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			if(prop == "CurrentCamera"){
				setCurrentCamera(val->asInstance());
				return;
			}
			if(prop == "Gravity"){
				setGravity(val->asVector3());
				return;
			}
			if(prop == "FallenPartsDestroyHeight"){
				setFallenPartsDestroyHeight(val->asDouble());
				return;
			}
			if(prop == "DestroyFallenParts"){
				setDestroyFallenParts(val->asBool());
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> Workspace::getProperty(std::string prop){
			if(prop == "CurrentCamera"){
				return make_shared<Type::VarWrapper>(getCurrentCamera());
			}
			if(prop == "Gravity"){
				return make_shared<Type::VarWrapper>(getGravity());
			}
			if(prop == "FallenPartsDestroyHeight"){
				return make_shared<Type::VarWrapper>(getFallenPartsDestroyHeight());
			}
			if(prop == "DestroyFallenParts"){
				return make_shared<Type::VarWrapper>(getDestroyFallenParts());
			}

			return Instance::getProperty(prop);
		}

		int Workspace::lua_getDistributedGameTime(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Workspace> instW = dynamic_pointer_cast<Workspace>(inst);
				if(instW){
					lua_pushnumber(L, instW->getDistributedGameTime());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Workspace::lua_setCurrentCamera(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Workspace> instWS = dynamic_pointer_cast<Workspace>(inst);
				if(instWS){
					shared_ptr<Instance> newV = checkInstance(L, 2, false);
					instWS->setCurrentCamera(newV);
				}
			}

			return 0;
		}

		int Workspace::lua_getCurrentCamera(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Workspace> instWS = dynamic_pointer_cast<Workspace>(inst);
				if(instWS){
					shared_ptr<Instance> tV = instWS->getCurrentCamera();
					if(tV){
						tV->wrap_lua(L);
					}else{
						lua_pushnil(L);
					}
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Workspace::lua_getGravity(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Workspace> instW = dynamic_pointer_cast<Workspace>(inst);
				if(instW){
					shared_ptr<Type::Vector3> vec3 = instW->getGravity();
					if(vec3){
						return vec3->wrap_lua(L);
					}else{
						lua_pushnil(L);
						return 1;
					}
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Workspace::lua_setGravity(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Workspace> instW = dynamic_pointer_cast<Workspace>(inst);
				if(instW){
					shared_ptr<Type::Vector3> vec3 = Type::checkVector3(L, 2, true, true);
					instW->setGravity(vec3);
				}
			}

			return 0;
		}

		int Workspace::lua_setFallenPartsDestroyHeight(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Workspace> instW = dynamic_pointer_cast<Workspace>(inst);
				if(instW){
					double newV = luaL_checknumber(L, 2);
					instW->setFallenPartsDestroyHeight(newV);
				}
			}

			return 0;
		}

		int Workspace::lua_getFallenPartsDestroyHeight(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Workspace> instW = dynamic_pointer_cast<Workspace>(inst);
				if(instW){
					lua_pushnumber(L, instW->getFallenPartsDestroyHeight());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Workspace::lua_setDestroyFallenParts(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Workspace> instW = dynamic_pointer_cast<Workspace>(inst);
				if(instW){
					bool newV = lua_toboolean(L, 2);
					instW->setDestroyFallenParts(newV);
				}
			}

			return 0;
		}

		int Workspace::lua_getDestroyFallenParts(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Workspace> instW = dynamic_pointer_cast<Workspace>(inst);
				if(instW){
					lua_pushboolean(L, instW->getDestroyFallenParts());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		void Workspace::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);

			luaL_Reg properties[] = {
				{"CurrentCamera", lua_setCurrentCamera},
				{"DistributedGameTime", lua_readOnlyProperty},
				{"Gravity", lua_setGravity},
				{"FallenPartsDestroyHeight", lua_setFallenPartsDestroyHeight},
				{"DestroyFallenParts", lua_setDestroyFallenParts},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Workspace::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);

			luaL_Reg properties[] = {
				{"CurrentCamera", lua_getCurrentCamera},
				{"DistributedGameTime", lua_getDistributedGameTime},
				{"Gravity", lua_getGravity},
				{"FallenPartsDestroyHeight", lua_setFallenPartsDestroyHeight},
				{"DestroyFallenParts", lua_setDestroyFallenParts},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}
	}
}
