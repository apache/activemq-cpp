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

#include "DeflaterOutputStreamTest.h"

#include <decaf/util/zip/DeflaterOutputStream.h>
#include <decaf/util/zip/InflaterInputStream.h>

#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/ByteArrayInputStream.h>

#include <decaf/util/zip/Deflater.h>
#include <decaf/util/zip/Inflater.h>
#include <decaf/util/zip/Adler32.h>
#include <decaf/util/zip/CRC32.h>

#include <decaf/lang/Integer.h>

#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

#include <vector>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
    namespace {

    class MyDeflaterOutputStream : public DeflaterOutputStream {
    private:

        bool deflateFlag;

    public:

        MyDeflaterOutputStream(OutputStream* out) : DeflaterOutputStream(out), deflateFlag(false) {}

        MyDeflaterOutputStream(OutputStream* out, Deflater* defl) :
            DeflaterOutputStream(out, defl), deflateFlag(false) {
        }

        MyDeflaterOutputStream(OutputStream* out, Deflater* defl, int size) :
            DeflaterOutputStream(out, defl, size), deflateFlag(false) {
        }

        virtual ~MyDeflaterOutputStream() {}

        std::vector<unsigned char>& getProtectedBuf() {
            return buf;
        }

        bool getDaflateFlag() const {
            return deflateFlag;
        }

    protected:

        void deflate() {
            deflateFlag = true;
            DeflaterOutputStream::deflate();
        }

    };

}

////////////////////////////////////////////////////////////////////////////////
DeflaterOutputStreamTest::DeflaterOutputStreamTest() : outputBuffer() {
}

////////////////////////////////////////////////////////////////////////////////
DeflaterOutputStreamTest::~DeflaterOutputStreamTest() {
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::tearDown() {

    this->outputBuffer.clear();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::setUp() {

    this->outputBuffer.clear();
    this->outputBuffer.resize( 500 );

    // setting up a deflater to be used
    unsigned char byteArray[] = { 1, 3, 4, 7, 8 };
    int x = 0;
    Deflater deflate( 1 );
    deflate.setInput( byteArray, 5, 0, 5 );

    while( !( deflate.needsInput() ) ) {
        x += deflate.deflate( outputBuffer, x, (int)outputBuffer.size() - x );
    }

    deflate.finish();

    while( !( deflate.finished() ) ) {
        x = x + deflate.deflate( outputBuffer, x, (int)outputBuffer.size() - x );
    }

    deflate.end();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::testConstructorOutputStreamDeflater() {

    unsigned char byteArray[] = { 1, 3, 4, 7, 8 };

    ByteArrayOutputStream baos;
    Deflater* nullDeflater = NULL;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        DeflaterOutputStream( &baos, nullDeflater ),
        NullPointerException );

    Deflater defl;
    MyDeflaterOutputStream dos( &baos, &defl );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Buffer Size for new DeflaterOutputStream",
                                  (std::size_t)512, dos.getProtectedBuf().size() );

    dos.write( byteArray, 5 );
    dos.close();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::testConstructorOutputStream() {

    ByteArrayOutputStream baos;
    MyDeflaterOutputStream dos( &baos );

    // Test to see if DeflaterOutputStream was created with the correct
    // buffer.
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Buffer Size",
                                  (std::size_t)512, dos.getProtectedBuf().size() );

    dos.write( &outputBuffer[0], (int)outputBuffer.size() );
    dos.close();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::testConstructorOutputStreamDeflaterI() {

    int buf = 5;
    int zeroBuf = 0;

    unsigned char byteArray[] = { 1, 3, 4, 7, 8, 3, 6 };
    ByteArrayOutputStream baos;
    Deflater* nullDeflater = NULL;

    // Test for a null Deflater.
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        DeflaterOutputStream( &baos, nullDeflater, buf ),
        NullPointerException );

    Deflater defl;

    // Test for a zero buf.
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IllegalArgumentException",
        DeflaterOutputStream( &baos, &defl, zeroBuf ),
        IllegalArgumentException );

    // Test to see if DeflaterOutputStream was created with the correct
    // buffer.
    MyDeflaterOutputStream dos( &baos, &defl, buf );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Buffer Size for new DeflaterOutputStream",
                                  (std::size_t)5, dos.getProtectedBuf().size() );

    dos.write( byteArray, 7, 0, 7 );
    dos.close();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::testClose() {

    ByteArrayOutputStream baos;
    DeflaterOutputStream dos( &baos );
    unsigned char byteArray[] = { 1, 3, 4, 6 };
    dos.write( byteArray, 4 );
    dos.close();

    std::pair<const unsigned char*, int> array = baos.toByteArray();
    ByteArrayInputStream bais( array.first, array.second, true );
    InflaterInputStream iis( &bais );

    // Test to see if the finish method wrote the bytes to the file.
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", 1, iis.read());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", 3, iis.read());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", 4, iis.read());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", 6, iis.read());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", -1, iis.read());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", -1, iis.read());
    iis.close();

    // Test for a zero buf.
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IOException",
        dos.write( 5 ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::testFinish() {

    ByteArrayOutputStream baos;
    DeflaterOutputStream dos( &baos );
    unsigned char byteArray[] = { 1, 3, 4, 6 };
    dos.write( byteArray, 4 );
    dos.finish();

    // Test to see if the same FileOutputStream can be used with the
    // DeflaterOutputStream after finish is called.
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        dos.write( 1 ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::testWriteI() {

    ByteArrayOutputStream baos;
    DeflaterOutputStream dos( &baos );

    for( int i = 0; i < 3; i++ ) {
        dos.write( (unsigned char)i );
    }
    dos.close();

    std::pair<const unsigned char*, int> array = baos.toByteArray();
    ByteArrayInputStream bais( array.first, array.second, true );
    InflaterInputStream iis( &bais );

    for( int i = 0; i < 3; i++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", i, iis.read() );
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned (EOF).", -1, iis.read() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned (EOF).", -1, iis.read() );
    iis.close();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::testWriteBIII() {

    unsigned char byteArray[] = { 1, 3, 4, 7, 8, 3, 6 };

    // Test to see if the correct bytes are saved.
    ByteArrayOutputStream baos;
    DeflaterOutputStream dos1( &baos );
    dos1.write( byteArray, 7, 2, 3 );
    dos1.close();

    std::pair<const unsigned char*, int> array = baos.toByteArray();
    ByteArrayInputStream bais( array.first, array.second, true );
    InflaterInputStream iis( &bais );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", 4, iis.read() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", 7, iis.read() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", 8, iis.read() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned (EOF).", -1, iis.read() );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned (EOF).", -1, iis.read() );
    iis.close();

    // Test for trying to write more bytes than available from the array
    ByteArrayOutputStream baos2;
    DeflaterOutputStream dos2( &baos2 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        dos2.write( byteArray, 7, 2, 10 ),
        IndexOutOfBoundsException );

    // Test for trying to start writing from a unsigned char > than the array
    // size.
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        dos2.write( byteArray, 7, 2, 10 ),
        IndexOutOfBoundsException );

    dos2.close();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::testDeflate() {

    ByteArrayOutputStream baos;
    MyDeflaterOutputStream dos( &baos );
    CPPUNIT_ASSERT( !dos.getDaflateFlag() );
    for( int i = 0; i < 3; i++ ) {
        dos.write( (unsigned char)i );
    }
    CPPUNIT_ASSERT( dos.getDaflateFlag() );
    dos.close();
}
