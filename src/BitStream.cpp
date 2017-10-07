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

#include "OBEngine.h"
#include "OBException.h"
#include "instance/DataModel.h"

#include <sstream>

#include "type/UDim2.h"
#include "type/UDim.h"
#include "type/Color3.h"
#include "type/Vector3.h"
#include "type/Vector2.h"

#include "type/Event.h"
#include "type/EventConnection.h"
#include "type/LuaEnum.h"
#include "type/LuaEnumItem.h"

#ifndef _WIN32
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

namespace OB{
	BitStream::BitStream() : BitStream(1){}

	BitStream::BitStream(int bytesToAlloc){
		numberBitsUsed = 0;
		readOffset = 0;

		if(bytesToAlloc > 0){
			_data = (unsigned char*)malloc(bytesToAlloc);
			numberBitsAlloc = bytesToAlloc << 3;
		}else{
			_data = NULL;
			numberBitsAlloc = 0;
		}

		_copyData = true;
	}

	BitStream::BitStream(unsigned char* data, unsigned int lenBytes, bool _copyData){
		numberBitsUsed = lenBytes << 3;
		readOffset = 0;
		this->_copyData = _copyData;
		numberBitsAlloc = lenBytes << 3;

		if(_copyData){
			if(lenBytes > 0){
				_data = (unsigned char*)malloc(lenBytes);
				if(data){
					memcpy(_data, data, lenBytes);
				}
			}else{
				_data = NULL;
			}
		}else{
			_data = data;
		}
	}

	BitStream::~BitStream(){
		if(_copyData && _data){
			free(_data);
		}
	}

	BitStream::BitStream(const BitStream &bs){
		throw new OBException("BitStream does not work by copy constructor.");
	}

	void BitStream::reset(){
		numberBitsUsed = 0;
		readOffset = 0;
	}

	void BitStream::_addBits(uint32_t numBits){
		uint32_t newBitsAlloc = numBits + numberBitsUsed;

		if(newBitsAlloc > 0 && ((numberBitsAlloc - 1) >> 3) < ((newBitsAlloc - 1) >> 3)){
			newBitsAlloc *= 2;
			if(newBitsAlloc - (numBits + numberBitsUsed) > 1048576){
				newBitsAlloc = numBits + numberBitsUsed + 1048576;
			}

			uint32_t amountToAlloc = BITS_TO_BYTES(newBitsAlloc) + 1;
			_data = (unsigned char*)realloc(_data, (size_t)amountToAlloc);
		}

		if(newBitsAlloc > numberBitsAlloc){
			numberBitsAlloc = newBitsAlloc;
		}
	}

	uint32_t BitStream::getNumBitsAlloc(){
		return numberBitsAlloc;
	}

	uint32_t BitStream::getNumBitsUsed(){
		return numberBitsUsed;
	}

	uint32_t BitStream::getReadOffset(){
		return readOffset;
	}

	void BitStream::padToByteLength(unsigned int bytes){
		if(getNumBytesUsed() < bytes){
			alignWriteToByteBoundary();
			unsigned int numToWrite = bytes - getNumBytesUsed();
			_addBits(BYTES_TO_BITS(numToWrite));
			memset(_data + BITS_TO_BYTES(numberBitsUsed), 0, (size_t)numToWrite);
			numberBitsUsed += BYTES_TO_BITS(numToWrite);
		}
	}

	void BitStream::ignoreBits(uint32_t numBits){
		readOffset += numBits;
	}

	void BitStream::ignoreBytes(unsigned int numBytes){
		ignoreBits(BYTES_TO_BITS(numBytes));
	}

	void BitStream::setWriteOffset(uint32_t offset){
		numberBitsUsed = offset;
	}

	bool BitStream::isNetOrderInternal(){
		static unsigned long htonlVal = htonl(12345);
		return htonlVal == 12345;
	}

	void BitStream::write0(){
		_addBits(1);

		if((numberBitsUsed & 7) == 0){
			_data[numberBitsUsed >> 3] = 0;
		}

		numberBitsUsed++;
	}

