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

#include "SSLSocketFactoryTest.h"

#include <decaf/net/ssl/SSLSocketFactory.h>
#include <decaf/io/IOException.h>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::net::ssl;

////////////////////////////////////////////////////////////////////////////////
SSLSocketFactoryTest::SSLSocketFactoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
SSLSocketFactoryTest::~SSLSocketFactoryTest() {
}

////////////////////////////////////////////////////////////////////////////////
void SSLSocketFactoryTest::testGetDefault() {

    SocketFactory* factory = SSLSocketFactory::getDefault();

    CPPUNIT_ASSERT( factory != NULL );

#ifdef HAVE_OPENSSL

    std::auto_ptr<Socket> sock( factory->createSocket() );
    CPPUNIT_ASSERT( sock.get() != NULL );

#else

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IOException",
        factory->createSocket(),
        IOException );

#endif
}
