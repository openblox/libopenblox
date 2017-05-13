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

#include "utility.h"

#include <stddef.h>
#ifndef _MSC_VER
#include <sys/time.h>
#endif

#include <algorithm>

#ifdef _MSC_VER
#include <winsock2.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#endif

namespace OB{
	ob_int64 currentTimeMillis(){
		struct timeval tp;
		gettimeofday(&tp, NULL);

		return (ob_int64)(tp.tv_sec * 1000 + tp.tv_usec / 1000);
	}

	bool ob_str_startsWith(std::string str, std::string prefix){
		return std::equal(prefix.begin(), prefix.end(), str.begin());
	}

	// Windows compat
    #ifdef _MSC_VER
	char* realpath(const char* path, char resolved_path[PATH_MAX]){
		char* return_path = 0;

		if(path){
			if(resolved_path){
				return_path = resolved_path;
			}else{
				return_path = (char*)malloc(PATH_MAX);
			}
			if(return_path){
				size_t size = GetFullPathNameA(path, PATH_MAX, return_path, 0);

				if(size > PATH_MAX){
					if(return_path != resolved_path){
						size_t new_size;

						free(return_path);
						return_path = (char*)malloc(size);

						if(return_path){
							new_size = GetFullPathNameA(path, size, return_path, 0);

							if(new_size > size){
								free(return_path);
								return_path = 0;
								errno = ENAMETOOLONG;
							}else{
								size = new_size;
							}
						}else{
							errno = EINVAL;
						}
					}else{
						return_path = 0;
						errno = ENAMETOOLONG;
					}
				}

				if(!size){
					if(return_path != resolved_path){
						free(return_path);
					}

					return_path = 0;

					switch(GetLastError()){
						case ERROR_FILE_NOT_FOUND: {
							errno = ENOENT;
							break;
						}
						case ERROR_PATH_NOT_FOUND:
						case ERROR_INVALID_DRIVE: {
							errno = ENOTDIR;
							break;
						}
						case ERROR_ACCESS_DENIED: {
							errno = EACCES;
							break;
						}
						default: {
							errno = EIO;
							break;
						}
					}
				}

				if(return_path){
					struct stat stat_buffer;

					if(stat(return_path, &stat_buffer)){
						if(return_path != resolved_path){
							free(return_path);
						}

						return_path = 0;
					}
				}
			}else{
				errno = EINVAL;
			}
		}else{
			errno = EINVAL;
		}

		return return_path;
	}

	char* get_current_dir_name(){
		char* path = (char*)malloc(FILENAME_MAX);
		return _getcwd(path, FILENAME_MAX);
	}

	void usleep(__int64 usec){
		HANDLE timer;
		LARGE_INTEGER ft;

		ft.QuadPart = -(10*usec);

		timer = CreateWaitableTimer(NULL, TRUE, NULL);
		SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
		WaitForSingleObject(timer, INFINITE);
		CloseHandle(timer);
	}

	int gettimeofday(struct timeval* tp, void* tzp){
		// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
		// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
		// until 00:00:00 January 1, 1970 
		static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

		SYSTEMTIME system_time;
		FILETIME file_time;
		uint64_t time;

		GetSystemTime(&system_time);
		SystemTimeToFileTime(&system_time, &file_time);
		time = ((uint64_t)file_time.dwLowDateTime);
		time += ((uint64_t)file_time.dwHighDateTime) << 32;

		tp->tv_sec = (long) ((time - EPOCH) / 10000000L);
		tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
		return 0;
	}
	#endif
}
