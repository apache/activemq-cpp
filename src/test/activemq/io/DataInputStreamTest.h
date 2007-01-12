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

#ifndef ACTIVEMQ_IO_DATAINPUTSTREAMTEST_H_
#define ACTIVEMQ_IO_DATAINPUTSTREAMTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/util/Endian.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/io/BufferedInputStream.h>
#include <activemq/io/ByteArrayInputStream.h>
#include <activemq/io/DataInputStream.h>

#ifdef min
#undef min
#endif

#include <algorithm>

namespace activemq{
namespace io{
    
    class DataInputStreamTest : public CppUnit::TestFixture {
        
      CPPUNIT_TEST_SUITE( DataInputStreamTest );
      CPPUNIT_TEST( test );
      CPPUNIT_TEST_SUITE_END();
            
    public:
    
        virtual void setUp(){}; 
        virtual void tearDown(){};
        void test(){
            
            unsigned char buffer[30];         
            int ix = 0; 
            
            unsigned char byteVal = (unsigned char)'T';
            unsigned short shortVal = 5;
            unsigned int intVal = 10000;
            unsigned long long longVal = 1000000000;
            float floatVal = 10.0f;
            double doubleVal = 100.0;
            unsigned char arrayVal[3] = {
                'a', 'b', 'c'
            };
            
            int size = sizeof(char);
            memcpy( (char*)(buffer+ix), (char*)&byteVal, size );
            ix += size;
            
            size = sizeof(unsigned short);
            unsigned short tempShort = util::Endian::byteSwap(shortVal);
            memcpy( (char*)(buffer+ix), (char*)&tempShort, size );
            ix += size;
            
            size = sizeof(unsigned int);
            unsigned int tempInt = util::Endian::byteSwap(intVal);
            memcpy( (char*)(buffer+ix), (char*)&tempInt, size );
            ix += size;
            
            size = sizeof(unsigned long long);
            unsigned long long tempLong = util::Endian::byteSwap(longVal);
            memcpy( (char*)(buffer+ix), (char*)&tempLong, size );
            ix += size;

            size = sizeof(float);
            float tempFloat = util::Endian::byteSwap(floatVal);
            memcpy( (char*)(buffer+ix), (char*)&tempFloat, size );
            ix += size;

            size = sizeof(double);
            double tempDouble = util::Endian::byteSwap(doubleVal);
            memcpy( (char*)(buffer+ix), (char*)&tempDouble, size );          
            ix += size;
            
            size = 3;
            memcpy( (char*)(buffer+ix), (char*)&arrayVal, size );
            ix += size;

            // Create the stream with the buffer we just wrote to.
            ByteArrayInputStream myStream( buffer, 30 );
            DataInputStream reader( &myStream );
            
            byteVal = reader.readByte();
            //std::cout << "Byte Value = " << byteVal << std::endl;
            CPPUNIT_ASSERT( byteVal == (unsigned char)'T' );
            
            shortVal = reader.readShort();
            //std::cout << "short Value = " << shortVal << std::endl;
            CPPUNIT_ASSERT( shortVal == 5 );
            
            intVal = reader.readInt();
            //std::cout << "int Value = " << intVal << std::endl;
            CPPUNIT_ASSERT( intVal == 10000 );
            
            longVal = reader.readLong();
            //std::cout << "long long Value = " << longVal << std::endl;
            CPPUNIT_ASSERT( longVal == 1000000000 );
            
            floatVal = reader.readFloat();
            //std::cout << "float Value = " << floatVal << std::endl;
            CPPUNIT_ASSERT( floatVal == 10.0f );
            
            doubleVal = reader.readDouble();
            //std::cout << "double Value = " << doubleVal << std::endl;
            CPPUNIT_ASSERT( doubleVal == 100.0 );
            
            reader.read( arrayVal, 0, 3 );
            //std::cout << "char[0] Value = " << (int)arrayVal[0] << std::endl;
            CPPUNIT_ASSERT( arrayVal[0] == 'a' );
            //std::cout << "char[1] Value = " << (int)arrayVal[1] << std::endl;
            CPPUNIT_ASSERT( arrayVal[1] == 'b' );
            //std::cout << "char[2] Value = " << (int)arrayVal[2] << std::endl;
            CPPUNIT_ASSERT( arrayVal[2] == 'c' );
        }

    };
    
}}

#endif /*ACTIVEMQ_IO_DATAINPUTSTREAMTEST_H_*/
