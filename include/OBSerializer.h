/*
 * Copyright (C) 2017-2019 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "oblibconfig.h"

#include "mem.h"

#include <string>
#include <map>

#ifndef OB_OBSERIALIZER
#define OB_OBSERIALIZER

// We're on version 1 of the XML format
#define OB_SERIALIZER_XML_CURRENT_VERSION_ID 1
// We're on version 1 of the binary format
#define OB_SERIALIZER_BINARY_CURRENT_VERSION_ID 1

namespace OB{
#ifndef OB_OBENGINE
	class OBEngine;
#endif
#ifndef OB_BITSTREAM
	class BitStream;
#endif

	namespace Instance{
		class Instance;
	}

	class OBSerializer{
		public:
			OBSerializer(OBEngine* eng);

			// XML Format
#if HAVE_PUGIXML
			shared_ptr<Instance::Instance> LoadModelFromMemory_XML(char* buf, size_t size);
			shared_ptr<Instance::Instance> LoadModel_XML(std::string resURI);
			bool LoadFromMemory_XML(char* buf, size_t size);
			bool Load_XML(std::string resURI);
			bool SaveModel_XML(shared_ptr<Instance::Instance> model, std::string file);
			std::string SaveModelInMemory_XML(shared_ptr<Instance::Instance> model);
			bool Save_XML(std::string file);
			std::string SaveInMemory_XML();
#endif

			// Binary format
			shared_ptr<Instance::Instance> LoadModelFromMemory_binary(char* buf, size_t size);
			shared_ptr<Instance::Instance> LoadModel_binary(std::string resURI);
			bool LoadFromMemory_binary(char* buf, size_t size);
			bool Load_binary(std::string resURI);
			bool SaveModel_binary(shared_ptr<Instance::Instance> model, std::string file);
		    BitStream* SaveModelInMemory_binary(shared_ptr<Instance::Instance> model);
			bool Save_binary(std::string file);
			BitStream* SaveInMemory_binary();

			bool IsBinaryFormat(char* buf, size_t size);

			shared_ptr<Instance::Instance> LoadModelFromMemory(char* buf, size_t size);
			shared_ptr<Instance::Instance> LoadModel(std::string resURI);
			bool LoadFromMemory(char* buf, size_t size);
			bool Load(std::string resURI);

			std::string GetID(shared_ptr<Instance::Instance> inst);
			bool HasID(shared_ptr<Instance::Instance> inst);
			shared_ptr<Instance::Instance> GetByID(std::string id);
			void SetID(shared_ptr<Instance::Instance> inst, std::string newId);

		private:
			OBEngine* eng;

			std::map<shared_ptr<Instance::Instance>, std::string> instanceMap;
			int dynamic_instance_count;
	};
}

#endif // OB_OBSERIALIZER

// Local Variables:
// mode: c++
// End:
