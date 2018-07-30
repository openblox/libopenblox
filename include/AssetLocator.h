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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef OB_ASSETLOCATOR
#define OB_ASSETLOCATOR

#include "obtype.h"
#include "mem.h"

#include <pthread.h>

#include <string>
#include <map>
#include <vector>

#include "oblibconfig.h"

#if HAVE_IRRLICHT
#include <irrlicht/irrlicht.h>
#endif

namespace OB{
	class OBEngine;

	namespace Instance{
		class Instance;
	}

	class AssetResponse{
		public:
			AssetResponse(size_t size, char* data, std::string resURI, OBEngine* eng);
			~AssetResponse();

			size_t getSize();
			char* getData();
			std::string getResURI();

#if HAVE_IRRLICHT
			irr::io::IReadFile* toIReadFile();
#endif

		private:
			size_t size;
			char* data;
			std::string resURI;

			OBEngine* eng;
	};

	class AssetLocator{
		public:
			AssetLocator(OBEngine* eng);
			virtual ~AssetLocator();

			void tick();

			void loadAssetSync(std::string url, bool decCount = false, bool allowFile = false);
			static int loadAssetAsyncTask(void* metad, ob_uint64 startTime);
			void loadAsset(std::string url);
			shared_ptr<AssetResponse> getAsset(std::string url, bool loadIfNotPresent = false);
			bool hasAsset(std::string url);
			void putAsset(std::string url, size_t size, char* data);

			void addWaitingInstance(shared_ptr<Instance::Instance> inst);

			int getRequestQueueSize();

		private:
			std::map<std::string, shared_ptr<AssetResponse>> contentCache;
			OBEngine* eng;

			std::vector<weak_ptr<Instance::Instance>> instancesWaiting;

			shared_ptr<AssetResponse> loadingResponse;
			pthread_mutex_t mmutex;

			int requestQueueSize;
	};
}

#endif // OB_ASSETLOCATOR

// Local Variables:
// mode: c++
// End:
