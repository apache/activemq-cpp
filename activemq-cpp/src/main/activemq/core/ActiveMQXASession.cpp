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

#include "ActiveMQXASession.h"

#include <cms/TransactionInProgressException.h>
#include <activemq/core/ActiveMQTransactionContext.h>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
ActiveMQXASession::ActiveMQXASession(Pointer<ActiveMQXASessionKernel> kernel) :
    ActiveMQSession(kernel), xaKernel(kernel) {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQXASession::~ActiveMQXASession() {
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQXASession::isTransacted() const {
    return this->xaKernel->isTransacted();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQXASession::isAutoAcknowledge() const {
    // Force this to always be true so the Session acts like an Auto Ack session
    // when there is no active XA Transaction.
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQXASession::doStartTransaction() {
    // Controlled by the XAResource so this method is now a No-op.
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQXASession::commit() {
    throw cms::TransactionInProgressException("Cannot commit inside an XASession");
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQXASession::rollback() {
    throw cms::TransactionInProgressException("Cannot rollback inside an XASession");
}

////////////////////////////////////////////////////////////////////////////////
cms::XAResource* ActiveMQXASession::getXAResource() const {
    return this->xaKernel->getXAResource();
}
