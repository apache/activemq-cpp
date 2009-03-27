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

#ifndef _DECAF_IO_BYTEARRAYOUTPUTSTREAMTEST_H_
#define _DECAF_IO_BYTEARRAYOUTPUTSTREAMTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/io/ByteArrayOutputStream.h>

namespace decaf{
namespace io{

    class ByteArrayOutputStreamTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ByteArrayOutputStreamTest );
        CPPUNIT_TEST( testStream );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testClose );
        CPPUNIT_TEST( testReset );
        CPPUNIT_TEST( testSize );
        CPPUNIT_TEST( testToByteArray );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testWrite1 );
        CPPUNIT_TEST( testWrite2 );
        CPPUNIT_TEST( testWrite3 );
        CPPUNIT_TEST( testWriteToDecaf_io_OutputStream );
        CPPUNIT_TEST_SUITE_END();

        std::string testString;

    public:

        ByteArrayOutputStreamTest() {}
        virtual ~ByteArrayOutputStreamTest() {}
        virtual void setUp(){
            testString = "Test_All_Tests\nTest_decaf_io_BufferedInputStream\nTest_BufferedOutputStream\nTest_decaf_io_ByteArrayInputStream\nTest_decaf_io_ByteArrayOutputStream\nTest_decaf_io_DataInputStream\nTest_decaf_io_File\nTest_decaf_io_FileDescriptor\nTest_decaf_io_FileInputStream\nTest_decaf_io_FileNotFoundException\nTest_decaf_io_FileOutputStream\nTest_decaf_io_FilterInputStream\nTest_decaf_io_FilterOutputStream\nTest_decaf_io_InputStream\nTest_decaf_io_IOException\nTest_decaf_io_OutputStream\nTest_decaf_io_PrintStream\nTest_decaf_io_RandomAccessFile\nTest_decaf_io_SyncFailedException\nTest_decaf_lang_AbstractMethodError\nTest_decaf_lang_ArithmeticException\nTest_decaf_lang_ArrayIndexOutOfBoundsException\nTest_decaf_lang_ArrayStoreException\nTest_decaf_lang_Boolean\nTest_decaf_lang_Byte\nTest_decaf_lang_Character\nTest_decaf_lang_Class\nTest_decaf_lang_ClassCastException\nTest_decaf_lang_ClassCircularityError\nTest_decaf_lang_ClassFormatError\nTest_decaf_lang_ClassLoader\nTest_decaf_lang_ClassNotFoundException\nTest_decaf_lang_CloneNotSupportedException\nTest_decaf_lang_Double\nTest_decaf_lang_Error\nTest_decaf_lang_Exception\nTest_decaf_lang_ExceptionInInitializerError\nTest_decaf_lang_Float\nTest_decaf_lang_IllegalAccessError\nTest_decaf_lang_IllegalAccessException\nTest_decaf_lang_IllegalArgumentException\nTest_decaf_lang_IllegalMonitorStateException\nTest_decaf_lang_IllegalThreadStateException\nTest_decaf_lang_IncompatibleClassChangeError\nTest_decaf_lang_IndexOutOfBoundsException\nTest_decaf_lang_InstantiationError\nTest_decaf_lang_InstantiationException\nTest_decaf_lang_Integer\nTest_decaf_lang_InternalError\nTest_decaf_lang_InterruptedException\nTest_decaf_lang_LinkageError\nTest_decaf_lang_Long\nTest_decaf_lang_Math\nTest_decaf_lang_NegativeArraySizeException\nTest_decaf_lang_NoClassDefFoundError\nTest_decaf_lang_NoSuchFieldError\nTest_decaf_lang_NoSuchMethodError\nTest_decaf_lang_NullPointerException\nTest_decaf_lang_Number\nTest_decaf_lang_NumberFormatException\nTest_decaf_lang_Object\nTest_decaf_lang_OutOfMemoryError\nTest_decaf_lang_RuntimeException\nTest_decaf_lang_SecurityManager\nTest_decaf_lang_Short\nTest_decaf_lang_StackOverflowError\nTest_decaf_lang_String\nTest_decaf_lang_StringBuffer\nTest_decaf_lang_StringIndexOutOfBoundsException\nTest_decaf_lang_System\nTest_decaf_lang_Thread\nTest_decaf_lang_ThreadDeath\nTest_decaf_lang_ThreadGroup\nTest_decaf_lang_Throwable\nTest_decaf_lang_UnknownError\nTest_decaf_lang_UnsatisfiedLinkError\nTest_decaf_lang_VerifyError\nTest_decaf_lang_VirtualMachineError\nTest_decaf_lang_vm_Image\nTest_decaf_lang_vm_MemorySegment\nTest_decaf_lang_vm_ROMStoreException\nTest_decaf_lang_vm_VM\nTest_decaf_lang_Void\nTest_decaf_net_BindException\nTest_decaf_net_ConnectException\nTest_decaf_net_DatagramPacket\nTest_decaf_net_DatagramSocket\nTest_decaf_net_DatagramSocketImpl\nTest_decaf_net_InetAddress\nTest_decaf_net_NoRouteToHostException\nTest_decaf_net_PlainDatagramSocketImpl\nTest_decaf_net_PlainSocketImpl\nTest_decaf_net_Socket\nTest_decaf_net_SocketException\nTest_decaf_net_SocketImpl\nTest_decaf_net_SocketInputStream\nTest_decaf_net_SocketOutputStream\nTest_decaf_net_UnknownHostException\nTest_decaf_util_ArrayEnumerator\nTest_decaf_util_Date\nTest_decaf_util_EventObject\nTest_decaf_util_HashEnumerator\nTest_decaf_util_Hashtable\nTest_decaf_util_Properties\nTest_decaf_util_ResourceBundle\nTest_decaf_util_tm\nTest_decaf_util_Vector\n";
        }
        virtual void tearDown(){}

        void testStream();
        void testConstructor1();
        void testConstructor2();
        void testClose();
        void testReset();
        void testSize();
        void testToByteArray();
        void testToString();
        void testWrite1();
        void testWrite2();
        void testWrite3();
        void testWriteToDecaf_io_OutputStream();

    };

}}

#endif /*_DECAF_IO_BYTEARRAYOUTPUTSTREAMTEST_H_*/
