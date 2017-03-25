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

#include "OBEngine.h"
#include "TaskScheduler.h"
#include "OBException.h"
#include "utility.h"

#include "instance/ContentProvider.h"

#include <iostream>

#include <cstdlib>
#include <cstring>
#include "unistd.h"

#include "oblibconfig.h"

#if HAVE_CURL
#include <curl/curl.h>
#endif

namespace OB{
	AssetResponse::AssetResponse(size_t size, char* data){
		this->size = size;
		this->data = data;
	}

	AssetResponse::~AssetResponse(){
		if(size > 0 && data){
			free(data);
		}
	}

	size_t AssetResponse::getSize(){
		return size;
	}

    char* AssetResponse::getData(){
		return data;
	}
	
    AssetLocator::AssetLocator(){
		requestQueueSize = 0;

		loadingResponse = make_shared<AssetResponse>(0, (char*)NULL);
		
		pthread_mutex_init(&mmutex, NULL);
	}

    AssetLocator::~AssetLocator(){
		pthread_mutex_destroy(&mmutex);
	}

	struct _ob_curl_body{
		public:
		    char* data;
			size_t size;
	};
	
	size_t _ob_assetlocator_write_data(void* ptr, size_t size, size_t nmemb, struct _ob_curl_body* data){
		size_t index = data->size;
		size_t n = (size * nmemb);
	    char* tmp;

		data->size += n;

		tmp = (char*)realloc(data->data, data->size);

		if(tmp){
			data->data = tmp;
		}else{
			if(data->data){
			    free(data->data);
				data->data = NULL;
			}
			std::cout << "[AssetLocator] Failed to allocate memory." << std::endl;
			return 0;
		}

		memcpy((data->data + index), ptr, n);

		return n;
	}

    void AssetLocator::loadAssetSync(std::string url, bool decCount){
	    pthread_mutex_lock(&mmutex);
		
		if(url.empty()){
			if(decCount){
				requestQueueSize--;
			}
			pthread_mutex_unlock(&mmutex);
			return;
		}

		if(ob_str_startsWith(url, "file://")){
			if(decCount){
			    requestQueueSize--;
			}
			pthread_mutex_unlock(&mmutex);
			return;
		}

		struct _ob_curl_body* body = new struct _ob_curl_body;
		body->size = 0;
		body->data = NULL;

		if(ob_str_startsWith(url, "res://")){
			std::string furl = url.substr(6);

			std::cout << "Looking for file: " << furl << std::endl;

			std::string canonPath = realpath(furl.c_str(), NULL);
			std::cout << "Looking for file: " << canonPath << std::endl;

			delete body;

			if(decCount){
				requestQueueSize--;
			}

			pthread_mutex_unlock(&mmutex);
			return;
		}

		#if HAVE_CURL

		CURL* curl;
		CURLcode res;
		
		curl = curl_easy_init();
		if(curl){
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
			//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _ob_assetlocator_write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, body);

			res = curl_easy_perform(curl);
			if(res != CURLE_OK){
				std::cout << "[AssetLocator] cURL Error: " << curl_easy_strerror(res) << std::endl;

				OBEngine* eng = OBEngine::getInstance();
				shared_ptr<Instance::DataModel> dm = eng->getDataModel();
				shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
				shared_ptr<Type::Event> AssetLoadFailed = cp->GetAssetLoadFailed();

				std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
				fireArgs.push_back(make_shared<Type::VarWrapper>(url));
				fireArgs.push_back(make_shared<Type::VarWrapper>(std::string(curl_easy_strerror(res))));

				AssetLoadFailed->Fire(fireArgs);

				curl_easy_cleanup(curl);

				if(decCount){
					requestQueueSize--;
				}
				
			    delete body;

				pthread_mutex_unlock(&mmutex);
				
				return;
			}

			curl_easy_cleanup(curl);

			if(body->data != NULL){
				OBEngine* eng = OBEngine::getInstance();
				shared_ptr<Instance::DataModel> dm = eng->getDataModel();
				shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
				shared_ptr<Type::Event> AssetLoaded = cp->GetAssetLoaded();

				std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
				fireArgs.push_back(make_shared<Type::VarWrapper>(url));
				
				AssetLoaded->Fire(fireArgs);
				
				putAsset(url, body->size, body->data);
			}else{
				std::cout << "[AssetLocator] No data" << std::endl;

				OBEngine* eng = OBEngine::getInstance();
				shared_ptr<Instance::DataModel> dm = eng->getDataModel();
				shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
				shared_ptr<Type::Event> AssetLoadFailed = cp->GetAssetLoadFailed();

				std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
				fireArgs.push_back(make_shared<Type::VarWrapper>(url));
				fireArgs.push_back(make_shared<Type::VarWrapper>("No data."));

				AssetLoadFailed->Fire(fireArgs);
			}
		}else{
			std::cout << "[AssetLocator] Failed to initialize cURL" << std::endl;

			OBEngine* eng = OBEngine::getInstance();
			shared_ptr<Instance::DataModel> dm = eng->getDataModel();
			shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
			shared_ptr<Type::Event> AssetLoadFailed = cp->GetAssetLoadFailed();

			std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
			fireArgs.push_back(make_shared<Type::VarWrapper>(url));
			fireArgs.push_back(make_shared<Type::VarWrapper>("Failed to initialize cURL."));

			AssetLoadFailed->Fire(fireArgs);
		}
		#endif

		if(decCount){
			requestQueueSize--;
		}

		delete body;
		
		pthread_mutex_unlock(&mmutex);
	}

	struct _ob_assetLocatorMetad{
	    char* url;
	};

	int AssetLocator::loadAssetAsyncTask(void* metad, ob_int64 startTime){
		if(metad == NULL){
			return 0;
		}
		
		struct _ob_assetLocatorMetad* locmetad = (struct _ob_assetLocatorMetad*)metad;

		OBEngine* eng = OBEngine::getInstance();
		shared_ptr<AssetLocator> assetLoc = eng->getAssetLocator();

		assetLoc->loadAssetSync(locmetad->url, true);

	    free(locmetad->url);
		free(metad);
		
		return 0;
	}

	void AssetLocator::loadAsset(std::string url){
		if(url.empty()){
			return;
		}

		if(ob_str_startsWith(url, "file://")){
			return;
		}

		OBEngine* eng = OBEngine::getInstance();
		shared_ptr<TaskScheduler> taskS = eng->getSecondaryTaskScheduler();

		struct _ob_assetLocatorMetad* metad = new struct _ob_assetLocatorMetad;
		metad->url = strdup(url.c_str());

		pthread_mutex_lock(&mmutex);
		
		requestQueueSize++;

		pthread_mutex_unlock(&mmutex);

		contentCache.emplace(url, loadingResponse);
		
		taskS->enqueue(loadAssetAsyncTask, metad, 0);
	}
	
	shared_ptr<AssetResponse> AssetLocator::getAsset(std::string url, bool loadIfNotPresent){
		if(url.empty()){
			return NULL;
		}

		if(hasAsset(url)){
		    shared_ptr<AssetResponse> resp = shared_ptr<AssetResponse>(contentCache.at(url));
			if(resp != loadingResponse){
				return resp;
			}
		}else{
			if(loadIfNotPresent){
				loadAsset(url);
				return getAsset(url, false);
			}
		}
		
		return NULL;
	}

	bool AssetLocator::hasAsset(std::string url){
		return contentCache.count(url) != 0;
	}

	void AssetLocator::putAsset(std::string url, size_t size, char* data){
	    contentCache.emplace(url, make_shared<AssetResponse>(size, data));
	}

	int AssetLocator::getRequestQueueSize(){
		return requestQueueSize;
	}
}
