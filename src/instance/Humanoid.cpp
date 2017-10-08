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

#include "instance/Humanoid.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Humanoid, true, false, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Humanoid::Humanoid(OBEngine* eng) : Instance(eng){
			Name = ClassName;

			Health = 100;
			MaxHealth = 100;
			Invincible = false;
			NameVisible = true;
			HealthVisible = true;
			JumpPower = 1;
			WalkSpeed = 1;
			MoveDirection = NULL;
			State = 1;
			WalkTarget = NULL;

			MoveToFinished = make_shared<Type::Event>("MoveToFinished");
			HealthChanged = make_shared<Type::Event>("HealthChanged");
			Died = make_shared<Type::Event>("Died");
		}

		Humanoid::~Humanoid(){}

		shared_ptr<Instance> Humanoid::cloneImpl(){
			shared_ptr<Humanoid> h = make_shared<Humanoid>(eng);
			h->Archivable = Archivable;
			h->Name = Name;
			h->ParentLocked = ParentLocked;

			h->Health = Health;
			h->MaxHealth = MaxHealth;
			h->Invincible = Invincible;
			h->NameVisible = NameVisible;
			h->HealthVisible = HealthVisible;
			h->JumpPower = JumpPower;
			h->WalkSpeed = WalkSpeed;
			h->MoveDirection = MoveDirection;
			h->State = State;
			h->WalkTarget = WalkTarget;

			return h;
		}

		double Humanoid::getHealth(){
			return Health;
		}
		void Humanoid::setHealth(double health){
			if(health != Health){
				double oldHealth = Health;
				Health = health;

				std::vector<shared_ptr<Type::VarWrapper>> args = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(oldHealth), make_shared<Type::VarWrapper>()});
				HealthChanged->Fire(eng, args);

				if(Health <= 0){
					Died->Fire(eng);
				}

				REPLICATE_PROPERTY_CHANGE(Health);
				propertyChanged("Health");
			}
		}

		double Humanoid::getMaxHealth(){
			return MaxHealth;
		}

		void Humanoid::setMaxHealth(double maxHealth){
			if(maxHealth != MaxHealth){
				MaxHealth = maxHealth;

				if(Health > MaxHealth){
					setHealth(MaxHealth);
				}

				REPLICATE_PROPERTY_CHANGE(MaxHealth);
				propertyChanged("MaxHealth");
			}
		}

		bool Humanoid::getInvincible(){
			return Invincible;
		}

		void Humanoid::setInvincible(bool invincible){
			if(invincible != Invincible){
				Invincible = invincible;

				REPLICATE_PROPERTY_CHANGE(Invincible);
				propertyChanged("Invincible");
			}
		}

		bool Humanoid::getNameVisible(){
			return NameVisible;
		}

		void Humanoid::setNameVisible(bool nameVisible){
			if(nameVisible != NameVisible){
				NameVisible = nameVisible;

				REPLICATE_PROPERTY_CHANGE(NameVisible);
				propertyChanged("NameVisible");
			}
		}

		bool Humanoid::getHealthVisible(){
			return HealthVisible;
		}

		void Humanoid::setHealthVisible(bool healthVisible){
			if(healthVisible != HealthVisible){
				HealthVisible = healthVisible;

				REPLICATE_PROPERTY_CHANGE(HealthVisible);
				propertyChanged("HealthVisible");
			}
		}

		double Humanoid::getJumpPower(){
			return JumpPower;
		}

		void Humanoid::setJumpPower(double jumpPower){
			if(jumpPower != JumpPower){
				JumpPower = jumpPower;

				REPLICATE_PROPERTY_CHANGE(JumpPower);
				propertyChanged("JumpPower");
			}
		}

		double Humanoid::getWalkSpeed(){
			return WalkSpeed;
		}

		void Humanoid::setWalkSpeed(double walkSpeed){
			if(walkSpeed != WalkSpeed){
				WalkSpeed = walkSpeed;

				REPLICATE_PROPERTY_CHANGE(WalkSpeed);
				propertyChanged("WalkSpeed");
			}
		}

		shared_ptr<Type::Vector3> Humanoid::getMoveDirection(){
			return MoveDirection;
		}

		int Humanoid::getState(){
			return State;
		}

		void Humanoid::setState(int state){
			if(state != State){
				State = state;

				REPLICATE_PROPERTY_CHANGE(State);
				propertyChanged("State");
			}
		}

		shared_ptr<Type::Vector3> Humanoid::getWalkTarget(){
			return WalkTarget;
		}

		double Humanoid::TakeDamage(double damage, std::string damage_metadata){
			double newHealth = Health - damage;

			if(!Invincible && Health != newHealth){
				double oldHealth = Health;
				Health = newHealth;

				std::vector<shared_ptr<Type::VarWrapper>> args = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(oldHealth), make_shared<Type::VarWrapper>(damage_metadata)});
				HealthChanged->Fire(eng, args);

				if(Health <= 0){
					Died->Fire(eng);
				}

				REPLICATE_PROPERTY_CHANGE(Health);
				propertyChanged("Health");
			}

			return 0;
		}
		void Humanoid::Move(shared_ptr<Type::Vector3> direction){
		}
		void Humanoid::MoveTo(shared_ptr<Type::Vector3> location){
		}

#if HAVE_ENET
		void Humanoid::replicateProperties(shared_ptr<NetworkReplicator> peer){
			Instance::replicateProperties(peer);

			peer->sendSetPropertyPacket(netId, "Health", make_shared<Type::VarWrapper>(Health));
			peer->sendSetPropertyPacket(netId, "MaxHealth", make_shared<Type::VarWrapper>(MaxHealth));
			peer->sendSetPropertyPacket(netId, "Invincible", make_shared<Type::VarWrapper>(Invincible));
			peer->sendSetPropertyPacket(netId, "NameVisible", make_shared<Type::VarWrapper>(NameVisible));
			peer->sendSetPropertyPacket(netId, "HealthVisible", make_shared<Type::VarWrapper>(HealthVisible));
			peer->sendSetPropertyPacket(netId, "JumpPower", make_shared<Type::VarWrapper>(JumpPower));
			peer->sendSetPropertyPacket(netId, "WalkSpeed", make_shared<Type::VarWrapper>(WalkSpeed));
			peer->sendSetPropertyPacket(netId, "MoveDirection", make_shared<Type::VarWrapper>(MoveDirection));
			peer->sendSetPropertyPacket(netId, "State", make_shared<Type::VarWrapper>(State));
			peer->sendSetPropertyPacket(netId, "WalkTarget", make_shared<Type::VarWrapper>(WalkTarget));
		}
#endif

		std::map<std::string, _PropertyInfo> Humanoid::getProperties(){
			std::map<std::string, _PropertyInfo> propMap = Instance::getProperties();
			propMap["Health"] = {"double", false, true, true};
			propMap["MaxHealth"] = {"double", false, true, true};
			propMap["Invincible"] = {"bool", false, true, true};
			propMap["NameVisible"] = {"bool", false, true, true};
			propMap["HealthVisible"] = {"bool", false, true, true};
			propMap["JumpPower"] = {"double", false, true, true};
			propMap["WalkSpeed"] = {"double", false, true, true};
			propMap["MoveDirection"] = {"Vector3", true, true, true};
			propMap["State"] = {"int", false, true, true};
			propMap["WalkTarget"] = {"Vector3", true, true, true};

			return propMap;
		}

		void Humanoid::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
			if(prop == "Health"){
				setHealth(val->asDouble());
				return;
			}
			if(prop == "MaxHealth"){
				setMaxHealth(val->asDouble());
				return;
			}
			if(prop == "Invincible"){
				setInvincible(val->asBool());
				return;
			}
			if(prop == "NameVisible"){
				setNameVisible(val->asBool());
				return;
			}
			if(prop == "HealthVisible"){
				setHealthVisible(val->asBool());
				return;
			}
			if(prop == "JumpPower"){
				setJumpPower(val->asDouble());
				return;
			}
			if(prop == "WalkSpeed"){
				setWalkSpeed(val->asDouble());
				return;
			}
			if(prop == "State"){
				setState(val->asInt());
				return;
			}
			if(prop == "WalkTarget" || prop == "MoveDirection"){
				// TODO: Detect whether from replication or
				// other, allow replication to set read-only
				throw new OBException(prop + " is a read-only property.");
				return;
			}

			Instance::setProperty(prop, val);
		}

		shared_ptr<Type::VarWrapper> Humanoid::getProperty(std::string prop){
			if(prop == "Health"){
				return make_shared<Type::VarWrapper>(getHealth());
			}
			if(prop == "MaxHealth"){
				return make_shared<Type::VarWrapper>(getMaxHealth());
			}
			if(prop == "Invincible"){
				return make_shared<Type::VarWrapper>(getInvincible());
			}
			if(prop == "NameVisible"){
				return make_shared<Type::VarWrapper>(getNameVisible());
			}
			if(prop == "HealthVisible"){
				return make_shared<Type::VarWrapper>(getNameVisible());
			}
			if(prop == "JumpPower"){
				return make_shared<Type::VarWrapper>(getJumpPower());
			}
			if(prop == "WalkSpeed"){
				return make_shared<Type::VarWrapper>(getWalkSpeed());
			}
			if(prop == "MoveDirection"){
				return make_shared<Type::VarWrapper>(getMoveDirection());
			}
			if(prop == "State"){
				return make_shared<Type::VarWrapper>(getState());
			}
			if(prop == "WalkTarget"){
				return make_shared<Type::VarWrapper>(getWalkTarget());
			}

			return Instance::getProperty(prop);
		}

		int Humanoid::lua_getHealth(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					lua_pushnumber(L, instH->getHealth());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Humanoid::lua_setHealth(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					double newV = luaL_checknumber(L, 2);
					instH->setHealth(newV);
				}
			}

			return 0;
		}

		int Humanoid::lua_getMaxHealth(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					lua_pushnumber(L, instH->getMaxHealth());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Humanoid::lua_setMaxHealth(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					double newV = luaL_checknumber(L, 2);
					instH->setMaxHealth(newV);
				}
			}

			return 0;
		}

		int Humanoid::lua_getInvincible(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					lua_pushboolean(L, instH->getInvincible());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Humanoid::lua_setInvincible(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					bool newV = lua_toboolean(L, 2);
					instH->setInvincible(newV);
				}
			}

			return 0;
		}

		int Humanoid::lua_getNameVisible(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					lua_pushboolean(L, instH->getNameVisible());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Humanoid::lua_setNameVisible(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					bool newV = lua_toboolean(L, 2);
					instH->setNameVisible(newV);
				}
			}

			return 0;
		}

		int Humanoid::lua_getHealthVisible(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					lua_pushboolean(L, instH->getHealthVisible());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Humanoid::lua_setHealthVisible(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					bool newV = lua_toboolean(L, 2);
					instH->setHealthVisible(newV);
				}
			}

			return 0;
		}

		int Humanoid::lua_getJumpPower(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					lua_pushnumber(L, instH->getJumpPower());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Humanoid::lua_setJumpPower(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					double newV = luaL_checknumber(L, 2);
					instH->setJumpPower(newV);
				}
			}

			return 0;
		}

		int Humanoid::lua_getWalkSpeed(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					lua_pushnumber(L, instH->getWalkSpeed());
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}

		int Humanoid::lua_setWalkSpeed(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					double newV = luaL_checknumber(L, 2);
					instH->setWalkSpeed(newV);
				}
			}

			return 0;
		}

		int Humanoid::lua_getMoveDirection(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					shared_ptr<Type::Vector3> vec3 = instH->getMoveDirection();
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

		int Humanoid::lua_getWalkTarget(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					shared_ptr<Type::Vector3> vec3 = instH->getWalkTarget();
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

		int Humanoid::lua_TakeDamage(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					double dmg = luaL_checknumber(L, 2);

					std::string dmg_meta = "";

					if(!lua_isnoneornil(L, 3)){
						dmg_meta = std::string(luaL_checkstring(L, 3));
					}

					lua_pushnumber(L, instH->TakeDamage(dmg, dmg_meta));
					return 1;
				}
			}

			return luaL_error(L, COLONERR, "TakeDamage");
		}

		int Humanoid::lua_Move(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					//TODO:
					return 0;
				}
			}

			return luaL_error(L, COLONERR, "Move");
		}

		int Humanoid::lua_MoveTo(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Humanoid> instH = dynamic_pointer_cast<Humanoid>(inst);
				if(instH){
					//TODO:
					return 0;
				}
			}

			return luaL_error(L, COLONERR, "MoveTo");
		}

		void Humanoid::register_lua_property_setters(lua_State* L){
			Instance::register_lua_property_setters(L);

			luaL_Reg properties[] = {
				{"Health", lua_setHealth},
				{"MaxHealth", lua_setMaxHealth},
				{"Invincible", lua_setInvincible},
				{"NameVisible", lua_setNameVisible},
				{"HealthVisible", lua_setHealthVisible},
				{"JumpPower", lua_setJumpPower},
				{"WalkSpeed", lua_setWalkSpeed},
				{"MoveDirection", lua_readOnlyProperty},
				{"WalkTarget", lua_readOnlyProperty},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Humanoid::register_lua_property_getters(lua_State* L){
			Instance::register_lua_property_getters(L);

			luaL_Reg properties[] = {
				{"Health", lua_setHealth},
				{"MaxHealth", lua_getMaxHealth},
				{"Invincible", lua_getInvincible},
				{"NameVisible", lua_getNameVisible},
				{"HealthVisible", lua_getHealthVisible},
				{"JumpPower", lua_getJumpPower},
				{"WalkSpeed", lua_getWalkSpeed},
				{"MoveDirection", lua_getMoveDirection},
				{"WalkTarget", lua_getWalkTarget},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Humanoid::register_lua_methods(lua_State* L){
			Instance::register_lua_methods(L);

			luaL_Reg methods[] = {
				{"TakeDamage", lua_TakeDamage},
				{"Move", lua_Move},
				{"MoveTo", lua_MoveTo},
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}
	}
}
