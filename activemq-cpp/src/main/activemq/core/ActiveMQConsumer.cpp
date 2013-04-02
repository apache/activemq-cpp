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
#include "ActiveMQConsumer.h"

#include <activemq/util/Config.h>
#include <activemq/util/CMSExceptionSupport.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/kernels/ActiveMQConsumerKernel.h>
#include <activemq/core/RedeliveryPolicy.h>
#include <cms/ExceptionListener.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace core {

    class ActiveMQConsumerData {
    private:

        ActiveMQConsumerData(const ActiveMQConsumerData&);
        ActiveMQConsumerData& operator=(const ActiveMQConsumerData&);

    public:

        Pointer<ActiveMQConsumerKernel> kernel;

        ActiveMQConsumerData(const Pointer<ActiveMQConsumerKernel>& kernel) : kernel(kernel) {}
    };

}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::ActiveMQConsumer(const Pointer<ActiveMQConsumerKernel>& kernel) : MessageConsumer(), config(NULL) {

    if (kernel == NULL) {
        throw ActiveMQException(__FILE__, __LINE__,
            "ActiveMQConsumer::ActiveMQConsumer - Constructor called with NULL Kernel");
    }

    this->config = new ActiveMQConsumerData(kernel);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::~ActiveMQConsumer() {

    try {

        try {
            this->config->kernel->close();
        } catch (...) {
        }

        delete this->config;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::start() {

    try {

        if (this->config->kernel->isClosed()) {
            return;
        }

        this->config->kernel->start();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::stop() {
    this->config->kernel->stop();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumer::isClosed() const {
    return this->config->kernel->isClosed();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::close() {

    try {
        this->config->kernel->close();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConsumer::getMessageSelector() const {

    try {
        return this->config->kernel->getMessageSelector();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receive() {

    try {
        return this->config->kernel->receive();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receive(int millisecs) {

    try {
        return this->config->kernel->receive(millisecs);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receiveNoWait() {

    try {
        return this->config->kernel->receiveNoWait();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setMessageListener(cms::MessageListener* listener) {

    try {
        this->config->kernel->setMessageListener(listener);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConsumer::getMessageAvailableCount() const {
    return this->config->kernel->getMessageAvailableCount();
}

////////////////////////////////////////////////////////////////////////////////
RedeliveryPolicy* ActiveMQConsumer::getRedeliveryPolicy() const {
    return this->config->kernel->getRedeliveryPolicy();
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageListener* ActiveMQConsumer::getMessageListener() const {
    return this->config->kernel->getMessageListener();
}

////////////////////////////////////////////////////////////////////////////////
const Pointer<commands::ConsumerInfo>& ActiveMQConsumer::getConsumerInfo() const {
    return this->config->kernel->getConsumerInfo();
}

////////////////////////////////////////////////////////////////////////////////
const Pointer<commands::ConsumerId>& ActiveMQConsumer::getConsumerId() const {
    return this->config->kernel->getConsumerId();
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Exception* ActiveMQConsumer::getFailureError() const {
    return this->config->kernel->getFailureError();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setMessageTransformer(cms::MessageTransformer* transformer) {
    this->config->kernel->setMessageTransformer(transformer);
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageTransformer* ActiveMQConsumer::getMessageTransformer() const {
    return this->config->kernel->getMessageTransformer();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setMessageAvailableListener(cms::MessageAvailableListener* listener) {
    this->config->kernel->setMessageAvailableListener(listener);
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageAvailableListener* ActiveMQConsumer::getMessageAvailableListener() const {
    return this->config->kernel->getMessageAvailableListener();
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConsumer::getOptimizedAckScheduledAckInterval() const {
    return this->config->kernel->getOptimizedAckScheduledAckInterval();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setOptimizedAckScheduledAckInterval(long long value) {
    this->config->kernel->setOptimizedAckScheduledAckInterval(value);
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumer::isOptimizeAcknowledge() const {
    return this->config->kernel->isOptimizeAcknowledge();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setOptimizeAcknowledge(bool value) {
    this->config->kernel->setOptimizeAcknowledge(value);
}
