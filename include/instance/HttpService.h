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

#include "instance/Instance.h"

#ifndef OB_INST_HTTPSERVICE
#define OB_INST_HTTPSERVICE

namespace OB{
	namespace Instance{
		class HttpService: public Instance{
			public:
			    HttpService(OBEngine* eng);
				virtual ~HttpService();

				virtual std::string fixedSerializedID();

				DECLARE_CLASS(HttpService);

				std::string GetAsync(std::string url, bool nocache);
				std::string PostAsync(std::string url, std::string data, int contentType);
				std::string UrlEncode(std::string input);
				std::string UrlDecode(std::string input);
				std::string GenerateGUID(bool wrapInCurlyBraces);
		};
	}
}

#endif // OB_INST_HTTPSERVICE


// Local Variables:
// mode: c++
// End:
