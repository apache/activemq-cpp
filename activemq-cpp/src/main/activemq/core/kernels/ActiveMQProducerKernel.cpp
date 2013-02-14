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

#include "ActiveMQProducerKernel.h"

#include <cms/Message.h>
#include <activemq/core/kernels/ActiveMQSessionKernel.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/util/CMSExceptionSupport.h>
#include <activemq/util/ActiveMQProperties.h>
#include <activemq/util/ActiveMQMessageTransformation.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Boolean.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQProducerKernel::ActiveMQProducerKernel(ActiveMQSessionKernel* session,
                                               const Pointer<commands::ProducerId>& producerId,
                                               const Pointer<ActiveMQDestination>& destination,
                                               long long sendTimeout) : disableTimestamps(false),
                                                                        disableMessageId(false),
                                                                        defaultDeliveryMode(cms::Message::DEFAULT_DELIVERY_MODE),
                                                                        defaultPriority(cms::Message::DEFAULT_MSG_PRIORITY),
                                                                        defaultTimeToLive(cms::Message::DEFAULT_TIME_TO_LIVE),
                                                                        sendTimeout(sendTimeout),
                                                                        session(session),
                                                                        producerInfo(),
                                                                        closed(false),
                                                                        memoryUsage(),
                                                                        destination(),
                                                                        messageSequence(),
                                                                        transformer() {

    if (session == NULL || producerId == NULL) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQProducerKernel::ActiveMQProducerKernel - Init with NULL Session" );
    }

    this->producerInfo.reset(new ProducerInfo());

    this->producerInfo->setProducerId(producerId);
    this->producerInfo->setDestination(destination);
    this->producerInfo->setWindowSize(session->getConnection()->getProducerWindowSize());

    // Get any options specified in the destination and apply them to the
    // ProducerInfo object.
    if (destination != NULL) {
        const ActiveMQProperties& options = destination->getOptions();
        this->producerInfo->setDispatchAsync(
            Boolean::parseBoolean(options.getProperty("producer.dispatchAsync", "false")));

        this->destination = destination.dynamicCast<cms::Destination>();
    }

    // TODO - Check for need of MemoryUsage if there's a producer Windows size
    //        and the Protocol version is greater than 3.
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQProducerKernel::~ActiveMQProducerKernel() {
    try {
        close();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::close() {

    try {

        if (!this->isClosed()) {

            dispose();

            // Remove at the Broker Side, if this fails the producer has already
            // been removed from the session and connection objects so its safe
            // for an exception to be thrown.
            Pointer<RemoveInfo> info(new RemoveInfo);
            info->setObjectId(this->producerInfo->getProducerId());
            this->session->oneway(info);

            this->closed = true;
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::dispose() {

    if (!this->isClosed()) {
        Pointer<ActiveMQProducerKernel> producer(this);
        try {
            this->session->removeProducer(producer);
        } catch(Exception& e) {
            producer.release();
            throw;
        }
        producer.release();
        this->closed = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::send(cms::Message* message) {

    try {
        this->checkClosed();
        this->send(this->destination.get(), message, defaultDeliveryMode, defaultPriority, defaultTimeToLive, NULL);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::send(cms::Message* message, cms::AsyncCallback* callback) {

    try {
        this->checkClosed();
        this->send(this->destination.get(), message, defaultDeliveryMode, defaultPriority, defaultTimeToLive, callback);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::send(cms::Message* message, int deliveryMode, int priority, long long timeToLive) {

    try {
        this->checkClosed();
        this->send(this->destination.get(), message, deliveryMode, priority, timeToLive, NULL);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::send(cms::Message* message, int deliveryMode, int priority, long long timeToLive, cms::AsyncCallback* callback) {

    try {
        this->checkClosed();
        this->send(this->destination.get(), message, deliveryMode, priority, timeToLive, callback);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::send(const cms::Destination* destination, cms::Message* message) {

    try {
        this->checkClosed();
        this->send(destination, message, defaultDeliveryMode, defaultPriority, defaultTimeToLive, NULL);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::send(const cms::Destination* destination, cms::Message* message, cms::AsyncCallback* callback) {

    try {
        this->checkClosed();
        this->send(destination, message, defaultDeliveryMode, defaultPriority, defaultTimeToLive, callback);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::send(const cms::Destination* destination, cms::Message* message,
                                  int deliveryMode, int priority, long long timeToLive) {

    try {
        this->checkClosed();
        this->send(destination, message, deliveryMode, priority, timeToLive, NULL);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::send(const cms::Destination* destination, cms::Message* message,
                                  int deliveryMode, int priority, long long timeToLive, cms::AsyncCallback* onComplete) {

    try {

        this->checkClosed();

        if (destination == NULL) {

            if (this->producerInfo->getDestination() == NULL) {
                throw cms::UnsupportedOperationException("A destination must be specified.", NULL);
            }

            throw cms::InvalidDestinationException("Don't understand null destinations", NULL);
        }

        Pointer<ActiveMQDestination> dest;
        const ActiveMQDestination* transformed;

        if (destination == this->destination.get()) {
            dest = this->producerInfo->getDestination();
        } else if (this->producerInfo->getDestination() == NULL) {
            // We always need to use a copy of the users destination since we want to control
            // its lifetime.  If the transform results in a new destination we can use that, but
            // if its already an ActiveMQDestination then we need to clone it.
            if (ActiveMQMessageTransformation::transformDestination(destination, &transformed)) {
                dest.reset(const_cast<ActiveMQDestination*>(transformed));
            } else {
                dest.reset(transformed->cloneDataStructure());
            }
        } else {
            throw cms::UnsupportedOperationException(
                string("This producer can only send messages to: ") +
                this->producerInfo->getDestination()->getPhysicalName(), NULL);
        }

        if (dest == NULL) {
            throw cms::CMSException("No destination specified", NULL);
        }

        cms::Message* outbound = message;
        Pointer<cms::Message> scopedMessage;
        if (this->transformer != NULL) {
            if (this->transformer->producerTransform(this->session, this, message, &outbound)) {
                // scopedMessage ensures that when we are responsible for the lifetime of the
                // transformed message, the message remains valid until the send operation either
                // succeeds or throws an exception.
                scopedMessage.reset(outbound);
            }
            if (outbound == NULL) {
                throw NullPointerException(__FILE__, __LINE__, "MessageTransformer set transformed message to NULL");
            }
        }

        if (this->memoryUsage.get() != NULL) {
            try {
                this->memoryUsage->waitForSpace();
            } catch (InterruptedException& e) {
                throw cms::CMSException("Send aborted due to thread interrupt.");
            }
        }

        this->session->send(this, dest, outbound, deliveryMode, priority, timeToLive,
                            this->memoryUsage.get(), this->sendTimeout, onComplete);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::onProducerAck(const commands::ProducerAck& ack) {

    try{

        if (this->memoryUsage.get() != NULL) {
            this->memoryUsage->decreaseUsage(ack.getSize());
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducerKernel::checkClosed() const {
    if (closed) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQProducerKernel - Producer Already Closed" );
    }
}

