#include "HttpService.h"

#include <curl/curl.h>
#include <AssetLocator.h>

BEGIN_INSTANCE
	DEFINE_CLASS(HttpService, false, isDataModel, Instance);

	HttpService::HttpService() : Instance(){
		Name = ClassName;
	}

	HttpService::~HttpService(){}

	DEFINE_SERVICE(HttpService);

	/**
	 * Sends an HTTP GET and returns the response as a QString.
	 * @param QString URL
	 * @param bool nocache (Not Implemented)
	 * @returns QString
	 * @author John M. Harris, Jr.
	 */
	QString HttpService::GetAsync(QString url, bool nocache){
		Q_UNUSED(nocache)

		OpenBlox::asset_response body;
		body.size = 0;
		body.data = new char[4096];
		if(body.data == NULL){
			LOGE("[HttpService] Failed to allocate memory.");
			return NULL;
		}

		body.data[0] = '\0';

		CURL* curl;
		CURLcode res;

		curl = curl_easy_init();
		if(curl){
			curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());

			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);

			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OpenBlox::AssetLocator::write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);

			res = curl_easy_perform(curl);
			if(res != CURLE_OK){
				LOGE("[HttpService] cURL Error: %s", curl_easy_strerror(res));
			}

			curl_easy_cleanup(curl);

			return QString(body.data);
		}
		return QString::null;
	}

	/**
	 * Sends an HTTP POST and returns the response as a QString.
	 * @param QString URL
	 * @param QString data
	 * @param OpenBlox::Enum::HttpContentType content type
	 * @returns QString
	 * @author John M. Harris, Jr.
	 */
	QString HttpService::PostAsync(QString url, QString data, OpenBlox::Enum::HttpContentType content_type){
		OpenBlox::asset_response body;
		body.size = 0;
		body.data = new char[4096];
		if(body.data == NULL){
			LOGE("[HttpService] Failed to allocate memory.");
			return NULL;
		}

		body.data[0] = '\0';

		CURL* curl;
		CURLcode res;

		curl = curl_easy_init();
		if(curl){
			struct curl_slist* chunk = NULL;

			switch(content_type){
				case OpenBlox::Enum::HttpContentType::ApplicationXml:
					chunk = curl_slist_append(chunk, "Content-Type: application/xml");
					break;
				case OpenBlox::Enum::HttpContentType::ApplicationUrlEncoded:
					chunk = curl_slist_append(chunk, "Content-Type: application/x-www-form-urlencoded");
					break;
				case OpenBlox::Enum::HttpContentType::TextPlain:
					chunk = curl_slist_append(chunk, "Content-Type: text/plain");
					break;
				case OpenBlox::Enum::HttpContentType::TextXml:
					chunk = curl_slist_append(chunk, "Content-Type: text/xml");
					break;
				default:
					chunk = curl_slist_append(chunk, "Content-Type: application/json");
			}

			res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

			if(res != CURLE_OK){
				LOGE("[HttpService] cURL Error: %s", curl_easy_strerror(res));
			}

			curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.toStdString().c_str());

			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);

			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OpenBlox::AssetLocator::write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);

			res = curl_easy_perform(curl);
			if(res != CURLE_OK){
				LOGE("[HttpService] cURL Error: %s", curl_easy_strerror(res));
			}

			curl_easy_cleanup(curl);

			return QString(body.data);
		}
		return QString::null;
	}

	/**
	 * URL encodes the input string.
	 * @param QString input
	 * @returns QString
	 * @author John M. Harris, Jr.
	 */
	QString HttpService::UrlEncode(QString input){
		CURL* curl;

		curl = curl_easy_init();
		if(curl){
			char* encoded = curl_easy_escape(curl, input.toStdString().c_str(), 0);

			size_t thingLen = strlen(encoded) + 1;
			char* newGuy = new char[thingLen];
			strcpy(newGuy, encoded);
			//newGuy[thingLen] = '\0';

			curl_free(encoded);
			curl_easy_cleanup(curl);

			return QString(newGuy);
		}
		return "";
	}

	/**
	 * Decodes a URL encoded string and returns it.
	 * @param QString input
	 * @returns QString
	 * @author John M. Harris, Jr.
	 */
	QString HttpService::UrlDecode(QString input){
		CURL* curl;

		curl = curl_easy_init();
		if(curl){
			char* encoded = curl_easy_unescape(curl, input.toStdString().c_str(), 0, NULL);

			size_t thingLen = strlen(encoded) + 1;
			char* newGuy = new char[thingLen];
			strcpy(newGuy, encoded);
			//newGuy[thingLen] = '\0';

			curl_free(encoded);
			curl_easy_cleanup(curl);

			return QString(newGuy);
		}
		return "";
	}

	/**
	 * Generates a GUID and returns it as a string.
	 * @param bool wrapInCurlyBraces
	 * @returns QString
	 * @author John M. Harris, Jr.
	 */
	QString HttpService::GenerateGUID(bool wrapInCurlyBraces){
		QUuid uuid = QUuid::createUuid();

		QString uuid_str = uuid.toString();

		if(!wrapInCurlyBraces){
			uuid_str = uuid_str.mid(1, uuid_str.size()-2);
		}

		return uuid_str;
	}

	void HttpService::register_lua_methods(lua_State* L){
		luaL_Reg methods[]{
			{"GetAsync", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(HttpService* hs = dynamic_cast<HttpService*>(inst)){
					QString url = QString(luaL_checkstring(L, 2));

					if(!url.startsWith("http://") && !url.startsWith("https://")){
						return luaL_error(L, "trust check failed");
					}

					bool nocache = false;
					if(!lua_isnoneornil(L, 2)){
						if(lua_isboolean(L, 2)){
							nocache = lua_toboolean(L, 3);
						}else{
							nocache = true;
						}
					}

					QString body = hs->GetAsync(url, nocache);
					lua_pushstring(L, body.toStdString().c_str());
					return 1;
				}
				return luaL_error(L, COLONERR, "GetAsync");
			}},
			{"PostAsync", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(HttpService* hs = dynamic_cast<HttpService*>(inst)){
					QString url = QString(luaL_checkstring(L, 2));
					QString data = "";
					OpenBlox::Enum::HttpContentType cont_type = OpenBlox::Enum::HttpContentType::ApplicationJson;

					if(!lua_isnoneornil(L, 3)){
						if(lua_istable(L, 3)){
							//TODO: Convert to JSON (DigiTechs, I'm lookin' at you!)
						}else{
							data = QString(luaL_checkstring(L, 3));
						}
					}

					if(!lua_isnoneornil(L, 4)){
						OpenBlox::Enum::LuaEnumItem* val = OpenBlox::Enum::checkEnumItem(L, 4, OpenBlox::Enum::LuaHttpContentType);
						cont_type = (OpenBlox::Enum::HttpContentType)val->value;
					}

					if(!url.startsWith("http://") && !url.startsWith("https://")){
						return luaL_error(L, "trust check failed");
					}

					QString body = hs->PostAsync(url, data, cont_type);
					lua_pushstring(L, body.toStdString().c_str());
					return 1;
				}
				return luaL_error(L, COLONERR, "GetAsync");
			}},
			{"GenerateGUID", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(HttpService* hs = dynamic_cast<HttpService*>(inst)){
					bool wrapInCurlyBraces = true;
					if(!lua_isnoneornil(L, 2)){
						if(lua_isboolean(L, 2)){
							wrapInCurlyBraces = lua_toboolean(L, 2);
						}else{
							wrapInCurlyBraces = false;
						}
					}

					QString body = hs->GenerateGUID(wrapInCurlyBraces);
					lua_pushstring(L, body.toStdString().c_str());
					return 1;
				}
				return luaL_error(L, COLONERR, "GenerateGUID");
			}},
			{"UrlEncode", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(HttpService* hs = dynamic_cast<HttpService*>(inst)){
					QString inputStr = QString(luaL_checkstring(L, 2));

					QString output = hs->UrlEncode(inputStr);

					lua_pushstring(L, output.toStdString().c_str());
					return 1;
				}
				return luaL_error(L, COLONERR, "UrlEncode");
			}},
			{"UrlDecode", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(HttpService* hs = dynamic_cast<HttpService*>(inst)){
					QString inputStr = QString(luaL_checkstring(L, 2));

					QString output = hs->UrlDecode(inputStr);

					lua_pushstring(L, output.toStdString().c_str());
					return 1;
				}
				return luaL_error(L, COLONERR, "UrlDecode");
			}},
			{NULL, NULL}
		};
		luaL_setfuncs(L, methods, 0);

		Instance::register_lua_methods(L);
	}
END_INSTANCE
