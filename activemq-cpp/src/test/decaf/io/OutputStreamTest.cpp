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

#include "OutputStreamTest.h"

#include <decaf/io/OutputStream.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MockOutputStream : public OutputStream {
    private:

        std::vector<unsigned char> buffer;

    public:

        MockOutputStream() : buffer() {}

        virtual ~MockOutputStream() {}

        virtual void doWriteByte( unsigned char c ) {
            buffer.push_back( c );
        }

        const std::vector<unsigned char>& getBuffer() {
            return this->buffer;
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
OutputStreamTest::OutputStreamTest() {
}

////////////////////////////////////////////////////////////////////////////////
OutputStreamTest::~OutputStreamTest() {
}

////////////////////////////////////////////////////////////////////////////////
void OutputStreamTest::test() {

    MockOutputStream ostream;

    ostream.write( 'h' );
    ostream.write( (const unsigned char*)std::string( "ello " ).c_str(), 5 );
    ostream.write( (const unsigned char*)std::string( "hello world" ).c_str(), 11, 6, 5 );

    std::string result( ostream.getBuffer().begin(), ostream.getBuffer().end() );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Written string not what was expected",
                                  std::string( "hello world" ), result );
}
