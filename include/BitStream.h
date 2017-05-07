/*
 * Copyright (C) 2017 John M. Harris, Jr. <johnmh@openblox.org>
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

#ifndef OB_BITSTREAM
#define OB_BITSTREAM

#include "OBException.h"
#include "obtype.h"

#include "type/VarWrapper.h"

#include <cstdlib>
#include <cstring>

namespace OB{
	/**
	 * This is a convenience class used to make crafting packets
	 * easy.
	 *
	 * @author John M. Harris, Jr.
	 * @date March 2017
	 */
	class BitStream{
		public:
		    BitStream(unsigned char* data = NULL, size_t length = 0);
			virtual ~BitStream();

			/**
			 * Returns a pointer to the current data of this
			 * BitStream.
			 *
			 * @returns data
			 * @author John M. Harris, Jr.
			 */
			unsigned char* getData();

			/**
			 * Returns the current length of this BitStream.
			 *
			 * @returns length
			 * @author John M. Harris, Jr.
			 */
			size_t getLength();

			/**
			 * Returns the current index of this BitStream.
			 *
			 * The index is the point in the allocated block that the
			 * BitStream will write to, or read from.
			 *
			 * @returns index
			 * @author John M. Harris, Jr.
			 */
			size_t getIndex();

			/**
			 * Sets the index of this BitStream.
			 *
			 * If the index is greater than the length, you will not
			 * be able to read anything, and buffer overflows will
			 * occur when writing. Be careful with this. There are
			 * no checks for this on purpose.
			 *
			 * @param idx Index
			 * @author John M. Harris, Jr.
			 */
			void setIndex(size_t idx);

			/**
			 * Writes arbitrary data to this BitStream.
			 *
			 * If dat is NULL or size is 0, this is no-op, and 0 will
			 * be returned.
			 * 
			 * @param dat Data to write
			 * @param size Size of the data to be written
			 * @returns Size written, -1 on error.
			 * @author John M. Harris, Jr.
			 */
		    size_t write(unsigned char* dat, size_t size);

			/**
			 * Convenience function to write data from one BitStream
			 * to another.
			 *
			 * One major difference between this and the write
			 * function is that if size is 0, it is assumed that
			 * all of the data from the source stream will be
			 * written to this stream.
			 *
			 * @param stream Source BitStream
			 * @param size Size of the data to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeStream(BitStream* stream, size_t size = 0);

			/**
			 * Reads arbitrary data from this BitStream.
			 * 
			 * If size is 0, or more data is requested than the stream
			 * contains, NULL is returned.
			 *
			 * @param size Size of the data to read
			 * @returns Pointer to data or NULL
			 * @author John M. Harris, Jr.
			 */
			unsigned char* read(size_t size);

			/**
			 * Convenience function to write a size_t to a stream.
			 *
			 * @param var size_t to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeSizeT(size_t var);

			/**
			 * Convenience function to read a size_t from a stream.
			 *
			 * @returns size_t
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
		    size_t readSizeT();

			/**
			 * Convenience function to write an int to a stream.
			 *
			 * @param var Integer to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeInt(int var);

			/**
			 * Convenience function to read an int from a stream.
			 *
			 * @returns Integer
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
		    int readInt();

			/**
			 * Convenience function to write an unsigned int to a
			 * stream.
			 *
			 * @param var Unsigned Integer to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeUInt(unsigned int var);

			/**
			 * Convenience function to read an int from a stream.
			 *
			 * @returns Unsigned Integer
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
		    unsigned int readUInt();

			/**
			 * Convenience function to write an ob_int64 to a stream.
			 *
			 * @param var ob_int64 to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeInt64(ob_int64 var);

			/**
			 * Convenience function to read an ob_int64 from a stream.
			 *
			 * @returns ob_int64
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
		    ob_int64 readInt64();

			/**
			 * Convenience function to write an ob_uint64 to a stream.
			 *
			 * @param var ob_uint64 to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeUInt64(ob_uint64 var);

			/**
			 * Convenience function to read an ob_uint64 from a
			 * stream.
			 *
			 * @returns ob_uint64
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
		    ob_uint64 readUInt64();

			/**
			 * Convenience function to write an C string to a
			 * stream.
			 *
			 * If size is -1, the size of the string will be
			 * detected.
			 *
			 * @param var char* to write
			 * @param size Size of the string
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeCString(char* var, size_t size = -1);

			/**
			 * Convenience function to read a C string from a
			 * stream.
			 *
			 * The data returned is malloc'd, and will need to be 
			 * later free'd.
			 *
			 * @returns Null-terminated C string
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
		    char* readCString();

			/**
			 * Convenience function to write an std::string to a
			 * stream.
			 *
			 * @param var std::string to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeString(std::string var);

			/**
			 * Convenience function to read an std::string from a
			 * stream.
			 *
			 * @returns std::string
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
			std::string readString();

			/**
			 * Convenience function to write a double to a
			 * stream.
			 *
			 * @param var double to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeDouble(double var);

			/**
			 * Convenience function to read a double from a
			 * stream.
			 *
			 * @returns double
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
			double readDouble();

			/**
			 * Convenience function to write a long to a
			 * stream.
			 *
			 * @param var long to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeLong(long var);

			/**
			 * Convenience function to read a long from a
			 * stream.
			 *
			 * @returns long
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
		    long readLong();

		    /**
			 * Convenience function to write an unsigned long to a
			 * stream.
			 *
			 * @param var unsigned long to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeULong(unsigned long var);

			/**
			 * Convenience function to read an unsigned long from a
			 * stream.
			 *
			 * @returns unsigned long
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
		    unsigned long readULong();

			/**
			 * Convenience function to write a bool to a
			 * stream.
			 *
			 * @param var bool to write
			 * @returns Same as write
			 * @author John M. Harris, Jr.
			 */
			size_t writeBool(bool var);

			/**
			 * Convenience function to read a bool from a
			 * stream.
			 *
			 * @returns bool
			 * @throws OBException
			 * @author John M. Harris, Jr.
			 */
		    bool readBool();

			/**
			 * Convenience function to read a Type::VarWrapper from a
			 * stream.
			 * 
			 * @returns shared_ptr<Type::VarWrapper> or NULL
			 * @author John M. Harris, Jr.
			 */
			shared_ptr<Type::VarWrapper> readVar();

			/**
			 * Convenience function to write a Type::VarWrapper to a
			 * stream.
			 * 
			 * @param var shared_ptr<Type::VarWrapper> or NULL
			 * @author John M. Harris, Jr.
			 */
			void writeVar(shared_ptr<Type::VarWrapper> var);
			
		private:
			unsigned char* data;
			size_t length;
			size_t idx;
	};
}

#endif // OB_BITSTREAM

// Local Variables:
// mode: c++
// End:
