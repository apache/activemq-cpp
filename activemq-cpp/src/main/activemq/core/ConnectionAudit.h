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

#ifndef _ACTIVEMQ_CORE_CONNECTIONAUDIT_H_
#define _ACTIVEMQ_CORE_CONNECTIONAUDIT_H_

#include <activemq/util/Config.h>

#include <activemq/commands/Message.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace core {

    class ConnectionAuditImpl;
    class Dispatcher;

    /**
     * Provides the Auditing functionality used by Connections to attempt to
     * filter out duplicate Messages.
     *
     * @since 3.7.0
     */
    class AMQCPP_API ConnectionAudit {
    private:

        ConnectionAudit(const ConnectionAudit&);
        ConnectionAudit& operator= (const ConnectionAudit&);

    private:

        ConnectionAuditImpl* impl;

        bool checkForDuplicates;
        int auditDepth;
        int auditMaximumProducerNumber;

    public:

        ConnectionAudit();

        ConnectionAudit(int auditDepth, int maxProducers);

        ~ConnectionAudit();

    public:

        void removeDispatcher(Dispatcher* dispatcher);

        bool isDuplicate(Dispatcher* dispatcher, decaf::lang::Pointer<commands::Message> message);

        void rollbackDuplicate(Dispatcher* dispatcher, decaf::lang::Pointer<commands::Message> message);

    public:

        bool isCheckForDuplicates() const {
            return this->checkForDuplicates;
        }

        void setCheckForDuplicates(bool checkForDuplicates) {
            this->checkForDuplicates = checkForDuplicates;
        }

        int getAuditDepth() {
            return auditDepth;
        }

        void setAuditDepth(int auditDepth) {
            this->auditDepth = auditDepth;
        }

        int getAuditMaximumProducerNumber() {
            return auditMaximumProducerNumber;
        }

        void setAuditMaximumProducerNumber(int auditMaximumProducerNumber) {
            this->auditMaximumProducerNumber = auditMaximumProducerNumber;
        }

    };

}}

#endif /* _ACTIVEMQ_CORE_CONNECTIONAUDIT_H_ */
