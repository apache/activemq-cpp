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

#include "OpenWireStringSupportBenchmark.h"

#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>

#include <decaf/util/Random.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::util;
using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::utils;

////////////////////////////////////////////////////////////////////////////////
OpenWireStringSupportBenchmark::OpenWireStringSupportBenchmark() {
    this->dataIn = NULL;
    this->dataOut = NULL;
}

////////////////////////////////////////////////////////////////////////////////
OpenWireStringSupportBenchmark::~OpenWireStringSupportBenchmark() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireStringSupportBenchmark::setUp(){
    this->dataIn = new DataInputStream( &bytesIn );
    this->dataOut = new DataOutputStream( &bytesOut );

    Random randGen;
    const int bufferSize = 32876;

    for( int i = 0; i < bufferSize; ++i ) {
        this->buffer.push_back( (char)( randGen.nextInt( 254 ) + 1 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireStringSupportBenchmark::tearDown(){

    delete this->dataIn;
    delete this->dataOut;
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireStringSupportBenchmark::run() {

    string testStr = "This is a test string for Openwire";

//    OpenwireStringSupport::writeString( *dataOut, &testStr );
//
//    // Move the output back to the input.
//    bytesIn.setByteArray( bytesOut.toByteArray(), bytesOut.size() );
//
//    string resultStr = OpenwireStringSupport::readString( *dataIn );
//
//    CPPUNIT_ASSERT( testStr == resultStr );

//    this->bytesOut.reset();
    OpenwireStringSupport::writeString( *dataOut, &buffer );
    bytesIn.setByteArray( bytesOut.toByteArray(), bytesOut.size() );
    string resultStr = OpenwireStringSupport::readString( *dataIn );

}
