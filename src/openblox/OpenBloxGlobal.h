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
 * @file OpenBloxGlobal.h
 * @author John M. Harris, Jr.
 * @date July 2015
 *
 * This file is included into OpenBlox.h, allowing anything here to be globally included.
 */

#ifndef OPENBLOX_OPENBLOXGLOBAL_H_
#define OPENBLOX_OPENBLOXGLOBAL_H_

//C++ stdlib
#include <string>
#include <vector>

//Qt
#include <QtCore>
#include <QtGlobal>

//SDL
#ifndef __ANDROID__
	#define SDL_MAIN_HANDLED
#endif
#include <SDL2/SDL.h>

#ifndef OB_NO_GRAPHICS
	//OGRE
	#include <OGRE/Ogre.h>
#endif

namespace OpenBlox{
	extern bool shouldQuit;
	extern int taskThreadFunc(void* ptr);
}

#define OB_UNUSED(any_thing) (void)any_thing
//TODO: Have OB_TRANSLATE call QCoreApplication::translate with the given context
#define OB_TRANSLATE(context, src) src

#ifdef __ANDROID__
	#include <android/log.h>

	#define ANDROID_LOG_TAG "OB"
	#define LOGI(...) __android_log_write(ANDROID_LOG_INFO, ANDROID_LOG_TAG, QString().sprintf(__VA_ARGS__).toStdString().c_str())
	#define LOGW(...) __android_log_write(ANDROID_LOG_WARN, ANDROID_LOG_TAG, QString().sprintf(__VA_ARGS__).toStdString().c_str())
	#define LOGE(...) __android_log_write(ANDROID_LOG_ERROR, ANDROID_LOG_TAG, QString().sprintf(__VA_ARGS__).toStdString().c_str())
	#define ILOG(out) __android_log_write(ANDROID_LOG_ERROR, ANDROID_LOG_TAG, out)
	#define WLOG(out) __android_log_write(ANDROID_LOG_ERROR, ANDROID_LOG_TAG, out)
	#define ELOG(out) __android_log_write(ANDROID_LOG_ERROR, ANDROID_LOG_TAG, out)
#else
	#define LOGI(...) OpenBlox::OBEngine::getInstance()->info(QString().sprintf(__VA_ARGS__))
	#define LOGW(...) OpenBlox::OBEngine::getInstance()->warn(QString().sprintf(__VA_ARGS__))
	#define LOGE(...) OpenBlox::OBEngine::getInstance()->print_error(QString().sprintf(__VA_ARGS__))
	#define ILOG(out) OpenBlox::OBEngine::getInstance()->info(out)
	#define WLOG(out) OpenBlox::OBEngine::getInstance()->warn(out)
	#define ELOG(out) OpenBlox::OBEngine::getInstance()->print_error(out)
#endif

#include <ob_lua.h>

#include "static_init.h"

#include "OBException.h"
#include "OBEngine.h"

#endif
