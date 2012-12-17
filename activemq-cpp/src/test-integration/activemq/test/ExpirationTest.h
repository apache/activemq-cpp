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

#ifndef _ACTIVEMQ_TEST_EXPIRATIONTEST_H_
#define _ACTIVEMQ_TEST_EXPIRATIONTEST_H_

#include <activemq/test/CMSTestFixture.h>
#include <activemq/util/IntegrationCommon.h>

namespace activemq{
namespace test{

    class ExpirationTest : public CMSTestFixture {
    public:

        static std::string messageTag;

    public:

        ExpirationTest(){}
        virtual ~ExpirationTest(){}

        virtual void setUp() {}
        virtual void tearDown() {}

        virtual void testExpired();
        virtual void testNotExpired();

    };

}}

#endif /*_ACTIVEMQ_TEST_EXPIRATIONTEST_H_*/
