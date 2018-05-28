/*
 * Copyright (C) 2018 John M. Harris, Jr. <johnmh@splentity.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef OB_JS_OBJS
#define OB_JS_OBJS

#include "oblibconfig.h"

#if HAVE_V8

#include <v8.h>
#include <libplatform/libplatform.h>

namespace OB{
	class OBEngine;

    namespace JS{
		struct _OBJState{
			v8::Isolate* isolate;
			v8::Isolate::CreateParams create_params;
		};
        /*
         * Initializes V8.
         */
        void init();

		/**
		 * Returns the OBEngine* associated with this V8 context.
		 *
		 * @returns OBEngine* engine
		 * @author John M. Harris, Jr.
		 */
		OBEngine* getEngine(v8::Local<v8::Context> ctx);

	    v8::Local<v8::Context> newContext();

		void runScript(v8::Local<v8::Context> ctx, char* src);
    }
}

#endif

#endif

// Local Variables:
// mode: c++
// End:
