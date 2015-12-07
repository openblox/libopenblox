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

#ifndef OPENBLOX_INSTANCE_CAMERA_H_
#define OPENBLOX_INSTANCE_CAMERA_H_

#include "Instance.h"

BEGIN_INSTANCE

class Camera: public Instance{
	public:
		Camera();
		virtual ~Camera();

		DECLARE_CLASS(Camera);
};

END_INSTANCE

#endif
