/*
 * Copyright 2015 John M. Harris, Jr.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenBlox.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <OpenBlox.h>

#include <ClassFactory.h>

//This is ugly, but oh well.
#define BEGIN_INSTANCE namespace OpenBlox{ namespace Instance{
#define END_INSTANCE }}

#ifndef INSTANCE_INSTANCE_H_
#define INSTANCE_INSTANCE_H_

#include "../type/Event.h"

BEGIN_INSTANCE

#define COLONERR "Expected ':' not '.' calling member function %s"

#ifndef OB_NO_NETWORKING
	#define NO_REPLICATE() \
	virtual inline void replicate(RakNet::RakPeerInterface* peer, RakNet::SystemAddress addr){ \
		OB_UNUSED(peer); \
		OB_UNUSED(addr); \
	}

	/*
	#define REPLICATE_PROPERTY_CHANGE(_repl_inst, prop, val) \
	{ \
		OpenBlox::OBEngine* _repl_game = OpenBlox::OBEngine::getInstance(); \
		if(_repl_game){ \
			if(_repl_game->isServer() && _repl_game()->getDataModel()->isServer()){ \
				NetworkPeer::sendSetPropertyPacket(_repl_inst->GetNetworkID(), prop, ob_type::VarWrapper(val)); \
			} \
		} \
	}
	*/
	#define REPLICATE_PROPERTY_CHANGE(_repl_inst, prop, val)
#endif

/**
 * Used to declare a class of the specified name.
 * @author John M. Harris, Jr.
 */
#define DECLARE_CLASS(Class_Name) \
	virtual Instance* cloneImpl(Instance* newOne); \
	virtual QString getClassName(); \
	virtual int wrap_lua(lua_State* L); \
	DECLARE_GAME_STATIC_INIT(Class_Name); \
	protected: \
		static QString ClassName; \
		static QString LuaClassName

/**
 * Used to make services harder to screw up.
 * @author John M. Harris, Jr.
 */
#define DECLARE_SERVICE(Class_Name) \
	virtual void Destroy(); \
	virtual void Remove(); \
	DECLARE_CLASS(Class_Name)

/**
 * Used to make services harder to screw up.
 * @author John M. Harris, Jr.
 */
#define DEFINE_SERVICE(Class_Name) \
	void Class_Name::Destroy(){} \
	void Class_Name::Remove(){} \
	Instance* Class_Name::cloneImpl(Instance* newGuy){ \
		return newGuy; \
	}

/**
 * Used to define a class of the specified name.
 * @author John M. Harris, Jr.
 */
#define DEFINE_CLASS(Class_Name, isInstable, isAService, ParentClass) \
	struct Class_Name##ClassMaker: public OpenBlox::ClassMaker{ \
		OpenBlox::Instance::Instance* getInstance() const{ \
			return new Class_Name; \
		} \
		bool isA(const OpenBlox::Instance::Instance* obj){ \
			return (dynamic_cast<const Class_Name*>(obj)) != 0; \
		} \
		bool isInstantiatable(){ \
			return isInstable; \
		} \
		bool isService(bool isDataModel){ \
			OB_UNUSED(isDataModel); \
			return isAService; \
		} \
		QString getParentClassName(){ \
			return #ParentClass; \
		} \
	}; \
	STATIC_GAME_INIT(Class_Name){ \
		OpenBlox::ClassFactory::getInstance()->addClass(ClassName, new Class_Name##ClassMaker()); \
		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events); \
	} \
	QString Class_Name::ClassName = #Class_Name; \
	QString Class_Name::LuaClassName = "luaL_Instance_" + ClassName; \
	int Class_Name::wrap_lua(lua_State* L){ \
		Class_Name** udata = (Class_Name**)lua_newuserdata(L, sizeof(*this)); \
		*udata = this; \
		luaL_getmetatable(L, LuaClassName.toStdString().c_str()); \
		lua_setmetatable(L, -2); \
		return 1; \
	} \
	QString Class_Name::getClassName(){ \
		return ClassName; \
	} \

