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

CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::utils::OpenwireStringSupportTest );

#include <activemq/connector/openwire/utils/OpenwireStringSupport.h>

#include <activemq/io/ByteArrayInputStream.h>
#include <activemq/io/ByteArrayOutputStream.h>
#include <activemq/io/DataInputStream.h>
#include <activemq/io/DataOutputStream.h>

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::utils;

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
    bytesIn.setByteArray( bytesOut.getByteArray(), bytesOut.getByteArraySize() );

    string resultStr = OpenwireStringSupport::readString( dataIn );
    
    CPPUNIT_ASSERT( testStr == resultStr );
}
