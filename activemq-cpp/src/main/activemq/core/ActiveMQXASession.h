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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQXASESSION_H_
#define _ACTIVEMQ_CORE_ACTIVEMQXASESSION_H_

#include <activemq/util/Config.h>

#include <cms/XASession.h>
#include <activemq/core/ActiveMQSession.h>

namespace activemq {
namespace core {

    using decaf::lang::Pointer;

    class AMQCPP_API ActiveMQXASession : public cms::XASession,
                                         public ActiveMQSession {
    public:

        ActiveMQXASession( ActiveMQConnection* connection,
                           const Pointer<commands::SessionId>& sessionId,
                           const decaf::util::Properties& properties );

        virtual ~ActiveMQXASession() throw();

    public:  // Override ActiveMQSession methods to make them XA Aware

        virtual bool isTransacted() const;

        virtual bool isAutoAcknowledge() const;

        virtual void doStartTransaction();

        virtual void commit();

        virtual void rollback();

    public:  // XASession overrides

        virtual cms::XAResource* getXAResource() const;

    };

}}

#endif /* _ACTIVEMQ_CORE_ACTIVEMQXASESSION_H_ */
