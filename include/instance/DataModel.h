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

#include "instance/ServiceProvider.h"

#ifndef OB_INST_DATAMODEL
#define OB_INST_DATAMODEL

namespace OB{
	namespace Instance{
		/**
		 * DataModel is the root singleton of the OpenBlox engine.
		 * The DataModel contains all of the core services of the engine,
		 * such as Workspace, Lighting, LogService and similar.
		 *
		 * @author John M. Harris, Jr.
		 */
		class DataModel: public ServiceProvider{
			public:
			    DataModel();
				virtual ~DataModel();

				/**
				 * Reimplementation of ServiceProvider's GetService,
				 * this time calling ClassFactory's createService with
				 * the isDataModel parameter being true.
				 *
				 * @param std::string className
				 * @returns Instance* The service requested, or NULL if it cannot be created.
				 * @author John M. Harris, Jr.
				 */
				virtual Instance* GetService(std::string className);
				
				DECLARE_CLASS(DataModel);
				
				static void register_lua_methods(lua_State* L);
		};
	}
}

#endif // OB_INST_DATAMODEL


// Local Variables:
// mode: c++
// End:
