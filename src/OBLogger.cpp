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

#include "OBEngine.h"
#include "instance/LogService.h"

#include <iostream>

namespace OB{
	OBLogLevel OBLogger::_logLevel;

	OBLogLevel OBLogger::getLogLevel(){
		return _logLevel;
	}

	void OBLogger::setLogLevel(OBLogLevel logLevel){
	    _logLevel = logLevel;
	}

	void OBLogger::log(std::string text, OBLogLevel logLevel){
		log(text, "", logLevel);
	}

	void OBLogger::log(std::string text, std::string extra, OBLogLevel logLevel){
		std::string logLevelStr = "";

		shared_ptr<Instance::LogService> ls;
		OBEngine* eng = OBEngine::getInstance();
		if(eng){
			shared_ptr<Instance::DataModel> dm = eng->getDataModel();
			if(dm){
				ls = dm->getLogService();
			}
		}

		std::string textMsg = text;

		if(extra.length() > 0){
		    textMsg = textMsg + " :" + extra;
		}

		if(logLevel == OLL_None){
			if(ls){
				ls->postLog(textMsg, Enum::MessageType::MessageOutput);
			}
		}

		switch(logLevel){
			case OLL_Debug: {
				logLevelStr = "[DEBUG] ";
				if(ls){
					ls->postLog(textMsg, Enum::MessageType::MessageOutput);
				}
				break;
			}
			case OLL_Information: {
				logLevelStr = "[INFO] ";
				if(ls){
					ls->postLog(textMsg, Enum::MessageType::MessageInfo);
				}
				break;
			}
			case OLL_Warning: {
				logLevelStr = "[WARN] ";
				if(ls){
					ls->postLog(textMsg, Enum::MessageType::MessageWarning);
				}
				break;
			}
			case OLL_Error: {
				logLevelStr = "[ERROR ]";
				if(ls){
					ls->postLog(textMsg, Enum::MessageType::MessageError);
				}
				break;
			}
		}

		if(logLevel >= _logLevel){
			if(logLevel == OLL_Error){
				std::cerr << logLevelStr << textMsg << std::endl;
			}else{
			    std::cout << logLevelStr << textMsg << std::endl;
			}
		}
	}
}
