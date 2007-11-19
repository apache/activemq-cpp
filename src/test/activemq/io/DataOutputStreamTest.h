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
#include <string.h>
#include <stdio.h>

namespace activemq{
namespace io{

    class DataOutputStreamTest : public CppUnit::TestFixture {

      CPPUNIT_TEST_SUITE( DataOutputStreamTest );
      CPPUNIT_TEST( test );
      CPPUNIT_TEST_SUITE_END();

    public:

        virtual ~DataOutputStreamTest(){}
        virtual void setUp(){};
         virtual void tearDown(){};
        void test(){

            unsigned char byteVal = (unsigned char)'T';
            unsigned short shortVal = 5;
            unsigned int intVal = 10000;
            unsigned long long longVal1 = 1000000000;
            unsigned long long longVal2 = 65535;
            unsigned long long longVal3 = 32769;
            unsigned long long longVal4 = 202;
            float floatVal = 10.0f;
            double doubleVal = 100.0;
            unsigned char arrayVal[3] = {
                'a', 'b', 'c'
            };

            // Create the stream with the buffer we just wrote to.
            ByteArrayOutputStream myStream;
            DataOutputStream writer( &myStream );

            writer.writeByte( byteVal );
            writer.writeShort( shortVal );
            writer.writeInt( intVal );
            writer.writeLong( longVal1 );
            writer.writeLong( longVal2 );
            writer.writeLong( longVal3 );
            writer.writeLong( longVal4 );
            writer.writeFloat( floatVal );
            writer.writeDouble( doubleVal );
            writer.write( arrayVal, 3 );

            const unsigned char* buffer = myStream.getByteArray();
            int ix = 0;

            unsigned char tempByte = buffer[ix];
            CPPUNIT_ASSERT( tempByte == byteVal );
            ix += sizeof( tempByte );

            unsigned short tempShort = 0;
            memcpy( &tempShort, buffer+ix, sizeof( unsigned short ) );
            tempShort = util::Endian::byteSwap( tempShort );
            CPPUNIT_ASSERT( tempShort == shortVal );
            ix += sizeof( tempShort );

            unsigned int tempInt = 0;
            memcpy( &tempInt, buffer+ix, sizeof( unsigned int ) );
            tempInt = util::Endian::byteSwap( tempInt );
            CPPUNIT_ASSERT( tempInt == intVal );
            ix += sizeof( tempInt );

            unsigned long long tempLong1 = 0;
            memcpy( &tempLong1, buffer+ix, sizeof( unsigned long long ) );
            tempLong1 = util::Endian::byteSwap( tempLong1 );
            CPPUNIT_ASSERT( tempLong1 == longVal1 );
            ix += sizeof( tempLong1 );
            unsigned long long tempLong2 = 0;
            memcpy( &tempLong2, buffer+ix, sizeof( unsigned long long ) );
            tempLong2 = util::Endian::byteSwap( tempLong2 );
            CPPUNIT_ASSERT( tempLong2 == longVal2 );
            ix += sizeof( tempLong2 );
            unsigned long long tempLong3 = 0;
            memcpy( &tempLong3, buffer+ix, sizeof( unsigned long long ) );
            tempLong3 = util::Endian::byteSwap( tempLong3 );
            CPPUNIT_ASSERT( tempLong3 == longVal3 );
            ix += sizeof( tempLong3 );
            unsigned long long tempLong4 = 0;
            memcpy( &tempLong4, buffer+ix, sizeof( unsigned long long ) );
            tempLong4 = util::Endian::byteSwap( tempLong4 );
            CPPUNIT_ASSERT( tempLong4 == longVal4 );
            ix += sizeof( tempLong4 );

            float tempFloat = 0;
            memcpy( &tempFloat, buffer+ix, sizeof( float ) );
            tempFloat = util::Endian::byteSwap( tempFloat );
            CPPUNIT_ASSERT( tempFloat == floatVal );
            ix += sizeof( tempFloat );

            double tempDouble = 0;
            memcpy( &tempDouble, buffer+ix, sizeof( double ) );
            tempDouble = util::Endian::byteSwap( tempDouble );
            CPPUNIT_ASSERT( tempDouble == doubleVal );
            ix += sizeof( tempDouble );

        }

    };

}}

#endif /*ACTIVEMQ_IO_DATAOUTPUTSTREAMTEST_H_*/
