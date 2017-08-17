/*
 * Copyright (C) 2017 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "instance/Script.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(Script, true, false, BaseScript){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    Script::Script(OBEngine* eng) : BaseScript(eng){
			Name = ClassName;

			Source = "";
		}

	    Script::~Script(){}

		shared_ptr<Instance> Script::cloneImpl(){
		    shared_ptr<Script> nScript = make_shared<Script>(getEngine());
			nScript->Archivable = Archivable;
			nScript->Name = Name;
			nScript->ParentLocked = ParentLocked;

			nScript->Disabled = Disabled;
			nScript->LinkedSource = LinkedSource;
			
			nScript->Source = Source;
			return nScript;
		}

		std::string Script::getSource(){
			std::string linkedSource = getLinkedSource();
			if(linkedSource.size() > 0){
				
			}else{
				return Source;
			}
		}

		void Script::setSource(std::string source){
			Source = source;
		}

		void Script::serialize(pugi::xml_node parentNode, shared_ptr<Instance> model){
			if(Archivable){
				pugi::xml_node thisNode = parentNode.append_child(pugi::node_element);
				thisNode.set_name("instance");

				pugi::xml_node srcNode = thisNode.append_child(pugi::node_element);
				srcNode.set_name("source");
				srcNode.append_child(pugi::node_cdata).set_value(Source.c_str());
				
				serializeThis(thisNode, model);
			}
		}

		void Script::deserializeProperties(pugi::xml_node thisNode){
			//This isn't a property, but it was either in deserializeCreate or deserializeProperties
		    pugi::xml_node csrc =thisNode.child("source");
			if(csrc.type() == pugi::node_element && !csrc.empty()){
				pugi::xml_node srcCode = csrc.first_child();
				if(srcCode.type() == pugi::node_cdata){
					Source = std::string(srcCode.value());
				}
			}
			
			Instance::deserializeProperties(thisNode);
		}
	}
}
