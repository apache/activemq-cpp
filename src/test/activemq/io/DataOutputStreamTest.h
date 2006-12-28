/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ACTIVEMQ_IO_DATAOUTPUTSTREAMTEST_H_
#define ACTIVEMQ_IO_DATAOUTPUTSTREAMTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/util/Endian.h>
#include <activemq/io/DataOutputStream.h>
#include <activemq/io/ByteArrayOutputStream.h>

namespace activemq{
namespace io{
	
	class DataOutputStreamTest : public CppUnit::TestFixture {
		
	  CPPUNIT_TEST_SUITE( DataOutputStreamTest );
	  CPPUNIT_TEST( test );
	  CPPUNIT_TEST_SUITE_END();
	  
	public:
	
		virtual void setUp(){};	
	 	virtual void tearDown(){};
		void test(){
			
			unsigned char byteVal = (unsigned char)'T';
			uint16_t shortVal = 5;
			uint32_t intVal = 10000;
			uint64_t longVal = 1000000000;
			float floatVal = 10.0f;
			double doubleVal = 100.0;
			unsigned char arrayVal[3] = {
				'a', 'b', 'c'
			};
            std::string stringVal1 = "ASCII_String";
            std::string stringVal2 = "UTF8_String";
			
			// Create the stream with the buffer we just wrote to.
			ByteArrayOutputStream myStream;
			DataOutputStream writer( &myStream );
			
			writer.writeByte( byteVal );
			writer.writeShort( shortVal );
			writer.writeInt( intVal );
			writer.writeLong( longVal );
			writer.writeFloat( floatVal );
			writer.writeDouble( doubleVal );
			writer.write( arrayVal, 3 );
            writer.writeBytes( stringVal1 );
            writer.writeUTF( stringVal2 );
			
			const unsigned char* buffer = myStream.getByteArray();
			int ix = 0;
			
			unsigned char tempByte = buffer[ix];
			CPPUNIT_ASSERT( tempByte == byteVal );
			ix += sizeof( tempByte );

			uint16_t tempShort = util::Endian::byteSwap( *(uint16_t*)(buffer+ix) );
			CPPUNIT_ASSERT( tempShort == shortVal );
			ix += sizeof( tempShort );
			
			uint32_t tempInt = util::Endian::byteSwap( *(uint32_t*)(buffer+ix) );
			CPPUNIT_ASSERT( tempInt == intVal );
			ix += sizeof( tempInt );
			
			uint64_t tempLong = util::Endian::byteSwap( *(uint64_t*)(buffer+ix) );
			CPPUNIT_ASSERT( tempLong == longVal );
			ix += sizeof( tempLong );
			
			float tempFloat = util::Endian::byteSwap( *(float*)(buffer+ix) );
			CPPUNIT_ASSERT( tempFloat == floatVal );
			ix += sizeof( tempFloat );
			
			double tempDouble = util::Endian::byteSwap( *(double*)(buffer+ix) );
			CPPUNIT_ASSERT( tempDouble == doubleVal );
			ix += sizeof( tempDouble );
            
            char tempChar1 = *(char*)(buffer+ix);
            CPPUNIT_ASSERT( tempChar1 == arrayVal[0] );
            ix += sizeof( tempChar1 );
            
            char tempChar2 = *(char*)(buffer+ix);
            CPPUNIT_ASSERT( tempChar2 == arrayVal[1] );
            ix += sizeof( tempChar2 );

            char tempChar3 = *(char*)(buffer+ix);
            CPPUNIT_ASSERT( tempChar3 == arrayVal[2] );
            ix += sizeof( tempChar3 );

		}

	};
	
}}

#endif /*ACTIVEMQ_IO_DATAOUTPUTSTREAMTEST_H_*/
