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

#include "BitStream.h"

namespace OB{
	BitStream::BitStream(unsigned char* data, size_t length){
		this->data = data;
		this->length = length;
		idx = 0;
	}

	BitStream::~BitStream(){
		if(data){
			free(data);
		}
	}

	unsigned char* BitStream::getData(){
		return data;
	}
	
	size_t BitStream::getLength(){
		return length;
	}

	size_t BitStream::getIndex(){
		return idx;
	}

	void BitStream::setIndex(size_t idx){
	    this->idx = idx;
	}

    size_t BitStream::write(unsigned char* dat, size_t size){
		if(!dat || size == 0){
			return 0;
		}
		
	    unsigned char* tmpDat = (unsigned char*)realloc(dat, length + size);
		if(tmpDat){
			data = tmpDat;

			memcpy(data + idx, dat, size);
			idx += size;
			length += size;
		}else{
			return -1;
		}
	}

	size_t BitStream::writeStream(BitStream* stream, size_t size){
		if(!stream){
			return 0;
		}

		if(size == 0){
			size = stream->getLength();
		}

		return write(stream->getData(), size);
	}

	unsigned char* BitStream::read(size_t size){
		if(!data || length == 0 || idx + size > length){
			return NULL;
		}
		
		idx += size;
		return data + idx;
	}

	size_t BitStream::writeInt(int var){
		return write((unsigned char*)&var, sizeof(int));
	}

	int BitStream::readInt(){
		unsigned char* intData = read(sizeof(int));
		if(intData){
			return (int)(*intData);
		}else{
		    throw new OBException("No data returned from stream");
		}
	}

	size_t BitStream::writeUInt(unsigned int var){
		return write((unsigned char*)&var, sizeof(unsigned int));
	}

	unsigned int BitStream::readUInt(){
		unsigned char* intData = read(sizeof(unsigned int));
		if(intData){
			return (unsigned int)(*intData);
		}else{
		    throw new OBException("No data returned from stream");
		}
	}

	size_t BitStream::writeInt64(ob_int64 var){
		return write((unsigned char*)&var, sizeof(ob_int64));
	}

	ob_int64 BitStream::readInt64(){
		unsigned char* intData = read(sizeof(ob_int64));
		if(intData){
			return (ob_int64)(*intData);
		}else{
		    throw new OBException("No data returned from stream");
		}
	}

	size_t BitStream::writeUInt64(ob_uint64 var){
		return write((unsigned char*)&var, sizeof(ob_uint64));
	}

	ob_uint64 BitStream::readUInt64(){
		unsigned char* intData = read(sizeof(ob_uint64));
		if(intData){
			return (ob_uint64)(*intData);
		}else{
		    throw new OBException("No data returned from stream");
		}
	}

	size_t BitStream::writeCString(char* var, size_t size){
		if(size == -1){
			size = strlen(var);
		}
		
		return write((unsigned char*)var, sizeof(*var));
	}

    char* BitStream::readCString(){
		size_t strLen = readSizeT();
		
		unsigned char* strData = read(strLen);
		if(strData){
			char* newDat = (char*)malloc(strLen + 1);
			if(!newDat){
				throw new OBException("Failed to allocate C string");
			}
			memcpy(newDat, strData, strLen);
			newDat[strLen] = '\0';
			return newDat;
		}else{
		    throw new OBException("No data returned from stream");
		}
	}

	size_t BitStream::writeString(std::string var){
		size_t strLen = var.length();
		
		return writeCString((char*)var.c_str(), strLen);
	}

    std::string BitStream::readString(){
		char* strDat = readCString();

		std::string tstr = std::string(strDat);
		free(strDat);

		return tstr;
	}
}
