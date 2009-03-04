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

#include "MockTransportFactoryTest.h"

#include <activemq/transport/mock/MockTransportFactory.h>
#include <activemq/transport/mock/MockTransport.h>
#include <decaf/net/URI.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::mock;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void MockTransportFactoryTest::test() {

    URI uri( "mock://mock?wireformat=openwire" );

    MockTransportFactory factory;

    Pointer<Transport> transport( factory.create( uri ) );

    CPPUNIT_ASSERT( transport.get() != NULL );

    transport = factory.createComposite( uri );

    CPPUNIT_ASSERT( transport.get() != NULL );

    transport.reset( NULL );

}
