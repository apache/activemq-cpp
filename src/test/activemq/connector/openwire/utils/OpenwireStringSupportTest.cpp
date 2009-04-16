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

#include "OpenwireStringSupportTest.h"

#include <activemq/connector/openwire/utils/OpenwireStringSupport.h>

#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::utils;

////////////////////////////////////////////////////////////////////////////////
void OpenwireStringSupportTest::writeTestHelper( unsigned char* input, int inputLength,
                                                 unsigned char* expect, int expectLength ) {

    ByteArrayOutputStream baos;
    DataOutputStream writer( &baos );

    std::string testStr( (char*)input, inputLength );
    OpenwireStringSupport::writeString( writer, &testStr );

    const unsigned char* result = baos.toByteArray();

    CPPUNIT_ASSERT( result[0] == 0x00 );
    CPPUNIT_ASSERT( result[1] == 0x00 );
    CPPUNIT_ASSERT( result[2] == 0x00 );
    CPPUNIT_ASSERT( result[3] == (unsigned char)( expectLength ) );

    for( std::size_t i = 4; i < baos.size(); ++i ) {
        CPPUNIT_ASSERT( result[i] == expect[i-4] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireStringSupportTest::testWriteString() {

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

    // Test data with 1-byte and 2-byte encoding with embedded NULL's.
    {
        ByteArrayOutputStream baos;
        ByteArrayInputStream bais;
        DataOutputStream writer( &baos );
        DataInputStream dataIn( &bais );

        OpenwireStringSupport::writeString( writer, NULL );

        bais.setByteArray( baos.toByteArray(), baos.size() );

        CPPUNIT_ASSERT( dataIn.readInt() == -1 );
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireStringSupportTest::readTestHelper( unsigned char* input, int inputLength,
                                                unsigned char* expect, int expectLength ) {

    ByteArrayInputStream myStream( input, inputLength );
    DataInputStream reader( &myStream );

    std::string result = OpenwireStringSupport::readString( reader );

    for( std::size_t i = 0; i < result.length(); ++i ) {
        CPPUNIT_ASSERT( (unsigned char)result[i] == expect[i] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireStringSupportTest::testReadString() {

    // Test data with 1-byte UTF8 encoding.
    {
        unsigned char expect[] = {0x00, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
        unsigned char input[] = { 0x00, 0x00, 0x00, 0x0E ,0xC0, 0x80, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};

        readTestHelper( input, sizeof(input)/sizeof(unsigned char),
                        expect, sizeof(expect)/sizeof(unsigned char) );
    }

    // Test data with 2-byte UT8 encoding.
    {
        unsigned char expect[] = {0x00, 0xC2, 0xA9, 0xC3, 0xA6 };
        unsigned char input[] = { 0x00, 0x00, 0x00, 0x0A, 0xC0, 0x80, 0xC3, 0x82, 0xC2, 0xA9, 0xC3, 0x83, 0xC2, 0xA6 };
        readTestHelper( input, sizeof(input)/sizeof(unsigned char),
                        expect, sizeof(expect)/sizeof(unsigned char)  );
    }

    // Test data with 1-byte and 2-byte encoding with embedded NULL's.
    {
        unsigned char expect[] = {0x00, 0x04, 0xC2, 0xA9, 0xC3, 0x00, 0xA6 };
        unsigned char input[] = { 0x00, 0x00, 0x00, 0x0D, 0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xA9, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };

        readTestHelper( input, sizeof(input)/sizeof(unsigned char),
                        expect, sizeof(expect)/sizeof(unsigned char) );
    }

    // Test with bad UTF-8 encoding, missing 2nd byte of two byte value
    {
        unsigned char input[] = { 0x00, 0x00, 0x00, 0x0D, 0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xC2, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };

        ByteArrayInputStream myStream( input, sizeof(input)/sizeof(unsigned char) );
        DataInputStream reader( &myStream );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a IOException",
            OpenwireStringSupport::readString( reader ),
            IOException );
    }

    // Test with bad UTF-8 encoding, encoded value greater than 255
    {
        unsigned char input[] = { 0x00, 0x00, 0x00, 0x0D, 0xC0, 0x80, 0x04, 0xC3, 0x82, 0xC2, 0xC2, 0xC3, 0x83, 0xC0, 0x80, 0xC2, 0xA6 };

        ByteArrayInputStream myStream( input, sizeof(input)/sizeof(unsigned char) );
        DataInputStream reader( &myStream );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a IOException",
            OpenwireStringSupport::readString( reader ),
            IOException );
    }

    // Test data with value greater than 255 in 2-byte encoding.
    {
        unsigned char input[] = { 0x00, 0x00, 0x00, 0x04, 0xC8, 0xA9, 0xC3, 0xA6};
        ByteArrayInputStream myStream( input, sizeof(input)/sizeof(unsigned char) );
        DataInputStream reader( &myStream );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a IOException",
            OpenwireStringSupport::readString( reader ),
            IOException );
    }

    // Test data with value greater than 255 in 3-byte encoding.
    {
        unsigned char input[] = { 0x00, 0x00, 0x00, 0x05, 0xE8, 0xA8, 0xA9, 0xC3, 0xA6};
        ByteArrayInputStream myStream( input, sizeof(input)/sizeof(unsigned char) );
        DataInputStream reader( &myStream );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a IOException",
            OpenwireStringSupport::readString( reader ),
            IOException );
    }

    // Test with three byte encode that's missing a last byte.
    {
        unsigned char input[] = { 0x00, 0x00, 0x00, 0x02, 0xE8, 0xA8};
        ByteArrayInputStream myStream( input, sizeof(input)/sizeof(unsigned char) );
        DataInputStream reader( &myStream );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should throw a IOException",
            OpenwireStringSupport::readString( reader ),
            IOException );
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireStringSupportTest::test() {

    ByteArrayInputStream bytesIn;
    ByteArrayOutputStream bytesOut;

    DataInputStream dataIn( &bytesIn );
    DataOutputStream dataOut( &bytesOut );

    string testStr = "This is a test string for Openwire";

    OpenwireStringSupport::writeString( dataOut, &testStr );

    // Move the output back to the input.
    bytesIn.setByteArray( bytesOut.toByteArray(), bytesOut.size() );

    string resultStr = OpenwireStringSupport::readString( dataIn );

    CPPUNIT_ASSERT( testStr == resultStr );
}
