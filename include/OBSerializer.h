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

#include "oblibconfig.h"

#if HAVE_PUGIXML
#include "instance/Instance.h"
#endif

#ifndef OB_OBSERIALIZER
#define OB_OBSERIALIZER

namespace OB{
	class OBEngine;
	
	class OBSerializer{
		public:
			OBSerializer(OBEngine* eng);
			
			#if HAVE_PUGIXML
			shared_ptr<Instance::Instance> LoadInstance(std::string resURI);
			shared_ptr<Instance::Instance> Load(std::string resURI);
		    bool SaveInstance(shared_ptr<Instance::Instance> inst, std::string file);
			bool Save(std::string file);
			std::string SaveInMemory();
			#endif
			
		private:
			OBEngine* eng;
	};
}

#endif // OB_OBSERIALIZER

// Local Variables:
// mode: c++
// End:
