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

#include "lua/OBLua.h"
#include "ClassFactory.h"

#ifndef OB_INST_INSTANCE
#define OB_INST_INSTANCE

typedef void (*luaRegisterFunc)(lua_State* L);

#define DECLARE_CLASS(Class_Name) \
	virtual Instance* cloneImpl(); \
	virtual std::string getClassName(); \
	virtual int wrap_lua(lua_State* L); \
	static OB::ClassMetadata* _ob_classmetadata; \
	static void _ob_init(); \
protected: \
	static std::string ClassName; \
	static std::string LuaClassName

#define DEFINE_CLASS(Class_Name, isInstable, isAService, ParentClass) \
	class Class_Name##ClassMetadata: public OB::ClassMetadata{ \
		public: \
		Class_Name##ClassMetadata(){ \
			OB::ClassFactory::addClass(#Class_Name, this); \
		} \
		virtual OB::Instance::Instance* newInstance(){ \
			return new Class_Name; \
		} \
		virtual bool isA(OB::Instance::Instance* obj){ \
			return (dynamic_cast<const Class_Name*>(obj)) != NULL; \
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
	}

#define DEFINE_CLASS_ABS(Class_Name, ParentClass) \
    class Class_Name##ClassMetadata: public OB::ClassMetadata{ \
		public: \
		Class_Name##ClassMetadata(){ \
			OB::ClassFactory::addClass(#Class_Name, this); \
		} \
		virtual OB::Instance::Instance* newInstance(){ \
			return NULL; \
		} \
		virtual bool isA(OB::Instance::Instance* obj){ \
			return (dynamic_cast<const Class_Name*>(obj)) != NULL; \
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
	}

#define REGISTER_CLASS(Class_Name) \
	OB::ClassMetadata* Class_Name::_ob_classmetadata = new Class_Name##ClassMetadata; \
	void Class_Name::_ob_init()
		

namespace OB{
	/**
	 * Instance is the base class of all world objects in OpenBlox.
	 * @author John M. Harris, Jr.
	 */
	namespace Instance{
		class Instance{
			public:
				Instance();
				virtual ~Instance();

				virtual void ClearAllChildren();
				virtual Instance* Clone();
				virtual void Destroy();
				virtual void Remove();
				virtual Instance* FindFirstChild(std::string name, bool recursive = false);
				virtual std::vector<Instance*> GetChildren();
				virtual std::string GetFullName();
				virtual bool IsA(std::string name);
				virtual bool IsAncestorOf(Instance* descendant);
				virtual bool IsDescendantOf(Instance* ancestor);

				virtual void tick();
				virtual void tickChildren();

				static void registerLuaClass(std::string className, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters, luaRegisterFunc register_events);

				virtual std::string toString();
				virtual void setParent(Instance* parent, bool useDMNotify);
				Instance* getParent();
				
				static Instance* checkInstance(lua_State* L, int index);

				static int lua_index(lua_State* L);
				static int lua_newindex(lua_State* L);

			    DECLARE_CLASS(Instance);	
		};
	}
}

#endif // OB_INST_INSTANCE


// Local Variables:
// mode: c++
// End:
