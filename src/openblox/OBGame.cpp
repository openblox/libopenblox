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

/**
 * @file OBGame.cpp
 * @author John M. Harris, Jr.
 * @date July 2015
 *
 * This file is implements the OBGame singleton.
 */

#include "OBGame.h"

namespace OpenBlox{
	OBGame* OBGame::inst = NULL;

	OBGame::OBGame(){
		if(inst != NULL){
			throw new OBException("Only one instance of OBGame can be created.");
		}
		inst = this;

		//Init Lua
		mainLuaState = ob_lua::init();
	}

	OBGame::~OBGame(){}

	/**
	 * Returns the OBGame instance.
	 * @return OBGame*, NULL if there is no instance yet.
	 * @author John M. Harris, Jr.
	 */
	OBGame* OBGame::getInstance(){
		return inst;
	}

	/**
	 * Called from the task thread, handles game logic.
	 * @return true on success, otherwise false
	 * @author John M. Harris, Jr.
	 */
	bool OBGame::tick(){
		return true;
	}

	/**
	 * Called when the window is resized.
	 * @author John M. Harris, Jr.
	 */
	void OBGame::resized(){
	}

	/**
	 * Injects a mouse press event at a specific position.
	 * @param Uint8 button
	 * @param QPoint position
	 * @author John M. Harris, Jr.
	 */
	void OBGame::mousePress(Uint8 btn, QPoint pos){
		/*
		ob_instance::GuiBase2d* gb2d = dm->pickGUI(pos);
		if(ob_instance::GuiObject* go = dynamic_cast<ob_instance::GuiObject*>(gb2d)){
			if(ob_instance::GuiButton* gb = dynamic_cast<ob_instance::GuiButton*>(go)){
				gb->mouseDown(btn, pos);
			}
		}
		*/
	}

	/**
	 * Injects a mouse release event at a specific position.
	 * @param Uint8 button
	 * @param QPoint position
	 * @author John M. Harris, Jr.
	 */
	void OBGame::mouseRelease(Uint8 btn, QPoint pos){
		/*
		ob_instance::GuiBase2d* gb2d = dm->pickGUI(pos);
		if(ob_instance::GuiObject* go = dynamic_cast<ob_instance::GuiObject*>(gb2d)){
			if(ob_instance::GuiButton* gb = dynamic_cast<ob_instance::GuiButton*>(go)){
				gb->mouseUp(btn, pos);
			}
		}
		*/
	}

	/**
	 * Injects a mouse movement event at a specific position.
	 * @param QPoint position
	 * @author John M. Harris, Jr.
	 */
	void OBGame::mouseMove(QPoint pos){
		//dm->mouseMoved(pos);
	}

	/**
	 * Injects a mouse wheel event at a specific position.
	 * @param int delta
	 * @param QPoint position
	 * @author John M. Harris, Jr.
	 */
	void OBGame::mouseWheel(int delta, QPoint pos){
		OB_UNUSED(delta);
		OB_UNUSED(pos);
	}

	void OBGame::processEvent(SDL_Event event){
		switch(event.type){
			#ifndef OB_NO_GRAPHICS
			case SDL_MOUSEMOTION: {
				SDL_MouseMotionEvent motion = event.motion;

				mouseMove(QPoint(motion.x, motion.y));
				break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				SDL_MouseButtonEvent btn = event.button;

				mousePress(btn.button, QPoint(btn.x, btn.y));
				break;
			}
			case SDL_KEYDOWN: {
				//SDL_KeyboardEvent keyEvt = event.key;
				//SDL_Keysym key = keyEvt.keysym;

				/*
				#ifndef OB_STUDIO
				if(key.sym == SDLK_F11){
					if((SDL_GetWindowFlags(mw) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN){
						SDL_SetWindowFullscreen(mw, 0);
					}else{
						SDL_DisplayMode dm;
						if(SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(mw), &dm) == 0){
							SDL_SetWindowDisplayMode(mw, &dm);
						}
						SDL_SetWindowFullscreen(mw, SDL_WINDOW_FULLSCREEN);
					}
				}
				#endif
				*/
				break;
			}
			case SDL_KEYUP: {
				SDL_KeyboardEvent keyEvt = event.key;
				SDL_Keysym key = keyEvt.keysym;
				Q_UNUSED(key)
				break;
			}
			case SDL_WINDOWEVENT: {
				SDL_WindowEvent wevt = event.window;
				switch(wevt.event){
					case SDL_WINDOWEVENT_RESIZED: {
						resized();
					}
				}
				break;
			}
			#endif
			case SDL_QUIT: {
				//shouldQuit = true;
				break;
			}
		}
	}
}