	void BitStream::write1(){
		_addBits(1);

		uint32_t numberBitsMod8 = numberBitsUsed & 7;

		if(numberBitsMod8 == 0){
			_data[numberBitsUsed >> 3] = 0x80;
		}else{
			_data[numberBitsUsed >> 3] |= 0x80 >> (numberBitsMod8);
		}

		numberBitsUsed++;
	}

	bool BitStream::readBit(){
		bool result = (_data[readOffset >> 3] & (0x80 >> (readOffset & 7))) != 0;
		readOffset++;
		return result;
	}

	unsigned char* BitStream::getData(){
		return _data;
	}

	unsigned char* BitStream::copyData(){
		size_t dataLen = getNumBytesUsed();

		unsigned char* dataCopy = (unsigned char*)malloc(dataLen);
		if(dataCopy){
			memcpy(dataCopy, _data, dataLen);
		}
		return dataCopy;
	}

	void BitStream::writeBits(unsigned char* inByteArray, uint32_t numToWrite, bool rightAligned){
		_addBits(numToWrite);

		uint32_t numBitsUsedMod8 = numberBitsUsed & 7;

		/*
		  if(numBitsUsedMod8 == 0 && (numToWrite & 7) == 0){
		  printf("numBitsUsed: %i\n", numberBitsUsed);
		  printf("numBitsAlloc: %i\n", numberBitsAlloc);
		  printf("Setting at: %i\n", numberBitsUsed >> 3);
		  printf("ToWrite: %i\n", numToWrite >> 3);
		  memcpy(&_data[numberBitsUsed >> 3], inByteArray, numToWrite >> 3);
		  numberBitsUsed += numToWrite;
		  return;
		  }*/

		unsigned char dataByte;
		unsigned char* inPtr = inByteArray;

		while(numToWrite > 0){
			dataByte = *(inPtr++);

			if(numToWrite < 8 && rightAligned){
				dataByte <<= 8 - numToWrite;
			}

			if(numBitsUsedMod8 == 0){
				*(&_data[numberBitsUsed >> 3]) = dataByte;
			}else{
				*(&_data[numberBitsUsed >> 3]) |= dataByte >> (numBitsUsedMod8);

				if(8 - (numBitsUsedMod8) < 8 && 8 - (numBitsUsedMod8) < numToWrite){
					*(&_data[(numberBitsUsed >> 3) + 1]) = (unsigned char)(dataByte << (8 - (numBitsUsedMod8)));
				}
			}

			if(numToWrite >= 8){
				numberBitsUsed += 8;
				numToWrite -= 8;
			}else{
				numberBitsUsed += numToWrite;
				numToWrite = 0;
			}
		}
	}

	void BitStream::write(char* inByteArray, unsigned int numToWrite){
		if(numToWrite == 0){
			return;
		}

		if((numberBitsUsed & 7) == 0){
			_addBits(BYTES_TO_BITS(numToWrite));
			memcpy(&_data[BITS_TO_BYTES(numberBitsUsed)], inByteArray, (size_t)numToWrite);
			numberBitsUsed += BYTES_TO_BITS(numToWrite);
		}else{
			writeBits((unsigned char*)inByteArray, numToWrite * 8, true);
		}
	}

	bool BitStream::read(char* outByteArray, unsigned int numToRead){
		if((readOffset & 7) == 0){
			if(readOffset + (numToRead << 3) > numberBitsUsed){
				return false;
			}

			memcpy(outByteArray, &_data[readOffset >> 3], (size_t)numToRead);

			readOffset += numToRead << 3;
			return true;
		}else{
			return readBits((unsigned char*)outByteArray, numToRead * 8);
		}
	}

	void BitStream::writeAlignedBytes(unsigned char* inByteArray, unsigned int numToWrite){
		alignWriteToByteBoundary();
		write((char*)inByteArray, numToWrite);
	}

	bool BitStream::readAlignedBytes(unsigned char* outByteArray, unsigned int numToRead){
		if(numToRead <= 0){
			return false;
		}

		alignReadToByteBoundary();

		if(readOffset + (numToRead << 3) > numberBitsUsed){
			return false;
		}

		memcpy(outByteArray, _data + (readOffset >> 3), (size_t)numToRead);

		readOffset += numToRead << 3;

		return true;
	}

