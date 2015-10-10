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

#ifndef OPENBLOX_OPENBLOX_H_
#define OPENBLOX_OPENBLOX_H_

#include "OpenBloxConfig.h"

//Global Macros

//OB_AS_STR_HELPER is used to expand macros and doesn't hurt if the value passed to OB_AS_STR isn't a macro.
#define OB_AS_STR_HELPER(x) #x
#define OB_AS_STR(x) OB_AS_STR_HELPER(x)

#define OB_VERSION_STR OB_AS_STR(OB_VERSION_MAJOR) "." OB_AS_STR(OB_VERSION_MINOR) "." OB_AS_STR(OB_VERSION_PATCH)

#include "OpenBloxGlobal.h"

#ifndef OB_NO_NETWORKING
	//Raknet
	#include <raknet/MessageIdentifiers.h>
	#include <raknet/RakPeerInterface.h>
	#include <raknet/RakNetTypes.h>
	#include <raknet/RakNetStatistics.h>
	#include <raknet/NetworkIDManager.h>
	#include <raknet/NetworkIDObject.h>
	#include <raknet/BitStream.h>
	#include <raknet/PacketPriority.h>
#endif

#endif
