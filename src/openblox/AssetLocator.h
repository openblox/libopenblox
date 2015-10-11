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

#ifndef OPENBLOX_ASSETLOCATOR_H_
#define OPENBLOX_ASSETLOCATOR_H_

#include "OpenBlox.h"

namespace OpenBlox{
	/**
	 * Structure used to describe an asset.
	 * @author John M. Harris, Jr.
	 */
	struct asset_response{
		/**
		 * The size of data.
		 */
		size_t size;
		/**
		 * The actual data of the asset.
		 */
		char* data;
	};

	/**
	 * Used to locate and load assets, both internally and using the ContentProvider service.
	 * @author John M. Harris, Jr.
	 */
	class AssetLocator{
		public:
			AssetLocator();
			virtual ~AssetLocator();

			static size_t write_data(void* ptr, size_t size, size_t nmemb, struct asset_response* data);

			static AssetLocator* getInstance();

			asset_response* getAsset(QString url);
			#ifndef OPENBLOX_NO_GRAPHICS
			SDL_RWops* openSDLRW(QString url);
			#endif
			void putAsset(QString url, QString content);
			bool hasAsset(QString url);
		private:
			std::map<QString, asset_response*> contentCache;

			static AssetLocator* inst;
	};
}

#endif
