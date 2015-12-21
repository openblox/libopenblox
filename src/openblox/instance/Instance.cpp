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

#include "Instance.h"

BEGIN_INSTANCE

struct InstanceClassMaker: public OpenBlox::ClassMaker{
	OpenBlox::Instance::Instance* getInstance() const{
		return NULL;
	}

	bool isA(const OpenBlox::Instance::Instance* obj){
		return (dynamic_cast<const Instance*>(obj)) != NULL;
	}

	bool isInstantiatable(){
		return false;
	}

	bool isService(bool isDataModel){
		OB_UNUSED(isDataModel);
		return false;
	}

	QString getParentClassName(){
		return QString::null;
	}
};

STATIC_GAME_INIT(Instance){
	OpenBlox::ClassFactory::getInstance()->addClass(ClassName, new InstanceClassMaker());

	registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
}

#ifndef OB_NO_NETWORKING
RakNet::NetworkIDManager* Instance::networkIDManager;
#endif

QString Instance::ClassName = "Instance";
QString Instance::LuaClassName = "luaL_Instance_Instance";

Instance::Instance(){
	Archivable = true;
	Name = getClassName();
	Parent = NULL;
	ParentLocked = false;

	children = std::vector<Instance*>();

	Changed = new OpenBlox::Type::Event("Changed");
	AncestryChanged = new OpenBlox::Type::Event("AncestryChanged");
	ChildAdded = new OpenBlox::Type::Event("ChildAdded");
	ChildRemoved = new OpenBlox::Type::Event("ChildRemoved");
	DescendantAdded = new OpenBlox::Type::Event("DescendantAdded");
	DescendantRemoving = new OpenBlox::Type::Event("DescendantRemoving");

	ogreNode = NULL;

	#ifndef OB_NO_NETWORKING
	if(!networkIDManager){
		networkIDManager = RakNet::NetworkIDManager::GetInstance();
	}
	SetNetworkIDManager(networkIDManager);
	#endif
}

Instance::~Instance(){
	delete Changed;
	delete AncestryChanged;
	delete ChildAdded;
	delete ChildRemoved;
	delete DescendantAdded;
	delete DescendantRemoving;
}

/**
 * Returns the OGRE3D Node associated with this Instance. This will be NULL, where we don't use OGRE.
 * @returns Ogre::Node*
 * @author John M. Harris, Jr.
 */
Ogre::Node* Instance::getOgreNode(){
	return ogreNode;
}

/**
 * Sets the OGRE3D Node associated with this Instance, returns the old one or NULL.
 * @param Ogre::Node* newNode
 * @returns Ogre::Node* oldNode
 * @author John M. Harris, Jr.
 */
Ogre::Node* Instance::setOgreNode(Ogre::Node* newNode){
	Ogre::Node* oNode = ogreNode;
	ogreNode = newNode;
	return oNode;
}

#ifndef OB_NO_NETWORKING

/**
 * Returns the RakNet::NetworkIDManager for this instance of OpenBlox.
 * @returns RakNet::NetworkIDManager*
 * @author John M. Harris, Jr.
 */
RakNet::NetworkIDManager* Instance::getNetworkIDManager(){
	if(!networkIDManager){
		networkIDManager = RakNet::NetworkIDManager::GetInstance();
	}
	return networkIDManager;
}

#endif

/**
 * Convienence method to recursively fire the AncestryChanged OpenBlox::Type::Event. Used internally.
 * @param std::vector<OpenBlox::Type::VarWrapper>* args
 * @author John M. Harris, Jr.
 */
void Instance::fireAncestryChanged(std::vector<OpenBlox::Type::VarWrapper>* args){
	AncestryChanged->Fire(args);

	for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
		Instance* kid = children[i];
		if(kid){
			kid->fireAncestryChanged(args);
		}
	}
}

/**
 * Convienence method to recursively fire the DescendantAdded OpenBlox::Type::Event. Used internally.
 * @param std::vector<OpenBlox::Type::VarWrapper>* args
 * @author John M. Harris, Jr.
 */
void Instance::fireDescendantAdded(std::vector<OpenBlox::Type::VarWrapper>* args){
	DescendantAdded->Fire(args);

	if(Parent){
		Parent->fireDescendantAdded(args);
	}
}

/**
 * Convienence method to recursively fire the DescendantRemoving OpenBlox::Type::Event. Used internally.
 * @param std::vector<OpenBlox::Type::VarWrapper>* args
 * @author John M. Harris, Jr.
 */
void Instance::fireDescendantRemoving(std::vector<OpenBlox::Type::VarWrapper>* args){
	DescendantRemoving->Fire(args);

	if(Parent){
		Parent->fireDescendantRemoving(args);
	}
}

/**
 * Convienence method to recursively fire the Changed OpenBlox::Type::Event. Used internally.
 * @param QString property
 * @author John M. Harris, Jr.
 */
void Instance::propertyChanged(QString property){
	std::vector<OpenBlox::Type::VarWrapper>* args = new std::vector<OpenBlox::Type::VarWrapper>({OpenBlox::Type::VarWrapper(property)});

	Changed->Fire(args);
}

/**
 * Convienence method to recursively fire the Changed OpenBlox::Type::Event from static methods. Used internally.
 * @param QString property
 * @param Instance* inst
 * @author John M. Harris, Jr.
 */
void Instance::propertyChanged(QString property, Instance* inst){
	std::vector<OpenBlox::Type::VarWrapper>* args = new std::vector<OpenBlox::Type::VarWrapper>({OpenBlox::Type::VarWrapper(property)});

	inst->Changed->Fire(args);
}

/**
 * Removes all children.
 * @author John M. Harris, Jr.
 */
void Instance::ClearAllChildren(){
	for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
		Instance* kid = children[i];
		if(kid){
			kid->Remove();
		}
	}
}

Instance* Instance::cloneImpl(Instance* newOne){
	if(newOne == NULL){
		return NULL;
	}
	newOne->Name = Name;
}

/**
 * Clones an Instance, if possible. This also clones children of the Instance.
 * @returns Instance*, NULL if not able to clone.
 * @author John M. Harris, Jr.
 */
Instance* Instance::Clone(){
	if(Archivable){
		Instance* cloneTo = cloneImpl(NULL);
		if(!cloneTo){
			return NULL;//This would, in any instantiatable, clonable, class create a new Instance
		}
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid){
				Instance* kidClone = kid->Clone();
				if(kidClone){
					kidClone->setParent(cloneTo, true);
				}
			}
		}
		return cloneTo;
	}
	return NULL;
}

/**
 * Locks the parent of this Instance. Used internally. This should NOT be exposed to Lua if possible. If there are any reasons to do so I don't know of them.
 * @author John M. Harris, Jr.
 */
void Instance::parentLock(){
	ParentLocked = true;
}

/**
 * Used to parent the object to NULL (nil in Lua) and remove all references. (This also parent-locks it)
 * @author John M. Harris, Jr.
 */
void Instance::Destroy(){
	if(ParentLocked){
		ParentLocked = false;
	}
	setParent(NULL, true);
	ParentLocked = true;
	Changed->disconnectAll();
	for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
		Instance* kid = children[i];
		if(kid){
			kid->Destroy();
		}
	}
}

/**
 * Used to set the parent of this object to NULL (nil in Lua). Calls Remove recursively on child objects.
 * @author John M. Harris, Jr.
 */
void Instance::Remove(){
	setParent(NULL, true);
	for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
		Instance* kid = children[i];
		if(kid){
			kid->Remove();
		}
	}
}

/**
 * Used to find the first child by name. This has an optional argument for searching recursively.
 * @param QString name
 * @param bool recursive
 * @returns Instance* Found child or NULL. (nil in Lua)
 * @author John M. Harris, Jr.
 */
Instance* Instance::FindFirstChild(QString name, bool recursive){
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

/**
 * Returns the children of an object.
 * @returns std::vector<Instance*> Vector containing the children of this object.
 * @author John M. Harris, Jr.
 */
std::vector<Instance*> Instance::GetChildren(){
	return children;
}

/**
 * Returns the full name of this object. Usually in the form of "game.Service.Thing", can return weird things when one of the parents of this Instance is parented to NULL.
 * @returns QString full name
 * @author John M. Harris, Jr.
 */
QString Instance::GetFullName(){
	QString fullName = Name;
	if(Parent){
		fullName = Parent->GetFullName() + "." + fullName;
	}
	return fullName;
}

/**
 * Used for class inheritance checking.
 * @param QString ClassName
 * @returns bool true if this class inherits from the class of the name given.
 * @author John M. Harris, Jr.
 */
bool Instance::IsA(QString name){
	return OpenBlox::ClassFactory::getInstance()->isA(this, name);
}

/**
 * Returns whether or not this Instance is the ancestor of another Instance.
 * @param Instance* Possible Descendant
 * @returns bool true if the possible descendant is not NULL and this Instance is an ancestor of the possible descendant.
 * @author John M. Harris, Jr.
 */
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

/**
 * Returns whether or not this Instance is a descendant of another Instance.
 * @param Instance* Possible Ancestor
 * @returns bool true if the possible ancestor is NULL or this Instance is a descendant of the possible ancestor.
 * @author John M. Harris, Jr.
 */
bool Instance::IsDescendantOf(Instance* ancestor){
	if(ancestor == NULL){
		return true;
	}
	return ancestor->IsAncestorOf(this);
}

/**
 * Returns the class name of this Instance.
 * @returns QString Class Name
 * @author John M. Harris, Jr.
 */
QString Instance::getClassName(){
	return ClassName;
}

/**
 * Sets the Name of this Instance.
 * @param QString Name
 * @author John M. Harris, Jr.
 */
void Instance::setName(QString name){
	Name = name;
}

/**
 * Gets the Name of this Instance.
 * @returns QString Name
 * @author John M. Harris, Jr.
 */
QString Instance::getName(){
	return Name;
}

/**
 * Called internally every tick.
 * @author John M. Harris, Jr.
 */
void Instance::tick(){
	tickChildren();
}

/**
 * Used by NetworkPeer implementations to set properties of instances.
 * @author John M. Harris, Jr.
 */
void Instance::setProperty(QString propName, OpenBlox::Type::VarWrapper* wrapper){
	if(propName == "Archivable"){
		if(wrapper){
			if(wrapper->type == Type::TYPE_STRING){
				bool newArchivable = reinterpret_cast<OpenBlox::Type::BoolWrapper*>(wrapper->wrapped)->val;
				if(newArchivable != Archivable){
					Archivable = newArchivable;

					propertyChanged("Archivable");
				}
			}
		}
		return;
	}
	if(propName == "Name"){
		if(wrapper){
			if(wrapper->type == Type::TYPE_STRING){
				QString newStr = reinterpret_cast<OpenBlox::Type::StringWrapper*>(wrapper->wrapped)->val;
				if(newStr != Name){
					Name = newStr;

					propertyChanged("Name");
				}
			}
		}
		return;
	}
}

/**
 * Calls tick on all children of this Instance.
 * @author John M. Harris, Jr.
 */
void Instance::tickChildren(){
	for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
		Instance* kid = children[i];
		if(kid){
			kid->tick();
		}
	}
}

/**
 * Replicates this Instance.
 * @param RakNet::RakPeerInterface* peer
 * @param RakNet::SystemAddress addr
 * @author John M. Harris, Jr.
 */
void Instance::replicate(RakNet::RakPeerInterface* peer, RakNet::SystemAddress addr){
	if(!peer){
		return;
	}
	//TODO
	/*

	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_OB_PACKET);
		bsOut.Write((RakNet::MessageID)OB_CREATE_INSTANCE);
		bsOut.Write(GetNetworkID());
		bsOut.Write(RakNet::RakString(getClassName().toStdString().c_str()));

		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, false);
	}

	{
		OB_SetParentPacket spp;
		spp.net_id = GetNetworkID();
		if(Parent){
			spp.parent_net_id = Parent->GetNetworkID();
		}else{
			spp.parent_net_id = ID_NULL;
		}

		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_OB_PACKET);
		bsOut.Write((RakNet::MessageID)OB_SET_PARENT);
		bsOut.Write(spp);

		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, false);
	}

	replicateProperties(peer, addr);
	replicateChildren(peer, addr);
	*/
}

