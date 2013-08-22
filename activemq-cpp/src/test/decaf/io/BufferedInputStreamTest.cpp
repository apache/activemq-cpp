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

#include "BufferedInputStreamTest.h"

#include <decaf/lang/Integer.h>
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
const std::string BufferedInputStreamTest::testString =
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

    class MyInputStream : public InputStream{
    private:
        std::string data;
        int pos;
        bool throwOnRead;
        bool closed;

    public:

        MyInputStream( const std::string& data ) : data(), pos(), throwOnRead(), closed() {
            this->data = data;
            this->pos = 0;
            this->throwOnRead = false;
            this->closed = false;
        }
        virtual ~MyInputStream(){}

        void setThrowOnRead( bool value ) {
            this->throwOnRead = value;
        }

        bool isThrowOnRead() const {
            return this->throwOnRead;
        }

        bool isClosed() const {
            return this->closed;
        }

        virtual int available() const {
            if( isClosed() ) {
                throw IOException(
                    __FILE__, __LINE__,
                    "MyInputStream::read - Stream already closed." );
            }
            int len = (int)data.length();
            return len - pos;
        }

        virtual int doReadByte() {
            if( this->isThrowOnRead() ) {
                throw IOException(
                    __FILE__, __LINE__,
                    "MyInputStream::read - Throw on Read on." );
            }

            if( pos >= (int)data.length() ){
                return -1;
            }

            return data.c_str()[pos++];
        }

        virtual int doReadArrayOffsetAndLength( unsigned char* buffer, int size, int offset, int length ) {

            int numToRead = std::min( length, available() );

            if( this->isThrowOnRead() ) {
                throw IOException(
                    __FILE__, __LINE__,
                    "MyInputStream::read - Throw on Read on." );
            }

            // Simulate EOF
            if( numToRead == 0 ) {
                return -1;
            }

            const char* str = data.c_str();
            for( int ix=0; ix<numToRead; ++ix ){
                buffer[ix+offset] = str[pos+ix];
            }

            pos += numToRead;

            return (int)numToRead;
        }

        virtual void close() {
            this->closed = true;
        }

        virtual long long skip( long long num ) {
            return ( pos += (int)std::min( num, (long long)available() ) );
        }

    };

}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testConstructor() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );

    // Create buffer with exact size of data
    BufferedInputStream is( &myStream, (int)testStr.length() );

    // Ensure buffer gets filled by evaluating one read
    CPPUNIT_ASSERT( is.read() != -1 );

    // Read the remaining buffered characters, no IOException should
    // occur.
    is.skip( testStr.length() - 2 );
    CPPUNIT_ASSERT( is.read() != -1 );
    // is.read should now return -1 as all data has been read.
    CPPUNIT_ASSERT( is.read() == -1 );

    {
        BufferedInputStream nullStream( NULL );
        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should have thrown an IOException",
            nullStream.read(),
            IOException );
    }
    {
        BufferedInputStream nullStream( NULL, 1 );
        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should have thrown an IOException",
            nullStream.read(),
            IOException );
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testAvailable() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    // Create buffer with exact size of data
    BufferedInputStream is( &myStream, (int)testStr.length() );

    // Test for method int BufferedInputStream.available()
    try {
        CPPUNIT_ASSERT_MESSAGE( "Returned incorrect number of available bytes",
                                 is.available() == (int)testStr.length() );
    } catch( IOException& e ) {
        CPPUNIT_ASSERT_MESSAGE("Exception during available test", false );
    }

    // Test that a closed stream throws an IOE for available()
    std::string testStr2 = "hello world";
    MyInputStream myStream2( testStr2 );
    BufferedInputStream bis( &myStream2, (int)testStr2.length() );

    int available;

    try {
        available = bis.available();
        bis.close();
    } catch( IOException& ex ) {
        CPPUNIT_ASSERT(false);
        return; // never reached.
    }
    CPPUNIT_ASSERT( available != 0 );

    try {
        bis.available();
        CPPUNIT_ASSERT_MESSAGE("Expected test to throw IOE.", false );
    } catch( IOException& ex ) {
        // expected
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testClose() {

    try{
        std::string testStr = "TEST12345678910";
        MyInputStream myStream( testStr );

        // Test for method void java.io.BufferedInputStream.close()
        BufferedInputStream i1( &myStream );
        BufferedInputStream i2( &myStream );

        // Test a null stream
        MyInputStream* ptr = NULL;
        BufferedInputStream buf( ptr, 5 );
        buf.close();
    } catch(...) {
        CPPUNIT_FAIL("Close shouldn't throw an error here" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testMarkSupported() {
    BufferedInputStream is( NULL );
    CPPUNIT_ASSERT_MESSAGE( "markSupported returned incorrect value", is.markSupported() );
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testRead() {

    try {
        // Test for method int BufferedInputStream.read()
        std::string testStr = "TEST12345678910";
        MyInputStream myStream( testStr );
        // Create buffer with exact size of data
        BufferedInputStream is( &myStream, (int)(int)testStr.length() );

        char c = (char)is.read();
        CPPUNIT_ASSERT_MESSAGE( "read returned incorrect char",
                                c == testStr.at(0) );
    } catch( IOException& e ) {
        CPPUNIT_FAIL( "Exception during read test" );
    }

    unsigned char bytes[256];
    for( int i = 0; i < 256; i++ ) {
        bytes[i] = (unsigned char)i;
    }

    // New stream, owns the inner one.
    BufferedInputStream is(
        new ByteArrayInputStream( &bytes[0], 256 ), 12, true );

    try {
        CPPUNIT_ASSERT_MESSAGE( "Wrong initial byte", 0 == is.read() );
        // Fill the buffer
        unsigned char buf[14];
        is.read( &buf[0], 14, 0, 14 );

        // Read greater than the buffer
        CPPUNIT_ASSERT_MESSAGE( "Wrong block read data",
                string( (const char*)&buf[0], 14 ) ==
                string( (const char*)&bytes[1], 14 ) );

        CPPUNIT_ASSERT_MESSAGE("Wrong bytes", 15 == is.read() ); // Check next byte

    } catch( IOException& e ) {
        CPPUNIT_FAIL("Exception during read test");
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testRead2() {

    std::string testStr;
    testStr.append( "A", 3000 );
    testStr.append( "B", 1000 );
    MyInputStream myStream( testStr );
    // Create buffer with exact size of data
    BufferedInputStream is( &myStream, (int)testStr.length() );

    // Test for method int BufferedInputStream.read(byte [], int,
    // int)
    unsigned char buf1[100];
    try {
        is.skip( 3000 );
        is.read( buf1, 100, 0, 100 );
        CPPUNIT_ASSERT_MESSAGE(
            "Failed to read correct data",
            string( (const char*)&buf1[0], 100 ) == testStr.substr( 3000, 100 ) );

    } catch( IOException& e ) {
        CPPUNIT_FAIL("Exception during read test");
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testReadException() {

    unsigned char array[1];

    BufferedInputStream bis( NULL );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw NullPointerException",
        bis.read( NULL, 1, 0, 1 ),
        NullPointerException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw IndexOutOfBoundsException",
        bis.read( array, 0, 1, 1 ),
        IndexOutOfBoundsException );

    bis.close();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw IOException",
        bis.read( NULL, 1, 0, 1 ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testSmallerBuffer(){

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    BufferedInputStream bufStream( &myStream, 1 );

    int available = bufStream.available();
    CPPUNIT_ASSERT( available == (int)testStr.length() );

    unsigned char dummy = (unsigned char)bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == ((int)testStr.length() - 1) );

    dummy = (unsigned char)bufStream.read();
    CPPUNIT_ASSERT( dummy == 'E' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == ((int)testStr.length() - 2 ) );

    dummy = (unsigned char)bufStream.read();
    CPPUNIT_ASSERT( dummy == 'S' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == ((int)testStr.length() - 3 ) );

    dummy = (unsigned char)bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    unsigned char dummyBuf[20];
    memset( dummyBuf, 0, 20 );
    int numRead = bufStream.read( dummyBuf, 20, 0, 10 );
    CPPUNIT_ASSERT( numRead == 10 );
    CPPUNIT_ASSERT( strcmp( (char*)dummyBuf, "1234567891" ) == 0 );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == 1 );
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testBiggerBuffer(){

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    BufferedInputStream bufStream( &myStream, 10 );

    int available = bufStream.available();
    CPPUNIT_ASSERT( available == (int)testStr.length() );

    unsigned char dummy = (unsigned char)bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == ((int)testStr.length() - 1 ) );

    dummy = (unsigned char)bufStream.read();
    CPPUNIT_ASSERT( dummy == 'E' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == ((int)testStr.length() - 2 ) );

    dummy = (unsigned char)bufStream.read();
    CPPUNIT_ASSERT( dummy == 'S' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == ((int)testStr.length() - 3 ) );

    dummy = (unsigned char)bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    unsigned char dummyBuf[20];
    memset( dummyBuf, 0, 20 );
    int numRead = bufStream.read( dummyBuf, 20, 0, 10 );
    CPPUNIT_ASSERT( numRead == 10 );
    CPPUNIT_ASSERT( strcmp( (char*)dummyBuf, "1234567891" ) == 0 );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == 1 );
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testSkipNullInputStream() {

    BufferedInputStream buf( NULL, 5 );
    CPPUNIT_ASSERT_EQUAL( 0LL, buf.skip( 0 ) );
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testMarkI() {

    ByteArrayInputStream stream;
    stream.setByteArray( (const unsigned char*)testString.c_str(), (int)testString.length() );

    BufferedInputStream is( &stream );

    unsigned char buf1[100];
    unsigned char buf2[100];

    is.skip( 3000 );
    is.mark( 1000 );
    is.read( buf1, 100 );
    is.reset();
    is.read( buf2, 100 );
    is.reset();

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to mark correct position",
                                  std::string( buf1, buf1 + 100 ),
                                  std::string( buf2, buf2 + 100 ) );

    unsigned char bytes[256];
    for( int i = 0; i < 256; i++ ) {
        bytes[i] = (unsigned char)i;
    }

    {
        ByteArrayInputStream bais;
        bais.setByteArray( bytes, 256 );
        BufferedInputStream in( &bais, 12 );

        in.skip( 6 );
        in.mark( 14 );

        unsigned char bitBucket[14];
        in.read( bitBucket, 14 );
        in.reset();
        CPPUNIT_ASSERT_MESSAGE( "Wrong bytes", in.read() == 6 && in.read() == 7 );
    }
    {
        ByteArrayInputStream bais;
        bais.setByteArray( bytes, 256 );
        BufferedInputStream in( &bais, 12 );

        in.skip( 6 );
        in.mark( 8 );
        in.skip( 7 );
        in.reset();
        CPPUNIT_ASSERT_MESSAGE( "Wrong bytes 2", in.read() == 6 && in.read() == 7 );
    }
    {
        unsigned char temp[] = { 0, 1, 2, 3, 4 };
        ByteArrayInputStream bais;
        bais.setByteArray( temp, 5 );
        BufferedInputStream buf( &bais, 2 );

        buf.mark( 3 );
        unsigned char bitBucket[3];
        int result = buf.read( bitBucket, 3 );
        CPPUNIT_ASSERT_EQUAL( 3, result );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Assert 0:", 0, (int)bytes[0] );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Assert 1:", 1, (int)bytes[1] );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Assert 2:", 2, (int)bytes[2] );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Assert 3:", 3, buf.read() );
    }
    {
        unsigned char temp[] = { 0, 1, 2, 3, 4 };
        ByteArrayInputStream bais;
        bais.setByteArray( temp, 5 );
        BufferedInputStream buf( &bais, 2 );

        buf.mark( 3 );
        unsigned char bitBucket[4];
        int result = buf.read( bitBucket, 4 );

        CPPUNIT_ASSERT_EQUAL(4, result);
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Assert 4:", 0, (int)bytes[0] );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Assert 5:", 1, (int)bytes[1] );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Assert 6:", 2, (int)bytes[2] );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Assert 7:", 3, (int)bytes[3] );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Assert 8:", 4, buf.read() );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Assert 9:", -1, buf.read() );
    }
    {
        unsigned char temp[] = { 0, 1, 2, 3, 4 };
        ByteArrayInputStream bais;
        bais.setByteArray( temp, 5 );
        BufferedInputStream buf( &bais, 2 );

        buf.mark( Integer::MAX_VALUE );
        buf.read();
        buf.close();
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testResetScenario1() {

    unsigned char input[] = { '1','2','3','4','5','6','7','8','9','0','0' };

    ByteArrayInputStream bais;
    bais.setByteArray( &input[0], 11 );

    BufferedInputStream buffis( &bais );

    buffis.read();
    buffis.mark( 5 );
    buffis.skip( 5 );
    buffis.reset();
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testResetScenario2() {

    unsigned char input[] = { '1','2','3','4','5','6','7','8','9','0','0' };

    ByteArrayInputStream bais;
    bais.setByteArray( &input[0], 11 );

    BufferedInputStream buffis( &bais );

    buffis.mark( 5 );
    buffis.skip( 6 );
    buffis.reset();
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testResetException() {

    BufferedInputStream bis( NULL );

    // throws IOException with message "Mark has been invalidated"
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw IOException",
        bis.reset(),
        IOException );

    // does not throw IOException
    bis.mark( 1 );
    bis.reset();
    bis.close();

    // throws IOException with message "stream is closed"
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw IOException",
        bis.reset(),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testReset() {

    ByteArrayInputStream stream;
    stream.setByteArray( (const unsigned char*)testString.c_str(), (int)testString.length() );

    BufferedInputStream is( &stream );

    unsigned char buf1[10];
    unsigned char buf2[10];

    is.mark( 2000 );
    is.read( buf1, 10 );
    is.reset();
    is.read( buf2, 10 );
    is.reset();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Reset failed",
                                  std::string( buf1, buf1 + 10 ),
                                  std::string( buf2, buf2 + 10 ) );

    unsigned char input[] = { '1','2','3','4','5','6','7','8','9','0' };
    ByteArrayInputStream bais;
    bais.setByteArray( &input[0], 10 );

    BufferedInputStream bIn( &bais );
    bIn.mark( 10 );

    for( int i = 0; i < 11; i++ ) {
        bIn.read();
    }

    bIn.reset();
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testSkipJ() {

    ByteArrayInputStream stream;
    stream.setByteArray( (const unsigned char*)testString.c_str(), (int)testString.length() );

    BufferedInputStream is( &stream );

    unsigned char buf1[10];
    is.mark( 2000 );
    is.skip( 1000 );
    is.read( buf1, 10 );
    is.reset();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Failed to skip to correct position",
                                  std::string( buf1, buf1 + 10 ),
                                  testString.substr( 1000, 10 ) );

    // throws IOException with message "stream is closed"
    BufferedInputStream buf( NULL, 5 );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw IOException",
        buf.skip( 10 ),
        IOException );
}