#define DEFINE_NONCREATE_CLASS(Class_Name, ParentClass) \
	struct Class_Name##ClassMaker: public OpenBlox::ClassMaker{ \
		OpenBlox::Instance::Instance* getInstance() const{ \
			return NULL; \
		} \
		bool isA(const OpenBlox::Instance::Instance* obj){ \
			return (dynamic_cast<const Class_Name*>(obj)) != 0; \
		} \
		bool isInstantiatable(){ \
			return false; \
		} \
		bool isService(bool isDataModel){ \
			OB_UNUSED(isDataModel); \
			return false; \
		} \
		QString getParentClassName(){ \
			return #ParentClass; \
		} \
	}; \
	STATIC_GAME_INIT(Class_Name){ \
		OpenBlox::ClassFactory::getInstance()->addClass(ClassName, new Class_Name##ClassMaker()); \
		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events); \
	} \
	QString Class_Name::ClassName = #Class_Name; \
	QString Class_Name::LuaClassName = "luaL_Instance_" + ClassName; \
	int Class_Name::wrap_lua(lua_State* L){ \
		Class_Name** udata = (Class_Name**)lua_newuserdata(L, sizeof(*this)); \
		*udata = this; \
		luaL_getmetatable(L, LuaClassName.toStdString().c_str()); \
		lua_setmetatable(L, -2); \
		return 1; \
	} \
	QString Class_Name::getClassName(){ \
		return ClassName; \
	} \

/**
 * Used to define an abstract class of the specified name.
 * @author John M. Harris, Jr.
 */
#define DEFINE_ABS_CLASS(Class_Name, ParentClass) \
	struct Class_Name##ClassMaker: public OpenBlox::ClassMaker{ \
		OpenBlox::Instance::Instance* getInstance() const{ \
			return NULL; \
		} \
		bool isA(const OpenBlox::Instance::Instance* obj){ \
			return (dynamic_cast<const Class_Name*>(obj)) != 0; \
		} \
		bool isInstantiatable(){ \
			return false; \
		} \
		bool isService(bool isDataModel){ \
			OB_UNUSED(isDataModel); \
			return false; \
		} \
		QString getParentClassName(){ \
			return #ParentClass; \
		} \
	}; \
	STATIC_GAME_INIT(Class_Name){ \
		OpenBlox::ClassFactory::getInstance()->addClass(ClassName, new Class_Name##ClassMaker()); \
		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events); \
	} \
	QString Class_Name::ClassName = #Class_Name; \
	QString Class_Name::LuaClassName = "luaL_Instance_" + ClassName; \
	int Class_Name::wrap_lua(lua_State* L){ \
		Class_Name** udata = (Class_Name**)lua_newuserdata(L, sizeof(*this)); \
		*udata = this; \
		luaL_getmetatable(L, LuaClassName.toStdString().c_str()); \
		lua_setmetatable(L, -2); \
		return 1; \
	} \
	QString Class_Name::getClassName(){ \
		return ClassName; \
	}

/**
 * Used in Instance::register_lua_events to quickly wrap an event in a getter function.
 * @author John M. Harris, Jr.
 */
#define WRAP_EVT(Event_Name) \
[](lua_State* L)->int{ \
	Instance* inst = checkInstance(L, 1); \
	if(inst){ \
		return inst->Event_Name->wrap_lua(L); \
	} \
	return 0; \
}

/**
 * Used in Instance::register_lua_events to quickly wrap an event in a getter function.
 * @author John M. Harris, Jr.
 */
#define WRAP_EVENT(Class_Name, Event_Name) \
[](lua_State* L)->int{ \
	Instance* inst = checkInstance(L, 1); \
	if(inst){ \
		if(Class_Name* rs = dynamic_cast<Class_Name*>(inst)){ \
			return rs->Event_Name->wrap_lua(L); \
		} \
	} \
	return 0; \
}

typedef void (*luaRegisterFunc)(lua_State* L);

