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

#include "instance/PVInstance.h"

#include "OBEngine.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS_ABS_WCLONE(PVInstance, Instance){
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

		PVInstance::PVInstance(){
			Name = ClassName;

			#if HAVE_IRRLICHT
			
			irrNode = NULL;
			
			#endif
		}

		PVInstance::~PVInstance(){
			#if HAVE_IRRLICHT

			if(irrNode){
				irr::scene::ISceneNode* irrPar = irrNode->getParent();
				if(irrPar){
					irrPar->removeChild(irrNode);
				}
				
				delete irrNode;
				irrNode = NULL;
			}
			
			#endif
		}

		#if HAVE_IRRLICHT

		irr::scene::ISceneNode* PVInstance::getIrrNode(){
			return irrNode;
		}

		void PVInstance::newIrrlichtNode(){
		    OBEngine* eng = OBEngine::getInstance();
			if(eng){
				irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
				if(irrDev){
					irr::scene::ISceneManager* sceneMgr = irrDev->getSceneManager();
					if(sceneMgr){
						irrNode = sceneMgr->addEmptySceneNode();
					}
				}
			}
		}

		void _ob_pvinstance_removeIrrlichtNode(shared_ptr<Instance> thisNode){
			std::vector<shared_ptr<Instance>> children = thisNode->GetChildren();
			for(int i = 0; i < children.size(); i++){
				shared_ptr<Instance> oInst = children.at(i);
				if(oInst){
					shared_ptr<PVInstance> poInst = dynamic_pointer_cast<PVInstance>(oInst);
					if(poInst){
						poInst->removeIrrlichtNode();
					}else{
						_ob_pvinstance_removeIrrlichtNode(oInst);
					}
				}
			}
		}

		void PVInstance::removeIrrlichtNode(){
			if(irrNode){
				irrNode->remove();
				irrNode = NULL;
			}

			for(int i = 0; i < children.size(); i++){
				shared_ptr<Instance> oInst = children.at(i);
				if(oInst){
					_ob_pvinstance_removeIrrlichtNode(oInst);
				}
			}
		}
		
		void PVInstance::removeChild(shared_ptr<Instance> kid){
			if(kid){
				if(shared_ptr<PVInstance> oInst = dynamic_pointer_cast<PVInstance>(kid)){
					if(irrNode && oInst->irrNode){
						oInst->removeIrrlichtNode();
					}
				}
				Instance::removeChild(kid);
			}
		}

		void PVInstance::addChild(shared_ptr<Instance> kid){
			if(kid){
				if(shared_ptr<PVInstance> oInst = dynamic_pointer_cast<PVInstance>(kid)){
					if(irrNode){
						oInst->newIrrlichtNode();
					    oInst->irrNode->setParent(irrNode);
					}
				}
				Instance::addChild(kid);
			}
		}

		#endif
	}
}
