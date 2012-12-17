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

#include "InflaterInputStreamTest.h"

#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/ByteArrayInputStream.h>

#include <decaf/util/zip/InflaterInputStream.h>
#include <decaf/util/zip/DeflaterOutputStream.h>

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
const std::string InflaterInputStreamTest::testString =
    "Test_All_Tests\nTest_BufferedInputStream\nTest_java_io_BufferedOutputStream\n"
    "Test_java_io_ByteArrayInputStream\nTest_java_io_ByteArrayOutputStream\n"
    "Test_java_io_DataInputStream\nTest_java_io_File\nTest_java_io_FileDescriptor\n"
    "Test_java_io_FileInputStream\nTest_java_io_FileNotFoundException\nTest_java_io_FileOutputStream\n"
    "Test_java_io_FilterInputStream\nTest_java_io_FilterOutputStream\nTest_java_io_InputStream\n"
    "Test_java_io_IOException\nTest_java_io_OutputStream\nTest_java_io_PrintStream\n"
    "Test_java_io_RandomAccessFile\nTest_java_io_SyncFailedException\nTest_java_lang_AbstractMethodError\n"
    "Test_java_lang_ArithmeticException\nTest_java_lang_ArrayIndexOutOfBoundsException\n"
    "Test_java_lang_ArrayStoreException\nTest_java_lang_Boolean\nTest_java_lang_Byte\n"
    "Test_java_lang_Character\nTest_java_lang_Class\nTest_java_lang_ClassCastException\n"
    "Test_java_lang_ClassCircularityError\nTest_java_lang_ClassFormatError\n"
    "Test_java_lang_ClassLoader\nTest_java_lang_ClassNotFoundException\n"
    "Test_java_lang_CloneNotSupportedException\nTest_java_lang_Double\nTest_java_lang_Error\n"
    "Test_java_lang_Exception\nTest_java_lang_ExceptionInInitializerError\nTest_java_lang_Float\n"
    "Test_java_lang_IllegalAccessError\nTest_java_lang_IllegalAccessException\n"
    "Test_java_lang_IllegalArgumentException\nTest_java_lang_IllegalMonitorStateException\n"
    "Test_java_lang_IllegalThreadStateException\nTest_java_lang_IncompatibleClassChangeError\n"
    "Test_java_lang_IndexOutOfBoundsException\nTest_java_lang_InstantiationError\n"
    "Test_java_lang_InstantiationException\nTest_java_lang_Integer\nTest_java_lang_InternalError\n"
    "Test_java_lang_InterruptedException\nTest_java_lang_LinkageError\nTest_java_lang_Long\n"
    "Test_java_lang_Math\nTest_java_lang_NegativeArraySizeException\nTest_java_lang_NoClassDefFoundError\n"
    "Test_java_lang_NoSuchFieldError\nTest_java_lang_NoSuchMethodError\n"
    "Test_java_lang_NullPointerException\nTest_java_lang_Number\nTest_java_lang_NumberFormatException\n"
    "Test_java_lang_Object\nTest_java_lang_OutOfMemoryError\nTest_java_lang_RuntimeException\n"
    "Test_java_lang_SecurityManager\nTest_java_lang_Short\nTest_java_lang_StackOverflowError\n"
    "Test_java_lang_String\nTest_java_lang_StringBuffer\nTest_java_lang_StringIndexOutOfBoundsException\n"
    "Test_java_lang_System\nTest_java_lang_Thread\nTest_java_lang_ThreadDeath\nTest_java_lang_ThreadGroup\n"
    "Test_java_lang_Throwable\nTest_java_lang_UnknownError\nTest_java_lang_UnsatisfiedLinkError\n"
    "Test_java_lang_VerifyError\nTest_java_lang_VirtualMachineError\nTest_java_lang_vm_Image\n"
    "Test_java_lang_vm_MemorySegment\nTest_java_lang_vm_ROMStoreException\n"
    "Test_java_lang_vm_VM\nTest_java_lang_Void\nTest_java_net_BindException\n"
    "Test_java_net_ConnectException\nTest_java_net_DatagramPacket\nTest_java_net_DatagramSocket\n"
    "Test_java_net_DatagramSocketImpl\nTest_java_net_InetAddress\nTest_java_net_NoRouteToHostException\n"
    "Test_java_net_PlainDatagramSocketImpl\nTest_java_net_PlainSocketImpl\nTest_java_net_Socket\n"
    "Test_java_net_SocketException\nTest_java_net_SocketImpl\nTest_java_net_SocketInputStream\n"
    "Test_java_net_SocketOutputStream\nTest_java_net_UnknownHostException\nTest_java_util_ArrayEnumerator\n"
    "Test_java_util_Date\nTest_java_util_EventObject\nTest_java_util_HashEnumerator\nTest_java_util_Hashtable\n"
    "Test_java_util_Properties\nTest_java_util_ResourceBundle\nTest_java_util_tm\nTest_java_util_Vector\n";

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MyInflaterInputStream : public InflaterInputStream {

        MyInflaterInputStream( InputStream* in ) : InflaterInputStream( in ) {
        }

        MyInflaterInputStream( InputStream* in, Inflater* infl ) : InflaterInputStream( in, infl ) {
        }

        MyInflaterInputStream( InputStream* in, Inflater* infl, int size )
            : InflaterInputStream( in, infl, size ) {
        }

        virtual ~MyInflaterInputStream() {}

        void myFill() {
            fill();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
InflaterInputStreamTest::InflaterInputStreamTest() : deflatedData(), inputBuffer() {
}

////////////////////////////////////////////////////////////////////////////////
InflaterInputStreamTest::~InflaterInputStreamTest() {
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::setUp() {

    this->inputBuffer.clear();
    this->inputBuffer.resize( 500 );
    this->deflatedData.clear();
    this->deflatedData.resize( testString.size() + 256 );

    Deflater deflater;

    deflater.setInput( (const unsigned char*)testString.c_str(), (int)testString.size(), 0, (int)testString.size() );
    deflater.finish();

    int x = 0;
    while( !deflater.finished() ) {
        x += deflater.deflate( deflatedData, x, (int)deflatedData.size() - x );
    }

    this->deflatedData.resize( x + 1 );

    deflater.end();
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testConstructorInputStreamInflater() {

    unsigned char byteArray[100];
    ByteArrayInputStream bais( deflatedData );

    Inflater inflate;
    InflaterInputStream inflatIP( &bais, &inflate );

    CPPUNIT_ASSERT( inflatIP.read( byteArray, 100 , 0, 5 ) == 5 );
    inflatIP.close();
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testConstructorInputStreamInflaterI() {

    int result = 0;

    ByteArrayInputStream bais( deflatedData );
    Inflater inflate;
    InflaterInputStream inflatIP( &bais, &inflate, 1 );

    int i = 0;
    while( ( result = inflatIP.read() ) != -1 ) {
        CPPUNIT_ASSERT( testString[i] == (char)result );
        i++;
    }

    inflatIP.close();
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testMark() {

    ByteArrayInputStream bais( deflatedData );
    InflaterInputStream iis( &bais );

    // mark do nothing, do no check
    iis.mark( 0 );
    iis.mark( 10000000 );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testMarkSupported() {

    ByteArrayInputStream bais( deflatedData );
    InflaterInputStream iis( &bais );

    CPPUNIT_ASSERT( !iis.markSupported() );
    CPPUNIT_ASSERT( bais.markSupported() );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testRead() {

    int result = 0;
    ByteArrayInputStream bais( deflatedData );
    Inflater inflate;
    InflaterInputStream inflatIP( &bais, &inflate, 1 );

    int i = 0;
    while( ( result = inflatIP.read() ) != -1 ) {
        CPPUNIT_ASSERT( testString[i] == (char)result );
        i++;
    }

    inflatIP.close();
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testAvailableNonEmptySource() {

    // this unsigned char[] is a deflation of these bytes: { 1, 3, 4, 6 }
    unsigned char deflated[] = {72, -119, 99, 100, 102, 97, 3, 0, 0, 31, 0, 15, 0};

    ByteArrayInputStream bais( deflated, 13 );
    InflaterInputStream in( &bais );

    // InflaterInputStream.available() returns either 1 or 0, even though
    // that contradicts the behavior defined in InputStream.available()
    CPPUNIT_ASSERT_EQUAL( 1, in.read() );
    CPPUNIT_ASSERT_EQUAL( 1, (int)in.available() );
    CPPUNIT_ASSERT_EQUAL( 3, in.read() );
    CPPUNIT_ASSERT_EQUAL( 1, (int)in.available() );
    CPPUNIT_ASSERT_EQUAL( 4, in.read() );
    CPPUNIT_ASSERT_EQUAL( 1, (int)in.available() );
    CPPUNIT_ASSERT_EQUAL( 6, in.read() );
    CPPUNIT_ASSERT_EQUAL( 0, (int)in.available() );
    CPPUNIT_ASSERT_EQUAL( -1, in.read() );
    CPPUNIT_ASSERT_EQUAL( -1, in.read() );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testAvailableSkip() {

    // this unsigned char[] is a deflation of these bytes: { 1, 3, 4, 6 }
    unsigned char deflated[] = { 72, -119, 99, 100, 102, 97, 3, 0, 0, 31, 0, 15, 0 };
    ByteArrayInputStream bais( deflated, 13 );
    InflaterInputStream in( &bais );

    CPPUNIT_ASSERT_EQUAL( 1, (int)in.available() );
    CPPUNIT_ASSERT_EQUAL( 4, (int)in.skip( 4 ) );
    CPPUNIT_ASSERT_EQUAL( 0, (int)in.available() );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testAvailableEmptySource() {

    // this unsigned char[] is a deflation of the empty file
    unsigned char deflated[] = { 120, -100, 3, 0, 0, 0, 0, 1 };
    ByteArrayInputStream bais( deflated, 13 );
    InflaterInputStream in( &bais );

    CPPUNIT_ASSERT_EQUAL( -1, in.read() );
    CPPUNIT_ASSERT_EQUAL( -1, in.read() );
    CPPUNIT_ASSERT_EQUAL( 0, (int)in.available() );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testReadBIII() {

    unsigned char test[507];
    for( int i = 0; i < 256; i++ ) {
        test[i] = (unsigned char)i;
    }
    for( int i = 256; i < 507; i++ ) {
        test[i] = (unsigned char)( 256 - i );
    }

    ByteArrayOutputStream baos;
    DeflaterOutputStream dos( &baos );
    dos.write( test, 507 );
    dos.close();

    std::pair<const unsigned char*, int> array = baos.toByteArray();
    ByteArrayInputStream bais( array.first, array.second, true );
    InflaterInputStream iis( &bais );
    unsigned char outBuf[530];

    int result = 0;
    while( true ) {
        result = iis.read( outBuf, 530, 0, 5 );
        if( result == -1 ) {
            //"EOF was reached";
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testReadBIII2() {

    ByteArrayInputStream bais( deflatedData );
    InflaterInputStream iis( &bais );
    unsigned char outBuf[530];
    iis.close();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        iis.read( outBuf, 530, 0, 5 ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testReadBIII3() {

    unsigned char byteArray[] = { 45, 6, 1, 0, 12, 56, 125 };
    ByteArrayInputStream bais( byteArray, 7 );
    InflaterInputStream iis( &bais );
    unsigned char outBuf[530];

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        iis.read( outBuf, 530, 0, 5 ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testReset() {

    ByteArrayInputStream bais( deflatedData );
    InflaterInputStream iis( &bais );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        iis.reset(),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testSkip() {

    ByteArrayInputStream bais( this->deflatedData );
    InflaterInputStream iis( &bais );

    // Tests for skipping a zero value
    iis.skip( 0 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", (int)'T', iis.read() );

    // Test to make sure the correct number of bytes were skipped
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Number Of Bytes Skipped.", 3, (int)iis.skip( 3 ) );

    // Test to see if the number of bytes skipped returned is true.
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", (int)'_', iis.read() );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Number Of Bytes Skipped.", 0, (int)iis.skip( 0 ) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", (int)'A', iis.read() );

    // Test for skipping more bytes than available in the stream
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Number Of Bytes Skipped.",
                                  (long long)testString.length() - 6, iis.skip( testString.length() ) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect Byte Returned.", -1, iis.read() );
    iis.close();
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testSkip2() {

    std::vector<unsigned char> buffer( testString.length() );

    // testing for number of bytes greater than input.
    ByteArrayInputStream bais1( this->deflatedData );
    InflaterInputStream iis1( &bais1 );

    long long skip = iis1.skip( Integer::MAX_VALUE );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "method skip() returned wrong number of bytes skipped",
                                  (long long)testString.size(), skip );

    // test for skipping of 2 bytes
    ByteArrayInputStream bais2( this->deflatedData );
    InflaterInputStream iis2( &bais2 );

    skip = iis2.skip( 2 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "the number of bytes returned by skip did not correspond with its input parameters",
                                  2LL, skip );
    int i = 0;
    int result = 0;
    while( ( result = iis2.read() ) != -1 ) {
        buffer[i] = (unsigned char)result;
        i++;
    }

    iis2.close();

    for( int j = 2; j < (int)testString.length(); j++ ) {
        CPPUNIT_ASSERT_MESSAGE( "original compressed data did not equal decompressed data",
                                buffer[j - 2] == testString.at( j ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testAvailable() {

    // this unsigned char[] is a deflation of these bytes: { 1, 3, 4, 6 }
    unsigned char deflated[] = {72, -119, 99, 100, 102, 97, 3, 0, 0, 31, 0, 15, 0};

    ByteArrayInputStream bais( deflated, 13 );
    InflaterInputStream iis( &bais );

    int available;
    for( int i = 0; i < 4; i++ ) {
        iis.read();
        available = iis.available();
        if( available == 0 ) {
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "Expected no more bytes to read", -1, iis.read() );
        } else {
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "Bytes Available Should Return 1.", 1, available );
        }
    }

    iis.close();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        iis.available(),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterInputStreamTest::testClose() {

    ByteArrayInputStream bais( deflatedData );
    InflaterInputStream iin( &bais );
    iin.close();
    // test for exception
    iin.close();
}