	bool BitStream::readBits(unsigned char* outByteArray, uint32_t numToRead, bool rightAligned){
		if(numToRead <= 0){
			return false;
		}

		if(readOffset + numToRead > numberBitsUsed){
			return false;
		}

		uint32_t readOffsetMod8 = readOffset & 7;

		if(readOffsetMod8 == 0 && (numToRead & 7) == 0){
			memcpy(outByteArray, &_data[readOffset >> 3], numToRead >> 3);
			readOffset += numToRead;
			return true;
		}

		uint32_t offset = 0;
		//memset(outByteArray, 0, (size_t)BITS_TO_BYTES(numToRead));

		while(numToRead > 0){
			*(outByteArray + offset) |= *(_data + (readOffset >> 3)) << (readOffsetMod8);

			if(readOffsetMod8 > 0 && numToRead > 8 - (readOffsetMod8)){
				*(outByteArray + offset) |= *(_data + (readOffset > 3) + 1) >> (8 - (readOffsetMod8));
			}

			if(numToRead >= 8){
				numToRead -= 8;
				readOffset += 8;
				offset++;
			}else{
				int neg = (int)numToRead - 8;
				if(neg < 0){
					if(rightAligned){
						*(outByteArray + offset) >>= -neg;
					}

					readOffset += 8 + neg;
				}else{
					readOffset += 8;
				}

				offset++;

				numToRead = 0;
			}
		}
		return true;
	}

	void BitStream::reverseBytes(unsigned char* inByteArray, unsigned char* outByteArray, unsigned int len){
		for(uint32_t i = 0; i < len; i++){
			outByteArray[i] = inByteArray[len - i - 1];
		}
	}

	void BitStream::reverseBytesInPlace(unsigned char* inByteArray, unsigned int len){
		unsigned char temp;
		uint32_t i;
		for(i = 0; i < (len >> 1); i++){
			temp = inByteArray[i];
			inByteArray[i] = inByteArray[len - i - 1];
			inByteArray[len - i - 1] = temp;
		}
	}

	void BitStream::writeCString(char* cstr){
		unsigned short len = strlen(cstr);

		writeSizeT(len);
		writeAlignedBytes((unsigned char*)cstr, len);
	}

	void BitStream::writeString(std::string str){
		writeCString((char*)str.c_str());
	}

	char* BitStream::readCString(){
		unsigned short len = readSizeT();

		char* cstr = (char*)malloc(len + 1);
		readAlignedBytes((unsigned char*)cstr, len);
		cstr[len] = '\0';
		return cstr;
	}

	std::string BitStream::readString(){
		char* cstr = readCString();
		if(!cstr){
			return "";
		}
		std::string str(cstr);
		free(cstr);
		return str;
	}

