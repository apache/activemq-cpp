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

#include "CommandVisitorAdapter.h"

using namespace activemq;
using namespace activemq::state;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
CommandVisitorAdapter::~CommandVisitorAdapter() {}

////////////////////////////////////////////////////////////////////////////////
Pointer<commands::Command> CommandVisitorAdapter::processTransactionInfo(commands::TransactionInfo* info) {

    if (info != decaf::lang::Pointer<commands::Command>()) {
        switch (info->getType()) {
            case core::ActiveMQConstants::TRANSACTION_STATE_BEGIN:
                return this->processBeginTransaction(info);
            case core::ActiveMQConstants::TRANSACTION_STATE_COMMITONEPHASE:
                return this->processCommitTransactionOnePhase(info);
            case core::ActiveMQConstants::TRANSACTION_STATE_COMMITTWOPHASE:
                return this->processCommitTransactionTwoPhase(info);
            case core::ActiveMQConstants::TRANSACTION_STATE_END:
                return this->processEndTransaction(info);
            case core::ActiveMQConstants::TRANSACTION_STATE_FORGET:
                return this->processForgetTransaction(info);
            case core::ActiveMQConstants::TRANSACTION_STATE_PREPARE:
                return this->processPrepareTransaction(info);
            case core::ActiveMQConstants::TRANSACTION_STATE_RECOVER:
                return this->processRecoverTransactions(info);
            case core::ActiveMQConstants::TRANSACTION_STATE_ROLLBACK:
                return this->processRollbackTransaction(info);
            default:
                throw exceptions::ActiveMQException(
                    __FILE__, __LINE__, "Unknown Transaction Info Type.");
        }
    }

    return Pointer<commands::Command>();
}

////////////////////////////////////////////////////////////////////////////////
Pointer<commands::Command> CommandVisitorAdapter::processRemoveInfo(commands::RemoveInfo* info) {

    if (info != decaf::lang::Pointer<commands::Command>()) {
        switch (info->getObjectId()->getDataStructureType()) {
            case commands::ConnectionId::ID_CONNECTIONID:
                return this->processRemoveConnection(
                    dynamic_cast<commands::ConnectionId*> (info->getObjectId().get()));
            case commands::SessionId::ID_SESSIONID:
                return this->processRemoveSession(
                    dynamic_cast<commands::SessionId*> (info->getObjectId().get()));
            case commands::ConsumerId::ID_CONSUMERID:
                return this->processRemoveConsumer(
                    dynamic_cast<commands::ConsumerId*> (info->getObjectId().get()));
            case commands::ProducerId::ID_PRODUCERID:
                return this->processRemoveProducer(
                    dynamic_cast<commands::ProducerId*> (info->getObjectId().get()));
            default:
                throw exceptions::ActiveMQException(
                    __FILE__, __LINE__, "Unknown Remove Info Type.");
        }
    }

    return Pointer<commands::Command>();
}