/**
 * Used to replicate properties. This makes it simple for classes to implement property replication.
 * @param RakNet::RakPeerInterface* peer
 * @param RakNet::SystemAddress addr
 * @author John M. harris, Jr.
 */
void Instance::replicateProperties(RakNet::RakPeerInterface* peer, RakNet::SystemAddress addr){
	OB_UNUSED(peer);

	//TODO
	//NetworkPeer::sendSetPropertyPacketTo(GetNetworkID(), "Archivable", OpenBlox::Type::VarWrapper(Archivable), peer, addr);
	//NetworkPeer::sendSetPropertyPacketTo(GetNetworkID(), "Name", OpenBlox::Type::VarWrapper(Name), peer, addr);
}

/**
 * Calls replicate on all children of this Instance.
 * @param RakNet::RakPeerInterface* peer
 * @param RakNet::SystemAddress addr
 * @author John M. Harris, Jr.
 */
void Instance::replicateChildren(RakNet::RakPeerInterface* peer, RakNet::SystemAddress addr){
	for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
		Instance* kid = children[i];
		if(kid){
			kid->replicate(peer, addr);
		}
	}
}

/**
 * Convenience method to register this class as an Instance class.
 * @author John M. Harris, Jr.
 */
void Instance::registerLuaClass(QString className, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters, luaRegisterFunc register_events){
	lua_State* L = OpenBlox::OBEngine::getInstance()->getLuaState();

	luaL_newmetatable(L, className.toStdString().c_str());
	register_metamethods(L);

	lua_pushstring(L, "__metatable");
	lua_pushstring(L, OB_TRANSLATE("LuaType", "This metatable is locked"));
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

/**
 * Returns the stringified version of this Instance, for C++ and Lua this will return the Name.
 * @returns QString Name
 * @author John M. Harris, Jr.
 */
QString Instance::toString(){
	return Name;
}

/**
 * Used to set the Parent property.
 * @param Instance* New Parent
 * @author John M. Harris, Jr.
 */
void Instance::setParent(Instance* parent, bool useDMNotify){
	if(ParentLocked){
		QString errMsg = "The Parent property of ";
		errMsg = errMsg + Name + " is locked.";
		throw std::runtime_error(errMsg.toStdString().c_str());
		return;
	}
	if(parent == this){
		QString errMsg = "Attempt to set ";
		errMsg = errMsg + GetFullName() + " as its own parent";
		throw std::runtime_error(errMsg.toStdString().c_str());
		return;
	}

	if(Parent){
		Parent->removeChild(this);
	}
	Parent = parent;
	if(Parent){
		Parent->addChild(this);

		OpenBlox::OBEngine* engine = OpenBlox::OBEngine::getInstance();
		if(engine && engine->isServer()){//This isn't the same as DataModel's isServer, which only returns true when there's a NetworkServer.
			if(useDMNotify){
				//TODO
				/*
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
				*/
			}
		}
	}

	fireAncestryChanged(new std::vector<OpenBlox::Type::VarWrapper>({OpenBlox::Type::VarWrapper(this), OpenBlox::Type::VarWrapper(Parent)}));
	propertyChanged("Parent");
}

/**
 * Returns the current parent of this Instance.
 * @returns Instance* Parent
 * @author John M. Harris, Jr.
 */
Instance* Instance::getParent(){
	return Parent;
}

/**
 * Called internally during reparenting of children.
 * @param Instance* kid
 * @author John M. Harris, Jr.
 */
void Instance::removeChild(Instance* kid){
	if(kid){
		children.erase(std::remove(children.begin(), children.end(), kid));

		if(ogreNode && kid->getOgreNode()){
			ogreNode->removeChild(kid->getOgreNode());
		}

		std::vector<OpenBlox::Type::VarWrapper>* args = new std::vector<OpenBlox::Type::VarWrapper>({OpenBlox::Type::VarWrapper(kid)});
		ChildRemoved->Fire(args);
		fireDescendantRemoving(args);
	}
}

/**
 * Called internally during reparenting of children.
 * @param Instance* New kid
 * @author John M. Harris, Jr.
 */
void Instance::addChild(Instance* kid){
	if(kid){
		children.push_back(kid);

		if(ogreNode && kid->getOgreNode()){
			ogreNode->addChild(kid->getOgreNode());
		}

		std::vector<OpenBlox::Type::VarWrapper>* args = new std::vector<OpenBlox::Type::VarWrapper>({OpenBlox::Type::VarWrapper(kid)});
		ChildAdded->Fire(args);
		fireDescendantAdded(args);
	}
}

/**
 * Lua Metamethods for the Instance class.
 * @param lua_State* Lua State
 * @author John M. Harris, Jr.
 * @author DigiTechs
 */
void Instance::register_lua_metamethods(lua_State* L){
	luaL_Reg metamethods[] = {
		{"__tostring", Instance::lua_toString},
		{"__eq", lua_eq},
		{NULL, NULL}
	};
	luaL_setfuncs(L, metamethods, 0);
}

/**
 * Lua property setters for the Instance class.
 * @param lua_State* Lua State
 * @author John M. Harris, Jr.
 * @author DigiTechs
 */
void Instance::register_lua_property_setters(lua_State* L){
	luaL_Reg properties[]{
		{"ClassName", lua_readOnlyProperty},
		{"Name", lua_setName},
		{"Parent", lua_setParent},
		{"Archivable", lua_setArchivable},
		{NULL, NULL}
	};
	luaL_setfuncs(L, properties, 0);
}

/**
 * Lua property getters for the Instance class.
 * @param lua_State* Lua State
 * @author John M. Harris, Jr.
 * @author DigiTechs
 */
void Instance::register_lua_property_getters(lua_State* L){
	luaL_Reg properties[]{
		{"ClassName", lua_getClassName},
		{"Name", lua_getName},
		{"Parent", lua_getParent},
		{"Archivable", lua_getArchivable},
		{NULL, NULL}
	};
	luaL_setfuncs(L, properties, 0);
}

/**
 * Lua methods for the Instance class.
 * @param lua_State* Lua State
 * @author John M. Harris, Jr.
 * @author DigiTechs
 */
void Instance::register_lua_methods(lua_State* L){
	luaL_Reg methods[]{
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

/**
 * Lua events for the Instance class.
 * @param lua_State* Lua State
 * @author John M. Harris, Jr.
 * @author DigiTechs
 */
void Instance::register_lua_events(lua_State* L){
	luaL_Reg events[]{
		{"Changed", WRAP_EVT(Changed)},
		{"AncestryChanged", WRAP_EVT(AncestryChanged)},
		{"ChildAdded", WRAP_EVT(ChildAdded)},
		{"ChildRemoved", WRAP_EVT(ChildRemoved)},
		{"DescendantAdded", WRAP_EVT(DescendantAdded)},
		{"DescendantRemoving", WRAP_EVT(DescendantRemoving)},
		{NULL, NULL}
	};
	luaL_setfuncs(L, events, 0);
}

//Lua Wrappers
//Metamethods

/**
 * Checks that the value at the specified index on the Lua state is an Instance, and if so returns it.
 * @param lua_State* Lua State
 * @param int Index
 * @author John M. Harris, Jr.
 * @author DigiTechs
 */
Instance* Instance::checkInstance(lua_State* L, int index){
	if(lua_isuserdata(L, index)){
		std::vector<QString> existing = OpenBlox::ClassFactory::getInstance()->getRegisteredMetatables();
		unsigned size = existing.size();
		void* udata = lua_touserdata(L, index);
		int meta = lua_getmetatable(L, index);
		if(meta != 0){
			for(unsigned i = 0; i<size; i++){
				QString name = "luaL_Instance_" + existing[i];
				luaL_getmetatable(L, name.toStdString().c_str());
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

/**
 * Handles attempts to set properties of this Instance.
 * @param lua_State* Lua State
 * @returns int 0 or error (Does not return if an error occurs)
 * @author DigiTechs
 * @author John M. Harris, Jr.
 */
int Instance::lua_newindex(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		const char* name = luaL_checkstring(L, 2);
		lua_getmetatable(L, 1);//-3
		lua_getfield(L, -1, "__propertysetters");//-2
		lua_getfield(L, -1, name);//-1
		if(lua_iscfunction(L, -1)){
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

/**
 * Handles attempts to get properties, methods or events of this Instance.
 * @param lua_State* Lua State
 * @returns int 1 if there is a corresponding value, otherwise errors. (No return)
 * @author DigiTechs
 * @author John M. Harris, Jr.
 */
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

/**
 * Handles equality tests from Lua.
 * @param lua_State* Lua State
 * @returns int 1, a bool value on the lua stack. (This value is true if the two Instances are the same)
 * @author John M. Harris, Jr.
 */
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

/**
 * Handles tostring calls on this Instance from Lua.
 * @param lua_State* Lua State
 * @returns int 1, a string value on the lua stack. (The result of Instance::toString)
 * @author John M. Harris, Jr.
 */
int Instance::lua_toString(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		lua_pushstring(L, inst->toString().toStdString().c_str());
		return 1;
	}
	return 0;
}

//Properties

/**
 * Handles the ClassName property for Lua.
 * @param lua_State* Lua State
 * @returns int 1, a string value on the lua stack. (The result of Instance::getClassName)
 * @author John M. Harris, Jr.
 */
int Instance::lua_getClassName(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		QString className = inst->getClassName();
		lua_pushstring(L, className.toStdString().c_str());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

/**
 * Handles read-only properties for Lua.
 * @param lua_State* Lua State
 * @returns Errors (Does not return)
 * @author John M. Harris, Jr.
 */
int Instance::lua_readOnlyProperty(lua_State* L){
	//Welp. This is how ROBLOX does it.
	luaL_error(L, "can't set value");
	return 0;
}

/**
 * Handles getting the Name property for Lua.
 * @param lua_State* Lua State
 * @returns int 1, a string value on the lua stack. (The value of the Name property)
 * @author John M. Harris, Jr.
 */
int Instance::lua_getName(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		lua_pushstring(L, inst->Name.toStdString().c_str());
		return 1;
	}
	return 0;
}

/**
 * Handles setting the Name property for Lua.
 * @param lua_State* Lua State
 * @returns int 0
 * @author John M. Harris, Jr.
 */
int Instance::lua_setName(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		QString desired = QString(luaL_checkstring(L, 2));
		if(inst->Name != desired){
			inst->Name = desired;

			#ifndef OB_NO_NETWORKING
			REPLICATE_PROPERTY_CHANGE(inst, "Name", inst->Name);
			#endif

			propertyChanged("Name", inst);
		}
		return 0;
	}
	return 0;
}

/**
 * Handles getting the Parent property for Lua.
 * @param lua_State* Lua State
 * @returns int 1, a userdata (or nil) value on the lua stack. (The value of the Parent property)
 * @author John M. Harris, Jr.
 */
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

/**
 * Handles setting the Parent property for Lua.
 * @param lua_State* Lua State
 * @returns int 0
 * @author John M. Harris, Jr.
 */
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
			}catch(std::runtime_error& ex){
				return luaL_error(L, ex.what());
			}
			return 0;
		}else{
			return luaL_argerror(L, 2, OB_TRANSLATE("Instance", "Instance expected"));
		}
		return 0;
	}
	return 0;
}

/**
 * Handles getting the Parent property for Lua.
 * @param lua_State* Lua State
 * @returns int 1, a bool value on the lua stack. (The value of the Archivable property)
 * @author John M. Harris, Jr.
 */
int Instance::lua_getArchivable(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		lua_pushboolean(L, inst->Archivable);
		return 1;
	}
	return 0;
}

/**
 * Handles setting the Archivable property for Lua.
 * @param lua_State* Lua State
 * @returns int 0
 * @author John M. Harris, Jr.
 */
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

			#ifndef OB_NO_NETWORKING
			REPLICATE_PROPERTY_CHANGE(inst, "Archivable", inst->Archivable);
			#endif

			propertyChanged("Archivable", inst);
		}
	}
	return 0;
}

