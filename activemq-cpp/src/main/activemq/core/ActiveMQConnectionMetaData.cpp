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

#include "ActiveMQConnectionMetaData.h"

using namespace std;
using namespace activemq;
using namespace activemq::core;

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionMetaData::ActiveMQConnectionMetaData() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionMetaData::~ActiveMQConnectionMetaData() {
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnectionMetaData::getCMSVersion() const {
    return "3.2";
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnectionMetaData::getCMSMajorVersion() const {
    return 3;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnectionMetaData::getCMSMinorVersion() const {
    return 2;
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnectionMetaData::getCMSProviderName() const {
    return "activemq-cpp";
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnectionMetaData::getProviderVersion() const {
    return "3.8.3";
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnectionMetaData::getProviderMajorVersion() const {
    return 3;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnectionMetaData::getProviderMinorVersion() const {
    return 8;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnectionMetaData::getProviderPatchVersion() const {
    return 3;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> ActiveMQConnectionMetaData::getCMSXPropertyNames() const {

    std::vector<std::string> jmxProperties;

    jmxProperties.push_back("JMSXUserID");
    jmxProperties.push_back("JMSXGroupID");
    jmxProperties.push_back("JMSXGroupSeq");
    jmxProperties.push_back("JMSXDeliveryCount");
    jmxProperties.push_back("JMSXProducerTXID");

    return jmxProperties;
}
