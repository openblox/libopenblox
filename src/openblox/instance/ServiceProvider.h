#ifndef OPENBLOX_INSTANCE_SERVICEPROVIDER_H_
#define OPENBLOX_INSTANCE_SERVICEPROVIDER_H_

#include "Instance.h"

BEGIN_INSTANCE

class ServiceProvider: public Instance{
	public:
		ServiceProvider();
		virtual ~ServiceProvider();

		virtual Instance* FindService(QString className);
		virtual Instance* GetService(QString className);

		static int lua_FindService(lua_State* L);
		static int lua_GetService(lua_State* L);

		DECLARE_CLASS(ServiceProvider);

		static void register_lua_methods(lua_State* L);
};

END_INSTANCE

#endif
