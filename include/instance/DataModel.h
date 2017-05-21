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
		class Workspace;
		class Lighting;
		class ContentProvider;
		class LogService;
		class RunService;
		class ReplicatedFirst;
		
		/**
		 * DataModel is the root singleton of the OpenBlox engine.
		 * The DataModel contains all of the core services of the engine,
		 * such as Workspace, Lighting, LogService and similar.
		 *
		 * @author John M. Harris, Jr.
		 */
		class DataModel: public ServiceProvider{
			public:
			    DataModel(OBEngine* eng);
				virtual ~DataModel();

				void Shutdown(int statusCode = 0);
				
				void initServices();

				/**
				 * Returns the Workspace service.
				 *
				 * @returns Workspace
				 * @author John M. Harris, Jr.
				 */
				shared_ptr<Workspace> getWorkspace();
				
				/**
				 * Returns the Lighting service.
				 *
				 * @returns Lighting
				 * @author John M. Harris, Jr.
				 */
				shared_ptr<Lighting> getLighting();

				/**
				 * Returns the ContentProvider service.
				 *
				 * @returns ContentProvider
				 * @author John M. Harris, Jr.
				 */
				shared_ptr<ContentProvider> getContentProvider();

				/**
				 * Returns the LogService service.
				 *
				 * @returns LogService
				 * @author John M. Harris, Jr.
				 */
				shared_ptr<LogService> getLogService();

				/**
				 * Returns the RunService service.
				 *
				 * @returns RunService
				 * @author John M. Harris, Jr.
				 */
				shared_ptr<RunService> getRunService();

				/**
				 * Reimplementation of ServiceProvider's GetService,
				 * this time calling ClassFactory's createService with
				 * the isDataModel parameter being true.
				 *
				 * @param className Class name
				 * @returns Instance* The service requested, or NULL if it cannot be created.
				 * @author John M. Harris, Jr.
				 */
				virtual shared_ptr<Instance> GetService(std::string className);

				/**
				 * Returns a weak reference to an Instance with a
				 * given Network ID, or NULL.
				 *
				 * @param netId Network ID
				 * @internal
				 * @returns weak reference to Instance or NULL
				 * @author John M. Harris, Jr.
				 */
			    weak_ptr<Instance> lookupInstance(ob_uint64 netId);

				void putInstance(shared_ptr<Instance> inst);
				void dropInstance(ob_uint64 reqNetId);

				/**
				 * Returns the next network ID.
				 *
				 * @returns Network ID
				 * @internal
				 * @author John M. Harris, Jr.
				 */
				ob_uint64 nextNetworkID();

				#if HAVE_ENET
				/**
				 * Replicates properties of this Instance.
				 * 
				 * @param peer Peer
				 * @author John M. Harris, Jr.
				 */
				virtual void replicateProperties(shared_ptr<NetworkReplicator> peer);

				/**
				 * Replicates children of this Instance.
				 * 
				 * @param peer Peer
				 * @author John M. Harris, Jr.
				 */
				virtual void replicateChildren(shared_ptr<NetworkReplicator> peer);
				#endif

				#if HAVE_PUGIXML
				virtual void serialize(pugi::xml_node parentNode);
				virtual void deserialize(pugi::xml_node thisNode);
				virtual std::string serializedID();
				#endif
				
				DECLARE_CLASS(DataModel);

				shared_ptr<Workspace> workspace;
				shared_ptr<Lighting> lighting;
				shared_ptr<ContentProvider> contentProvider;
				shared_ptr<LogService> logService;
				shared_ptr<RunService> runService;
				shared_ptr<ReplicatedFirst> replicatedFirst;

				DECLARE_LUA_METHOD(Shutdown);

				ob_uint64 netIdStartIdx;
				ob_uint64 netIdNextIdx;
				std::map<ob_uint64, weak_ptr<Instance>> instMap;
				std::vector<ob_uint64> freedNetIDs;
				
				static void register_lua_methods(lua_State* L);
		};
	}
}

#endif // OB_INST_DATAMODEL


// Local Variables:
// mode: c++
// End:
