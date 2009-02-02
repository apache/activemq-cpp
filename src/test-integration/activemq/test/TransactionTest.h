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

#ifndef _ACTIVEMQ_TEST_TRANSACTIONTEST_H_
#define _ACTIVEMQ_TEST_TRANSACTIONTEST_H_

#include <activemq/test/CMSTestFixture.h>
#include <activemq/util/IntegrationCommon.h>

namespace activemq{
namespace test{

    class TransactionTest : public CMSTestFixture {
    private:

        static const int batchCount = 10;
        static const int batchSize = 20;

    public:

        TransactionTest() {}
        virtual ~TransactionTest() {}

        virtual void setUp() {
            cmsProvider.reset(
                new util::CMSProvider( getBrokerURL(), cms::Session::SESSION_TRANSACTED ) );
        };

        void testSendReceiveTransactedBatches();
        void testSendRollback();
        void testSendSessionClose();
        void testWithTTLSet();

    };

}}

#endif /*_ACTIVEMQ_TEST_TRANSACTIONTEST_H_*/
