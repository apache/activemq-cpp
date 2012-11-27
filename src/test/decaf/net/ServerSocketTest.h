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

#ifndef _DECAF_NET_SERVERSOCKETTEST_H_
#define _DECAF_NET_SERVERSOCKETTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/lang/Thread.h>

namespace decaf {
namespace net {

    class Socket;

    class ServerSocketTest  : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ServerSocketTest );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testClose );
        CPPUNIT_TEST( testAccept );
        CPPUNIT_TEST( testGetLocalPort );
        CPPUNIT_TEST( testGetSoTimeout );
        CPPUNIT_TEST( testGetReuseAddress );
        CPPUNIT_TEST( testGetReceiveBufferSize );
        CPPUNIT_TEST_SUITE_END();

    private:

        Socket* ssconn;

        decaf::lang::Thread* theThread;

    private:

        ServerSocketTest(const ServerSocketTest&);
        ServerSocketTest& operator= (const ServerSocketTest&);

    public:

        ServerSocketTest();
        virtual ~ServerSocketTest();

        void setUp();
        void tearDown();

        void testConstructor();
        void testClose();
        void testAccept();
        void testGetLocalPort();
        void testGetSoTimeout();
        void testGetReuseAddress();
        void testGetReceiveBufferSize();

    protected:

        void startClient( int port );

    };

}}

#endif /* _DECAF_NET_SERVERSOCKETTEST_H_ */
