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

#ifndef _DECAF_NET_SOCKETTEST_H_
#define _DECAF_NET_SOCKETTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf{
namespace net{

    class SocketTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( SocketTest );
        //CPPUNIT_TEST( testConnectUnknownHost );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testGetReuseAddress );
        CPPUNIT_TEST( testClose );
        CPPUNIT_TEST( testGetPort );
        CPPUNIT_TEST( testGetInputStream );
        CPPUNIT_TEST( testGetOutputStream );
        CPPUNIT_TEST( testGetKeepAlive );
        CPPUNIT_TEST( testGetLocalPort );
        CPPUNIT_TEST( testGetSoLinger );
        CPPUNIT_TEST( testGetSoTimeout );
        CPPUNIT_TEST( testGetTcpNoDelay );
        CPPUNIT_TEST( testIsConnected );
        CPPUNIT_TEST( testIsClosed );
        CPPUNIT_TEST( testIsInputShutdown );
        CPPUNIT_TEST( testIsOutputShutdown );
        CPPUNIT_TEST( testConnectPortOutOfRange );

        CPPUNIT_TEST( testTx );
        CPPUNIT_TEST( testTrx );
        CPPUNIT_TEST( testTrxNoDelay );
        CPPUNIT_TEST( testRxFail );
        CPPUNIT_TEST_SUITE_END();

    public:

        virtual ~SocketTest() {}

        void testConnectUnknownHost();
        void testConstructor();
        void testGetReuseAddress();
        void testClose();
        void testGetPort();
        void testGetInputStream();
        void testGetOutputStream();
        void testGetKeepAlive();
        void testGetLocalPort();
        void testGetSoLinger();
        void testGetSoTimeout();
        void testGetTcpNoDelay();
        void testIsConnected();
        void testIsClosed();
        void testIsInputShutdown();
        void testIsOutputShutdown();
        void testConnectPortOutOfRange();

        // Old Tests
        void testConnect();
        void testTx();
        void testTrx();
        void testRxFail();
        void testTrxNoDelay();

    };

}}

#endif /*_DECAF_NET_SOCKETTEST_H_*/
