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

// START SNIPPET: demo

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/lang/Integer.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/Config.h>
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/util/Date.h>

#include <stdlib.h>
#include <iostream>

using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
void testDataInputStream() {

    std::cout << "-----------------------------------------------------\n";
    std::cout << "Testing performance of the DataInputStream:" << std::endl;
    std::cout << "-----------------------------------------------------\n";

    const int bufferSize = 1024 * 2000;
    const int totalRuns = 100;

    unsigned char* buffer = new unsigned char[bufferSize];

    // init to full String Buffer
    for( int ix = 0; ix < bufferSize - 1; ++ix ) {
        buffer[ix] = 'z';
    }
    buffer[bufferSize-1] = '\0';

    long long startTime = 0LL;
    long long endTime = 0LL;
    long long totalDelta = 0LL;
    long long average = 0LL;
    std::vector<long long> deltaTs;

    ByteArrayInputStream bis( buffer, bufferSize );
    DataInputStream dis( &bis );

    for( int i = 0; i < totalRuns; ++i ) {

        startTime = Date::getCurrentTimeMilliseconds();

        long long result = 0;
        for( size_t iy = 0; iy < bufferSize / sizeof( result ); ++iy ){
            result = dis.readLong();
        }

        endTime = Date::getCurrentTimeMilliseconds();

        // Save the result
        deltaTs.push_back( endTime - startTime );

        bis.reset();
    }

    // Sum the Time Deltas for all runs.
    for( int j = 0; j < totalRuns; ++j ) {
        totalDelta += deltaTs[j];
    }

    average = totalDelta / totalRuns;

    std::cout << "Averqage time of competion was: "
              << average << " Milliseconds." << std::endl;

    delete [] buffer;
}

////////////////////////////////////////////////////////////////////////////////
void testDataOutputStream() {

    std::cout << "-----------------------------------------------------\n";
    std::cout << "Testing performance of the DataOutputStream:" << std::endl;
    std::cout << "-----------------------------------------------------\n";

    long long totalRuns = 2000LL;
    long long startTime = 0LL;
    long long endTime = 0LL;
    long long totalDelta = 0LL;
    long long average = 0LL;
    std::vector<long long> deltaTs;
    std::vector<unsigned char> outputBuffer;

    std::string testString;
    for( size_t i = 0; i < totalRuns * 10; ++i ) {
        testString += 'a';
    }

    ByteArrayOutputStream bos( outputBuffer );
    DataOutputStream dos( &bos );

    for( int i = 0; i < totalRuns; ++i ) {

        startTime = Date::getCurrentTimeMilliseconds();

        for( int iy = 0; iy < totalRuns * 10; ++iy ){
            dos.writeLong( 0xFF00FF00FF00FF00LL );
        }
        for( int iy = 0; iy < totalRuns * 10; ++iy ){
            dos.writeInt( 312568 );
        }
        for( int iy = 0; iy < totalRuns * 10; ++iy ){
            dos.writeShort( 12568 );
        }
        for( int iy = 0; iy < totalRuns * 10; ++iy ){
            dos.writeUnsignedShort( 12568 );
        }
        for( int iy = 0; iy < totalRuns * 10; ++iy ){
            dos.writeBoolean( true );
        }
        for( int iy = 0; iy < totalRuns * 10; ++iy ){
            dos.writeDouble( 10.34235234 );
        }
        for( int iy = 0; iy < totalRuns; ++iy ){
            dos.writeFloat( 32.4f );
        }

        dos.writeChars( testString );
        dos.writeBytes( testString );
        dos.writeUTF( testString );

        endTime = Date::getCurrentTimeMilliseconds();

        // Save the result
        deltaTs.push_back( endTime - startTime );

        outputBuffer.clear();
    }

    // Sum the Time Deltas for all runs.
    for( int j = 0; j < totalRuns; ++j ) {
        totalDelta += deltaTs[j];
    }

    average = totalDelta / totalRuns;

    std::cout << "Averqage time of competion was: "
              << average << " Milliseconds." << std::endl;
}

int main(int argc DECAF_UNUSED, char* argv[] DECAF_UNUSED) {

    std::cout << "=====================================================\n";
    std::cout << "Starting the example:" << std::endl;
    std::cout << "-----------------------------------------------------\n";

    //testDataInputStream();
    testDataOutputStream();

    std::cout << "-----------------------------------------------------\n";
    std::cout << "Finished with the example\n";
    std::cout << "=====================================================\n";
}

// END SNIPPET: demo
