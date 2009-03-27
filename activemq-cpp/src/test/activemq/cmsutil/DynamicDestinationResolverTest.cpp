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

#include "DynamicDestinationResolverTest.h"
#include <activemq/cmsutil/DynamicDestinationResolver.h>
#include <activemq/cmsutil/ResourceLifecycleManager.h>
#include "DummySession.h"

using namespace activemq;
using namespace activemq::cmsutil;

////////////////////////////////////////////////////////////////////////////////
void DynamicDestinationResolverTest::testTopics() {

    ResourceLifecycleManager mgr;
    DynamicDestinationResolver resolver;
    resolver.init(&mgr);

    DummySession session(NULL);

    // Test topic
    cms::Destination* testTopic = dynamic_cast<cms::Topic*>(resolver.resolveDestinationName(&session,
            (std::string)"test", true ));

    CPPUNIT_ASSERT(testTopic != NULL);

    // Hello topic
    cms::Destination* helloTopic = resolver.resolveDestinationName(&session,
            (std::string)"hello", true );

    CPPUNIT_ASSERT(helloTopic != NULL);
    CPPUNIT_ASSERT(helloTopic != testTopic);

    cms::Destination* testTopic2 = dynamic_cast<cms::Topic*>(resolver.resolveDestinationName(&session,
                (std::string)"test", true ));

    CPPUNIT_ASSERT(testTopic2 != NULL);
    CPPUNIT_ASSERT(testTopic == testTopic2);

    mgr.destroy();

}

////////////////////////////////////////////////////////////////////////////////
void DynamicDestinationResolverTest::testQueues() {

    ResourceLifecycleManager mgr;
    DynamicDestinationResolver resolver;
    resolver.init(&mgr);

    DummySession session(NULL);

    // Queue topic
    cms::Destination* testQueue = dynamic_cast<cms::Queue*>(resolver.resolveDestinationName(&session,
            (std::string)"test", false ));

    CPPUNIT_ASSERT(testQueue != NULL);

    // Hello queue
    cms::Destination* helloQueue = resolver.resolveDestinationName(&session,
            (std::string)"hello", false );

    CPPUNIT_ASSERT(helloQueue != NULL);
    CPPUNIT_ASSERT(helloQueue != testQueue);

    cms::Destination* testQueue2 = dynamic_cast<cms::Queue*>(resolver.resolveDestinationName(&session,
                (std::string)"test", false ));

    CPPUNIT_ASSERT(testQueue2 != NULL);
    CPPUNIT_ASSERT(testQueue == testQueue2);

    mgr.destroy();
}
