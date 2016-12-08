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

#include "instance/Instance.h"

#include <algorithm>

#include "OBException.h"
#include "OBEngine.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS(Instance, NULL){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Instance::Instance(){
			Archivable = true;
			Name = ClassName;
			Parent = NULL;
			ParentLocked = false;

			netId = OB_NETID_UNASSIGNED;

			//TODO: Events
			TestEvent = new Type::Event("TestEvent");
		}

		Instance::~Instance(){
			//TODO: Events
		}

		void Instance::ClearAllChildren(){
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid){
					kid->Remove();
				}
			}
		}

		Instance* Instance::Clone(){
			if(Archivable){
				Instance* newGuy = cloneImpl();
				if(newGuy == NULL){
					return NULL;
				}
				for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
					Instance* kidClone = children[i];
					if(kidClone){
						kidClone->setParent(newGuy, true);
					}
				}
			}
			return NULL;
		}

		void Instance::Destroy(){
			if(ParentLocked){
				ParentLocked = false;
			}
			setParent(NULL, true);
			ParentLocked = true;
			//TODO:
			//Changed->disconnectAll();
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid){
					kid->Destroy();
				}
			}
		}

		void Instance::Remove(){
			setParent(NULL, true);
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid){
					kid->Remove();
				}
			}
		}

		Instance* Instance::FindFirstChild(std::string name, bool recursive){
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid){
					if(kid->Name == name){
						return kid;
					}
				}
			}
			if(recursive){
				for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
					Instance* kid = children[i];
					if(kid){
						Instance* myFind = kid->FindFirstChild(name, recursive);
						if(myFind){
							return myFind;
						}
					}
				}
			}
			return NULL;
		}

		Instance* Instance::FindFirstChildOfClass(std::string className, bool recursive){
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid){
					if(kid->ClassName == className){
						return kid;
					}
				}
			}
			if(recursive){
				for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
					Instance* kid = children[i];
					if(kid){
						Instance* myFind = kid->FindFirstChildOfClass(className, recursive);
						if(myFind){
							return myFind;
						}
					}
				}
			}
			return NULL;
		}

		std::vector<Instance*> Instance::GetChildren(){
			return std::vector<Instance*>(children);
		}

		std::string Instance::GetFullName(){
			std::string fullName = Name;
			if(Parent){
				fullName = Parent->GetFullName() + "." + fullName;
			}
			return fullName;
		}

		bool Instance::IsA(std::string name){
			OB::ClassFactory::isA(this, name);
		}

		bool Instance::IsAncestorOf(Instance* descendant){
			if(descendant == NULL){
				return false;
			}
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid){
					if(kid == descendant){
						return true;
					}
				}
			}
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid){
					bool isAncestor = kid->IsAncestorOf(descendant);
					if(isAncestor){
						return true;
					}
				}
			}
			return false;
		}

		bool Instance::IsDescendantOf(Instance* ancestor){
			if(ancestor == NULL){
				return true;
			}
			return ancestor->IsAncestorOf(this);
		}

		ob_int64 Instance::GetNetworkID(){
			return netId;
		}
		
		void Instance::tick(){
			tickChildren();
		}
	
		void Instance::tickChildren(){
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid){
					kid->tick();
				}
			}
		}

		void Instance::registerLuaClass(std::string className, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters, luaRegisterFunc register_events){
			lua_State* L = OB::OBEngine::getInstance()->getGlobalLuaState();

			luaL_newmetatable(L, className.c_str());
			register_metamethods(L);

			lua_pushstring(L, "__metatable");
			lua_pushstring(L, "This metatable is locked");
			lua_rawset(L, -3);

			//Methods
			lua_pushstring(L, "__methods");
			lua_newtable(L);
			register_methods(L);
			lua_rawset(L, -3);

			//Property getters
			lua_pushstring(L, "__propertygetters");
			lua_newtable(L);
			register_getters(L);
			lua_rawset(L, -3);

			//Property setters
			lua_pushstring(L, "__propertysetters");
			lua_newtable(L);
			register_setters(L);
			lua_rawset(L, -3);

			//Events
			lua_pushstring(L, "__events");
			lua_newtable(L);
			register_events(L);
			lua_rawset(L, -3);

			//Item get
			lua_pushstring(L, "__index");
			lua_pushcfunction(L, lua_index);
			lua_rawset(L, -3);

			//Item set
			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, lua_newindex);
			lua_rawset(L, -3);

			lua_pop(L, 1);
		}

		void Instance::setParent(Instance* parent, bool useDMNotify){
			if(Parent == parent){
				//noop
				return;
			}
			if(ParentLocked){
				std::string errMsg = "The Parent property of ";
				errMsg = errMsg + Name + " is locked.";
				throw OBException(errMsg.c_str());
				return;
			}
			if(parent == this){
				std::string errMsg = "Attempt to set ";
				errMsg = errMsg + GetFullName() + " as its own parent";
				throw OBException(errMsg.c_str());
				return;
			}

			if(Parent){
				Parent->removeChild(this);
			}
			Parent = parent;
			if(Parent){
				Parent->addChild(this);

				//TODO: Just look at this cruft!

#ifdef OPENBLOX_SERVER
				if(useDMNotify){
					OpenBlox::OBGame* game = OpenBlox::OBGame::getInstance();
					if(game){
						ob_instance::DataModel* dm = game->getDataModel();
						if(dm){
							if(dm->isServer()){
								if(this->IsDescendantOf(dm)){
									Instance* nsInst = dm->FindService("NetworkServer");
									if(ob_instance::NetworkServer* ns = dynamic_cast<ob_instance::NetworkServer*>(nsInst)){
										ns->sendCreateInstancePacket(GetNetworkID(), RakNet::RakString(getClassName().toStdString().c_str()));

										OB_SetParentPacket spp;
										spp.net_id = GetNetworkID();
										spp.parent_net_id = Parent->GetNetworkID();

										ns->sendPacket(OB_SET_PARENT, spp);
									}
								}
							}
						}
					}
				}
#else
				(void)useDMNotify;
#endif
			}

			//fireAncestryChanged(new std::vector<ob_type::VarWrapper>({ob_type::VarWrapper(this), ob_type::VarWrapper(Parent)}));
			//propertyChanged("Parent");
		}

		std::string Instance::toString(){
			return Name;
		}

		Instance* Instance::cloneImpl(){
			return NULL;
		}

		Instance* Instance::getParent(){
			return Parent;
		}

		void Instance::removeChild(Instance* kid){
			if(kid){
				children.erase(std::remove(children.begin(), children.end(), kid));

				//TODO:
				/*std::vector<ob_type::VarWrapper>* args = new std::vector<ob_type::VarWrapper>({ob_type::VarWrapper(kid)});
				ChildRemoved->Fire(args);
				fireDescendantRemoving(args);*/
			}
		}

		void Instance::addChild(Instance* kid){
			if(kid){
				children.push_back(kid);

				//TODO:
				/*std::vector<ob_type::VarWrapper>* args = new std::vector<ob_type::VarWrapper>({ob_type::VarWrapper(kid)});
				ChildAdded->Fire(args);
				fireDescendantAdded(args);*/
			}
		}

		void Instance::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Instance::lua_toString},
				{"__eq", lua_eq},
				{NULL, NULL}
			};
			luaL_setfuncs(L, metamethods, 0);
		}

		void Instance::register_lua_methods(lua_State* L){
			luaL_Reg methods[] = {
				{"ClearAllChildren", lua_ClearAllChildren},
				{"Clone", lua_Clone},
				{"Destroy", lua_Destroy},
				{"Remove", lua_Remove},
				{"FindFirstChild", lua_FindFirstChild},
				{"GetChildren", lua_GetChildren},
				{"GetFullName", lua_GetFullName},
				{"IsA", lua_IsA},
				{"IsAncestorOf", lua_IsAncestorOf},
				{"IsDescendantOf", lua_IsDescendantOf},
				{"GetNetworkID", lua_GetNetworkID},
				{NULL, NULL}
			};
			luaL_setfuncs(L, methods, 0);
		}

		void Instance::register_lua_property_setters(lua_State* L){
			luaL_Reg properties[] = {
				{"ClassName", lua_readOnlyProperty},
				{"Name", lua_setName},
				{"Parent", lua_setParent},
				{"Archivable", lua_setArchivable},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Instance::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[] = {
				{"ClassName", lua_getClassName},
				{"Name", lua_getName},
				{"Parent", lua_getParent},
				{"Archivable", lua_getArchivable},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Instance::register_lua_events(lua_State* L){
			luaL_Reg events[] = {
				{"TestEvent", WRAP_EVT(TestEvent)},
				//TODO: Events don't even exist yet.
				/*{"Changed", WRAP_EVT(Changed)},
				  {"AncestryChanged", WRAP_EVT(AncestryChanged)},
				  {"ChildAdded", WRAP_EVT(ChildAdded)},
				  {"ChildRemoved", WRAP_EVT(ChildRemoved)},
				  {"DescendantAdded", WRAP_EVT(DescendantAdded)},
				  {"DescendantRemoving", WRAP_EVT(DescendantRemoving)},*/
				{NULL, NULL}
			};
			luaL_setfuncs(L, events, 0);
		}

		Instance* Instance::checkInstance(lua_State* L, int index){
			if(lua_isuserdata(L, index)){
				std::vector<std::string> existing = OB::ClassFactory::getRegisteredClasses();
				unsigned size = existing.size();
				void* udata = lua_touserdata(L, index);
				int meta = lua_getmetatable(L, index);
				if(meta != 0){
					for(unsigned i = 0; i<size; i++){
						std::string name = "luaL_Instance_" + existing[i];
						luaL_getmetatable(L, name.c_str());
						if(lua_rawequal(L, -1, -2)){
							lua_pop(L, 2);
							return *(Instance**)udata;
						}
						lua_pop(L, 1);
					}
				}
				return NULL;
			}
			return NULL;
		}

		int Instance::lua_newindex(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				const char* name = luaL_checkstring(L, 2);
				lua_getmetatable(L, 1);//-3
				lua_getfield(L, -1, "__propertysetters");//-2
				lua_getfield(L, -1, name);//-1
				if (lua_iscfunction(L, -1)){
					lua_remove(L, -2);
					lua_remove(L, -2);

					lua_pushvalue(L, 1);
					lua_pushvalue(L, 3);
					lua_call(L, 2, 0);

					return 0;
				}else{
					lua_pop(L, 3);

					return luaL_error(L, "attempt to index '%s' (a nil value)", name);
				}
			}
			return 0;
		}

		int Instance::lua_index(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				const char* name = luaL_checkstring(L, 2);

				lua_getmetatable(L, 1);//-3
				lua_getfield(L, -1, "__propertygetters");//-2
				lua_getfield(L, -1, name);//-1
				if(lua_iscfunction(L, -1)){
					lua_remove(L, -2);
					lua_remove(L, -2);

					lua_pushvalue(L, 1);
					lua_call(L, 1, 1);
					return 1;
				}else{
					lua_pop(L, 2);
					//Check methods
					lua_getfield(L, -1, "__methods");//-2
					lua_getfield(L, -1, name);//-1
					if(lua_iscfunction(L, -1)){
						lua_remove(L, -2);
						lua_remove(L, -3);

						return 1;
					}else{
						lua_pop(L, 2);
						//Check events
						lua_getfield(L, -1, "__events");//-2
						lua_getfield(L, -1, name);//-1
						if(lua_iscfunction(L, -1)){
							lua_remove(L, -2);
							lua_remove(L, -3);

							lua_pushvalue(L, 1);
							lua_call(L, 1, 1);
							return 1;
						}else{
							lua_pop(L, 3);

							Instance* kiddie = inst->FindFirstChild(name, false);
							if(kiddie){
								return kiddie->wrap_lua(L);
							}

							return luaL_error(L, "attempt to index '%s' (a nil value)", name);
						}
					}
				}
			}
			return 0;
		}

		int Instance::lua_eq(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				Instance* oinst = checkInstance(L, 2);
				if(oinst){
					lua_pushboolean(L, inst == oinst);
					return 1;
				}
			}
			lua_pushboolean(L, false);
			return 1;
		}

		int Instance::lua_toString(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				lua_pushstring(L, inst->toString().c_str());
				return 1;
			}
			return 0;
		}

		int Instance::lua_getClassName(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				std::string className = inst->getClassName();
				lua_pushstring(L, className.c_str());
				return 1;
			}
			lua_pushnil(L);
			return 1;
		}

		int Instance::lua_readOnlyProperty(lua_State* L){
			//Welp. This is how ROBLOX does it.
			luaL_error(L, "can't set value");
			return 0;
		}

		int Instance::lua_getName(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				lua_pushstring(L, inst->Name.c_str());
				return 1;
			}
			return 0;
		}

		int Instance::lua_setName(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				std::string desired = std::string(luaL_checkstring(L, 2));
				if(inst->Name != desired){
					inst->Name = desired;

					//TODO:
					//REPLICATE_PROPERTY_CHANGE(inst, "Name", inst->Name);

					//propertyChanged("Name", inst);
				}
				return 0;
			}
			return 0;
		}

		int Instance::lua_getParent(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				if(inst->Parent){
					return inst->Parent->wrap_lua(L);
				}
				lua_pushnil(L);
				return 1;
			}
			return 0;
		}

		int Instance::lua_setParent(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				bool throwErrorIf = true;
				Instance* otherInst = NULL;
				if(lua_isnil(L, 2)){
					throwErrorIf = false;
				}else{
					otherInst = checkInstance(L, 2);
				}
				if(otherInst || !throwErrorIf){
					try{
						inst->setParent(otherInst, true);
					}catch(OBException& ex){
						return luaL_error(L, ex.getMessage().c_str());
					}
					return 0;
				}else{
					return luaL_argerror(L, 2, "Instance expected");
				}
				return 0;
			}
			return 0;
		}

		int Instance::lua_getArchivable(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				lua_pushboolean(L, inst->Archivable);
				return 1;
			}
			return 0;
		}

		int Instance::lua_setArchivable(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				//Again, following ROBLOX's ways....
				bool newVal = false;
				if(lua_isboolean(L, 2)){
					newVal = lua_toboolean(L, 2);
				}else if(!lua_isnoneornil(L, 2)){
					newVal = true;
				}
				if(inst->Archivable != newVal){
					inst->Archivable = newVal;

					//TODO:
				
					//REPLICATE_PROPERTY_CHANGE(inst, "Archivable", inst->Archivable);

					//propertyChanged("Archivable", inst);
				}
			}
			return 0;
		}

		int Instance::lua_ClearAllChildren(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				inst->ClearAllChildren();
				return 0;
			}
			return luaL_error(L, COLONERR, "ClearAllChildren");
		}

		int Instance::lua_Clone(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				Instance* newGuy = inst->Clone();
				if(newGuy){
					return newGuy->wrap_lua(L);
				}
				return 0;
			}
			return luaL_error(L, COLONERR, "Clone");
		}

		int Instance::lua_Destroy(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				inst->Destroy();
				return 0;
			}
			return luaL_error(L, COLONERR, "Destroy");
		}

		int Instance::lua_Remove(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				inst->Remove();
				return 0;
			}
			return luaL_error(L, COLONERR, "Remove");
		}

		int Instance::lua_FindFirstChild(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				const char* kidName = luaL_checkstring(L, 2);
				bool recursive = false;
				if(!lua_isnoneornil(L, 3)){
					if(lua_isboolean(L, 3)){
						recursive = lua_toboolean(L, 3);
					}else{
						luaL_argerror(L, 3, "boolean expected");
					}
				}
				Instance* foundStuff = inst->FindFirstChild(kidName, recursive);
				if(foundStuff){
					return foundStuff->wrap_lua(L);
				}
				lua_pushnil(L);
				return 1;
			}
			return luaL_error(L, COLONERR, "FindFirstChild");
		}

		int Instance::lua_GetChildren(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				lua_newtable(L);

				for(std::vector<Instance*>::size_type i = 0; i != inst->children.size(); i++){
					Instance* kid = inst->children[i];
					if(kid){
						int lIndex = i + 1;
						//lua_pushnumber(L, lIndex);
						kid->wrap_lua(L);
						lua_rawseti(L, -2, lIndex);
					}
				}
				return 1;
			}
			return luaL_error(L, COLONERR, "GetChildren");
		}

		int Instance::lua_GetFullName(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				std::string fullName = inst->GetFullName();
				lua_pushstring(L, fullName.c_str());
				return 1;
			}
			return luaL_error(L, COLONERR, "GetFullName");
		}

		int Instance::lua_IsA(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				const char* checkName = luaL_checkstring(L, 2);
				if(checkName){
					bool isIt = inst->IsA(checkName);
					lua_pushboolean(L, isIt);
					return 1;
				}
				lua_pushboolean(L, false);
				return 1;
			}
			return luaL_error(L, COLONERR, "IsA");
		}

		int Instance::lua_IsAncestorOf(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				bool throwErrorIf = true;
				Instance* otherInst = NULL;
				if(lua_isnil(L, 2)){
					throwErrorIf = false;
				}else{
					otherInst = checkInstance(L, 2);
				}
				if(otherInst || !throwErrorIf){
					bool isIt = inst->IsAncestorOf(otherInst);
					lua_pushboolean(L, isIt);
					return 1;
				}else{
					return luaL_argerror(L, 2, "Instance expected");
				}
				return 0;
			}
			return luaL_error(L, COLONERR, "IsAncestorOf");
		}

		int Instance::lua_IsDescendantOf(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				bool throwErrorIf = true;
				Instance* otherInst = NULL;
				if(lua_isnil(L, 2)){
					throwErrorIf = false;
				}else{
					otherInst = checkInstance(L, 2);
				}
				if(otherInst || !throwErrorIf){
					bool isIt = inst->IsDescendantOf(otherInst);
					lua_pushboolean(L, isIt);
					return 1;
				}else{
					return luaL_argerror(L, 2, "Instance expected");
				}
				return 0;
			}
			return luaL_error(L, COLONERR, "IsDescendantOf");
		}

		int Instance::lua_GetNetworkID(lua_State* L){
			Instance* inst = checkInstance(L, 1);
			if(inst){
				lua_pushnumber(L, inst->GetNetworkID());
				return 1;
			}
			return luaL_error(L, COLONERR, "GetNetworkID");
		}
	}
}
