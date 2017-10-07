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
		class CoreGui;
		class Lighting;
		class Players;
		class ContentProvider;
		class LogService;
		class RunService;
		class ReplicatedFirst;
		class UserInputService;

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
			 * Returns whether or not Roblox Compatibility Mode is
			 * enabled.
			 *
			 * @sa \ref roblox-compat "Roblox Compatibility Mode"
			 * @sa setRobloxCompatMode
			 * @returns bool, true if compat mode is on, otherwise false
			 * @author John M. Harris, Jr.
			 */
			bool getRobloxCompatMode();

			/**
			 * Sets whether or not Roblox Compatibility Mode is
			 * enabled.
			 *
			 * @sa \ref roblox-compat "Roblox Compatibility Mode"
			 * @sa getRobloxCompatMode
			 * @author John M. Harris, Jr.
			 */
			void setRobloxCompatMode(bool robloxCompatMode);

			/**
			 * Returns the Workspace service.
			 *
			 * @returns Workspace
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<Workspace> getWorkspace();

			/**
			 * Returns the CoreGui service.
			 *
			 * @returns coreGui
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<CoreGui> getCoreGui();

			/**
			 * Returns the Lighting service.
			 *
			 * @returns Lighting
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<Lighting> getLighting();

			/**
			 * Returns the Players service.
			 *
			 * @returns Players
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<Players> getPlayers();

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
			 * Returns the UserInputService service.
			 *
			 * @returns UserInputService
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<UserInputService> getUserInputService();

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
			virtual void deserialize(pugi::xml_node thisNode);
			virtual std::string serializedID();
#endif

			virtual std::map<std::string, _PropertyInfo> getProperties();
			virtual shared_ptr<Type::VarWrapper> getProperty(std::string prop);
			virtual void setProperty(std::string prop, shared_ptr<Type::VarWrapper> val);

			virtual void preRender();
			virtual void render();

			DECLARE_LUA_METHOD(Shutdown);

			DECLARE_LUA_METHOD(getRobloxCompatMode);
			DECLARE_LUA_METHOD(setRobloxCompatMode);

			static void register_lua_property_getters(lua_State* L);
			static void register_lua_property_setters(lua_State* L);

			DECLARE_CLASS(DataModel);

			shared_ptr<Workspace> workspace;
			shared_ptr<CoreGui> coreGui;
			shared_ptr<Lighting> lighting;
			shared_ptr<Players> players;
			shared_ptr<ContentProvider> contentProvider;
			shared_ptr<LogService> logService;
			shared_ptr<RunService> runService;
			shared_ptr<ReplicatedFirst> replicatedFirst;
			shared_ptr<UserInputService> userInputService;

			bool RobloxCompatMode;
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