	void BitStream::writeVar(shared_ptr<Type::VarWrapper> var){
		size_t var_type = var->type;
		if(var){
			switch(var_type){
				case Type::TYPE_INT: {
					writeSizeT(var_type);
					writeInt(static_cast<Type::IntWrapper*>(var->wrapped)->val);
					break;
				}
				case Type::TYPE_DOUBLE: {
					writeSizeT(var_type);
					writeDouble(static_cast<Type::DoubleWrapper*>(var->wrapped)->val);
					break;
				}
				case Type::TYPE_FLOAT: {
					writeSizeT(var_type);
					writeFloat(static_cast<Type::FloatWrapper*>(var->wrapped)->val);
					break;
				}
				case Type::TYPE_LONG: {
					writeSizeT(var_type);
					writeLong(static_cast<Type::LongWrapper*>(var->wrapped)->val);
					break;
				}
				case Type::TYPE_UNSIGNED_LONG: {
					writeSizeT(var_type);
					writeULong(static_cast<Type::UnsignedLongWrapper*>(var->wrapped)->val);
					break;
				}
				case Type::TYPE_BOOL: {
					writeSizeT(var_type);
					writeBool(static_cast<Type::BoolWrapper*>(var->wrapped)->val);
					break;
				}
				case Type::TYPE_STRING: {
					writeSizeT(var_type);
					writeString(static_cast<Type::StringWrapper*>(var->wrapped)->val);
					break;
				}
				case Type::TYPE_INSTANCE: {
					writeSizeT(var_type);

					shared_ptr<Instance::Instance> inst = *static_cast<shared_ptr<Instance::Instance>*>(var->wrapped);
					ob_uint64 netId = inst->GetNetworkID();
					writeUInt64(netId);

					break;
				}
				case Type::TYPE_TYPE: {
					writeSizeT(var_type);

					shared_ptr<Type::Type> typ = *static_cast<shared_ptr<Type::Type>*>(var->wrapped);
					if(typ){
						std::string typName = typ->getClassName();

						if(typName == "UDim2"){
							writeSizeT(OB_NET_TYPE_UDIM2);
							writeUDim2(dynamic_pointer_cast<Type::UDim2>(typ));
						}else if(typName == "UDim"){
							writeSizeT(OB_NET_TYPE_UDIM);
							writeUDim(dynamic_pointer_cast<Type::UDim>(typ));
						}else if(typName == "Color3"){
							writeSizeT(OB_NET_TYPE_COLOR3);
							writeColor3(dynamic_pointer_cast<Type::Color3>(typ));
						}else if(typName == "Vector3"){
							writeSizeT(OB_NET_TYPE_VECTOR3);
							writeVector3(dynamic_pointer_cast<Type::Vector3>(typ));
						}else if(typName == "Vector2"){
							writeSizeT(OB_NET_TYPE_VECTOR2);
							writeVector2(dynamic_pointer_cast<Type::Vector2>(typ));
						}else if(typName == "LuaEnum"){
							writeSizeT(OB_NET_TYPE_LUAENUM);
							writeLuaEnum(dynamic_pointer_cast<Type::LuaEnum>(typ));
						}else if(typName == "LuaEnumItem"){
							writeSizeT(OB_NET_TYPE_LUAENUMITEM);
							writeLuaEnumItem(dynamic_pointer_cast<Type::LuaEnumItem>(typ));
						}
					}else{
						writeSizeT(1);
					}

					break;
				}
				case Type::TYPE_LUA_OBJECT:
				case Type::TYPE_NULL:
				case Type::TYPE_UNKNOWN: {
					writeSizeT(Type::TYPE_NULL);
					break;
				}
			}
		}else{
			writeSizeT(Type::TYPE_NULL);
		}
	}

	shared_ptr<Type::VarWrapper> BitStream::readVar(OBEngine* eng){
		size_t var_type = readSizeT();

		switch(var_type){
			case Type::TYPE_INT: {
				return make_shared<Type::VarWrapper>(readInt());
			}
			case Type::TYPE_DOUBLE: {
				return make_shared<Type::VarWrapper>(readDouble());
			}
			case Type::TYPE_FLOAT: {
				return make_shared<Type::VarWrapper>(readFloat());
			}
			case Type::TYPE_LONG: {
				return make_shared<Type::VarWrapper>(readLong());
			}
			case Type::TYPE_UNSIGNED_LONG: {
				return make_shared<Type::VarWrapper>(readULong());
			}
			case Type::TYPE_BOOL: {
				return make_shared<Type::VarWrapper>(readBool());
			}
			case Type::TYPE_STRING: {
				return make_shared<Type::VarWrapper>(readString());
			}
			case Type::TYPE_INSTANCE: {
				ob_uint64 netId = readUInt64();

				shared_ptr<Instance::DataModel> dm = eng->getDataModel();
				if(dm){
					weak_ptr<Instance::Instance> weakInst = dm->lookupInstance(netId);
					if(!weakInst.expired()){
						return make_shared<Type::VarWrapper>(weakInst.lock());
					}
				}

				return make_shared<Type::VarWrapper>(shared_ptr<Instance::Instance>(NULL));
			}
			case Type::TYPE_TYPE: {
				size_t typeType = readSizeT();

				switch(typeType){
					case OB_NET_TYPE_COLOR3: {
						return make_shared<Type::VarWrapper>(readColor3());
					}
					case OB_NET_TYPE_VECTOR3: {
						return make_shared<Type::VarWrapper>(readVector3());
					}
					case OB_NET_TYPE_VECTOR2: {
						return make_shared<Type::VarWrapper>(readVector3());
					}
					case OB_NET_TYPE_LUAENUM: {
						return make_shared<Type::VarWrapper>(readLuaEnum());
					}
					case OB_NET_TYPE_LUAENUMITEM: {
						return make_shared<Type::VarWrapper>(readLuaEnumItem());
					}
				}

				return make_shared<Type::VarWrapper>();
			}
			case Type::TYPE_LUA_OBJECT:
			case Type::TYPE_NULL:
			case Type::TYPE_UNKNOWN: {
				return make_shared<Type::VarWrapper>();
			}
		}
		return make_shared<Type::VarWrapper>();
	}

