/*
 * Copyright (C) 2018 John M. Harris, Jr. <johnmh@splentity.com>
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

#include "js/OBJS.h"

#if HAVE_V8

namespace OB{
	namespace JS{
		void init(){
			std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
			v8::V8::InitializePlatform(platform.get());
			v8::V8::Initialize();
		}
	}
}

#endif
