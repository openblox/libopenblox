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

/**
 * @file utility.h
 * @author John M. Harris, Jr.
 * @date May 2016
 * 
 * This file defines utility functions used internally by the OpenBlox
 * engine.
 */

#include "obtype.h"

#include <string>
#include <cstddef>
#include <algorithm>

#ifdef _WIN32
#include <winsock2.h>
#endif

#ifndef OB_UTILITY
#define OB_UTILITY

namespace OB{
	/**
	 * Returns the current time in milliseconds.
	 * 
	 * @returns Current time in millis
	 * @author John M. Harris, Jr.
	 */
	ob_int64 currentTimeMillis();

	/**
	 * Returns true if str starts with prefix.
	 *
	 * @author John M. Harris, Jr.
	 */
	bool ob_str_startsWith(std::string str, std::string prefix);

    enum empties_t{
		empties_ok,
		no_empties
	};

	template <typename Container>
	static inline Container& split(Container& result, const typename Container::value_type& s, const typename Container::value_type& delimiters, empties_t empties = empties_t::empties_ok){
		result.clear();
		size_t current;
		size_t next = -1;
		do{
			if(empties == empties_t::no_empties){
				next = s.find_first_not_of(delimiters, next + 1);
				if (next == Container::value_type::npos) break;
				next -= 1;
			}
			current = next + 1;
			next = s.find_first_of(delimiters, current);
			result.push_back(s.substr(current, next - current));
		}
		while(next != Container::value_type::npos);
		return result;
	}

	static inline std::string &ltrim(std::string &s){
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	static inline std::string &rtrim(std::string &s){
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	static inline std::string &trim(std::string &s){
		return ltrim(rtrim(s));
	}

	#ifdef _WIN32
	#define PATH_MAX 260
	
	char* realpath(const char* path, char resolved_path[PATH_MAX]);
	char* get_current_dir_name();
	
	void usleep(ob_int64 usec);

	#ifdef _MSC_VER
	int gettimeofday(struct timeval* tp, void* tzp);
	#endif
	#endif
}

#endif // OB_UTILITY

// Local Variables:
// mode: c++
// End:
