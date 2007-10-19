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

#include "DataInputStreamTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( activemq::io::DataInputStreamTest );

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace activemq::util;

void DataInputStreamTest::test(){

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

void DataInputStreamTest::testString() {

    std::string data1 = "This is a Test";
    std::string data2 = "of the readString method";
    std::string data3 = "This one should fail";

    std::vector<unsigned char> buffer;

    buffer.insert( buffer.begin(), data1.begin(), data1.end() );
    buffer.push_back( '\0' );
    buffer.insert( buffer.end(), data2.begin(), data2.end() );
    buffer.push_back( '\0' );
    buffer.insert( buffer.end(), data3.begin(), data3.end() );

    // Create the stream with the buffer we just wrote to.
    ByteArrayInputStream myStream( buffer );
    DataInputStream reader( &myStream );

    std::string result1 = reader.readString();
    std::string result2 = reader.readString();

    CPPUNIT_ASSERT( result1 == data1 );
    CPPUNIT_ASSERT( result2 == data2 );

    try{
        std::string result3 = reader.readString();
        CPPUNIT_ASSERT( false );
    } catch(...){
        CPPUNIT_ASSERT( true );
    }

    std::vector<unsigned char> buffer2;
    buffer2.resize( 0 );
    try{
        reader.readFully( buffer2 );
    } catch(...){
        CPPUNIT_ASSERT( false );
    }

}

void DataInputStreamTest::testUTF() {

    std::string data1 = "This is a Test";
    std::string data2 = "of the readString method";
    std::string data3 = "This one should fail";

    char sizeData[sizeof(short)] = {0};
    short tempShort = 0;

    std::vector<unsigned char> buffer;

    tempShort = util::Endian::byteSwap( ((unsigned short)data1.size()) );
    memcpy( sizeData, (char*)&tempShort, sizeof( short ) );
    buffer.insert( buffer.end(), sizeData, sizeData + sizeof(short) );
    buffer.insert( buffer.end(), data1.begin(), data1.end() );

    tempShort = util::Endian::byteSwap( ((unsigned short)data2.size()) );
    memcpy( sizeData, (char*)&tempShort, sizeof( short ) );
    buffer.insert( buffer.end(), sizeData, sizeData + sizeof(short) );
    buffer.insert( buffer.end(), data2.begin(), data2.end() );

    tempShort = util::Endian::byteSwap( (unsigned short)(data3.size() + 10 ) );
    memcpy( sizeData, (char*)&tempShort, sizeof( short ) );
    buffer.insert( buffer.end(), sizeData, sizeData + sizeof(short) );
    buffer.insert( buffer.end(), data3.begin(), data3.end() );

    // Create the stream with the buffer we just wrote to.
    ByteArrayInputStream myStream( buffer );
    DataInputStream reader( &myStream );

    std::string result1 = reader.readUTF();
    std::string result2 = reader.readUTF();

    CPPUNIT_ASSERT( result1 == data1 );
    CPPUNIT_ASSERT( result2 == data2 );

    try{
        std::string result3 = reader.readUTF();
        CPPUNIT_ASSERT( false );
    } catch(...){
        CPPUNIT_ASSERT( true );
    }
}
