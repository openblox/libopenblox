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

#ifndef OB_ASSETLOCATOR
#define OB_ASSETLOCATOR

#include "obtype.h"
#include "mem.h"

#include <string>
#include <map>

namespace OB{
	class AssetResponse{
		public:
			AssetResponse(size_t size, char* data);
			~AssetResponse();

			size_t getSize();
		    char* getData();

		private:
			size_t size;
			char* data;
			
	};
	
	class AssetLocator{
		public:
		    AssetLocator();
			virtual ~AssetLocator();

			void tick();

			void loadAssetSync(std::string url, bool decCount = false);
			static int loadAssetAsyncTask(void* metad, ob_int64 startTime);
			void loadAsset(std::string url);
			shared_ptr<AssetResponse> getAsset(std::string url, bool loadIfNotPresent = false);
			bool hasAsset(std::string url);
			void putAsset(std::string url, size_t size, char* data);

			int getRequestQueueSize();
			
		private:
			std::map<std::string, shared_ptr<AssetResponse>> contentCache;

			int requestQueueSize;
	};
}

#endif // OB_ASSETLOCATOR

// Local Variables:
// mode: c++
// End:
