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

#include "instance/Humanoid.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Humanoid, true, false, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    Humanoid::Humanoid(){
			Name = ClassName;
			
			Health = 100;
			MaxHealth = 100;
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
			shared_ptr<Humanoid> h = make_shared<Humanoid>();
			h->Archivable = Archivable;
			h->Name = Name;
			h->ParentLocked = ParentLocked;

			h->Health = Health;
			h->MaxHealth = MaxHealth;
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
			if(health > MaxHealth){
			    health = MaxHealth;
			}

			if(health != Health){
				double oldHealth = Health;
				Health = health;

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

				propertyChanged("MaxHealth");
			}
		}

		bool Humanoid::getNameVisible(){
			return NameVisible;
		}
		
		bool Humanoid::setNameVisible(bool nameVisible){
			if(nameVisible != NameVisible){
			    NameVisible = nameVisible;

				propertyChanged("NameVisible");
			}
		}
				
		bool Humanoid::getHealthVisible(){
			return HealthVisible;
		}
		
		bool Humanoid::setHealthVisible(bool healthVisible){
			if(healthVisible != HealthVisible){
			    HealthVisible = healthVisible;

				propertyChanged("HealthVisible");
			}
		}

		double Humanoid::getJumpPower(){
			return JumpPower;
		}
		
		void Humanoid::setJumpPower(double jumpPower){
			if(jumpPower != JumpPower){
			    JumpPower = jumpPower;

				propertyChanged("JumpPower");
			}
		}

		double Humanoid::getWalkSpeed(){
			return WalkSpeed;
		}
		
		void Humanoid::setWalkSpeed(double walkSpeed){
			if(walkSpeed != WalkSpeed){
			    WalkSpeed = walkSpeed;

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

				propertyChanged("State");
			}
		}

		shared_ptr<Type::Vector3> Humanoid::getWalkTarget(){
			return WalkTarget;
		}

		double Humanoid::TakeDamage(double damage, void* damage_metadata){
			return 0;
		}
		std::vector<shared_ptr<Type::StatusEffect>> Humanoid::GetStatusEffects(){
			return std::vector<shared_ptr<Type::StatusEffect>>();
		}
		void Humanoid::SetStatusEffects(std::vector<shared_ptr<Type::StatusEffect>> statusEffects){
		}
		void Humanoid::AddStatusEffect(shared_ptr<Type::StatusEffect> statusEffect){
		}
		void Humanoid::RemoveStatusEffect(shared_ptr<Type::StatusEffect> statusEffect){
		}
		void Humanoid::Move(shared_ptr<Type::Vector3> direction){
		}
		void Humanoid::MoveTo(shared_ptr<Type::Vector3> location){
		}
	}
}
