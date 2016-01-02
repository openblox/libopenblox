/*
 * Copyright 2016 John M. Harris, Jr.
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
 * @file OpenBloxConfig.h
 * @author John M. Harris, Jr.
 * @date August 2015
 *
 * This file contains configuration of OpenBlox, and explains the various configuration options.
 *
 * This file is included into OpenBlox.h, allowing anything here to be globally included.
 */

#ifndef OPENBLOX_OPENBLOXCONFIG_H_
#define OPENBLOX_OPENBLOXCONFIG_H_

//Self explanatory.
#define OB_VERSION_MAJOR 0
#define OB_VERSION_MINOR 1
#define OB_VERSION_PATCH 1

//Global defines go here. What is considered global? Things like OB_SERVER, OB_NO_GRAPHICS and that sort.
//If OB_SERVER is defined, server-only classes and functionality will be available.
//#define OB_SERVER

//If OB_NO_GRAPHICS is defined, no graphics capabilities will be available.
//#define OB_NO_GRAPHICS

//If OB_STUDIO is defined, Instances made for Studio (such as Selection and ChangeHistoryService) will be defined.
//#define OB_STUDIO

//If OB_NO_PHYS is defined, no physics functionality will be available.
//This is available for people who wish to create 2D games, 3D classes will be available for rendering only.
//#define OB_NO_PHYS

//If OB_NO_NETWORKING is defined, server/client networking will not be available.
//#define OB_NO_NETWORKING

#endif
