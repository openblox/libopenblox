/*
 * Copyright 2016 John M. Harris, Jr.
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

#include "AssetLocator.h"

#include <curl/curl.h>

namespace OpenBlox{
	AssetLocator* AssetLocator::inst = NULL;

	AssetLocator::AssetLocator(){
		if(inst != NULL){
			throw new OBException(OB_TRANSLATE("AssetLocator", "Only one instance of AssetLocator can be created."));
		}
		inst = this;

		contentCache = std::map<QString, asset_response*>();
	}

	AssetLocator::~AssetLocator(){}

	/**
	 * Returns the AssetLocator instance.
	 * @return AssetLocator*, NULL if there is no instance yet.
	 * @author John M. Harris, Jr.
	 */
	AssetLocator* AssetLocator::getInstance(){
		return inst;
	}

	/**
	 * Used to strip the content-type from base64 encoded strings.
	 * @author John M. Harris, Jr.
	 */
	QString stripContentType(QString origin){
		std::vector<QString> knownMimeTypes = {"text/plain", "text/html", "image/png", "image/jpeg", "audio/mp3"};

		for(std::vector<QString>::size_type i = 0; i < knownMimeTypes.size(); i++){
			QString knownType = knownMimeTypes[i];
			if(origin.startsWith(knownType)){
				origin = origin.mid(knownType.length());
				break;
			}
		}

		if(origin[0] == ';'){
			origin = origin.mid(1);
		}

		return origin;
	}

	/**
	 * Used internally by the AssetLocator, when making a request with cURL.
	 * @author John M. Harris, Jr.
	 */
	size_t AssetLocator::write_data(void* ptr, size_t size, size_t nmemb, struct asset_response* data){
		size_t index = data->size;
		size_t n = (size * nmemb);
		char* tmp;

		data->size += n;

		tmp = (char*)realloc(data->data, data->size + 1);

		if(tmp){
			data->data = tmp;
		}else{
			if(data->data){
				delete[] data->data;
			}
			LOGE("[AssetLocator] " OB_TRANSLATE("AssetLocator", "Failed to allocate memory."));
			return 0;
		}

		memcpy((data->data + index), ptr, n);
		data->data[data->size] = '\0';

		return n;
	}

	/**
	 * Used to get information on an asset.
	 * @param QString url The url to load.
	 * @returns asset_response* containing the asset's information.
	 * @author John M. Harris, Jr.
	 */
	asset_response* AssetLocator::getAsset(QString url){
		if(url.isNull()){
			return NULL;
		}

		if(hasAsset(url)){
			return contentCache[url];
		}

		if(url.startsWith("file://")){
			return NULL;
		}

		asset_response* body = new asset_response();
		body->size = 0;
		body->data = new char[4096];

		if(body->data == NULL){
			LOGE("[AssetLocator] " OB_TRANSLATE("AssetLocator", "Failed to allocate memory."));
			return NULL;
		}

		body->data[0] = '\0';

		if(url.startsWith("data:")){
			QString nURL = url.mid(5);
			url = stripContentType(nURL);

			bool isBase64 = false;
			if(nURL.startsWith("base64,")){
				nURL = nURL.mid(7);
				isBase64 = true;
			}else if(nURL.startsWith(',')){
				nURL = nURL.mid(1);
			}

			if(nURL.length() == 0){
				return NULL;
			}

			if(isBase64){
				QByteArray decoded = QByteArray::fromBase64(nURL.toUtf8());

				body->data = decoded.data();
				body->size = decoded.length();

				return body;
			}

			char* data = new char[nURL.length() + 1];
			strcat(data, nURL.toStdString().c_str());

			data[nURL.length()] = '\0';

			body->data = data;
			body->size = nURL.length();

			return body;
		}

		if(url.startsWith("res://")){
			QString toAppend = url.mid(6);

			//Try resources first
			QFile resPath(":/res/" + toAppend);
			if(resPath.exists()){
				QFileInfo info(resPath);

				qint64 file_size = info.size();
				char* file_contents = new char[file_size + 1];

				if(resPath.open(QIODevice::ReadOnly)){
					if(resPath.read(file_contents, file_size) == file_size){
						file_contents[file_size] = '\0';

						body->data = file_contents;
						body->size = file_size;

						contentCache[url] = body;

						return body;
					}
					return NULL;
				}

				return NULL;
			}

			QDir startPath("res");
			if(startPath.exists()){
				QString pathStart = startPath.canonicalPath();

				QFile filePath(pathStart + QDir::separator() + toAppend);
				if(filePath.exists()){
					QFileInfo info(filePath);
					QString path = info.canonicalFilePath();

					if(path.startsWith(pathStart)){//Prevents CDUP
						if(info.isFile() && info.isReadable()){
							qint64 file_size = info.size();
							char* file_contents = new char[file_size + 1];

							if(filePath.open(QIODevice::ReadOnly)){
								if(filePath.read(file_contents, file_size) == file_size){
									file_contents[file_size] = '\0';

									body->data = file_contents;
									body->size = file_size;

									contentCache[url] = body;

									return body;
								}
								return NULL;
							}
						}
					}
				}
			}

			#ifdef __linux__
			QDir share_startPath("/usr/share/openblox/res");
			if(share_startPath.exists()){
				QString pathStart = share_startPath.canonicalPath();

				QFile filePath(pathStart + QDir::separator() + toAppend);
				if(filePath.exists()){
					QFileInfo info(filePath);
					QString path = info.canonicalFilePath();

					if(path.startsWith(pathStart)){//Prevents CDUP
						if(info.isFile() && info.isReadable()){
							qint64 file_size = info.size();
							char* file_contents = new char[file_size + 1];

							if(filePath.open(QIODevice::ReadOnly)){
								if(filePath.read(file_contents, file_size) == file_size){
									file_contents[file_size] = '\0';

									body->data = file_contents;
									body->size = file_size;

									contentCache[url] = body;

									return body;
								}
								return NULL;
							}
						}
					}
				}
			}
			#endif

			return NULL;
		}

		CURL* curl;
		CURLcode res;

		curl = curl_easy_init();
		if(curl){
			curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
			//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, body);

			res = curl_easy_perform(curl);
			if(res != CURLE_OK){
				LOGE("[AssetLocator] cURL Error: %s", curl_easy_strerror(res));
			}

			curl_easy_cleanup(curl);

			contentCache[url] = body;

			return body;
		}

		return NULL;
	}

	#ifndef OPENBLOX_NO_GRAPHICS

	/**
	 * Used to get information on an asset for use with SDL.
	 * @param QString url The url to load.
	 * @returns SDL_RWops* containing the asset's information.
	 * @author John M. Harris, Jr.
	 */
	SDL_RWops* AssetLocator::openSDLRW(QString url){
		asset_response* resp = getAsset(url);
		if(resp){
			return SDL_RWFromConstMem(resp->data, resp->size);
		}else{
			return NULL;
		}
	}
	#endif

	/**
	 * Used to put virtual assets into the AssetLocator. Content can be plain text or base64 encoded data.
	 * @param QString url The url to use for this asset.
	 * @param QString content The content for this asset.
	 * @author John M. Harris, Jr.
	 */
	void AssetLocator::putAsset(QString url, QString content){
		asset_response* body = new asset_response();
		body->data = new char{'\0'};
		body->size = 0;

		if(content.startsWith("data:")){
			QString nURL = content.mid(5);

			url = stripContentType(nURL);

			bool isBase64 = false;
			if(nURL.startsWith("base64,")){
				nURL = nURL.mid(7);
				isBase64 = true;
			}else if(nURL.startsWith(',')){
				nURL = nURL.mid(1);
			}

			if(nURL.length() == 0){
				body->data = new char{'\0'};
				body->size = 0;

				contentCache[url] = body;

				return;
			}

			if(isBase64){
				QByteArray decoded = QByteArray::fromBase64(nURL.toUtf8());

				body->data = decoded.data();
				body->size = decoded.length();

				contentCache[url] = body;

				return;
			}

			char* data = new char[nURL.length() + 1];
			strcat(data, nURL.toStdString().c_str());
			data[nURL.length() + 1] = '\0';

			body->data = data;
			body->size = nURL.length();

			contentCache[url] = body;

			return;
		}

		char* data = new char[content.length() + 1];
		strcat(data, content.toStdString().c_str());
		data[content.length() + 1] = '\0';

		body->data = data;
		body->size = content.length();

		contentCache[url] = body;
	}

	/**
	 * Used to tell if the AssetLocator has already loaded an asset.
	 * @param QString url The url to test.
	 * @returns bool true if there is an asset with the provided url, false if there is not.
	 * @author John M. Harris, Jr.
	 */
	bool AssetLocator::hasAsset(QString url){
		return contentCache.count(url) != 0;
	}
}
