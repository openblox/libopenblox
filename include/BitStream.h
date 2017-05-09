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
	namespace Type{
		class Color3;
		class Vector3;
		class Vector2;
		class LuaEnum;
		class LuaEnumItem;
	}
	
	#define BITS_TO_BYTES(x) (((x)+7)>>3)
	#define BYTES_TO_BITS(x) ((x)<<3)
	
	/**
	 * This is a convenience class used to make crafting packets
	 * easy.
	 *
	 * @author John M. Harris, Jr.
	 * @date March 2017
	 */
	class BitStream{
		public:
		    BitStream();
			BitStream(int bytesToAlloc);
			BitStream(unsigned char* dat, unsigned int lenBytes, bool _copyData);
			BitStream(const BitStream &bs);
			virtual ~BitStream();

			void reset();

			void _addBits(uint32_t numBits);
			uint32_t getNumBitsAlloc();
			uint32_t getNumBitsUsed();
			uint32_t getReadOffset();
			void padToByteLength(unsigned int bytes);
			void ignoreBits(uint32_t numBits);
			void ignoreBytes(unsigned int numBytes);
			void setWriteOffset(uint32_t offset);
			bool isNetOrderInternal();

			void write0();
			void write1();
			bool readBit();

			unsigned char* getData();
			unsigned char* copyData();

			void writeBits(unsigned char* inByteArray, uint32_t numToWrite, bool rightAligned = true);
		    void write(char* inByteArray, unsigned int numToWrite);
			bool read(char* outBytearray, unsigned int numToRead);
			void writeAlignedBytes(unsigned char* inByteArray, unsigned int numToWrite);
			bool readAlignedBytes(unsigned char* outByteArray, unsigned int numToRead);
			bool readBits(unsigned char* outByteArray, uint32_t numToRead, bool rightAligned = true);
			void reverseBytes(unsigned char* inByteArray, unsigned char* outByteArray, unsigned int len);
			void reverseBytesInPlace(unsigned char* inByteArray, unsigned int len);

			void writeCString(char* str);
			void writeString(std::string str);
			char* readCString();
			std::string readString();

		    void writeVar(shared_ptr<Type::VarWrapper> var);
			shared_ptr<Type::VarWrapper> readVar();

			void writeColor3(shared_ptr<Type::Color3> var);
			shared_ptr<Type::Color3> readColor3();

			void writeVector3(shared_ptr<Type::Vector3> var);
			shared_ptr<Type::Vector3> readVector3();

			void writeVector2(shared_ptr<Type::Vector2> var);
			shared_ptr<Type::Vector2> readVector2();

			void writeLuaEnum(shared_ptr<Type::LuaEnum> var);
			shared_ptr<Type::LuaEnum> readLuaEnum();

			void writeLuaEnumItem(shared_ptr<Type::LuaEnumItem> var);
			shared_ptr<Type::LuaEnumItem> readLuaEnumItem();

			template<class T> T read(T &outVar);
			template<class T> T read();
			template<class T> void write(T data);

			inline bool doEndianSwap(){
				return !isNetOrderInternal();
			}
			
			inline void alignWriteToByteBoundary(){
				numberBitsUsed += 8 - (((numberBitsUsed - 1) & 7) + 1);
			}

			inline void alignReadToByteBoundary(){
				readOffset += 8 - (((readOffset - 1) & 7) + 1);
			}

			inline uint32_t getNumBytesUsed() const{
				return BITS_TO_BYTES(numberBitsUsed);
			}
			
		private:
		    unsigned char* _data;
			bool _copyData;
			uint32_t numberBitsUsed;
			uint32_t numberBitsAlloc;
			uint32_t readOffset;
	};

	#ifndef BITSTREAM_SPECIALIZATION
	#define BITSTREAM_SPECIALIZATION
	template<class T> inline T BitStream::read(T &outVar){
		size_t sot = sizeof(T);
		
		unsigned char output[sot];
		if(readAlignedBytes(output, sot)){
			outVar = *((T*)(output));
		}else{
			outVar = 0;
		}

		return outVar;
	}

	template<class T> inline T BitStream::read(){
		T tVar = 0;
		return read<T>(tVar);
	}
			
	template<class T> inline void BitStream::write(T data){
		size_t sot = sizeof(T);

		writeAlignedBytes((unsigned char*)&data, sot);
	}
	
	template<> inline bool BitStream::read(bool &outVar){
		return readBit();
	}

	template<> inline void BitStream::write(bool data){
		if(data){
			write1();
		}else{
			write0();
		}
	}

	template<> inline void BitStream::write(std::string data){
	    writeString(data);
	}

	template<> inline std::string BitStream::read(){
	    return readString();
	}

	template<> inline void BitStream::write(shared_ptr<Type::VarWrapper> data){
	    writeVar(data);
	}

	template<> inline shared_ptr<Type::VarWrapper> BitStream::read(){
	    return readVar();
	}

	template<> inline void BitStream::write(shared_ptr<Type::Color3> data){
	    writeColor3(data);
	}

	template<> inline shared_ptr<Type::Color3> BitStream::read(){
	    return readColor3();
	}

	template<> inline void BitStream::write(shared_ptr<Type::Vector3> data){
	    writeVector3(data);
	}

	template<> inline shared_ptr<Type::Vector3> BitStream::read(){
	    return readVector3();
	}
	
	template<> inline void BitStream::write(shared_ptr<Type::Vector2> data){
	    writeVector2(data);
	}

	template<> inline shared_ptr<Type::Vector2> BitStream::read(){
	    return readVector2();
	}

	template<> inline void BitStream::write(shared_ptr<Type::LuaEnum> data){
	    writeLuaEnum(data);
	}

	template<> inline shared_ptr<Type::LuaEnum> BitStream::read(){
	    return readLuaEnum();
	}

	template<> inline void BitStream::write(shared_ptr<Type::LuaEnumItem> data){
	    writeLuaEnumItem(data);
	}

	template<> inline shared_ptr<Type::LuaEnumItem> BitStream::read(){
	    return readLuaEnumItem();
	}
	#endif

	template<class T> BitStream& operator<<(BitStream& out, T& c){
		out.write(c);
		return out;
	}

	template<class T> BitStream& operator>>(BitStream& in, T& c){
		bool success = in.read(c);
		(void)success;
				
		return in;
	}
}

#endif // OB_BITSTREAM

// Local Variables:
// mode: c++
// End:
