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

#include "DataOutputStreamBenchmark.h"
#include <decaf/io/ByteArrayOutputStream.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;

////////////////////////////////////////////////////////////////////////////////
DataOutputStreamBenchmark::DataOutputStreamBenchmark(){
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamBenchmark::setUp(){

    for( size_t i = 0; i < 8096; ++i ) {
        testString += 'a';
    }
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStreamBenchmark::run(){

    std::vector<unsigned char> outputBuffer;
    int numRuns = 500;

    ByteArrayOutputStream bos( outputBuffer );
    DataOutputStream dos( &bos );

    for( int iy = 0; iy < numRuns * 40; ++iy ){
        dos.writeLong( 0xFF00FF00FF00FF00LL );
    }
    for( int iy = 0; iy < numRuns * 40; ++iy ){
        dos.writeInt( 312568 );
    }
    for( int iy = 0; iy < numRuns * 40; ++iy ){
        dos.writeShort( 12568 );
    }
    for( int iy = 0; iy < numRuns * 40; ++iy ){
        dos.writeUnsignedShort( 12568 );
    }
    for( int iy = 0; iy < numRuns * 40; ++iy ){
        dos.writeBoolean( true );
    }
    for( int iy = 0; iy < numRuns * 40; ++iy ){
        dos.writeDouble( 10.34235234 );
    }
    for( int iy = 0; iy < numRuns + 40; ++iy ){
        dos.writeFloat( 32.4f );
    }

    outputBuffer.clear();

    for( int iy = 0; iy < numRuns; ++iy ){
        dos.writeChars( testString );
        outputBuffer.clear();
    }
    for( int iy = 0; iy < numRuns; ++iy ){
        dos.writeBytes( testString );
        outputBuffer.clear();
    }
    for( int iy = 0; iy < numRuns; ++iy ){
        dos.writeUTF( testString );
        outputBuffer.clear();
    }

    outputBuffer.clear();
}
