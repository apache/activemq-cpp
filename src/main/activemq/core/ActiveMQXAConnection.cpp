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

#include "ActiveMQXAConnection.h"

#include <activemq/core/ActiveMQXASession.h>
#include <activemq/core/kernels/ActiveMQXASessionKernel.h>
#include <activemq/util/CMSExceptionSupport.h>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQXAConnection::ActiveMQXAConnection(const Pointer<transport::Transport>& transport,
                                           const Pointer<decaf::util::Properties>& properties)
  : ActiveMQConnection(transport, properties) {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQXAConnection::~ActiveMQXAConnection() {
}

////////////////////////////////////////////////////////////////////////////////
cms::XASession* ActiveMQXAConnection::createXASession() {
    return dynamic_cast<cms::XASession*>(this->createSession(cms::Session::SESSION_TRANSACTED));
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* ActiveMQXAConnection::createSession(cms::Session::AcknowledgeMode ackMode AMQCPP_UNUSED) {

    try {

        checkClosedOrFailed();
        ensureConnectionInfoSent();

        Pointer<ActiveMQXASessionKernel> session(
            new ActiveMQXASessionKernel(this, getNextSessionId(), this->getProperties()));

        this->addSession(session);

        return new ActiveMQXASession(session);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