	void BitStream::writeUDim2(shared_ptr<Type::UDim2> var){
		if(var){
			shared_ptr<Type::UDim> x = var->getX();
			shared_ptr<Type::UDim> y = var->getY();
			writeDouble(x->getScale());
			writeDouble(x->getOffset());
			writeDouble(y->getScale());
			writeDouble(y->getOffset());
		}else{
			writeDouble(0);
			writeDouble(0);
			writeDouble(0);
			writeDouble(0);
		}
	}

	shared_ptr<Type::UDim2> BitStream::readUDim2(){
		double xScale = readDouble();
		double xOffset = readDouble();
		double yScale = readDouble();
		double yOffset = readDouble();

		return make_shared<Type::UDim2>(xScale, xOffset, yScale, yOffset);
	}

	void BitStream::writeUDim(shared_ptr<Type::UDim> var){
		if(var){
			writeDouble(var->getScale());
			writeDouble(var->getOffset());
		}else{
			writeDouble(0);
			writeDouble(0);
		}
	}

	shared_ptr<Type::UDim> BitStream::readUDim(){
		double scale = readDouble();
		double offset = readDouble();

		return make_shared<Type::UDim>(scale, offset);
	}

	void BitStream::writeColor3(shared_ptr<Type::Color3> var){
		if(var){
			writeInt(var->getRi());
			writeInt(var->getGi());
			writeInt(var->getBi());
		}else{
			writeInt(0);
			writeInt(0);
			writeInt(0);
		}
	}

	shared_ptr<Type::Color3> BitStream::readColor3(){
		int r = readInt();
		int g = readInt();
		int b = readInt();

		return make_shared<Type::Color3>(r, g, b);
	}

	void BitStream::writeVector3(shared_ptr<Type::Vector3> var){
		if(var){
			writeDouble(var->getX());
			writeDouble(var->getY());
			writeDouble(var->getZ());
		}else{
			writeDouble(0);
			writeDouble(0);
			writeDouble(0);
		}
	}

	shared_ptr<Type::Vector3> BitStream::readVector3(){
		double x = readDouble();
		double y = readDouble();
		double z = readDouble();

		return make_shared<Type::Vector3>(x, y, z);
	}

	void BitStream::writeVector2(shared_ptr<Type::Vector2> var){
		if(var){
			writeDouble(var->getX());
			writeDouble(var->getY());
		}else{
			writeDouble(0);
			writeDouble(0);
		}
	}

	shared_ptr<Type::Vector2> BitStream::readVector2(){
		double x = readDouble();
		double y = readDouble();

		return make_shared<Type::Vector2>(x, y);
	}

	void BitStream::writeLuaEnum(shared_ptr<Type::LuaEnum> var){
		if(var){
			writeString(var->getType());
		}else{
			writeString(" ");// Prevents an error, and lets the other end detect this error
		}
	}

	shared_ptr<Type::LuaEnum> BitStream::readLuaEnum(){
		if(!Type::LuaEnum::enums){
			return NULL;
		}

		std::string enumType = readString();
		if(enumType.length() > 0 && enumType != " "){
			return Type::LuaEnum::enums->at(enumType);
		}
		return NULL;
	}

	void BitStream::writeLuaEnumItem(shared_ptr<Type::LuaEnumItem> var){
		if(var){
			writeString(var->getType());
			writeString(var->getName());
		}else{
			writeString(" ");// Prevents an error, and lets the other end detect this error
			writeString(" ");
		}
	}

