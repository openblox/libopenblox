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

#include <string>

#ifndef OB_OBLOGGER
#define OB_OBLOGGER

namespace OB{
	/**
	 * All possible log levels. OLL_Debug means to log everything, OLL_None
	 * means to log (just about) nothing. When used to print messages,
	 * messages marked OLL_Debug have the lowest priority, while OLL_None
	 * has the highest. Messages marked OLL_None will always be printed,
	 * regardless of log level.
	 *
	 * @author John M. Harris, Jr.
	 */
	enum OBLogLevel{
		/**
		 * Used for printing information used for debugging
		 */
		OLL_Debug,
		/**
		 * Used for printing useful information such as hardware
		 * information
		 */
		OLL_Information,
		/**
		 * Used to print warnings, such as something unexpected occurring
		 */
		OLL_Warning,
		/**
		 * Used to print critical errors
		 */
		OLL_Error,
		/**
		 * Messages with this log level will always be printed.
		 * 
		 * When used as a filter, nothing other than messages with the
		 * level 'OLL_None' will be logged.
		 */
		OLL_None
	};

	/**
	 * Logging utility class for the OpenBlox engine.
	 *
	 * @author John M. Harris, Jr.
	 */
	class OBLogger{
		public:
			/**
			 * Constructor for the OBLogger singleton.
			 *
			 * @author John M. Harris, Jr.
			 */
			OBLogger();

			virtual ~OBLogger();

			/**
			 * Returns the current log level, corresponding
			 * to one of the items in the OBLogLevel enumeration
			 * class.
			 *
			 * @returns OBLogLevel log level
			 * @author John M. Harris, Jr.
			 */
			OBLogLevel getLogLevel();

			/**
			 * Sets a new log level. The default is OLL_Information.
			 *
			 * @param logLevel log level
			 * @author John M. Harris, Jr.
			 */
			void setLogLevel(OBLogLevel logLevel);

			/**
			 * Prints a message to the log.
			 *
			 * @param message Text to be printed
			 * @param logLevel Log level of this message. Defaults to OLL_Information.
			 * @author John M. Harris, Jr.
			 */
			void log(std::string message, OBLogLevel logLevel = OLL_Information);

			/**
			 * Prints a message to the log, with additional information.
			 *
			 * @param message Text to be printed
			 * @param extra Additional information, which will be printed after a " :".
			 * @param logLevel Log level of this message. Defaults to OLL_Information.
			 * @author John M. Harris, Jr.
			 */
			void log(std::string message, std::string extra, OBLogLevel logLevel = OLL_Information);

		private:
			OBLogLevel logLevel;
	};
}

#endif // OB_OBLOGGER

// Local Variables:
// mode: c++
// End:
