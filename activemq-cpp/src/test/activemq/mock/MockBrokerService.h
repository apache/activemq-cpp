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

#ifndef _ACTIVEMQ_MOCK_MOCKBROKERSERVICE_H_
#define _ACTIVEMQ_MOCK_MOCKBROKERSERVICE_H_

#include <activemq/util/Config.h>

#include <string>

namespace activemq {
namespace mock {

    class MockBrokerServiceImpl;

    class MockBrokerService {
    private:

        MockBrokerService(const MockBrokerService&);
        MockBrokerService& operator= (const MockBrokerService&);

    private:

        MockBrokerServiceImpl* impl;

    public:

        MockBrokerService();

        MockBrokerService(int port);

        virtual ~MockBrokerService();

    public:

        void start();

        void stop();

        void waitUntilStarted();

        void waitUntilStopped();

        std::string getConnectString() const;

    };

}}

#endif /* _ACTIVEMQ_MOCK_MOCKBROKERSERVICE_H_ */
