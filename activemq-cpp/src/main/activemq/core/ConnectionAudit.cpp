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

#include "ConnectionAudit.h"

#include <decaf/util/LinkedHashMap.h>
#include <decaf/util/StlMap.h>

#include <activemq/core/Dispatcher.h>
#include <activemq/core/ActiveMQMessageAudit.h>
#include <activemq/commands/ActiveMQDestination.h>

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

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace core {

    class ConnectionAuditImpl {
    private:

        ConnectionAuditImpl(const ConnectionAuditImpl&);
        ConnectionAuditImpl& operator= (const ConnectionAuditImpl&);

    public:

        Mutex mutex;

        StlMap<Pointer<ActiveMQDestination>, Pointer<ActiveMQMessageAudit>, ActiveMQDestination::COMPARATOR> destinations;
        LinkedHashMap<Dispatcher*, Pointer<ActiveMQMessageAudit> > dispatchers;

        ConnectionAuditImpl() : mutex(), destinations(), dispatchers(1000) {
        }
    };
}}

////////////////////////////////////////////////////////////////////////////////
ConnectionAudit::ConnectionAudit() : impl(new ConnectionAuditImpl),
                                     checkForDuplicates(true),
                                     auditDepth(ActiveMQMessageAudit::DEFAULT_WINDOW_SIZE),
                                     auditMaximumProducerNumber(ActiveMQMessageAudit::MAXIMUM_PRODUCER_COUNT) {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionAudit::ConnectionAudit(int auditDepth, int maxProducers) :
    impl(new ConnectionAuditImpl),
    checkForDuplicates(true),
    auditDepth(auditDepth),
    auditMaximumProducerNumber(maxProducers) {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionAudit::~ConnectionAudit() {
    try {
        delete this->impl;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionAudit::removeDispatcher(Dispatcher* dispatcher) {
    synchronized(&this->impl->mutex) {
        this->impl->dispatchers.remove(dispatcher);
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ConnectionAudit::isDuplicate(Dispatcher* dispatcher, Pointer<commands::Message> message) {
    synchronized(&this->impl->mutex) {
        if (checkForDuplicates && message != NULL) {
            Pointer<ActiveMQDestination> destination = message->getDestination();
            if (destination != NULL) {
                if (destination->isQueue()) {
                    Pointer<ActiveMQMessageAudit> audit;
                    try {
                        audit = this->impl->destinations.get(destination);
                    } catch (NoSuchElementException& ex) {
                        audit.reset(new ActiveMQMessageAudit(auditDepth, auditMaximumProducerNumber));
                        this->impl->destinations.put(destination, audit);
                    }
                    bool result = audit->isDuplicate(message->getMessageId());
                    return result;
                }
                Pointer<ActiveMQMessageAudit> audit;
                try {
                    audit = this->impl->dispatchers.get(dispatcher);
                } catch (NoSuchElementException& ex) {
                    audit.reset(new ActiveMQMessageAudit(auditDepth, auditMaximumProducerNumber));
                    this->impl->dispatchers.put(dispatcher, audit);
                }
                bool result = audit->isDuplicate(message->getMessageId());
                return result;
            }
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionAudit::rollbackDuplicate(Dispatcher* dispatcher, Pointer<commands::Message> message) {
    synchronized(&this->impl->mutex) {
        if (checkForDuplicates && message != NULL) {
            Pointer<ActiveMQDestination> destination = message->getDestination();
            if (destination != NULL) {
                if (destination->isQueue()) {
                    try {
                        Pointer<ActiveMQMessageAudit> audit = this->impl->destinations.get(destination);
                        audit->rollback(message->getMessageId());
                    } catch (NoSuchElementException& ex) {}
                } else {
                    try {
                        Pointer<ActiveMQMessageAudit> audit = this->impl->dispatchers.get(dispatcher);
                        audit->rollback(message->getMessageId());
                    } catch (NoSuchElementException& ex) {}
                }
            }
        }
    }
}
