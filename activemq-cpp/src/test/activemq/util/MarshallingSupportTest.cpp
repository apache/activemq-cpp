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

#include "MarshallingSupportTest.h"

#include <activemq/util/MarshallingSupport.h>
#include <activemq/util/PrimitiveValueNode.h>

#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Short.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace activemq;
using namespace activemq::util;

////////////////////////////////////////////////////////////////////////////////
MarshallingSupportTest::MarshallingSupportTest() {
}

////////////////////////////////////////////////////////////////////////////////
MarshallingSupportTest::~MarshallingSupportTest() {
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupportTest::writeTestHelper( unsigned char* input, int inputLength,
                                              unsigned char* expect, int expectLength ) {

    std::string testStr( (char*)input, inputLength );
    std::string result = MarshallingSupport::asciiToModifiedUtf8( testStr );

    for( int i = 0; i < expectLength; ++i ) {
        CPPUNIT_ASSERT( (unsigned char)result[i] == expect[i] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupportTest::testAsciiToModifiedUtf8() {

    // Test data with 1-byte UTF8 encoding.
    {
        unsigned char input[] = {0x00, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
        unsigned char expect[] = {0xC0, 0x80, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};

        writeTestHelper( input, sizeof(input)/sizeof(unsigned char),
                         expect, sizeof(expect)/sizeof(unsigned char) );
    }

    // Test data with 2-byte UT8 encoding.
    {
        unsigned char input[] = {0x00, 0xC2, 0xA9, 0xC3, 0xA6 };
        unsigned char expect[] = {0xC0, 0x80, 0xC3, 0x82, 0xC2, 0xA9, 0xC3, 0x83, 0xC2, 0xA6 };
        writeTestHelper( input, sizeof(input)/sizeof(unsigned char),
                         expect, sizeof(expect)/sizeof(unsigned char)  );
    }

    // Test data with 1-byte and 2-byte encoding with embedded NULL's.
    {
        unsigned char input[] = {0x00, 0x04, 0xC2, 0xA9, 0xC3, 0x00, 0xA6 };
        unsigned char expect[] = {0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xA9, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };

        writeTestHelper( input, sizeof(input)/sizeof(unsigned char),
                         expect, sizeof(expect)/sizeof(unsigned char) );
    }

}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupportTest::readTestHelper( unsigned char* input, int inputLength,
                                             unsigned char* expect, int expectLength ) {

    std::string inputString( (char*)input, inputLength );
    std::string result = MarshallingSupport::modifiedUtf8ToAscii( inputString );

    for( std::size_t i = 0; i < result.length(); ++i ) {
        CPPUNIT_ASSERT( (unsigned char)result[i] == expect[i] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupportTest::testModifiedUtf8ToAscii() {

    // Test data with 1-byte UTF8 encoding.
    {
        unsigned char expect[] = { 0x00, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64 };
        unsigned char input[] = { 0xC0, 0x80, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64 };

        readTestHelper( input, sizeof(input)/sizeof(unsigned char),
                        expect, sizeof(expect)/sizeof(unsigned char) );
    }

    // Test data with 2-byte UT8 encoding.
    {
        unsigned char expect[] = { 0x00, 0xC2, 0xA9, 0xC3, 0xA6 };
        unsigned char input[] = { 0xC0, 0x80, 0xC3, 0x82, 0xC2, 0xA9, 0xC3, 0x83, 0xC2, 0xA6 };
        readTestHelper( input, sizeof(input)/sizeof(unsigned char),
                        expect, sizeof(expect)/sizeof(unsigned char)  );
    }

    // Test data with 1-byte and 2-byte encoding with embedded NULL's.
    {
        unsigned char expect[] = { 0x00, 0x04, 0xC2, 0xA9, 0xC3, 0x00, 0xA6 };
        unsigned char input[] = { 0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xA9, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };

        readTestHelper( input, sizeof(input)/sizeof(unsigned char),
                        expect, sizeof(expect)/sizeof(unsigned char) );
    }

    // Test with bad UTF-8 encoding, missing 2nd byte of two byte value
    {
        unsigned char input[] = { 0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xC2, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };
        std::string inputString( (char*)input, sizeof(input)/sizeof(unsigned char) );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a UTFDataFormatException",
            MarshallingSupport::modifiedUtf8ToAscii( inputString ),
            UTFDataFormatException );
    }

    // Test with bad UTF-8 encoding, encoded value greater than 255
    {
        unsigned char input[] = { 0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xC2, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };
        std::string inputString( (char*)input, sizeof(input)/sizeof(unsigned char) );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a UTFDataFormatException",
            MarshallingSupport::modifiedUtf8ToAscii( inputString ),
            UTFDataFormatException );
    }

    // Test data with value greater than 255 in 2-byte encoding.
    {
        unsigned char input[] = { 0xC8, 0xA9, 0xC3, 0xA6};
        std::string inputString( (char*)input, sizeof(input)/sizeof(unsigned char) );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a UTFDataFormatException",
            MarshallingSupport::modifiedUtf8ToAscii( inputString ),
            UTFDataFormatException );
    }

    // Test data with value greater than 255 in 3-byte encoding.
    {
        unsigned char input[] = { 0xE8, 0xA8, 0xA9, 0xC3, 0xA6};
        std::string inputString( (char*)input, sizeof(input)/sizeof(unsigned char) );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a UTFDataFormatException",
            MarshallingSupport::modifiedUtf8ToAscii( inputString ),
            UTFDataFormatException );
    }

    // Test with three byte encode that's missing a last byte.
    {
        unsigned char input[] = { 0x00, 0x00, 0x00, 0x02, 0xE8, 0xA8};
        std::string inputString( (char*)input, sizeof(input)/sizeof(unsigned char) );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a UTFDataFormatException",
            MarshallingSupport::modifiedUtf8ToAscii( inputString ),
            UTFDataFormatException );
    }
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupportTest::testWriteString() {

    {
        ByteArrayOutputStream baos;
        ByteArrayInputStream bais;
        DataOutputStream writer( &baos );
        DataInputStream dataIn( &bais );

        MarshallingSupport::writeString( writer, "" );

        std::pair<const unsigned char*, int> array = baos.toByteArray();
        bais.setByteArray( array.first, array.second );

        CPPUNIT_ASSERT( dataIn.read() == PrimitiveValueNode::STRING_TYPE );
        CPPUNIT_ASSERT( dataIn.readShort() == 0 );

        delete [] array.first;
    }
    {
        ByteArrayOutputStream baos;
        ByteArrayInputStream bais;
        DataOutputStream writer( &baos );
        DataInputStream dataIn( &bais );

        MarshallingSupport::writeString( writer, "Hello World" );

        std::pair<const unsigned char*, int> array = baos.toByteArray();
        bais.setByteArray( array.first, array.second );

        CPPUNIT_ASSERT( dataIn.read() == PrimitiveValueNode::STRING_TYPE );
        CPPUNIT_ASSERT( dataIn.readShort() == 11 );

        delete [] array.first;
    }
    {
        ByteArrayOutputStream baos;
        ByteArrayInputStream bais;
        DataOutputStream writer( &baos );
        DataInputStream dataIn( &bais );

        MarshallingSupport::writeString( writer, std::string( Short::MAX_VALUE, 'A' ) );

        std::pair<const unsigned char*, int> array = baos.toByteArray();
        bais.setByteArray( array.first, array.second );

        CPPUNIT_ASSERT( dataIn.read() == PrimitiveValueNode::BIG_STRING_TYPE );
        CPPUNIT_ASSERT( dataIn.readInt() == Short::MAX_VALUE );

        delete [] array.first;
    }
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupportTest::testWriteString16() {

    {
        ByteArrayOutputStream baos;
        ByteArrayInputStream bais;
        DataOutputStream writer( &baos );
        DataInputStream dataIn( &bais );

        MarshallingSupport::writeString16( writer, "Hello World" );

        std::pair<const unsigned char*, int> array = baos.toByteArray();
        bais.setByteArray( array.first, array.second );

        CPPUNIT_ASSERT( dataIn.readShort() == 11 );

        delete [] array.first;
    }
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupportTest::testWriteString32() {

    {
        ByteArrayOutputStream baos;
        ByteArrayInputStream bais;
        DataOutputStream writer( &baos );
        DataInputStream dataIn( &bais );

        MarshallingSupport::writeString32( writer, "Hello World" );

        std::pair<const unsigned char*, int> array = baos.toByteArray();
        bais.setByteArray( array.first, array.second );

        CPPUNIT_ASSERT( dataIn.readInt() == 11 );

        delete [] array.first;
    }
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupportTest::testReadString16() {

    ByteArrayInputStream bytesIn;
    ByteArrayOutputStream bytesOut;

    DataInputStream dataIn( &bytesIn );
    DataOutputStream dataOut( &bytesOut );

    string testStr = "This is a test string for Openwire";

    MarshallingSupport::writeString( dataOut, testStr );

    // Move the output back to the input.
    std::pair<const unsigned char*, int> array = bytesOut.toByteArray();
    bytesIn.setByteArray( array.first, array.second );

    string resultStr = "";
    int type = dataIn.read();

    CPPUNIT_ASSERT( type == PrimitiveValueNode::STRING_TYPE );

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "Should not have thrown a message for valid String type",
        resultStr = MarshallingSupport::readString16( dataIn ) );

    CPPUNIT_ASSERT( testStr == resultStr );

    delete [] array.first;
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupportTest::testReadString32() {

    ByteArrayInputStream bytesIn;
    ByteArrayOutputStream bytesOut;

    DataInputStream dataIn( &bytesIn );
    DataOutputStream dataOut( &bytesOut );

    string testStr( (std::size_t)Short::MAX_VALUE, 'a' );

    MarshallingSupport::writeString( dataOut, testStr );

    // Move the output back to the input.
    std::pair<const unsigned char*, int> array = bytesOut.toByteArray();
    bytesIn.setByteArray( array.first, array.second );

    string resultStr = "";
    int type = dataIn.read();

    CPPUNIT_ASSERT( type == PrimitiveValueNode::BIG_STRING_TYPE );

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "Should not have thrown a message for valid Big String type",
        resultStr = MarshallingSupport::readString32( dataIn ) );

    CPPUNIT_ASSERT( testStr == resultStr );

    delete [] array.first;
}
