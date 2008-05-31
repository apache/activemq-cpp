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

#include "CmsAccessorTest.h"
#include <activemq/cmsutil/DynamicDestinationResolver.h>
#include <activemq/cmsutil/ResourceLifecycleManager.h>
#include "DummyConnectionFactory.h"

using namespace activemq;
using namespace activemq::cmsutil;

////////////////////////////////////////////////////////////////////////////////
void CmsAccessorTest::setUp() {
    cf = new DummyConnectionFactory();
    accessor = new MyAccessor();
    accessor->setConnectionFactory(cf);
}

////////////////////////////////////////////////////////////////////////////////
void CmsAccessorTest::tearDown() {
    delete accessor;
    delete cf;
}

////////////////////////////////////////////////////////////////////////////////
void CmsAccessorTest::testConnectionFactory() {

    CPPUNIT_ASSERT(accessor->getConnectionFactory() == cf);
}

////////////////////////////////////////////////////////////////////////////////
void CmsAccessorTest::testAckMode() {

    CPPUNIT_ASSERT(accessor->getSessionAcknowledgeMode() == cms::Session::AUTO_ACKNOWLEDGE);

    accessor->setSessionAcknowledgeMode(cms::Session::CLIENT_ACKNOWLEDGE);

    CPPUNIT_ASSERT(accessor->getSessionAcknowledgeMode() == cms::Session::CLIENT_ACKNOWLEDGE) ;
}

////////////////////////////////////////////////////////////////////////////////
void CmsAccessorTest::testCreateResources() {

    cms::Connection* c = accessor->createConnection();
    CPPUNIT_ASSERT( c != NULL);

    cms::Session* s = accessor->createSession(c);
    CPPUNIT_ASSERT( s != NULL);

    CPPUNIT_ASSERT(s->getAcknowledgeMode() == cms::Session::AUTO_ACKNOWLEDGE);

    accessor->setSessionAcknowledgeMode(cms::Session::CLIENT_ACKNOWLEDGE);

    s = accessor->createSession(c);
    CPPUNIT_ASSERT( s != NULL);

    CPPUNIT_ASSERT(s->getAcknowledgeMode() == cms::Session::CLIENT_ACKNOWLEDGE);
}

