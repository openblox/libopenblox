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

/**
 * @file static_init.h
 * @author John M. Harris, Jr.
 * @date July 2015
 *
 * This file is defines and implements a header-only "static initialization" system.
 */

#ifndef OPENBLOX_STATIC_INIT_H_
#define OPENBLOX_STATIC_INIT_H_

#include <vector>

/**
 * Function-style macro used to declare a static initializer.
 * Omits the semicolon at the end to allow this to be used as a function, just looks a bit cleaner.
 */
#define DECLARE_STATIC_INIT(ClassName) \
   static void static_init_func(); \
   static OpenBlox::static_init_helper ClassName##_static_init_helper

/**
 * Function-style macro used to declare a static initializer that will be fired after OBGame is created.
 * Omits the semicolon at the end to allow this to be used as a function, just looks a bit cleaner.
 */
#define DECLARE_GAME_STATIC_INIT(ClassName) \
   static void static_game_init_func(); \
   static OpenBlox::static_init_helper ClassName##_static_game_init_helper

/**
 * Function-style macro for implementation of static initializers.
 * Expects implementation after this macro.
 */
#define STATIC_INIT(ClassName) \
	OpenBlox::static_init_helper ClassName::ClassName##_static_init_helper(&ClassName::static_init_func, false); \
	void ClassName::static_init_func()

/**
 * Function-style macro for implementation of static initializers that require OBGame to have been created.
 * Expects implementation after this macro.
 */
#define STATIC_GAME_INIT(ClassName) \
	OpenBlox::static_init_helper ClassName::ClassName##_static_game_init_helper(&ClassName::static_game_init_func, true); \
	void ClassName::static_game_init_func()

typedef void (*init_func_type)();

namespace OpenBlox{
	/**
	 * Static initializer implementation for registration of OpenBlox classes.
	 */
	class static_init{
		public:
			/**
			 * Returns an instance of static_init, used internally by static_init::execute as well as by static_init_helper classes.
			 */
			static static_init& instance(){
				static static_init inst;
				return inst;
			}

			/**
			 * Registers a static initializer.
			 */
			void add_init_func(init_func_type f){
				funcs_.push_back(f);
			}

			/**
			 * Registers a game static initializer.
			 */
			void add_game_init_func(init_func_type f){
				game_funcs_.push_back(f);
			}

			/**
			 * Runs all registered static initializers.
			 * @param bool True if OBGame is created, otherwise false.
			 */
			static void execute(bool gameCreated){
				auto& inst = instance();
				if(gameCreated){
					for(auto c : inst.game_funcs_){
						c();
					}
				}else{
					for(auto c : inst.funcs_){
						c();
					}
				}
			}

		private:
			static_init(){}

			std::vector<init_func_type> funcs_;
			std::vector<init_func_type> game_funcs_;
	};

	/**
	 * Static initializer helper class, used by the DECLARE_STATIC_INIT to register a static initialization function.
	 */
	class static_init_helper{
		public:
			static_init_helper(init_func_type f, bool requiresGame){
				if(requiresGame){
					static_init::instance().add_game_init_func(f);
				}else{
					static_init::instance().add_init_func(f);
				}
			}
	};
}

#endif
