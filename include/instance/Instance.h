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
#include "ClassFactory.h"
#include "obtype.h"

#include "type/Event.h"

#include <iostream>

#ifndef OB_INST_INSTANCE
#define OB_INST_INSTANCE

typedef void (*luaRegisterFunc)(lua_State* L);

#define COLONERR "Expected ':' not '.' calling member function %s"

#define DECLARE_CLASS(Class_Name) \
	virtual shared_ptr<Instance> cloneImpl(); \
	virtual std::string getClassName(); \
	virtual std::string getLuaClassName(); \
	static OB::ClassMetadata* _ob_classmetadata; \
	static void _ob_init(); \
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
	OB::ClassMetadata* Class_Name::_ob_classmetadata = new Class_Name##ClassMetadata; \
	void Class_Name::_ob_init()

#define DEFINE_CLASS(Class_Name, isInstable, isAService, ParentClass) \
	class Class_Name##ClassMetadata: public OB::ClassMetadata{ \
		public: \
		Class_Name##ClassMetadata(){ \
			OB::ClassFactory::addClass(#Class_Name, this); \
		} \
		virtual shared_ptr<OB::Instance::Instance> newInstance(){ \
			return make_shared<Class_Name>(); \
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
    class Class_Name##ClassMetadata: public OB::ClassMetadata{ \
		public: \
		Class_Name##ClassMetadata(){ \
			OB::ClassFactory::addClass(#Class_Name, this); \
		} \
		virtual shared_ptr<OB::Instance::Instance> newInstance(){ \
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
    class Class_Name##ClassMetadata: public OB::ClassMetadata{ \
		public: \
		Class_Name##ClassMetadata(){ \
			OB::ClassFactory::addClass(#Class_Name, this); \
		} \
		virtual shared_ptr<OB::Instance::Instance> newInstance(){ \
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
		shared_ptr<Instance> inst = checkInstance(L, 1); \
		if(inst){ \
				 return inst->Event_Name->wrap_lua(L); \
		} \
		return 0; \
	}

#define WRAP_EVT(Clazz, Event_Name) \
	[](lua_State* L)->int{ \
		shared_ptr<Instance> inst = checkInstance(L, 1); \
		if(inst){ \
			if(shared_ptr<Clazz> wi = dynamic_pointer_cast<Clazz>(inst)){ \
				 return wi->Event_Name->wrap_lua(L); \
			} \
		} \
		return 0; \
	}

namespace OB{
	namespace Instance{
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
				Instance();
				virtual ~Instance();

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
				 * Called internally every tick.
				 *
				 * @author John M. Harris, Jr.
				 */
				virtual void tick();

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
				static void registerLuaClass(std::string className, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters, luaRegisterFunc register_events);

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
				static shared_ptr<Instance> checkInstance(lua_State* L, int index);

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

				//TODO: Events
				shared_ptr<Type::Event> TestEvent;

				bool Archivable;
				std::string Name;
				shared_ptr<Instance> Parent;

				int wrap_lua(lua_State* L, shared_ptr<Instance> ptr);

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

				DECLARE_CLASS(Instance);

				ob_int64 netId;

				std::vector<shared_ptr<Instance>> children;
		};
	}
}

#endif // OB_INST_INSTANCE


// Local Variables:
// mode: c++
// End:
