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

#include "instance/LuaSourceContainer.h"

#ifndef OB_INST_BASESCRIPT
#define OB_INST_BASESCRIPT

namespace OB{
	namespace Instance{
		class BaseScript: public LuaSourceContainer{
			public:
			    BaseScript(OBEngine* eng);
				virtual ~BaseScript();
				
				DECLARE_CLASS(BaseScript);
		};
	}
}

#endif // OB_INST_BASESCRIPT


// Local Variables:
// mode: c++
// End:
