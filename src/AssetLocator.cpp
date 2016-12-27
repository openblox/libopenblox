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

#include "AssetLocator.h"

#include "OBException.h"
#include "utility.h"

#include <cstdlib>

namespace OB{
	AssetResponse::AssetResponse(size_t size, void* data){
		this->size = size;
		this->data = data;
	}

	AssetResponse::~AssetResponse(){
		free(data);
	}
	
    AssetLocator::AssetLocator(){}

    AssetLocator::~AssetLocator(){}

    void AssetLocator::loadAsset(std::string url){
		if(url.empty()){
			return;
		}

		if(ob_str_startsWith(url, "file://")){
			return;
		}
	}
	
	shared_ptr<AssetResponse> AssetLocator::getAsset(std::string url, bool loadIfNotPresent){
		if(url.empty()){
			return NULL;
		}
		
		if(hasAsset(url)){
			return contentCache[url];
		}else{
			if(loadIfNotPresent){
				loadAsset(url);
				return getAsset(url, false);
			}
		}
	}

	bool AssetLocator::hasAsset(std::string url){
		return contentCache.count(url) != 0;
	}
}
