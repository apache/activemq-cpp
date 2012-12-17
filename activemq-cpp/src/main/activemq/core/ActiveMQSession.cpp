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
#include "ActiveMQSession.h"

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/core/ActiveMQQueueBrowser.h>
#include <activemq/util/CMSExceptionSupport.h>

#include <activemq/commands/ActiveMQTempDestination.h>

#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQSession::ActiveMQSession(Pointer<ActiveMQSessionKernel> kernel) : cms::Session(), kernel(kernel) {

    if (kernel == NULL) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQSession::ActiveMQSession - Constructor called with NULL session kernel");
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSession::~ActiveMQSession() {
    try {
        this->kernel->close();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::close() {
    try {
        this->kernel->close();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::commit() {
    try {
        this->kernel->commit();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::rollback() {
    try {
        this->kernel->rollback();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::recover() {
    try {
        this->kernel->recover();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(const cms::Destination* destination) {
    try {
        return this->kernel->createConsumer(destination, "", false);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(const cms::Destination* destination, const std::string& selector) {
    try {
        return this->kernel->createConsumer(destination, selector, false);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(const cms::Destination* destination, const std::string& selector, bool noLocal) {
    try {
        return this->kernel->createConsumer(destination, selector, noLocal);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createDurableConsumer(const cms::Topic* destination, const std::string& name,
                                                             const std::string& selector, bool noLocal) {
    try {
        return this->kernel->createDurableConsumer(destination, name, selector, noLocal);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* ActiveMQSession::createProducer(const cms::Destination* destination) {
    try {
        return this->kernel->createProducer(destination);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::QueueBrowser* ActiveMQSession::createBrowser(const cms::Queue* queue) {
    try {
        return this->kernel->createBrowser(queue);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::QueueBrowser* ActiveMQSession::createBrowser(const cms::Queue* queue, const std::string& selector) {
    try {
        return this->kernel->createBrowser(queue, selector);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Queue* ActiveMQSession::createQueue(const std::string& queueName) {
    try {
        return this->kernel->createQueue(queueName);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Topic* ActiveMQSession::createTopic(const std::string& topicName) {
    try {
        return this->kernel->createTopic(topicName);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryQueue* ActiveMQSession::createTemporaryQueue() {
    try {
        return this->kernel->createTemporaryQueue();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryTopic* ActiveMQSession::createTemporaryTopic() {
    try {
        return this->kernel->createTemporaryTopic();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQSession::createMessage() {
    try {
        return this->kernel->createMessage();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSession::createBytesMessage() {
    try {
        return this->kernel->createBytesMessage();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSession::createBytesMessage(const unsigned char* bytes, int bytesSize) {
    try {
        return this->kernel->createBytesMessage(bytes, bytesSize);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::StreamMessage* ActiveMQSession::createStreamMessage() {
    try {
        return this->kernel->createStreamMessage();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* ActiveMQSession::createTextMessage() {
    try {
        return this->kernel->createTextMessage();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* ActiveMQSession::createTextMessage(const std::string& text) {
    try {
        return this->kernel->createTextMessage(text);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MapMessage* ActiveMQSession::createMapMessage() {
    try {
        return this->kernel->createMapMessage();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::unsubscribe(const std::string& name) {
    try{
        this->kernel->unsubscribe(name);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::start() {
    try{
        this->kernel->start();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::stop() {
    try{
        this->kernel->stop();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
