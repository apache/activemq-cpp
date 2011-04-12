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

#include "ByteArrayOutputStreamBenchmark.h"

using namespace decaf;
using namespace decaf::io;

////////////////////////////////////////////////////////////////////////////////
const int ByteArrayOutputStreamBenchmark::bufferSize = 200000;

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStreamBenchmark::ByteArrayOutputStreamBenchmark() : buffer(), stlBuffer() {
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStreamBenchmark::~ByteArrayOutputStreamBenchmark() {
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamBenchmark::setUp() {

    buffer = new unsigned char[bufferSize];

    // init to full String Buffer
    stlBuffer.reserve( bufferSize );
    for( int ix = 0; ix < bufferSize - 1; ++ix ) {
        buffer[ix] = 65;
        stlBuffer.push_back( 65 );
    }
    buffer[bufferSize-1] = 0;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamBenchmark::tearDown(){

    delete [] buffer;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStreamBenchmark::run(){

    int numRuns = 100;

    ByteArrayOutputStream bos;

    for( int iy = 0; iy < numRuns; ++iy ){
        bos.write( (char)65 );
    }
    bos.reset();

    for( int iy = 0; iy < numRuns; ++iy ){
        bos.write( buffer, bufferSize, 0, bufferSize );
    }
    bos.reset();

    for( int iy = 0; iy < numRuns; ++iy ){
        bos.write( &stlBuffer[0], bufferSize );
    }
    bos.reset();
}
