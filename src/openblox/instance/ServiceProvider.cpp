
#include "ServiceProvider.h"

BEGIN_INSTANCE
	DEFINE_ABS_CLASS(ServiceProvider, Instance);

	ServiceProvider::ServiceProvider(){}

	ServiceProvider::~ServiceProvider(){}

	Instance* ServiceProvider::cloneImpl(Instance* newOne){
		return newOne;
	}

	/**
	 * Pretty much the same as FindFirstChild, but made specifically for services. Checks by ClassName instead of Name.
	 * @param QString className
	 * @returns Instance* The service requested or NULL
	 * @author John M. Harris, Jr.
	 */
	Instance* ServiceProvider::FindService(QString className){
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(kid->getClassName() == className){
					return kid;
				}
			}
		}
		return NULL;
	}

	/**
	 * Calls FindService, and attempts to create the service if it is not found.
	 * @param QString className
	 * @returns Instance* The service requested, or NULL if it cannot be created.
	 * @author John M. Harris, Jr.
	 */
	Instance* ServiceProvider::GetService(QString className){
		Instance* foundService = FindService(className);
		if(foundService != NULL){
			return foundService;
		}
		Instance* newGuy = OpenBlox::ClassFactory::getInstance()->createService(className, false);
		if(newGuy){
			newGuy->setParent(this, false);
			newGuy->parentLock();
		}
		return newGuy;
	}

	/**
	 * Handles the ServiceProvider::FindService method for Lua.
	 * @param lua_State* Lua State
	 * @returns int 1, a userdata value on the Lua stack representing the requested service or nil.
	 * @author John M. Harris, Jr.
	 */
	int ServiceProvider::lua_FindService(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(ServiceProvider* sp = dynamic_cast<ServiceProvider*>(inst)){
			QString serviceName = QString(luaL_checkstring(L, 2));
			Instance* foundGuy = sp->FindService(serviceName);
			if(foundGuy != NULL){
				return foundGuy->wrap_lua(L);
			}
			lua_pushnil(L);
			return 1;
		}
		return luaL_error(L, COLONERR, "FindService");
	}

	/**
	 * Handles the ServiceProvider::GetService method for Lua.
	 * @param lua_State* Lua State
	 * @returns int 1, a userdata value on the Lua stack representing the requested service or nil.
	 * @author John M. Harris, Jr.
	 */
	int ServiceProvider::lua_GetService(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(ServiceProvider* sp = dynamic_cast<ServiceProvider*>(inst)){
			QString serviceName = QString(luaL_checkstring(L, 2));
			Instance* foundGuy = sp->GetService(serviceName);
			if(foundGuy != NULL){
				return foundGuy->wrap_lua(L);
			}
			lua_pushnil(L);
			return 0;
		}
		return luaL_error(L, COLONERR, "GetService");
	}

	void ServiceProvider::register_lua_methods(lua_State* L){
		Instance::register_lua_methods(L);

		luaL_Reg methods[]{
			{"FindService", lua_FindService},
			{"GetService", lua_GetService},
			{NULL, NULL}
		};
		luaL_setfuncs(L, methods, 0);
	}

END_INSTANCE
