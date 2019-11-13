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

#include "AssetLocator.h"

#include "OBEngine.h"
#include "TaskScheduler.h"
#include "OBException.h"
#include "utility.h"

#include "instance/ContentProvider.h"

#include <iostream>
#include <fstream>

#include <cstdlib>
#include <cstring>

#include "oblibconfig.h"

#if HAVE_CURL
#include <curl/curl.h>
#endif

namespace OB{
    AssetResponse::AssetResponse(size_t size, char* data, std::string resURI, OBEngine* eng){
        this->size = size;
        this->data = data;
        this->resURI = resURI;
        this->eng = eng;
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

    std::string AssetResponse::getResURI(){
        return resURI;
    }

#if HAVE_IRRLICHT
    irr::io::IReadFile* AssetResponse::toIReadFile(){
        irr::IrrlichtDevice* irrDev = eng->getIrrlichtDevice();
        if(irrDev){
            irr::scene::ISceneManager* smgr = irrDev->getSceneManager();
            if(smgr){
                irr::io::IFileSystem* ifs = smgr->getFileSystem();
                if(ifs){
                    return ifs->createMemoryReadFile((void*)data, size, resURI.c_str(), false);
                }
            }
        }
        return NULL;
    }
#endif

    AssetLocator::AssetLocator(OBEngine* eng){
        this->eng = eng;

        requestQueueSize = 0;

        loadingResponse = make_shared<AssetResponse>(0, (char*)NULL, "loading://null", eng);

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

    void AssetLocator::loadAssetSync(std::string url, bool decCount, bool allowFile){
        pthread_mutex_lock(&mmutex);

        if(url.empty()){
            if(decCount){
                requestQueueSize--;
            }
            pthread_mutex_unlock(&mmutex);
            return;
        }

        if(!allowFile && ob_str_startsWith(url, "file://")){
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

            char* ccanonPath = realpath(furl.c_str(), NULL);
            if(!ccanonPath){
                ccanonPath = realpath(("res/" + furl).c_str(), NULL);
            }

            if(ccanonPath){
                std::string canonPath = ccanonPath;

                char* realRes = realpath("res/", NULL);
                char* thisDir = get_current_dir_name();
                if(realRes){
                    if(!ob_str_startsWith(canonPath, std::string(realRes)) || !ob_str_startsWith(canonPath, std::string(thisDir))){
                        shared_ptr<Instance::DataModel> dm = eng->getDataModel();
                        shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
                        shared_ptr<Type::Event> AssetLoadFailed = cp->GetAssetLoadFailed();

                        std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
                        fireArgs.push_back(make_shared<Type::VarWrapper>(url));
                        fireArgs.push_back(make_shared<Type::VarWrapper>("File not under resource directory."));

                        AssetLoadFailed->Fire(eng, fireArgs);

                        if(decCount){
                            requestQueueSize--;
                        }

                        delete body;

                        pthread_mutex_unlock(&mmutex);
                        free(thisDir);
                        return;
                    }
                }else{
                    if(!ob_str_startsWith(canonPath, std::string(thisDir))){
                        shared_ptr<Instance::DataModel> dm = eng->getDataModel();
                        shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
                        shared_ptr<Type::Event> AssetLoadFailed = cp->GetAssetLoadFailed();

                        std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
                        fireArgs.push_back(make_shared<Type::VarWrapper>(url));
                        fireArgs.push_back(make_shared<Type::VarWrapper>("File not under resource directory."));

                        AssetLoadFailed->Fire(eng, fireArgs);

                        if(decCount){
                            requestQueueSize--;
                        }

                        delete body;

                        pthread_mutex_unlock(&mmutex);
                        free(thisDir);
                        return;
                    }
                }
                free(thisDir);

                std::ifstream file(canonPath, std::ios::binary | std::ios::ate);
                size_t fileLen = file.tellg();
                file.seekg(0, std::ios::beg);

                char* bodyDat = (char*)malloc(fileLen);
                if(file.read(bodyDat, fileLen)){
                    bodyDat[fileLen] = '\0';

                    body->data = bodyDat;
                    body->size = fileLen;
                }else{
                    shared_ptr<Instance::DataModel> dm = eng->getDataModel();
                    shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
                    shared_ptr<Type::Event> AssetLoadFailed = cp->GetAssetLoadFailed();

                    std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
                    fireArgs.push_back(make_shared<Type::VarWrapper>(url));
                    fireArgs.push_back(make_shared<Type::VarWrapper>("Failed to read file."));

                    AssetLoadFailed->Fire(eng, fireArgs);

                    if(decCount){
                        requestQueueSize--;
                    }

                    delete body;

                    pthread_mutex_unlock(&mmutex);
                    free(bodyDat);
                    return;
                }
            }else{
                shared_ptr<Instance::DataModel> dm = eng->getDataModel();
                shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
                shared_ptr<Type::Event> AssetLoadFailed = cp->GetAssetLoadFailed();

                std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
                fireArgs.push_back(make_shared<Type::VarWrapper>(url));
                fireArgs.push_back(make_shared<Type::VarWrapper>("File not found."));

                AssetLoadFailed->Fire(eng, fireArgs);

                if(decCount){
                    requestQueueSize--;
                }

                delete body;

                pthread_mutex_unlock(&mmutex);
                return;
            }
        }else{

#if HAVE_CURL

            CURL* curl;
            CURLcode res;

            curl = curl_easy_init();
            if(curl){
				long curl_protos = CURLPROTO_FTP | CURLPROTO_FTPS | CURLPROTO_GOPHER | CURLPROTO_HTTP | CURLPROTO_HTTPS | CURLPROTO_SCP | CURLPROTO_SFTP | CURLPROTO_SMB | CURLPROTO_SMBS | CURLPROTO_TFTP;
				if(allowFile){
					curl_protos = CURLPROTO_FILE | curl_protos;
				}

                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
                curl_easy_setopt(curl, CURLOPT_PROTOCOLS, curl_protos);
				curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _ob_assetlocator_write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, body);

                res = curl_easy_perform(curl);
                if(res != CURLE_OK){
                    std::cout << "[AssetLocator] cURL Error: " << curl_easy_strerror(res) << std::endl;

                    shared_ptr<Instance::DataModel> dm = eng->getDataModel();
                    shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
                    shared_ptr<Type::Event> AssetLoadFailed = cp->GetAssetLoadFailed();

                    std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
                    fireArgs.push_back(make_shared<Type::VarWrapper>(url));
                    fireArgs.push_back(make_shared<Type::VarWrapper>(std::string(curl_easy_strerror(res))));

                    AssetLoadFailed->Fire(eng, fireArgs);

                    curl_easy_cleanup(curl);

                    if(decCount){
                        requestQueueSize--;
                    }

                    delete body;

                    pthread_mutex_unlock(&mmutex);

                    return;
                }

                curl_easy_cleanup(curl);
            }else{
                std::cout << "[AssetLocator] Failed to initialize cURL" << std::endl;

                shared_ptr<Instance::DataModel> dm = eng->getDataModel();
                shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
                shared_ptr<Type::Event> AssetLoadFailed = cp->GetAssetLoadFailed();

                std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
                fireArgs.push_back(make_shared<Type::VarWrapper>(url));
                fireArgs.push_back(make_shared<Type::VarWrapper>("Failed to initialize cURL."));

                AssetLoadFailed->Fire(eng, fireArgs);

                if(decCount){
                    requestQueueSize--;
                }

                delete body;

                pthread_mutex_unlock(&mmutex);
                return;
            }
#endif
        }

        if(body->data != NULL){
            shared_ptr<Instance::DataModel> dm = eng->getDataModel();
            shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
            shared_ptr<Type::Event> AssetLoaded = cp->GetAssetLoaded();

            std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
            fireArgs.push_back(make_shared<Type::VarWrapper>(url));

            putAsset(url, body->size, body->data);

            AssetLoaded->Fire(eng, fireArgs);

            std::vector<weak_ptr<Instance::Instance>>::iterator i = instancesWaiting.begin();
            while(i != instancesWaiting.end()){
                if(i->expired()){
                    i = instancesWaiting.erase(i);
                    continue;
                }

                shared_ptr<Instance::Instance> inst = i->lock();
                if(inst){
                    bool didLoad = inst->assetLoaded(url);
                    if(didLoad){
                        i = instancesWaiting.erase(i);
                        continue;
                    }
                }

                i++;
            }
        }else{
            std::cout << "[AssetLocator] No data" << std::endl;

            shared_ptr<Instance::DataModel> dm = eng->getDataModel();
            shared_ptr<Instance::ContentProvider> cp = dm->getContentProvider();
            shared_ptr<Type::Event> AssetLoadFailed = cp->GetAssetLoadFailed();

            std::vector<shared_ptr<Type::VarWrapper>> fireArgs;
            fireArgs.push_back(make_shared<Type::VarWrapper>(url));
            fireArgs.push_back(make_shared<Type::VarWrapper>("No data."));

            AssetLoadFailed->Fire(eng, fireArgs);
        }

        if(decCount){
            requestQueueSize--;
        }

        delete body;

        pthread_mutex_unlock(&mmutex);
    }

    struct _ob_assetLocatorMetad{
        char* url;
        OBEngine* eng;
    };

    int AssetLocator::loadAssetAsyncTask(void* metad, ob_uint64 startTime){
        if(metad == NULL){
            return 0;
        }

        struct _ob_assetLocatorMetad* locmetad = (struct _ob_assetLocatorMetad*)metad;

        OBEngine* eng = locmetad->eng;
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

        shared_ptr<TaskScheduler> taskS = eng->getSecondaryTaskScheduler();

        struct _ob_assetLocatorMetad* metad = new struct _ob_assetLocatorMetad;

		// Because Windows is fucking annoying
#ifdef _MSC_VER
		metad->url = _strdup(url.c_str());
#else
		metad->url = strdup(url.c_str());
#endif
        metad->eng = eng;

        pthread_mutex_lock(&mmutex);

        requestQueueSize++;

        pthread_mutex_unlock(&mmutex);

        contentCache.emplace(url, loadingResponse);

        taskS->enqueue(loadAssetAsyncTask, metad, 0, false, false);
    }

    shared_ptr<AssetResponse> AssetLocator::getAsset(std::string url, bool loadIfNotPresent){
        if(url.empty()){
            return NULL;
        }

        if(hasAsset(url)){
            shared_ptr<AssetResponse> resp = contentCache.at(url);
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
        std::map<std::string, shared_ptr<AssetResponse>>::iterator i = contentCache.find(url);

        if(i != contentCache.end()){
            contentCache.erase(i);
        }

        contentCache.emplace(url, make_shared<AssetResponse>(size, data, url, eng));
    }

    void AssetLocator::addWaitingInstance(shared_ptr<Instance::Instance> inst){
        if(inst){
            instancesWaiting.push_back(inst);
        }
    }

    int AssetLocator::getRequestQueueSize(){
        return requestQueueSize;
    }
}
