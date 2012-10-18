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
#include "ActiveMQProducer.h"

#include <cms/Message.h>
#include <activemq/util/CMSExceptionSupport.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQProducer::ActiveMQProducer(Pointer<ActiveMQProducerKernel> kernel) : kernel(kernel) {

    if (kernel == NULL) {
        throw ActiveMQException(__FILE__, __LINE__,
            "ActiveMQProducer::ActiveMQProducer - Initialized with NULL Kernel");
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQProducer::~ActiveMQProducer() {
    try {
        this->kernel->close();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::close() {

    try {
        this->kernel->close();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send(cms::Message* message) {

    try {
        this->kernel->send(message);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send(cms::Message* message, cms::AsyncCallback* callback) {

    try {
        this->kernel->send(message, callback);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send(cms::Message* message, int deliveryMode, int priority, long long timeToLive) {

    try {
        this->kernel->send(message, deliveryMode, priority, timeToLive);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send(cms::Message* message, int deliveryMode, int priority, long long timeToLive, cms::AsyncCallback* callback) {

    try {
        this->kernel->send(message, deliveryMode, priority, timeToLive, callback);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send(const cms::Destination* destination, cms::Message* message) {

    try {
        this->kernel->send(destination, message);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send(const cms::Destination* destination, cms::Message* message, cms::AsyncCallback* callback) {

    try {
        this->kernel->send(destination, message, callback);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send(const cms::Destination* destination, cms::Message* message,
                            int deliveryMode, int priority, long long timeToLive) {

    try {
        this->kernel->send(destination, message, deliveryMode, priority, timeToLive);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send(const cms::Destination* destination, cms::Message* message,
                            int deliveryMode, int priority, long long timeToLive, cms::AsyncCallback* callback) {

    try {
        this->kernel->send(destination, message, deliveryMode, priority, timeToLive, callback);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
