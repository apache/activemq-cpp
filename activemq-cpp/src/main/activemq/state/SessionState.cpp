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

#include "SessionState.h"

#include <activemq/state/TransactionState.h>

#include <decaf/lang/exceptions/IllegalStateException.h>

using namespace activemq;
using namespace activemq::state;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
SessionState::SessionState(Pointer<SessionInfo> info) :
    info(info), producers(), consumers(), disposed(false) {
}

////////////////////////////////////////////////////////////////////////////////
SessionState::~SessionState() {
    try {
        this->shutdown();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
std::string SessionState::toString() const {

    if (this->info.get() != NULL) {
        return this->info->toString();
    }

    return "NULL";
}

////////////////////////////////////////////////////////////////////////////////
void SessionState::shutdown() {
    this->disposed.set(true);
    this->producers.clear();
    this->consumers.clear();
}

////////////////////////////////////////////////////////////////////////////////
void SessionState::checkShutdown() const {
    if (this->disposed.get()) {
        throw decaf::lang::exceptions::IllegalStateException(
            __FILE__, __LINE__, "Session already Disposed");
    }
}

////////////////////////////////////////////////////////////////////////////////
void SessionState::addProducer(Pointer<ProducerInfo> info) {
    checkShutdown();
    producers.put(info->getProducerId(), Pointer<ProducerState>(new ProducerState(info)));
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ProducerState> SessionState::removeProducer(Pointer<ProducerId> id) {
    Pointer<ProducerState> producerState = producers.remove(id);
    if (producerState != NULL) {
        Pointer<TransactionState> transactionState = producerState->getTransactionState();
        if (transactionState != NULL) {
            // allow the transaction to recreate dependent producer on recovery, we
            // hand off the producer state to the Transaction and NULL the producer's
            // reference to avoid a circular link to it.
            producerState->setTransactionState(Pointer<TransactionState>());
            transactionState->addProducerState(producerState);
        }
    }

    return producerState;
}

////////////////////////////////////////////////////////////////////////////////
void SessionState::addConsumer(Pointer<ConsumerInfo> info) {
    checkShutdown();
    consumers.put(info->getConsumerId(), Pointer<ConsumerState>(new ConsumerState(info)));
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ConsumerState> SessionState::removeConsumer(Pointer<ConsumerId> id) {
    return consumers.remove(id);
}
