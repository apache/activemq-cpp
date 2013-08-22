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

#include "WriterTest.h"

#include <decaf/io/Writer.h>
#include <decaf/nio/CharBuffer.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::nio;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MockWriter : public Writer {
    private:

        char* contents;
        int length;
        int offset;

    private:

        MockWriter(const MockWriter&);
        MockWriter& operator= (const MockWriter&);

    public:

        MockWriter( int capacity ) : contents(), length(capacity), offset(0) {
            contents = new char[capacity];
        }

        virtual ~MockWriter() {
            close();
        }

        virtual void close() {
            flush();
            delete [] contents;
            contents = NULL;
        }

        virtual void flush() {
        }

        virtual void doWriteArrayBounded(
            const char* buffer, int size, int offset, int length ) {

            if( NULL == contents ) {
                throw IOException( __FILE__, __LINE__, "Writer was already closed." );
            }

            if( offset + length > size ) {
                throw IndexOutOfBoundsException(
                    __FILE__, __LINE__, "offset + length must be less than size." );
            }

            for( int i = 0; i < length; i++ ) {
                contents[this->offset + i] = buffer[offset + i];
            }

            this->offset += length;

        }

        std::vector<char> getContents() {

            std::vector<char> result( offset );

            for( int i = 0; i < offset; i++ ) {
                result[i] = contents[i];
            }

            return result;
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
WriterTest::WriterTest() {
}

////////////////////////////////////////////////////////////////////////////////
WriterTest::~WriterTest() {
}

////////////////////////////////////////////////////////////////////////////////
void WriterTest::testWriteChar() {

    std::string testString = "My Test String";
    MockWriter writer( 20 );

    std::string::const_iterator iter = testString.begin();
    for( ; iter != testString.end(); ++iter ) {
        writer.write( *iter );
    }

    std::vector<char> result = writer.getContents();
    CPPUNIT_ASSERT_EQUAL( testString, std::string( result.begin(), result.end() ) );
}

////////////////////////////////////////////////////////////////////////////////
void WriterTest::testWriteVector() {

    std::string testString = "My Test String";
    MockWriter writer( 20 );

    std::vector<char> buffer( testString.begin(), testString.end() );
    writer.write( buffer );

    std::vector<char> result = writer.getContents();
    CPPUNIT_ASSERT_EQUAL( testString, std::string( result.begin(), result.end() ) );
}

////////////////////////////////////////////////////////////////////////////////
void WriterTest::testWriteString() {

    std::string testString = "My Test String";
    MockWriter writer( 20 );

    writer.write( testString );

    std::vector<char> result = writer.getContents();
    CPPUNIT_ASSERT_EQUAL( testString, std::string( result.begin(), result.end() ) );
}

////////////////////////////////////////////////////////////////////////////////
void WriterTest::testWriteStringOffsetCount() {

    std::string testString = "My Test String";
    MockWriter writer( 20 );

    writer.write( testString, 0, (int)testString.length() );

    std::vector<char> result = writer.getContents();
    CPPUNIT_ASSERT_EQUAL( testString, std::string( result.begin(), result.end() ) );
}

////////////////////////////////////////////////////////////////////////////////
void WriterTest::testAppendChar() {

    char testChar = ' ';
    MockWriter writer(20);

    writer.append( testChar );

    CPPUNIT_ASSERT_EQUAL( testChar, writer.getContents()[0] );

    writer.close();
}

////////////////////////////////////////////////////////////////////////////////
void WriterTest::testAppendCharSequence() {

    std::string testString = "My Test String";
    MockWriter writer( 20 );

    CharBuffer* buffer = CharBuffer::allocate( (int)testString.size() );
    buffer->put( testString );
    buffer->rewind();

    writer.append( buffer );

    std::vector<char> result = writer.getContents();
    CPPUNIT_ASSERT_EQUAL( testString, std::string( result.begin(), result.end() ) );

    writer.close();

    delete buffer;
}

////////////////////////////////////////////////////////////////////////////////
void WriterTest::testAppendCharSequenceIntInt() {

    std::string testString = "My Test String";
    MockWriter writer(20);

    CharBuffer* buffer = CharBuffer::allocate( (int)testString.size() );
    buffer->put( testString );
    buffer->rewind();

    writer.append( buffer, 1, 3 );

    std::vector<char> result = writer.getContents();
    CPPUNIT_ASSERT_EQUAL( testString.substr( 1, 2 ), std::string( result.begin(), result.end() ) );

    writer.close();

    delete buffer;
}
