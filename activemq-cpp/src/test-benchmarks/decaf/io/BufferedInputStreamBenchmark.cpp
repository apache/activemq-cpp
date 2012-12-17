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

#include "BufferedInputStreamBenchmark.h"

using namespace decaf;
using namespace decaf::io;

////////////////////////////////////////////////////////////////////////////////
const int BufferedInputStreamBenchmark::bufferSize = 200000;

////////////////////////////////////////////////////////////////////////////////
BufferedInputStreamBenchmark::BufferedInputStreamBenchmark() : buffer(), source() {
}

////////////////////////////////////////////////////////////////////////////////
BufferedInputStreamBenchmark::~BufferedInputStreamBenchmark() {
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamBenchmark::setUp() {

    buffer = new unsigned char[bufferSize];
    source.setByteArray( buffer, bufferSize );
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamBenchmark::tearDown(){

    delete [] buffer;
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamBenchmark::run(){

    int numRuns = 25;

    std::vector<unsigned char> bucket( bufferSize );
    BufferedInputStream bis( &source );

    for( int iy = 0; iy < numRuns; ++iy ){
        BufferedInputStream local( &source );
    }

    for( int iy = 0; iy < numRuns; ++iy ){

        for( int iz = 0; iz < bufferSize; ++iz ) {
            bucket[iy] = (unsigned char)bis.read();
        }
        source.reset();
    }

    for( int iy = 0; iy < numRuns; ++iy ){
        bis.read( &bucket[0], bufferSize, 0, bufferSize );
        source.reset();
    }

}