	shared_ptr<Type::LuaEnumItem> BitStream::readLuaEnumItem(){
		if(!Type::LuaEnum::enums){
			return NULL;
		}

		std::string enumType = readString();
		std::string enumName = readString();
		if(enumType.length() > 0 && enumType != " " && enumName.length() > 0 && enumName != " "){
			shared_ptr<Type::LuaEnum> lEn = Type::LuaEnum::enums->at(enumType);
			if(lEn){
				return lEn->enumValues.at(enumName);
			}
		}
		return NULL;
	}

	size_t BitStream::readSizeT(){
		size_t outVal = 0;

		const size_t sot = sizeof(size_t);

		if(!readAlignedBytes((unsigned char*)&outVal, sot)){
			outVal = 0;
		}

		return outVal;
	}

	int BitStream::readInt(){
		int outVal = 0;

		const size_t sot = sizeof(int);

		if(!readAlignedBytes((unsigned char*)&outVal, sot)){
			outVal = 0;
		}

		return outVal;
	}

	double BitStream::readDouble(){
		double outVal = 0;

		const size_t sot = sizeof(double);

		if(!readAlignedBytes((unsigned char*)&outVal, sot)){
			outVal = 0;
		}

		return outVal;
	}

	float BitStream::readFloat(){
		float outVal = 0;

		const size_t sot = sizeof(float);

		if(!readAlignedBytes((unsigned char*)&outVal, sot)){
			outVal = 0;
		}

		return outVal;
	}

	long BitStream::readLong(){
		long outVal = 0;

		const size_t sot = sizeof(long);

		if(!readAlignedBytes((unsigned char*)&outVal, sot)){
			outVal = 0;
		}

		return outVal;
	}

	unsigned long BitStream::readULong(){
		unsigned long outVal = 0;

		const size_t sot = sizeof(unsigned long);

		if(!readAlignedBytes((unsigned char*)&outVal, sot)){
			outVal = 0;
		}

		return outVal;
	}

	ob_uint64 BitStream::readUInt64(){
		ob_uint64 outVal = 0;

		const size_t sot = sizeof(ob_uint64);

		if(!readAlignedBytes((unsigned char*)&outVal, sot)){
			outVal = 0;
		}

		return outVal;
	}

	ob_int64 BitStream::readInt64(){
		ob_int64 outVal = 0;

		const size_t sot = sizeof(ob_int64);

		if(!readAlignedBytes((unsigned char*)&outVal, sot)){
			outVal = 0;
		}

		return outVal;
	}

	bool BitStream::readBool(){
		return readBit();
	}

	void BitStream::writeSizeT(size_t val){
		const size_t sot = sizeof(size_t);

		writeAlignedBytes((unsigned char*)&val, sot);
	}

	void BitStream::writeInt(int val){
		const size_t sot = sizeof(int);

		writeAlignedBytes((unsigned char*)&val, sot);
	}

	void BitStream::writeDouble(double val){
		const size_t sot = sizeof(double);

		writeAlignedBytes((unsigned char*)&val, sot);
	}

	void BitStream::writeFloat(float val){
		const size_t sot = sizeof(float);

		writeAlignedBytes((unsigned char*)&val, sot);
	}

	void BitStream::writeLong(long val){
		const size_t sot = sizeof(long);

		writeAlignedBytes((unsigned char*)&val, sot);
	}

	void BitStream::writeULong(unsigned long val){
		const size_t sot = sizeof(unsigned long);

		writeAlignedBytes((unsigned char*)&val, sot);
	}

	void BitStream::writeUInt64(ob_uint64 val){
		const size_t sot = sizeof(ob_uint64);

		writeAlignedBytes((unsigned char*)&val, sot);
	}

	void BitStream::writeInt64(ob_int64 val){
		const size_t sot = sizeof(ob_int64);

		writeAlignedBytes((unsigned char*)&val, sot);
	}

	void BitStream::writeBool(bool val){
		if(val){
			write1();
		}else{
			write0();
		}
	}
}
