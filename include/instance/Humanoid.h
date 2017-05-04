/*
 * Copyright (C) 2016 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "instance/Instance.h"

#include "type/Vector3.h"

#ifndef OB_INST_HUMANOID
#define OB_INST_HUMANOID

namespace OB{
	namespace Instance{
		/**
		 * The Humanoid class provides a simple convenience class
		 * created with the intention of allowing you to easily create
		 * character objects, player controlled or otherwise.
		 *
		 * @author John M. Harris, Jr.
		 */
		class Humanoid: public Instance{
			public:
			    Humanoid();
				virtual ~Humanoid();

				double getHealth();
				void setHealth(double health);

				double getMaxHealth();
				void setMaxHealth(double maxHealth);

				bool getInvincible();
				bool setInvincible(bool invincible);

				bool getNameVisible();
				bool setNameVisible(bool nameVisible);
				
				bool getHealthVisible();
				bool setHealthVisible(bool healthVisible);

				double getJumpPower();
				void setJumpPower(double jumpPower);

				double getWalkSpeed();
				void setWalkSpeed(double walkSpeed);

				shared_ptr<Type::Vector3> getMoveDirection();

				int getState();
				void setState(int state);

				shared_ptr<Type::Vector3> getWalkTarget();

				double TakeDamage(double damage, std::string damage_metadata = "");
				void Move(shared_ptr<Type::Vector3> direction);
				void MoveTo(shared_ptr<Type::Vector3> location);

				DECLARE_LUA_METHOD(getHealth);
				DECLARE_LUA_METHOD(setHealth);
				DECLARE_LUA_METHOD(getMaxHealth);
				DECLARE_LUA_METHOD(setMaxHealth);
				DECLARE_LUA_METHOD(getInvincible);
				DECLARE_LUA_METHOD(setInvincible);
				DECLARE_LUA_METHOD(getNameVisible);
				DECLARE_LUA_METHOD(setNameVisible);
				DECLARE_LUA_METHOD(getHealthVisible);
				DECLARE_LUA_METHOD(setHealthVisible);
				DECLARE_LUA_METHOD(getJumpPower);
				DECLARE_LUA_METHOD(setJumpPower);
				DECLARE_LUA_METHOD(getWalkSpeed);
				DECLARE_LUA_METHOD(setWalkSpeed);
				DECLARE_LUA_METHOD(getMoveDirection);
				DECLARE_LUA_METHOD(getState);
				DECLARE_LUA_METHOD(setState);
				DECLARE_LUA_METHOD(getWalkTarget);

				DECLARE_LUA_METHOD(TakeDamage);
				DECLARE_LUA_METHOD(Move);
				DECLARE_LUA_METHOD(MoveTo);

				static void register_lua_property_getters(lua_State* L);
				static void register_lua_property_setters(lua_State* L);
				static void register_lua_methods(lua_State* L);

				DECLARE_CLASS(Humanoid);

				shared_ptr<Type::Event> MoveToFinished; // Params: bool success
				shared_ptr<Type::Event> HealthChanged; // Params: double previousHealth, string metadata (nil if not damage)
				shared_ptr<Type::Event> Died;

				double Health;
				double MaxHealth;
				bool Invincible;
				bool NameVisible;
				bool HealthVisible;
				double JumpPower;
				double WalkSpeed;
				shared_ptr<Type::Vector3> MoveDirection;
				int State;// TODO:
				shared_ptr<Type::Vector3> WalkTarget;
		};
	}
}

#endif // OB_INST_HUMANOID


// Local Variables:
// mode: c++
// End:
