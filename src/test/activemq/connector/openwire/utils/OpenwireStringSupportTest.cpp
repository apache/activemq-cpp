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
void OpenwireStringSupportTest::testHelper( unsigned char* input, int inputLength,
                                            unsigned char* output, int outputLength,
                                            bool negative ) {
    try {

        ByteArrayInputStream bytesIn;
        ByteArrayOutputStream bytesOut;

        DataInputStream dataIn( &bytesIn );
        DataOutputStream dataOut( &bytesOut );

        bytesIn.setByteArray( input, inputLength );

        string resultStr = OpenwireStringSupport::readString( dataIn );
        if( !negative ) {
            CPPUNIT_ASSERT( resultStr == std::string( (char*)output, outputLength ) );

            OpenwireStringSupport::writeString( dataOut, &resultStr );
            CPPUNIT_ASSERT( bytesOut.toString() == std::string( (char*)input, inputLength ) );
        } else {
            CPPUNIT_ASSERT( 0 );
        }

    } catch( Exception& e ) {
        CPPUNIT_ASSERT( negative );
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireStringSupportTest::test()
{
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

    // Test data with 1-byte UTF8 encoding.
    {
        unsigned char input[] = {0x00, 0x0B, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
        unsigned char output[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};

        testHelper( input, sizeof(input)/sizeof(unsigned char),
                    output, sizeof(output)/sizeof(unsigned char), false );
    }

    // Test data with 2-byte UT8 encoding.
    {
        unsigned char input[] = {0x00, 0x04, 0xC2, 0xA9, 0xC3, 0xA6};
        unsigned char output[] = {0xA9, 0xE6};
        testHelper( input, sizeof(input)/sizeof(unsigned char),
                    output, sizeof(output)/sizeof(unsigned char), false );
    }

    // Test data with value greater than 255 in 2-byte encoding.
    // Expect : IO Exception
    {
        unsigned char input[] = {0x00, 0x04, 0xC8, 0xA9, 0xC3, 0xA6};
        testHelper( input, sizeof(input)/sizeof(unsigned char), NULL, 0, true );
    }

    // Test data with value greater than 255 in 3-byte encoding.
    // Expect : IO Exception
    {
        unsigned char input[] = {0x00, 0x05, 0xE8, 0xA8, 0xA9, 0xC3, 0xA6};
        testHelper( input, sizeof(input)/sizeof(unsigned char), NULL, 0, true );
    }

    // Test data with 1-byte encoding with embedded NULL's.
    {
        unsigned char input[] = {0x00, 0x0D, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x00, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x00};
        unsigned char output[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x00, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x00};

        testHelper( input, sizeof(input)/sizeof(unsigned char),
                    output, sizeof(output)/sizeof(unsigned char), false );
    }

    // Test data with 1-byte and 2-byte encoding with embedded NULL's.
    {
        unsigned char input[] = {0x00, 0x11, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x00, 0xC2, 0xA9, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x00, 0xC3, 0xA6};
        unsigned char output[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x00, 0xA9, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x00, 0xE6};

        testHelper( input, sizeof(input)/sizeof(unsigned char),
                    output, sizeof(output)/sizeof(unsigned char), false );
    }
}