class Instance
	#ifndef OB_NO_NETWORKING
	: public RakNet::NetworkIDObject
	#endif
	{
	public:
		Instance();
		virtual ~Instance();

		Ogre::Node* getOgreNode();
		Ogre::Node* setOgreNode(Ogre::Node* newNode);

		#ifndef OB_NO_NETWORKING
		static RakNet::NetworkIDManager* getNetworkIDManager();
		#endif

		virtual Instance* cloneImpl(Instance* newOne);

		virtual void ClearAllChildren();
		virtual Instance* Clone();
		virtual void Destroy();
		virtual void Remove();
		virtual Instance* FindFirstChild(QString name, bool recursive = false);
		virtual std::vector<Instance*> GetChildren();
		virtual QString GetFullName();
		virtual bool IsA(QString name);
		virtual bool IsAncestorOf(Instance* descendant);
		virtual bool IsDescendantOf(Instance* ancestor);

		void parentLock();

		virtual QString getClassName();

		void setName(QString name);
		QString getName();

		virtual void tick();
		virtual void tickChildren();

		virtual void setProperty(QString propName, OpenBlox::Type::VarWrapper* wrapper);

		#ifndef OB_NO_NETWORKING
		virtual void replicate(RakNet::RakPeerInterface* peer, RakNet::SystemAddress addr);
		virtual void replicateProperties(RakNet::RakPeerInterface* peer, RakNet::SystemAddress addr);
		virtual void replicateChildren(RakNet::RakPeerInterface* peer, RakNet::SystemAddress addr);
		#endif

		static void registerLuaClass(QString className, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters, luaRegisterFunc register_events);

		virtual QString toString();
		virtual void setParent(Instance* parent, bool useDMNotify);
		Instance* getParent();

		virtual int wrap_lua(lua_State* L) = 0;

		static Instance* checkInstance(lua_State*, int index);

		static int lua_index(lua_State* L);
		static int lua_newindex(lua_State* L);

		static int lua_eq(lua_State* L);

		static int lua_toString(lua_State* L);

		static int lua_getClassName(lua_State* L);
		static int lua_getName(lua_State* L);
		static int lua_setName(lua_State* L);
		static int lua_getParent(lua_State* L);
		static int lua_setParent(lua_State* L);
		static int lua_getArchivable(lua_State* L);
		static int lua_setArchivable(lua_State* L);

		static int lua_readOnlyProperty(lua_State* L);

		static int lua_ClearAllChildren(lua_State* L);
		static int lua_Clone(lua_State* L);
		static int lua_Destroy(lua_State* L);
		static int lua_Remove(lua_State* L);
		static int lua_FindFirstChild(lua_State* L);
		static int lua_GetChildren(lua_State* L);
		static int lua_GetFullName(lua_State* L);
		static int lua_IsA(lua_State* L);
		static int lua_IsAncestorOf(lua_State* L);
		static int lua_IsDescendantOf(lua_State* L);
		static int lua_GetNetworkID(lua_State* L);

		bool ParentLocked;

		OpenBlox::Type::Event* Changed;
		OpenBlox::Type::Event* AncestryChanged;
		OpenBlox::Type::Event* ChildAdded;
		OpenBlox::Type::Event* ChildRemoved;
		OpenBlox::Type::Event* DescendantAdded;
		OpenBlox::Type::Event* DescendantRemoving;

		DECLARE_GAME_STATIC_INIT(Instance);
	protected:
		bool Archivable;
		static QString ClassName;
		static QString LuaClassName;
		QString Name;
		Instance* Parent;

		Ogre::Node* ogreNode;

		#ifndef OB_NO_NETWORKING
		static RakNet::NetworkIDManager* networkIDManager;
		#endif

		void fireAncestryChanged(std::vector<OpenBlox::Type::VarWrapper>* args);
		void fireDescendantAdded(std::vector<OpenBlox::Type::VarWrapper>* args);
		void fireDescendantRemoving(std::vector<OpenBlox::Type::VarWrapper>* args);

		void propertyChanged(QString property);
		static void propertyChanged(QString property, Instance* inst);

		virtual void removeChild(Instance* kid);
		virtual void addChild(Instance* kid);

		static void register_lua_metamethods(lua_State* L);
		static void register_lua_methods(lua_State* L);
		static void register_lua_property_setters(lua_State* L);
		static void register_lua_property_getters(lua_State* L);
		static void register_lua_events(lua_State* L);

		std::vector<Instance*> children;
};

END_INSTANCE

#endif
