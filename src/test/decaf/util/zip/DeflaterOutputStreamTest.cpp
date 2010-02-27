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
namespace{

    class MyDeflaterOutputStream : public DeflaterOutputStream {
    private:

        bool deflateFlag;

    public:

        MyDeflaterOutputStream( OutputStream* out ) : DeflaterOutputStream( out ) {
            this->deflateFlag = false;
        }

        MyDeflaterOutputStream( OutputStream* out, Deflater* defl ) :
            DeflaterOutputStream( out, defl ) {

            this->deflateFlag = false;
        }

        MyDeflaterOutputStream( OutputStream* out, Deflater* defl, std::size_t size ) :
            DeflaterOutputStream( out, defl, size ) {

            this->deflateFlag = false;
        }

        std::vector<unsigned char>& getProtectedBuf() {
            return buf;
        }

        bool getDaflateFlag() const {
            return deflateFlag;
        }

    protected:

        void deflate() throw( IOException ) {
            deflateFlag = true;
            DeflaterOutputStream::deflate();
        }

    };

}

////////////////////////////////////////////////////////////////////////////////
DeflaterOutputStreamTest::DeflaterOutputStreamTest() {
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
        x += deflate.deflate( outputBuffer, x, outputBuffer.size() - x );
    }

    deflate.finish();

    while( !( deflate.finished() ) ) {
        x = x + deflate.deflate( outputBuffer, x, outputBuffer.size() - x );
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

    dos.write( &outputBuffer[0], outputBuffer.size() );
    dos.close();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::testConstructorOutputStreamDeflaterI() {

    std::size_t buf = 5;
    std::size_t zeroBuf = 0;

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

    ByteArrayInputStream bais( baos.toByteArrayRef() );
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

    // Test to write to a ByteArrayOutputStream that should have been closed
    // by the DeflaterOutputStream.
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IOException",
        baos.write( 5 ),
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

//void testWriteI() {
//    File f1 = new File("writeI1.tst");
//    FileOutputStream fos = new FileOutputStream(f1);
//    DeflaterOutputStream dos = new DeflaterOutputStream(fos);
//    for (int i = 0; i < 3; i++) {
//        dos.write(i);
//    }
//    dos.close();
//    FileInputStream fis = new FileInputStream(f1);
//    InflaterInputStream iis = new InflaterInputStream(fis);
//    for (int i = 0; i < 3; i++) {
//        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", i, iis.read());
//    }
//    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned (EOF).", -1, iis.read());
//    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned (EOF).", -1, iis.read());
//    iis.close();
//
//    // Not sure if this test is that important.
//    // Checks to see if you can write using the DeflaterOutputStream
//    // after
//    // the FileOutputStream has been closed.
//    FileOutputStream fos2 = new FileOutputStream(f1);
//    DeflaterOutputStream dos2 = new DeflaterOutputStream(fos2);
//    fos2.close();
//    try {
//        dos2.write(2);
//        CPPUNIT_FAIL("IOException not thrown");
//    } catch (IOException e) {
//    }
//
//}
//
//void testWriteBIII() {
//    unsigned char byteArray[] = { 1, 3, 4, 7, 8, 3, 6 };
//
//    // Test to see if the correct bytes are saved.
//    File f1 = new File("writeBII.tst");
//    FileOutputStream fos1 = new FileOutputStream(f1);
//    DeflaterOutputStream dos1 = new DeflaterOutputStream(fos1);
//    dos1.write(byteArray, 2, 3);
//    dos1.close();
//    FileInputStream fis = new FileInputStream(f1);
//    InflaterInputStream iis = new InflaterInputStream(fis);
//    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", 4, iis.read());
//    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", 7, iis.read());
//    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", 8, iis.read());
//    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned (EOF).", -1, iis.read());
//    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned (EOF).", -1, iis.read());
//    iis.close();
//    f1.delete();
//
//    // Test for trying to write more bytes than available from the array
//    File f2 = new File("writeBII2.tst");
//    FileOutputStream fos2 = new FileOutputStream(f2);
//    DeflaterOutputStream dos2 = new DeflaterOutputStream(fos2);
//    try {
//        dos2.write(byteArray, 2, 10);
//        CPPUNIT_FAIL("IndexOutOfBoundsException not thrown");
//    } catch (IndexOutOfBoundsException e) {
//    }
//
//    // Test for trying to write a negative number of bytes.
//    try {
//        dos2.write(byteArray, 2, Integer.MIN_VALUE);
//        CPPUNIT_FAIL("IndexOutOfBoundsException not thrown");
//    } catch (IndexOutOfBoundsException e) {
//    }
//
//    // Test for trying to start writing from a unsigned char < 0 from the array.
//    try {
//        dos2.write(byteArray, Integer.MIN_VALUE, 2);
//        CPPUNIT_FAIL("IndexOutOfBoundsException not thrown");
//    } catch (IndexOutOfBoundsException e) {
//    }
//
//    // Test for trying to start writing from a unsigned char > than the array
//    // size.
//    try {
//        dos2.write(byteArray, 10, 2);
//        CPPUNIT_FAIL("IndexOutOfBoundsException not thrown");
//    } catch (IndexOutOfBoundsException e) {
//    }
//    dos2.close();
//
//    // Not sure if this test is that important.
//    // Checks to see if you can write using the DeflaterOutputStream
//    // after
//    // the FileOutputStream has been closed.
//    FileOutputStream fos3 = new FileOutputStream(f2);
//    DeflaterOutputStream dos3 = new DeflaterOutputStream(fos3);
//    fos3.close();
//    try {
//        dos3.write(byteArray, 2, 3);
//        CPPUNIT_FAIL("IOException not thrown");
//    } catch (IOException e) {
//    }
//
//}

////////////////////////////////////////////////////////////////////////////////
void DeflaterOutputStreamTest::testDeflate() {

    ByteArrayOutputStream baos;
    MyDeflaterOutputStream dos( &baos );
    CPPUNIT_ASSERT( !dos.getDaflateFlag() );
    for( int i = 0; i < 3; i++ ) {
        dos.write( i );
    }
    CPPUNIT_ASSERT( dos.getDaflateFlag() );
    dos.close();
}
