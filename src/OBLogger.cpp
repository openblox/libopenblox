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

#include "OBLogger.h"

#include <iostream>

namespace OB{
	OBLogger::OBLogger(){
		logLevel = OLL_Information;
	}

	OBLogger::~OBLogger(){}

	OBLogLevel OBLogger::getLogLevel(){
		return logLevel;
	}

	void OBLogger::setLogLevel(OBLogLevel logLevel){
		this->logLevel = logLevel;
	}

	void OBLogger::log(std::string text, OBLogLevel logLevel){
		log(text, "", logLevel);
	}

	void OBLogger::log(std::string text, std::string extra, OBLogLevel logLevel){
		std::string logLevelStr = "";

		switch(logLevel){
			case OLL_Debug: {
				logLevelStr = "[DEBUG] ";
				break;
			}
			case OLL_Information: {
				logLevelStr = "[INFO] ";
				break;
			}
			case OLL_Warning: {
				logLevelStr = "[WARN] ";
				break;
			}
			case OLL_Error: {
				logLevelStr = "[ERROR ]";
				break;
			}
		}

		if(logLevel >= this->logLevel){
			if(logLevel == OLL_Error){
				if(extra.length() > 0){
					std::cerr << logLevelStr << text << " :" << extra << std::endl;
				}else{
					std::cerr << logLevelStr << text << std::endl;
				}
			}else{
				if(extra.length() > 0){
					std::cout << logLevelStr << text << " :" << extra << std::endl;
				}else{
					std::cout << logLevelStr << text << std::endl;
				}
			}
		}
	}
}