//Methods

/**
 * Handles the Instance::ClearAllChildren method for Lua.
 * @param lua_State* Lua State
 * @returns int 0
 * @author John M. Harris, Jr.
 */
int Instance::lua_ClearAllChildren(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		inst->ClearAllChildren();
		return 0;
	}
	return luaL_error(L, COLONERR, "ClearAllChildren");
}

/**
 * Handles the Instance::Clone method for Lua.
 * @param lua_State* Lua State
 * @returns int 1 (A userdata value on the Lua stack representing the cloned Instance) or 0 if we could not clone.
 * @author John M. Harris, Jr.
 */
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

/**
 * Handles the Instance::Destroy method for Lua.
 * @param lua_State* Lua State
 * @returns int 0
 * @author John M. Harris, Jr.
 */
int Instance::lua_Destroy(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		inst->Destroy();
		return 0;
	}
	return luaL_error(L, COLONERR, "Destroy");
}

/**
 * Handles the Instance::Remove method for Lua.
 * @param lua_State* Lua State
 * @returns int 0
 * @author John M. Harris, Jr.
 */
int Instance::lua_Remove(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		inst->Remove();
		return 0;
	}
	return luaL_error(L, COLONERR, "Remove");
}

/**
 * Handles the Instance::FindFirstChild method for Lua.
 * @param lua_State* Lua State
 * @returns int 1 (A userdata value on the Lua stack representing the found Instance) or 0 if we could not find one.
 * @author John M. Harris, Jr.
 */
