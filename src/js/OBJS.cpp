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

#include "js/OBJS.h"

#include "OBException.h"

#if HAVE_V8

namespace OB{
	namespace JS{
		_OBJState* globalJSState = NULL;

		void init(){
			if(globalJSState != NULL){
				return;
			}

		    v8::Platform* platform = v8::platform::CreateDefaultPlatform(0, v8::platform::IdleTaskSupport::kDisabled, v8::platform::InProcessStackDumping::kDisabled, NULL);
			v8::V8::InitializePlatform(platform);
			v8::V8::Initialize();

		    _OBJState* newState = new _OBJState;
			newState->create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
			newState->isolate = v8::Isolate::New(newState->create_params);

			globalJSState = newState;
		}

		OBEngine* getEngine(v8::Local<v8::Context> ctx){
			return NULL;
		}

		v8::Local<v8::Context> newContext(){
			if(globalJSState == NULL){
				throw new OBException("V8 engine not initialized.");
			}

			v8::Isolate* isolate = globalJSState->isolate;

			v8::Isolate::Scope isolate_scope(isolate);

			v8::HandleScope handle_scope(isolate);

			return v8::Context::New(isolate);
		}

		void runScript(v8::Local<v8::Context> ctx, char* src){
			if(globalJSState == NULL){
				throw new OBException("V8 engine not initialized.");
			}

			v8::Isolate* isolate = globalJSState->isolate;

			v8::Context::Scope context_scope(ctx);

			v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, src, v8::NewStringType::kNormal).ToLocalChecked();

			v8::Local<v8::Script> script = v8::Script::Compile(ctx, source).ToLocalChecked();

			v8::MaybeLocal<v8::Value> res = script->Run(ctx);
		}
	}
}

#endif
