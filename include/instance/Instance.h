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

#include <string>
#include <vector>

#include "mem.h"

#include "lua/OBLua.h"
#include "OBEngine.h"
#include "ClassFactory.h"
#include "obtype.h"

#include "type/Event.h"

#include <iostream>

#include "oblibconfig.h"

#if HAVE_ENET
#include <enet/enet.h>
#endif

#if HAVE_PUGIXML
#include <pugixml.hpp>
#endif

#ifndef OB_INST_INSTANCE
#define OB_INST_INSTANCE

#if HAVE_ENET
	#define REPLICATE_PROPERTY_CHANGE(__repl_prop) \
	{ \
		if(netId > 4){ \
			shared_ptr<OB::Instance::DataModel> __repl_dm = eng->getDataModel(); \
			if(__repl_dm){ \
				if((netId < OB_NETID_WORKSPACE) || IsDescendantOf(__repl_dm)){ \
					shared_ptr<OB::Instance::Instance> __repl_nsInst = __repl_dm->FindService("NetworkServer"); \
					if(shared_ptr<OB::Instance::NetworkServer> __repl_ns = dynamic_pointer_cast<OB::Instance::NetworkServer>(__repl_nsInst)){ \
						BitStream __repl_bs; \
				   		__repl_bs.writeSizeT(OB_NET_PKT_SET_PROPERTY); \
						__repl_bs.writeUInt64(netId); \
						__repl_bs.writeString(#__repl_prop); \
		    			__repl_bs.writeVar(make_shared<Type::VarWrapper>(__repl_prop)); \
						__repl_ns->broadcast(OB_NET_CHAN_REPLICATION, __repl_bs); \
					} \
				} \
			} \
		} \
	}
#else
#define REPLICATE_PROPERTY_CHANGE(__repl_prop)
#endif

typedef void (*luaRegisterFunc)(lua_State* L);

#define COLONERR "Expected ':' not '.' calling member function %s"

#define DECLARE_CLASS(Class_Name) \
	virtual shared_ptr<Instance> cloneImpl(); \
	virtual std::string getClassName(); \
	virtual std::string getLuaClassName(); \
	static OB::ClassMetadata* _ob_classmetadata; \
	static void registerClass(); \
	static void _ob_init(OBEngine* eng); \
protected: \
	static std::string ClassName; \
	static std::string LuaClassName

#define _OB_DEFCLASS_SHARED(Class_Name) \
	std::string Class_Name::ClassName = #Class_Name; \
	std::string Class_Name::LuaClassName = "luaL_Instance_" #Class_Name; \
	std::string Class_Name::getClassName(){ \
		return ClassName; \
	} \
	std::string Class_Name::getLuaClassName(){ \
		return LuaClassName; \
	} \
	OB::ClassMetadata* Class_Name::_ob_classmetadata = NULL; \
    void Class_Name::registerClass(){ _ob_classmetadata = new Class_Name##_ClassMetadata; } \
	void Class_Name::_ob_init(OBEngine* eng)

#define DEFINE_CLASS(Class_Name, isInstable, isAService, ParentClass) \
	class Class_Name##_ClassMetadata: public OB::ClassMetadata{ \
		public: \
		Class_Name##_ClassMetadata(){ \
			OB::ClassFactory::addClass(#Class_Name, this); \
		} \
		virtual shared_ptr<OB::Instance::Instance> newInstance(OBEngine* eng){ \
			return make_shared<Class_Name>(eng); \
		} \
		virtual bool isA(shared_ptr<OB::Instance::Instance> obj){ \
			return (dynamic_pointer_cast<Class_Name>(obj)) != NULL; \
		} \
		virtual bool isInstantiatable(){ \
			return isInstable; \
		} \
		virtual bool isService(bool isDataModel){ \
			(void)isDataModel; \
			return isAService; \
		} \
		virtual std::string getClassName(){ \
			return #Class_Name; \
		} \
		virtual std::string getParentClassName(){ \
			return #ParentClass; \
		} \
		virtual InstanceInitFnc getInitFunc(){ \
			return Class_Name::_ob_init; \
		} \
	}; \
    _OB_DEFCLASS_SHARED(Class_Name)

#define DEFINE_CLASS_ABS(Class_Name, ParentClass) \
    class Class_Name##_ClassMetadata: public OB::ClassMetadata{ \
		public: \
		Class_Name##_ClassMetadata(){ \
			OB::ClassFactory::addClass(#Class_Name, this); \
		} \
		virtual shared_ptr<OB::Instance::Instance> newInstance(OBEngine* eng){ \
			return NULL; \
		} \
		virtual bool isA(shared_ptr<OB::Instance::Instance> obj){ \
			return (dynamic_pointer_cast<Class_Name>(obj)) != NULL; \
		} \
		virtual bool isInstantiatable(){ \
			return false; \
		} \
		virtual bool isService(bool isDataModel){ \
			(void)isDataModel; \
			return false; \
		} \
		virtual std::string getClassName(){ \
			return #Class_Name; \
		} \
		virtual std::string getParentClassName(){ \
			return #ParentClass; \
		} \
		virtual InstanceInitFnc getInitFunc(){ \
			return Class_Name::_ob_init; \
		} \
	}; \
	_OB_DEFCLASS_SHARED(Class_Name)

#define DEFINE_CLASS_ABS_WCLONE(Class_Name, ParentClass) \
    class Class_Name##_ClassMetadata: public OB::ClassMetadata{ \
		public: \
		Class_Name##_ClassMetadata(){ \
			OB::ClassFactory::addClass(#Class_Name, this); \
		} \
		virtual shared_ptr<OB::Instance::Instance> newInstance(OBEngine* eng){ \
			return NULL; \
		} \
		virtual bool isA(shared_ptr<OB::Instance::Instance> obj){ \
			return (dynamic_pointer_cast<Class_Name>(obj)) != NULL; \
		} \
		virtual bool isInstantiatable(){ \
			return false; \
		} \
		virtual bool isService(bool isDataModel){ \
			(void)isDataModel; \
			return false; \
		} \
		virtual std::string getClassName(){ \
			return #Class_Name; \
		} \
		virtual std::string getParentClassName(){ \
			return #ParentClass; \
		} \
		virtual InstanceInitFnc getInitFunc(){ \
			return Class_Name::_ob_init; \
		} \
	}; \
	shared_ptr<Instance> Class_Name::cloneImpl(){ return NULL; } \
	_OB_DEFCLASS_SHARED(Class_Name)

#define DECLARE_LUA_METHOD(MethodName) \
	static int lua_##MethodName(lua_State* L)


#define WRAP_EVTI(Event_Name) \
	[](lua_State* L)->int{ \
		shared_ptr<Instance> inst = checkInstance(L, 1, false);	\
		if(inst){ \
			return inst->Event_Name->wrap_lua(L); \
		} \
		return 0; \
	}

#define WRAP_EVT(Clazz, Event_Name) \
	[](lua_State* L)->int{ \
		shared_ptr<Instance> inst = checkInstance(L, 1, false);	\
		if(inst){ \
			if(shared_ptr<Clazz> wi = dynamic_pointer_cast<Clazz>(inst)){ \
				return wi->Event_Name->wrap_lua(L); \
			} \
		} \
		return 0; \
	}

namespace OB{
	namespace Instance{
		class NetworkReplicator;

		struct _PropertyInfo{
			public:
				std::string type;
				bool readOnly;
				bool isPublic;
				bool isSerialized;
		};
		
		/**
		 * Instance is the base class of all world objects in OpenBlox.
		 * @author John M. Harris, Jr.
		 */
		class Instance:
				// @cond DOXYGEN_IGNORE
				public std::enable_shared_from_this<Instance>
				// @endcond
		{
			public:
				Instance(OBEngine* eng);
				virtual ~Instance();

				//Accessors and Mutators
				virtual void setName(std::string name);
				virtual std::string getName();
				
			    virtual void setArchivable(bool archivable);
				virtual bool getArchivable();

				/**
				 * Returns the OBEngine instance associated with this
				 * Instance.
				 *
				 * @returns OBEngine* engine
				 * @author John M. Harris, Jr.
				 */
				OBEngine* getEngine();

				/**
				 * Calls Remove on all children.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual void ClearAllChildren();

				/**
				 * Clones an Instance, if possible. This also clones
				 * children of the Instance.
				 *
				 * @returns Instance, NULL if not able to clone.
				 * @author John M. Harris, Jr.
				 */
				virtual shared_ptr<Instance> Clone();

				/**
				 * Used to parent the object to NULL (nil in Lua) and
				 * remove all references. This also parent-locks it.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual void Destroy();

				/**
				 * Used to set the parent of this object to NULL
				 * (nil in Lua). Calls Remove recursively on child
				 * objects.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual void Remove();

				/**
				 * Finds the first child with a given name. This has
				 * an optional argument for searching recursively.
				 *
				 * @param name Name of the Instance to search for
				 * @param recursive Whether or not to search recursively.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual shared_ptr<Instance> FindFirstChild(std::string name, bool recursive = false);

				/**
				 * Finds the first child with a given class name. This
				 * has an optional argument for searching recursively.
				 *
				 * @param className Name of the class to search for
				 * @param recursive Whether or not to search recursively.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual shared_ptr<Instance> FindFirstChildOfClass(std::string className, bool recursive = false);

				/**
				 * Returns the children of an object.
				 *
				 * @returns Vector containing the children of this object.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual std::vector<shared_ptr<Instance>> GetChildren();

				/**
				 * Returns the full name of this object. Usually in
				 * the form of "game.Service.Thing", can return weird
				 * things when one of the parents of this Instance is
				 * parented to NULL.
				 *
				 * @returns Full name of this Instance
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual std::string GetFullName();

				/**
				 * Used for class inheritance checking.
				 * @param name Name of the class to test against.
				 *
				 * @returns true if this class inherits from the class
				 * of the name given.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual bool IsA(std::string name);

				/**
				 * Returns whether or not this Instance is the
				 * ancestor of another Instance.
				 *
				 * @param descendant Possible Descendant
				 *
				 * @returns true if the possible descendant is not
				 * NULL and this Instance is an ancestor of the
				 * possible descendant.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual bool IsAncestorOf(shared_ptr<Instance> descendant);

				/**
				 * Returns whether or not this Instance is a
				 * descendant of another Instance.
				 *
				 * @param ancestor Possible Ancestor
				 * @returns true if the possible ancestor is NULL or
				 * this Instance is a descendant of the possible
				 * ancestor.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual bool IsDescendantOf(shared_ptr<Instance> ancestor);

				/**
				 * Returns a unique 64 bit integer, globally used to
				 * identify this Instance. Defaults to
				 * OB_NETID_UNASSIGNED.
				 *
				 * @returns Unique 64 bit integer, 0 or a static
				 * assignment
				 *
				 * @author John M. Harris, Jr.
				 */
				ob_int64 GetNetworkID();

				/**
				 * Sets the number used to identify this Instance.
				 * Defaults to OB_NETID_UNASSIGNED.
				 *
				 * @internal
				 * @param Unique 64 bit integer
				 * @author John M. Harris, Jr.
				 */
				void setNetworkID(ob_int64 netId);

				/**
				 * Generates a unique network ID for this Instance.
				 * 
				 * This should only be called on servers. It's useless
				 * to assign network IDs on client-only objects, which
				 * is why I leave them as OB_NETID_UNASSIGNED.
				 *
				 * @author John M. Harris, Jr.
				 */
				void generateNetworkID();

				#if HAVE_ENET
				/**
				 * Replicates this Instance.
				 * 
				 * @param peer Peer
				 * @author John M. Harris, Jr.
				 */
				virtual void replicate(shared_ptr<NetworkReplicator> peer);

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
				virtual void serializeThis(pugi::xml_node parentNode, shared_ptr<Instance> model);
				virtual void serialize(pugi::xml_node parentNode, shared_ptr<Instance> model);
				virtual void serializeChildren(pugi::xml_node parentNode, shared_ptr<Instance> model);
				virtual void serializeProperties(pugi::xml_node thisNode, shared_ptr<Instance> model);
				virtual void deserializeCreate(pugi::xml_node thisNode);
				virtual void deserialize(pugi::xml_node thisNode);
				virtual void deserializeProperties(pugi::xml_node thisNode);
				virtual std::string serializedID();
				#endif

				virtual std::map<std::string, _PropertyInfo> getProperties();

				virtual shared_ptr<Type::VarWrapper> getProperty(std::string prop);
				virtual void setProperty(std::string prop, shared_ptr<Type::VarWrapper> val);

				/**
				 * Called internally every tick.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual void tick();

				/**
				 * Called before the scene is rendered.
				 * Called from OBEngine::render
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual void preRender();

				/**
				 * Renders additional scene objects, including GUI.
				 * Called from OBEngine::render
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual void render();

				/**
				 * Convenience method to call tick on all children.

				 * @author John M. Harris, Jr.
				 */
				virtual void tickChildren();

				/**
				 * Convenience method to register this class as an
				 * Instance class.
				 *
				 * @author John M. Harris, Jr.
				 */
				static void registerLuaClass(OBEngine* eng, std::string className, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters, luaRegisterFunc register_events);

				/**
				 * Returns the stringified version of this Instance,
				 * for C++ and Lua this will return the Name.
				 *
				 * @returns Name
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual std::string toString();

				/**
				 * Used to set the Parent property.
				 *
				 * @param parent New Parent
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual void setParent(shared_ptr<Instance> parent, bool useDMNotify);

				virtual void removeChild(shared_ptr<Instance> kid);
				virtual void addChild(shared_ptr<Instance> kid);

				/**
				 * Returns the current Parent of this Instance.
				 *
				 * @returns Parent
				 *
				 * @author John M. Harris, Jr.
				 */
				shared_ptr<Instance> getParent();

				/**
				 * Checks that the value at the specified index on the
				 * Lua state is an Instance, and if so returns it.
				 *
				 * @param L Lua State
				 * @param index Index on the Lua stack
				 *
				 * @author John M. Harris, Jr.
				 * @author DigiTechs
				 */
				static shared_ptr<Instance> checkInstance(lua_State* L, int index, bool errIfNot = true, bool allowNil = true);

			    /**
				 * Handles attempts to set properties of this Instance.
				 *
				 * @param L Lua State
				 *
				 * @returns 0 or error (Does not return if an
				 * error occurs)
				 *
				 * @author DigiTechs
				 * @author John M. Harris, Jr.
				 */
				static int lua_index(lua_State* L);

				/**
				 * Handles attempts to get properties, methods or
				 * events of this Instance.
				 *
				 * @param L Lua State
				 *
				 * @returns 1 if there is a corresponding value,
				 * otherwise errors. (No return)
				 *
				 * @author DigiTechs
				 * @author John M. Harris, Jr.
				 */
				static int lua_newindex(lua_State* L);

			    /**
				 * Handles equality tests from Lua.
				 *
				 * @param L Lua State
				 *
				 * @returns 1, a bool value on the lua stack.
				 * (This value is true if the two Instances are
				 * the same)
				 *
				 * @author John M. Harris, Jr.
				 */
				static int lua_eq(lua_State* L);

				/**
				 * Handles garbage collection from Lua.
				 *
				 * @param L Lua State
				 *
				 * @returns 0
				 *
				 * @author John M. Harris, Jr.
				 */
				static int lua_gc(lua_State* L);

				/**
				 * Handles tostring calls on this Instance from Lua.
				 *
				 * @param L Lua State
				 *
				 * @returns 1, a string value on the lua stack. (The
				 * result of Instance::toString)
				 *
				 * @author John M. Harris, Jr.
				 */
				static int lua_toString(lua_State* L);
				
				DECLARE_LUA_METHOD(getClassName);
				DECLARE_LUA_METHOD(getUseCount);
				DECLARE_LUA_METHOD(getName);
				DECLARE_LUA_METHOD(setName);
				DECLARE_LUA_METHOD(getParent);
				DECLARE_LUA_METHOD(setParent);
				DECLARE_LUA_METHOD(getArchivable);
				DECLARE_LUA_METHOD(setArchivable);

				/**
				 * Used as the 'setter' for read-only properties.
				 *
				 * @param L Lua state
				 *
				 * @author John M. Harris, Jr.
				 */
				static int lua_readOnlyProperty(lua_State* L);

				DECLARE_LUA_METHOD(ClearAllChildren);
				DECLARE_LUA_METHOD(Clone);
				DECLARE_LUA_METHOD(Destroy);
				DECLARE_LUA_METHOD(Remove);
				DECLARE_LUA_METHOD(FindFirstChild);
				DECLARE_LUA_METHOD(GetChildren);
				DECLARE_LUA_METHOD(GetFullName);
				DECLARE_LUA_METHOD(IsA);
				DECLARE_LUA_METHOD(IsAncestorOf);
				DECLARE_LUA_METHOD(IsDescendantOf);
				DECLARE_LUA_METHOD(GetNetworkID);

				bool ParentLocked;

				shared_ptr<Type::Event> Changed;
				shared_ptr<Type::Event> AncestryChanged;
				shared_ptr<Type::Event> ChildAdded;
				shared_ptr<Type::Event> ChildRemoved;
				shared_ptr<Type::Event> DescendantAdded;
				shared_ptr<Type::Event> DescendantRemoving;

				int wrap_lua(lua_State* L);

				/**
				 * Lua Metamethods for the Instance class.
				 *
				 * @param L Lua State
				 *
				 * @author John M. Harris, Jr.
				 * @author DigiTechs
				 */
				static void register_lua_metamethods(lua_State* L);

				/**
				 * Lua methods for the Instance class.
				 *
				 * @param L Lua State
				 *
				 * @author John M. Harris, Jr.
				 * @author DigiTechs
				 */
				static void register_lua_methods(lua_State* L);

				/**
				 * Lua property setters for the Instance class.
				 *
				 * @param L Lua State
				 *
				 * @author John M. Harris, Jr.
				 * @author DigiTechs
				 */
				static void register_lua_property_setters(lua_State* L);

				/**
				 * Lua property getters for the Instance class.
				 *
				 * @param L Lua State
				 *
				 * @author John M. Harris, Jr.
				 * @author DigiTechs
				 */
				static void register_lua_property_getters(lua_State* L);

				/**
				 * Lua events for the Instance class.
				 *
				 * @param L Lua State
				 *
				 * @author John M. Harris, Jr.
				 * @author DigiTechs
				 */
				static void register_lua_events(lua_State* L);

				/**
				 * When writing a class that uses an asset, we
				 * implement this to efficiently handle loading
				 * of assets.
				 *
				 * Returns true if the assets required by this
				 * Instance have been loaded, which signals
				 * that this can be removed from the list of
				 * assets waiting for assets.
				 *
				 * If false, it stays in the list.
				 *
				 * @param res The resource URI loaded
				 * 
				 * @author John M. Harris, Jr.
				 * @returns bool, true if all assets have been loaded
				 */
				virtual bool assetLoaded(std::string res);

				OBEngine* eng;

				DECLARE_CLASS(Instance);

				bool Archivable;
				std::string Name;
				shared_ptr<Instance> Parent;

				void fireAncestryChanged(std::vector<shared_ptr<Type::VarWrapper>> args);
				void fireDescendantAdded(std::vector<shared_ptr<Type::VarWrapper>> args);
				void fireDescendantRemoving(std::vector<shared_ptr<Type::VarWrapper>> args);
				virtual void propertyChanged(std::string property);
				static void propertyChanged(std::string property, shared_ptr<Instance> inst);
				
				ob_int64 netId;

				std::vector<shared_ptr<Instance>> children;
		};
	}
}

#endif // OB_INST_INSTANCE


// Local Variables:
// mode: c++
// End:
