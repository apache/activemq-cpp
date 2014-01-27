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

#ifndef _ACTIVEMQ_TEST_OPENWIRE_OPENWIREQUEUEBROWSERTEST_H_
#define _ACTIVEMQ_TEST_OPENWIRE_OPENWIREQUEUEBROWSERTEST_H_

#include <activemq/test/QueueBrowserTest.h>

namespace activemq {
namespace test {
namespace openwire {

    class OpenwireQueueBrowserTest : public QueueBrowserTest {
    private:

        CPPUNIT_TEST_SUITE( OpenwireQueueBrowserTest );
        CPPUNIT_TEST( testReceiveBrowseReceive );
        CPPUNIT_TEST( testBrowseReceive );
        CPPUNIT_TEST( testQueueBrowserWith2Consumers );
        CPPUNIT_TEST( testRepeatedQueueBrowserCreateDestroy );
        CPPUNIT_TEST_SUITE_END();

    public:

        OpenwireQueueBrowserTest();
        virtual ~OpenwireQueueBrowserTest();

        virtual std::string getBrokerURL() const {
            return activemq::util::IntegrationCommon::getInstance().getOpenwireURL();
        }

    };

}}}

#endif /* _ACTIVEMQ_TEST_OPENWIRE_OPENWIREQUEUEBROWSERTEST_H_ */
