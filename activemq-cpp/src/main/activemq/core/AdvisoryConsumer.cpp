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

#include "AdvisoryConsumer.h"

#include <activemq/core/ActiveMQConstants.h>
#include <activemq/util/AdvisorySupport.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/lang/exceptions/ClassCastException.h>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::commands;
using namespace activemq::exceptions;

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace core {

    class AdvisoryConsumerConfig {
    public:

        int deliveredCounter;
        Pointer<ConsumerInfo> info;
        AtomicBoolean closed;
        int hashCode;

        AdvisoryConsumerConfig() : deliveredCounter(0), info(), closed(false), hashCode() {
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
AdvisoryConsumer::AdvisoryConsumer(ActiveMQConnection* connection, Pointer<commands::ConsumerId> consumerId) :
    Dispatcher(), config(new AdvisoryConsumerConfig()), connection(connection) {

    if (connection == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Parent Connection pointer was NULL");
    }

    this->config->info.reset(new ConsumerInfo());

    this->config->info->setConsumerId(consumerId);
    this->config->info->setDestination(
        Pointer<ActiveMQDestination>(AdvisorySupport::getTempDestinationCompositeAdvisoryTopic()));
    this->config->info->setPrefetchSize(1000);
    this->config->info->setNoLocal(true);
    this->config->info->setDispatchAsync(true);

    this->config->hashCode = consumerId->getHashCode();

    try {
        this->connection->addDispatcher(this->config->info->getConsumerId(), this);
        this->connection->syncRequest(this->config->info);
    } catch(...) {
        delete this->config;
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
AdvisoryConsumer::~AdvisoryConsumer() {
    try {
        delete config;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void AdvisoryConsumer::dispose() {

    if (!this->config->closed.compareAndSet(false, true)) {

        try {
            this->connection->oneway(this->config->info->createRemoveCommand());
        } catch (cms::CMSException& e) {
        }

        this->connection->removeDispatcher(this->config->info->getConsumerId());
    }
}

////////////////////////////////////////////////////////////////////////////////
void AdvisoryConsumer::dispatch(const Pointer<MessageDispatch>& message) {

    // Auto ack messages when we reach 75% of the prefetch
    this->config->deliveredCounter++;
    if (this->config->deliveredCounter > (0.75 * this->config->info->getPrefetchSize())) {
        try {

            Pointer<MessageAck> ack(new MessageAck());

            ack->setAckType(ActiveMQConstants::ACK_TYPE_CONSUMED);
            ack->setConsumerId(this->config->info->getConsumerId());
            ack->setDestination(message->getDestination());
            ack->setMessageCount(this->config->deliveredCounter);
            ack->setLastMessageId(message->getMessage()->getMessageId());

            this->connection->oneway(ack);

            this->config->deliveredCounter = 0;
        } catch (Exception& e) {
            this->connection->onClientInternalException(e);
        }
    }

    Pointer<DataStructure> object = message->getMessage()->getDataStructure();
    if (object != NULL) {
        try {
            Pointer<DestinationInfo> info = object.dynamicCast<DestinationInfo>();
            processDestinationInfo(info);
        } catch (ClassCastException& ex) {
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void AdvisoryConsumer::processDestinationInfo(Pointer<commands::DestinationInfo> info) {

    Pointer<ActiveMQDestination> dest = info->getDestination();
    if (!dest->isTemporary()) {
        return;
    }

    Pointer<ActiveMQTempDestination> tempDest = dest.dynamicCast<ActiveMQTempDestination>();
    if (info->getOperationType() == ActiveMQConstants::DESTINATION_ADD_OPERATION) {
        this->connection->addTempDestination(tempDest);
    } else if (info->getOperationType() == ActiveMQConstants::DESTINATION_REMOVE_OPERATION) {
        this->connection->removeTempDestination(tempDest);
    }
}

////////////////////////////////////////////////////////////////////////////////
int AdvisoryConsumer::getHashCode() const {
   return this->config->hashCode;
}
