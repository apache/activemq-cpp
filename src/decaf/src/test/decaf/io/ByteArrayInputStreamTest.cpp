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

#include "ByteArrayInputStreamTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::ByteArrayInputStreamTest );

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;

void ByteArrayInputStreamTest::testStream()
{
    std::vector<unsigned char> testBuffer;

    testBuffer.push_back('t');
    testBuffer.push_back('e');
    testBuffer.push_back('s');
    testBuffer.push_back('t');

    ByteArrayInputStream stream_a(&testBuffer[0], testBuffer.size());

    CPPUNIT_ASSERT( stream_a.available() == 4 );

    char a = stream_a.read();
    char b = stream_a.read();
    char c = stream_a.read();
    char d = stream_a.read();

    CPPUNIT_ASSERT( a == 't' && b == 'e' && c == 's' && d == 't' );
    CPPUNIT_ASSERT( stream_a.available() == 0 );

    testBuffer.push_back('e');

    stream_a.setByteArray(&testBuffer[0], testBuffer.size());

    CPPUNIT_ASSERT( stream_a.available() == 5 );

    unsigned char* buffer = new unsigned char[6];

    buffer[5] = '\0';

    CPPUNIT_ASSERT( stream_a.read(buffer, 5) == 5 );
    CPPUNIT_ASSERT( std::string((const char*)buffer) == std::string("teste") );
    CPPUNIT_ASSERT( stream_a.available() == 0 );

    stream_a.setByteArray(&testBuffer[0], testBuffer.size());

    memset(buffer, 0, 6);

    CPPUNIT_ASSERT( stream_a.read(buffer, 3) == 3 );
    CPPUNIT_ASSERT( stream_a.read(&buffer[3], 2) == 2 );
    CPPUNIT_ASSERT( std::string((const char*)buffer) == std::string("teste") );

    stream_a.close();

    delete [] buffer;
}
