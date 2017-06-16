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
#include "BitStream.h"

#include "instance/NetworkReplicator.h"
#include "instance/NetworkServer.h"

#include "type/Color3.h"
#include "type/Vector3.h"
#include "type/Vector2.h"

#include <iostream>

namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS(Instance, NULL){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		Instance::Instance(OBEngine* eng){
			this->eng = eng;
			
			Archivable = true;
			Name = ClassName;
			ParentLocked = false;

			netId = OB_NETID_UNASSIGNED;

			//TODO: Events
		    Changed = make_shared<Type::Event>("Changed");
			AncestryChanged = make_shared<Type::Event>("AncestryChanged");
		    ChildAdded = make_shared<Type::Event>("ChildAdded");
		    ChildRemoved = make_shared<Type::Event>("ChildRemoved");
			DescendantAdded = make_shared<Type::Event>("DescendantAdded");
			DescendantRemoving = make_shared<Type::Event>("DescendantRemoving");
		}

		Instance::~Instance(){
			if(netId >= OB_NETID_START){
				shared_ptr<DataModel> dm = eng->getDataModel();
				if(dm){
					dm->dropInstance(netId);
				}
			}
		}

		void Instance::setName(std::string name){
		    if(Name != name){
			    Name = name;

				REPLICATE_PROPERTY_CHANGE(Name);
				propertyChanged("Name");
			}
		}

		std::string Instance::getName(){
			return Name;
		}

		void Instance::setArchivable(bool archivable){
			if(Archivable != archivable){
			    Archivable = archivable;

				REPLICATE_PROPERTY_CHANGE(Archivable);
				propertyChanged("Archivable");
			}
		}

		bool Instance::getArchivable(){
			return Archivable;
		}

		OBEngine* Instance::getEngine(){
			return eng;
		}

		void Instance::ClearAllChildren(){
			std::vector<shared_ptr<Instance>> kids = GetChildren();
			
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != kids.size(); i++){
				shared_ptr<Instance> kid = kids[i];
				if(kid){
					kid->Remove();
				}
			}
		}

		shared_ptr<Instance> Instance::Clone(){
			if(Archivable){
				shared_ptr<Instance> newGuy = cloneImpl();
				if(newGuy == NULL){
					return NULL;
				}
				for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != children.size(); i++){
					shared_ptr<Instance> kidClone = children[i];
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
			Changed->disconnectAll();

			std::vector<shared_ptr<Instance>> kids = GetChildren();
			
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != kids.size(); i++){
				shared_ptr<Instance> kid = kids[i];
				if(kid){
					kid->Destroy();
				}
			}
		}

		void Instance::Remove(){
			setParent(NULL, true);

			std::vector<shared_ptr<Instance>> kids = GetChildren();
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != kids.size(); i++){
				shared_ptr<Instance> kid = kids[i];
				if(kid){
					kid->Remove();
				}
			}
		}

		shared_ptr<Instance> Instance::FindFirstChild(std::string name, bool recursive){
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != children.size(); i++){
				shared_ptr<Instance> kid = children[i];
				if(kid){
					if(kid->Name == name){
						return kid;
					}
				}
			}
			if(recursive){
				for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != children.size(); i++){
					shared_ptr<Instance> kid = children[i];
					if(kid){
						shared_ptr<Instance> myFind = kid->FindFirstChild(name, recursive);
						if(myFind){
							return myFind;
						}
					}
				}
			}
			return NULL;
		}

		shared_ptr<Instance> Instance::FindFirstChildOfClass(std::string className, bool recursive){
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != children.size(); i++){
				shared_ptr<Instance> kid = children[i];
				if(kid){
					if(kid->ClassName == className){
						return kid;
					}
				}
			}
			if(recursive){
				for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != children.size(); i++){
					shared_ptr<Instance> kid = children[i];
					if(kid){
						shared_ptr<Instance> myFind = kid->FindFirstChildOfClass(className, recursive);
						if(myFind){
							return myFind;
						}
					}
				}
			}
			return NULL;
		}

		std::vector<shared_ptr<Instance>> Instance::GetChildren(){
			return std::vector<shared_ptr<Instance>>(children);
		}

		std::string Instance::GetFullName(){
			std::string fullName = Name;
			if(Parent){
				fullName = Parent->GetFullName() + "." + fullName;
			}
			return fullName;
		}

		bool Instance::IsA(std::string name){
			return OB::ClassFactory::isA(shared_from_this(), name);
		}

		bool Instance::IsAncestorOf(shared_ptr<Instance> descendant){
			if(descendant == NULL){
				return false;
			}
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != children.size(); i++){
				shared_ptr<Instance> kid = children[i];
				if(kid){
					if(kid == descendant){
						return true;
					}
				}
			}
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != children.size(); i++){
				shared_ptr<Instance> kid = children[i];
				if(kid){
					bool isAncestor = kid->IsAncestorOf(descendant);
					if(isAncestor){
						return true;
					}
				}
			}
			return false;
		}

		bool Instance::IsDescendantOf(shared_ptr<Instance> ancestor){
			if(ancestor == NULL){
				return true;
			}
			return ancestor->IsAncestorOf(shared_from_this());
		}

		ob_int64 Instance::GetNetworkID(){
			return netId;
		}

		void Instance::setNetworkID(ob_int64 netId){
			if(netId >= OB_NETID_START){
				shared_ptr<DataModel> dm = eng->getDataModel();
				if(dm){
					this->netId = netId;
					dm->putInstance(shared_from_this());
				}
			}else{
				this->netId = netId;
			}
		}

		void Instance::generateNetworkID(){
			shared_ptr<DataModel> dm = eng->getDataModel();
			if(dm){
				setNetworkID(dm->nextNetworkID());
			}
		}

		#if HAVE_ENET
		void Instance::replicate(shared_ptr<NetworkReplicator> peer){
			if(!peer){
				return;
			}

			if(netId < OB_NETID_DATAMODEL){
				return;
			}

		    BitStream bsOut;
			bsOut.writeSizeT(OB_NET_PKT_CREATE_INSTANCE);
			bsOut.writeUInt64(netId);
			bsOut.writeString(getClassName());

			peer->Send(OB_NET_CHAN_REPLICATION, bsOut);

			bsOut.reset();

			bsOut.writeSizeT(OB_NET_PKT_SET_PARENT);
			bsOut.writeUInt64(netId);
			if(Parent){
				bsOut.writeUInt64(Parent->GetNetworkID());
			}else{
				bsOut.writeUInt64(OB_NETID_NULL);
			}

			peer->Send(OB_NET_CHAN_REPLICATION, bsOut);

			replicateProperties(peer);
			replicateChildren(peer);
		}

		void Instance::replicateProperties(shared_ptr<NetworkReplicator> peer){
			peer->sendSetPropertyPacket(netId, "Name", make_shared<Type::VarWrapper>(Name));
			peer->sendSetPropertyPacket(netId, "Archivable", make_shared<Type::VarWrapper>(Archivable));
		}

		void Instance::replicateChildren(shared_ptr<NetworkReplicator> peer){
			std::vector<shared_ptr<Instance>> kids = children;
			
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != kids.size(); i++){
				shared_ptr<Instance> kid = kids[i];
				if(kid){
					if(kid->GetNetworkID() >= OB_NETID_DATAMODEL){
						kid->replicate(peer);
					}
				}
			}
		}
		#endif

		#if HAVE_PUGIXML
		void Instance::serializeThis(pugi::xml_node thisNode, shared_ptr<Instance> model_ptr){
			thisNode.append_attribute("type").set_value(getClassName().c_str());
			thisNode.append_attribute("id").set_value(serializedID().c_str());
			
			serializeProperties(thisNode, model_ptr);
			serializeChildren(thisNode, model_ptr);
		}
		
	    void Instance::serialize(pugi::xml_node parentNode, shared_ptr<Instance> model){
			if(Archivable){
				pugi::xml_node thisNode = parentNode.append_child(pugi::node_element);
				thisNode.set_name("instance");
				
				serializeThis(thisNode, model);
			}
		}

		void Instance::serializeChildren(pugi::xml_node parentNode, shared_ptr<Instance> model){
			std::vector<shared_ptr<Instance>> kids = children;
			
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != kids.size(); i++){
				shared_ptr<Instance> kid = kids[i];
				if(kid){
					if(kid->Archivable){
						kid->serialize(parentNode, model);
					}
				}
			}
		}

		void Instance::serializeProperties(pugi::xml_node thisNode, shared_ptr<Instance> model){
			std::map<std::string, _PropertyInfo> props = getProperties();
			for(auto it = props.begin(); it != props.end(); ++it){
				std::string name = it->first;
				_PropertyInfo pi = it->second;
				std::string stype = pi.type;

				if(pi.isSerialized){
					pugi::xml_node propNode = thisNode.append_child(pugi::node_element);
					propNode.set_name("property");
					propNode.append_attribute("name").set_value(name.c_str());
					propNode.append_attribute("type").set_value(stype.c_str());

					if(stype == "string"){
						propNode.text().set(getProperty(name)->asString().c_str());
					}
					if(stype == "int"){
						propNode.text().set(getProperty(name)->asInt());
					}
					if(stype == "bool"){
						propNode.text().set(getProperty(name)->asBool());
					}
					if(stype == "double"){
						propNode.text().set(getProperty(name)->asDouble());
					}
					if(stype == "float"){
						propNode.text().set(getProperty(name)->asFloat());
					}
					if(stype == "Color3"){
						shared_ptr<Type::Color3> vval = getProperty(name)->asColor3();
						if(vval){
							propNode.text().set(vval->toString().c_str());
						}else{
							propNode.text().set("0, 0, 0");
						}
					}
					if(stype == "Vector2"){
						shared_ptr<Type::Vector2> vval = getProperty(name)->asVector2();
						if(vval){
							propNode.text().set(vval->toString().c_str());
						}else{
							propNode.text().set("0, 0");
						}
					}
					if(stype == "Vector3"){
						shared_ptr<Type::Vector3> vval = getProperty(name)->asVector3();
						if(vval){
							propNode.text().set(vval->toString().c_str());
						}else{
							propNode.text().set("0, 0, 0");
						}
					}
					if(stype == "Instance"){
						shared_ptr<Instance> vval = getProperty(name)->asInstance();
						if(model && !model->IsAncestorOf(vval)){
							// We don't want a reference to something we don't know about.
						    vval = NULL;
						}
						if(vval){
							propNode.text().set(vval->serializedID().c_str());
						}else{
							propNode.text().set("NULL");
						}
					}
				}
			}
		}

		void Instance::deserializeCreate(pugi::xml_node thisNode){
			for(pugi::xml_node cinst : thisNode.children("instance")){
				pugi::xml_attribute itype = cinst.attribute("type");
				pugi::xml_attribute iid = cinst.attribute("id");
				
				if(!itype.empty() && !iid.empty()){
					std::string stype = itype.as_string();
					std::string sid = iid.as_string();

					shared_ptr<OBSerializer> serializer = eng->getSerializer();
					if(serializer){
						shared_ptr<Instance> tInst = serializer->GetByID(sid);
						if(!tInst){
						    tInst = ClassFactory::createReplicate(stype, eng);
						}

						if(tInst){
							serializer->SetID(tInst, sid);
						
						    tInst->setParent(shared_from_this(), true);

						    tInst->deserializeCreate(cinst);
						}
					}
				}
			}
		}

		void Instance::deserialize(pugi::xml_node thisNode){
			deserializeCreate(thisNode);
			
			deserializeProperties(thisNode);

			for(pugi::xml_node cinst : thisNode.children("instance")){
				pugi::xml_attribute iid = cinst.attribute("id");

				if(!iid.empty()){
					shared_ptr<OBSerializer> serializer = eng->getSerializer();
					if(serializer){
						shared_ptr<Instance> childInst = serializer->GetByID(iid.as_string());
						if(childInst){
							childInst->deserializeProperties(cinst);
						}
					}
				}
			}
		}
		
		void Instance::deserializeProperties(pugi::xml_node thisNode){
			std::map<std::string, _PropertyInfo> props = getProperties();
			for(auto it = props.begin(); it != props.end(); ++it){
				std::string name = it->first;
				_PropertyInfo pi = it->second;
				std::string stype = pi.type;

				if(pi.isSerialized){
					pugi::xml_node propNode = thisNode.find_child_by_attribute("property", "name", name.c_str());

					if(!propNode.empty()){
						pugi::xml_text propVal = propNode.text();

						if(stype == "string"){
						    setProperty(name, make_shared<Type::VarWrapper>(std::string(propVal.as_string())));
						}
						if(stype == "int"){
							setProperty(name, make_shared<Type::VarWrapper>(propVal.as_int()));
						}
						if(stype == "bool"){
							setProperty(name, make_shared<Type::VarWrapper>(propVal.as_bool()));
						}
						if(stype == "double"){
						    setProperty(name, make_shared<Type::VarWrapper>(propVal.as_double()));
						}
						if(stype == "float"){
						    setProperty(name, make_shared<Type::VarWrapper>(propVal.as_float()));
						}
						if(stype == "Color3"){
						    setProperty(name, make_shared<Type::VarWrapper>(make_shared<Type::Color3>(propVal.as_string())));
						}
						if(stype == "Vector2"){
						    setProperty(name, make_shared<Type::VarWrapper>(make_shared<Type::Vector2>(propVal.as_string())));
						}
						if(stype == "Vector3"){
						    setProperty(name, make_shared<Type::VarWrapper>(make_shared<Type::Vector3>(propVal.as_string())));
						}
						if(stype == "Instance"){
							shared_ptr<OBSerializer> serializer = eng->getSerializer();
							if(serializer){
								std::string iid = propVal.as_string();
								shared_ptr<Instance> iinst = serializer->GetByID(iid);
							}
						}
					}
				}
			}
		}

		std::string Instance::serializedID(){
		    shared_ptr<OBSerializer> serializer = eng->getSerializer();
			return serializer->GetID(shared_from_this());
		}
		#endif

		std::map<std::string, _PropertyInfo> Instance::getProperties(){
			std::map<std::string, _PropertyInfo> propMap;
			propMap["Name"] = {"string", false, true, true};
			propMap["Archivable"] = {"bool", false, true, false};
			propMap["ClassName"] = {"string", true, true, false};

			return propMap;
		}

		void Instance::setProperty(std::string prop, shared_ptr<Type::VarWrapper> val){
		    if(prop == "Name"){
			    setName(val->asString());
				return;
			}
			if(prop == "Archivable"){
				setArchivable(val->asBool());
				return;
			}
		}

		shared_ptr<Type::VarWrapper> Instance::getProperty(std::string prop){
			if(prop == "Name"){
				return make_shared<Type::VarWrapper>(getName());
			}
			if(prop == "Archivable"){
				return make_shared<Type::VarWrapper>(getArchivable());
			}
			if(prop == "ClassName"){
				return make_shared<Type::VarWrapper>(getClassName());
			}
			
			return make_shared<Type::VarWrapper>();
		}
		
		void Instance::tick(){
			tickChildren();
		}
	
		void Instance::tickChildren(){
			std::vector<shared_ptr<Instance>> kids = GetChildren();
			
			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != kids.size(); i++){
				shared_ptr<Instance> kid = kids[i];
				if(kid){
					kid->tick();
				}
			}
		}

		void Instance::registerLuaClass(OBEngine* eng, std::string className, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters, luaRegisterFunc register_events){
			lua_State* L = eng->getGlobalLuaState();

			luaL_newmetatable(L, className.c_str());
			register_metamethods(L);

			lua_pushstring(L, "__metatable");
			lua_pushstring(L, "This metatable is locked");
			lua_rawset(L, -3);

			//Name
			lua_pushstring(L, "__name");
			/*
			For now, I think all Instance classes being called
			'Instance' is for the best, in this context. I say this
		    only because we typecheck against 'Instance' in Lua.
			*/
			lua_pushstring(L, "Instance");
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

		void Instance::propertyChanged(std::string property){
		    std::vector<shared_ptr<Type::VarWrapper>> args = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(property)});

			Changed->Fire(eng, args);
		}

		void Instance::propertyChanged(std::string property, shared_ptr<Instance> inst){
			inst->propertyChanged(property);
		}

		void Instance::setParent(shared_ptr<Instance> parent, bool useDMNotify){
			if(Parent == parent){
				//noop
				return;
			}
			if(ParentLocked){
				std::string errMsg = "The Parent property of " + Name + " is locked.";
				throw OBException(errMsg);
				return;
			}
			if(parent == shared_from_this()){
				std::string errMsg = "Attempt to set " + GetFullName() + " as its own parent";
				throw OBException(errMsg);
				return;
			}

			if(Parent){
				Parent->removeChild(shared_from_this());
			}
			Parent = parent;
			if(Parent){
				Parent->addChild(shared_from_this());

				#ifdef HAVE_ENET
				if(useDMNotify){
					shared_ptr<DataModel> dm = eng->getDataModel();
					if(dm){
						if(IsDescendantOf(dm)){
							if(netId == OB_NETID_UNASSIGNED){
								generateNetworkID();
							}
							if(netId >= OB_NETID_DATAMODEL){
								shared_ptr<Instance> nsInst = dm->FindService("NetworkServer");
								if(shared_ptr<NetworkServer> ns = dynamic_pointer_cast<NetworkServer>(nsInst)){
									BitStream bsOut;
									bsOut.writeSizeT(OB_NET_PKT_CREATE_INSTANCE);
									bsOut.writeUInt64(netId);
									bsOut.writeString(getClassName());

									ns->broadcast(OB_NET_CHAN_REPLICATION, bsOut);

									bsOut.reset();

									bsOut.writeSizeT(OB_NET_PKT_SET_PARENT);
									bsOut.writeUInt64(netId);
									if(Parent){
										bsOut.writeUInt64(Parent->GetNetworkID());
									}else{
										bsOut.writeUInt64(OB_NETID_NULL);
									}

									ns->broadcast(OB_NET_CHAN_REPLICATION, bsOut);
								}
							}
						}
					}
				}
				#else
				(void)useDMNotify;
				#endif
			}

			fireAncestryChanged(std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(std::enable_shared_from_this<Instance>::shared_from_this()), make_shared<Type::VarWrapper>(Parent)}));
			propertyChanged("Parent");
		}

		std::string Instance::toString(){
			return Name;
		}

		shared_ptr<Instance> Instance::cloneImpl(){
			return NULL;
		}

		shared_ptr<Instance> Instance::getParent(){
			return Parent;
		}

		void Instance::fireAncestryChanged(std::vector<shared_ptr<Type::VarWrapper>> args){
			AncestryChanged->Fire(eng, args);

			for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != children.size(); i++){
				shared_ptr<Instance> kid = children[i];
				if(kid){
					kid->fireAncestryChanged(args);
				}
			}
		}

		void Instance::fireDescendantAdded(std::vector<shared_ptr<Type::VarWrapper>> args){
			DescendantAdded->Fire(eng, args);

			if(Parent){
				Parent->fireDescendantAdded(args);
			}
		}

		void Instance::fireDescendantRemoving(std::vector<shared_ptr<Type::VarWrapper>> args){
			DescendantRemoving->Fire(eng, args);

			if(Parent){
				Parent->fireDescendantRemoving(args);
			}
		}

		void Instance::removeChild(shared_ptr<Instance> kid){
			if(kid){
				children.erase(std::remove(children.begin(), children.end(), kid));


			    std::vector<shared_ptr<Type::VarWrapper>> args = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(kid)});
				ChildRemoved->Fire(eng, args);
				fireDescendantRemoving(args);
			}
		}

		void Instance::addChild(shared_ptr<Instance> kid){
			if(kid){
				children.push_back(kid);

				std::vector<shared_ptr<Type::VarWrapper>> args = std::vector<shared_ptr<Type::VarWrapper>>({make_shared<Type::VarWrapper>(kid)});
				ChildAdded->Fire(eng, args);
				fireDescendantAdded(args);
			}
		}

		int Instance::wrap_lua(lua_State* L){
			shared_ptr<Instance> shared_this = std::enable_shared_from_this<Instance>::shared_from_this();
			
		    shared_ptr<Instance>* udata = static_cast<shared_ptr<Instance>*>(lua_newuserdata(L, sizeof(shared_ptr<Instance>)));
			new(udata) shared_ptr<Instance>(shared_this);
			
			luaL_getmetatable(L, getLuaClassName().c_str());
			lua_setmetatable(L, -2);
			return 1;
		}

		void Instance::register_lua_metamethods(lua_State* L){
			luaL_Reg metamethods[] = {
				{"__tostring", Instance::lua_toString},
				{"__eq", lua_eq},
				{"__gc", lua_gc},
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
				{"UseCount", lua_readOnlyProperty},
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
				{"UseCount", lua_getUseCount},
				{"Name", lua_getName},
				{"Parent", lua_getParent},
				{"Archivable", lua_getArchivable},
				{NULL, NULL}
			};
			luaL_setfuncs(L, properties, 0);
		}

		void Instance::register_lua_events(lua_State* L){
			luaL_Reg events[] = {
				{"Changed", WRAP_EVTI(Changed)},
				{"AncestryChanged", WRAP_EVTI(AncestryChanged)},
				{"ChildAdded", WRAP_EVTI(ChildAdded)},
				{"ChildRemoved", WRAP_EVTI(ChildRemoved)},
				{"DescendantAdded", WRAP_EVTI(DescendantAdded)},
				{"DescendantRemoving", WRAP_EVTI(DescendantRemoving)},
				{NULL, NULL}
			};
			luaL_setfuncs(L, events, 0);
		}

		shared_ptr<Instance> Instance::checkInstance(lua_State* L, int index, bool errIfNot, bool allowNil){
			if(allowNil){
				if(lua_isnoneornil(L, index)){
					return NULL;
				}
			}
			
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
						    return *static_cast<shared_ptr<Instance>*>(udata);
						}
						lua_pop(L, 1);
					}
				}
			}

			if(errIfNot){
				luaO_typeerror(L, index, "Instance");
			}
			return NULL;
		}

		int Instance::lua_newindex(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			if(!inst){
				return 0;
			}
			
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
			
			return 0;
		}

		int Instance::lua_index(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			if(!inst){
				return 0;
			}
			
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

						shared_ptr<Instance> kiddie = inst->FindFirstChild(name, false);
						if(kiddie){
							return kiddie->wrap_lua(L);
						}

						return luaL_error(L, "attempt to index '%s' (a nil value)", name);
					}
				}
			}
				
			return 0;
		}

		int Instance::lua_eq(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Instance> oinst = checkInstance(L, 2, false);
				if(oinst){
					lua_pushboolean(L, inst == oinst);
					return 1;
				}
			}
			
			lua_pushboolean(L, false);
			return 1;
		}
		
		int Instance::lua_gc(lua_State* L){
		    if(lua_isuserdata(L, 1)){
				std::vector<std::string> existing = OB::ClassFactory::getRegisteredClasses();
				unsigned size = existing.size();
				void* udata = lua_touserdata(L, 1);
				int meta = lua_getmetatable(L, 1);
				if(meta != 0){
					for(unsigned i = 0; i < size; i++){
						std::string name = "luaL_Instance_" + existing[i];
						luaL_getmetatable(L, name.c_str());
						if(lua_rawequal(L, -1, -2)){
							lua_pop(L, 2);
							
							(*static_cast<shared_ptr<Instance>*>(udata)).reset();
						}
						lua_pop(L, 1);
					}
				}
			}
			
			return 0;
		}

		int Instance::lua_toString(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				lua_pushstring(L, inst->toString().c_str());
			}
			
			return 1;
		}

		int Instance::lua_getClassName(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				std::string className = inst->getClassName();
				lua_pushstring(L, className.c_str());
				return 1;
			}
			
			return 0;
		}

		int Instance::lua_getUseCount(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				lua_pushinteger(L, inst.use_count() - 1);
				return 1;
			}
			
			return 0;
		}

		int Instance::lua_readOnlyProperty(lua_State* L){
			//Welp. This is how ROBLOX does it.
			//We do love compatibility.
			luaL_error(L, "can't set value");
			return 0;
		}

		int Instance::lua_getName(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				lua_pushstring(L, inst->getName().c_str());
				return 1;
			}
			
			return 0;
		}

		int Instance::lua_setName(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				std::string desired = std::string(luaL_checkstring(L, 2));
				inst->setName(desired);
			}
			
			return 0;
		}

		int Instance::lua_getParent(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				shared_ptr<Instance> otherInst = checkInstance(L, 2, false);
			
				try{
					inst->setParent(otherInst, true);
				}catch(OBException& ex){
					return luaL_error(L, ex.getMessage().c_str());
				}
			}
			
			return 0;
		}

		int Instance::lua_getArchivable(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				lua_pushboolean(L, inst->getArchivable());
				return 1;
			}
			
			return 0;
		}

		int Instance::lua_setArchivable(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);

			if(inst){
				//Again, following ROBLOX's ways....
				bool newVal = false;
				if(lua_isboolean(L, 2)){
					newVal = lua_toboolean(L, 2);
				}else if(!lua_isnoneornil(L, 2)){
					newVal = true;
				}
				inst->setArchivable(newVal);
			}
			
			return 0;
		}

		int Instance::lua_ClearAllChildren(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				inst->ClearAllChildren();
				return 0;
			}
			
			return luaL_error(L, COLONERR, "ClearAllChildren");
		}

		int Instance::lua_Clone(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<Instance> newGuy = inst->Clone();
				if(newGuy){
					return newGuy->wrap_lua(L);
				}

				lua_pushnil(L);
				return 1;
			}
			
			return luaL_error(L, COLONERR, "Clone");
		}

		int Instance::lua_Destroy(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				inst->Destroy();
				return 0;
			}
			
			return luaL_error(L, COLONERR, "Destroy");
		}

		int Instance::lua_Remove(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				inst->Remove();
				return 0;
			}
			
			return luaL_error(L, COLONERR, "Remove");
		}

		int Instance::lua_FindFirstChild(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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
				shared_ptr<Instance> foundStuff = inst->FindFirstChild(kidName, recursive);
				if(foundStuff){
					return foundStuff->wrap_lua(L);
				}
				lua_pushnil(L);
				return 1;
			}
			
			return luaL_error(L, COLONERR, "FindFirstChild");
		}

		int Instance::lua_GetChildren(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				lua_newtable(L);

				for(std::vector<shared_ptr<Instance>>::size_type i = 0; i != inst->children.size(); i++){
					shared_ptr<Instance> kid = inst->children[i];
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				std::string fullName = inst->GetFullName();
				lua_pushstring(L, fullName.c_str());
				return 1;
			}
			
			return luaL_error(L, COLONERR, "GetFullName");
		}

		int Instance::lua_IsA(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
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
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<Instance> otherInst = checkInstance(L, 2);

				lua_pushboolean(L, inst->IsAncestorOf(otherInst));
				return 1;
			}
			
			return luaL_error(L, COLONERR, "IsAncestorOf");
		}

		int Instance::lua_IsDescendantOf(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				shared_ptr<Instance> otherInst = checkInstance(L, 2);
				lua_pushboolean(L, inst->IsDescendantOf(otherInst));
				return 1;
			}
			
			return luaL_error(L, COLONERR, "IsDescendantOf");
		}

		int Instance::lua_GetNetworkID(lua_State* L){
			shared_ptr<Instance> inst = checkInstance(L, 1, false);
			
			if(inst){
				lua_pushnumber(L, inst->GetNetworkID());
				return 1;
			}
			
			return luaL_error(L, COLONERR, "GetNetworkID");
		}

	    bool Instance::assetLoaded(std::string res){
			return true;
		}
	}
}