int Instance::lua_FindFirstChild(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		const char* kidName = luaL_checkstring(L, 2);
		bool recursive = false;
		if(!lua_isnoneornil(L, 3)){
			if(lua_isboolean(L, 3)){
				recursive = lua_toboolean(L, 3);
			}else{
				luaL_argerror(L, 3, OB_TRANSLATE("Instance", "Instance expected"));
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

/**
 * Works similarly to the Instance::GetChildren method, modified for Lua.
 * @param lua_State* Lua State
 * @returns int 1, A table value on the Lua stack containing all children of this Instance.
 * @author John M. Harris, Jr.
 */
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

/**
 * Handles the Instance::GetFullName method for Lua.
 * @param lua_State* Lua State
 * @returns int 1, A string value on the Lua stack containing the result of Instance::GetFullName
 * @author John M. Harris, Jr.
 */
int Instance::lua_GetFullName(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		QString fullName = inst->GetFullName();
		lua_pushstring(L, fullName.toStdString().c_str());
		return 1;
	}
	return luaL_error(L, COLONERR, "GetFullName");
}

/**
 * Handles the Instance::IsA method for Lua.
 * @param lua_State* Lua State
 * @returns int 1, A bool value on the Lua stack with the result of Instance::IsA
 * @author John M. Harris, Jr.
 */
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

/**
 * Handles the Instance::IsAncestorOf method for Lua.
 * @param lua_State* Lua State
 * @returns int 1, A bool value on the Lua stack with the result of Instance::IsAncestorOf
 * @author John M. Harris, Jr.
 */
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
			return luaL_argerror(L, 2, OB_TRANSLATE("Instance", "Instance expected"));
		}
		return 0;
	}
	return luaL_error(L, COLONERR, "IsAncestorOf");
}

/**
 * Handles the Instance::IsDescendantOf method for Lua.
 * @param lua_State* Lua State
 * @returns int 1, A bool value on the Lua stack with the result of Instance::IsDescendantOf
 * @author John M. Harris, Jr.
 */
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
			return luaL_argerror(L, 2, OB_TRANSLATE("Instance", "Instance expected"));
		}
		return 0;
	}
	return luaL_error(L, COLONERR, "IsDescendantOf");
}

int Instance::lua_GetNetworkID(lua_State* L){
	Instance* inst = checkInstance(L, 1);
	if(inst){
		#ifndef OB_NO_NETWORKING
		lua_pushnumber(L, inst->GetNetworkID());
		#elif
		lua_pushnumber(L, 0);
		#endif
		return 1;
	}
	return luaL_error(L, COLONERR, "GetNetworkID");
}

END_INSTANCE
