/*
 * Copyright (C) 2019 John M. Harris, Jr. <johnmh@openblox.org>
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

#include "instance/HttpService.h"

#if HAVE_CURL
#include <curl/curl.h>
#endif

#ifdef _MSC_VER // Windows
#include <objbase.h>
#elif HAVE_UUID // Most Unix-like
#include <uuid/uuid.h>
#endif

#include "AssetLocator.h"
#include "OBException.h"

namespace OB{
	namespace Instance{
		DEFINE_CLASS(HttpService, false, isDataModel, Instance){
			registerLuaClass(eng, LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
		}

	    HttpService::HttpService(OBEngine* eng) : Instance(eng){
			Name = ClassName;
			netId = OB_NETID_HTTPSERVICE;
		}

	    HttpService::~HttpService(){}

		std::string HttpService::fixedSerializedID(){
		    return "HttpService";
		}

		shared_ptr<Instance> HttpService::cloneImpl(){
			return NULL;
		}

		std::string HttpService::GetAsync(std::string url, bool nocache){
			(void)nocache;
#if HAVE_CURL

			struct _ob_curl_body* body = new struct _ob_curl_body;
			body->size = 0;
			body->data = NULL;

			CURL* curl;
			CURLcode res;

			curl = curl_easy_init();
			if(curl){
				long curl_protos = CURLPROTO_FTP | CURLPROTO_FTPS | CURLPROTO_GOPHER | CURLPROTO_HTTP | CURLPROTO_HTTPS;
						
				curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
				curl_easy_setopt(curl, CURLOPT_PROTOCOLS, curl_protos);
				curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, AssetLocator::_ob_assetlocator_write_data);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, body);

				res = curl_easy_perform(curl);
				if(res != CURLE_OK){
					std::cout << "[HttpService] cURL Error: " << curl_easy_strerror(res) << std::endl;

					curl_easy_cleanup(curl);

					if(body->data){
						delete[] body->data;
					}

					delete body;

					throw new OBException("A cURL error occurred");
				}

				curl_easy_cleanup(curl);
			}else{
				throw new OBException("Failed to initialize cURL.");
			}

			if(body->data){
				std::string retStr = std::string(body->data);

				delete[] body->data;
				delete body;

				return retStr;
			}else{
				delete body;
				return "";
			}
#else
			(void)url;
			throw new OBException("No cURL support.");
#endif
		}

		std::string HttpService::PostAsync(std::string url, std::string data, int contentType){
#if HAVE_CURL

			struct _ob_curl_body* body = new struct _ob_curl_body;
			body->size = 0;
			body->data = NULL;

			CURL* curl;
			CURLcode res;

			curl = curl_easy_init();
			if(curl){
				struct curl_slist* chunk = NULL;

				switch(contentType){
					case 0:
						chunk = curl_slist_append(chunk, "Content-Type: application/xml");
						break;
					case 1:
						chunk = curl_slist_append(chunk, "Content-Type: application/x-www-form-urlencoded");
						break;
					case 2:
						chunk = curl_slist_append(chunk, "Content-Type: text/plain");
						break;
					case 3:
						chunk = curl_slist_append(chunk, "Content-Type: text/xml");
						break;
					default:
						chunk = curl_slist_append(chunk, "Content-Type: application/json");
				}

				res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
				if(res != CURLE_OK){
					std::cout << "[HttpService] cURL Error: " << curl_easy_strerror(res) << std::endl;
				}
				
				long curl_protos = CURLPROTO_HTTP | CURLPROTO_HTTPS;
						
				curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
				curl_easy_setopt(curl, CURLOPT_PROTOCOLS, curl_protos);
				curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, AssetLocator::_ob_assetlocator_write_data);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, body);

				res = curl_easy_perform(curl);
				if(res != CURLE_OK){
					std::cout << "[HttpService] cURL Error: " << curl_easy_strerror(res) << std::endl;

					curl_easy_cleanup(curl);

					if(body->data){
						delete[] body->data;
					}

					delete body;

					throw new OBException("A cURL error occurred");
				}

				curl_easy_cleanup(curl);
			}else{
				throw new OBException("Failed to initialize cURL.");
			}

			if(body->data){
				std::string retStr = std::string(body->data);

				delete[] body->data;
				delete body;

				return retStr;
			}else{
				delete body;
				return "";
			}
#else
			(void)url;
			throw new OBException("No cURL support.");
#endif
		}

		std::string HttpService::UrlEncode(std::string input){
#if HAVE_CURL
			CURL* curl;

			curl = curl_easy_init();
			if(curl){
				char* encoded = curl_easy_escape(curl, input.c_str(), 0);

				std::string retStr = std::string(encoded);

				curl_free(encoded);
				curl_easy_cleanup(curl);

				return retStr;
			}else{
				throw new OBException("Failed to initialize cURL");
			}
#else
			(void)input;
			throw new OBException("No cURL support.");
#endif
		}

		std::string HttpService::UrlDecode(std::string input){
#if HAVE_CURL
			CURL* curl;

			curl = curl_easy_init();
			if(curl){
				char* decoded = curl_easy_unescape(curl, input.c_str(), 0, NULL);

				std::string retStr = std::string(decoded);

				curl_free(decoded);
				curl_easy_cleanup(curl);

				return retStr;
			}else{
				throw new OBException("Failed to initialize cURL");
			}
#else
			(void)input;
			throw new OBException("No cURL support.");
#endif
		}

		std::string HttpService::GenerateGUID(bool wrapInCurlyBraces){
			std::string guidString;
				
#ifdef _MSC_VER
			//Somebody that actually uses Windows is going to have to confirm this
			GUID guid;
			CoCreateGuid(&guid);

		    char guidString[100];
			snprintf(guidString, 100, "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
					 guid.Data1, guid.Data2, guid.Data3,
					 guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
					 guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

			guidString = std::string(guidString);
#elif HAVE_UUID // Most Unix-like
			uuid_t guid;
			uuid_generate_random(guid);

			char guidString[36];
			uuid_unparse_lower(guid, guidString);

			guidString = std::string(guidString);
#endif

			if(guid.empty()){
				throw new OBException("Failed to generate a GUID.");
			}

		    if(wrapInCurlyBraces){
				return "{" + guidString + "}";
			}else{
				return guidString;
			}
		}
	}
}
