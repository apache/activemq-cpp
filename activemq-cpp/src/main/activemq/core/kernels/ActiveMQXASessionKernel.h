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

#ifndef _ACTIVEMQ_CORE_KERNELS_ACTIVEMQXASESSIONKERNEL_H_
#define _ACTIVEMQ_CORE_KERNELS_ACTIVEMQXASESSIONKERNEL_H_

#include <activemq/util/Config.h>

#include <cms/XASession.h>
#include <activemq/core/kernels/ActiveMQSessionKernel.h>

namespace activemq {
namespace core {
namespace kernels {

    using decaf::lang::Pointer;

    class AMQCPP_API ActiveMQXASessionKernel : public cms::XASession, public ActiveMQSessionKernel {
    public:

        ActiveMQXASessionKernel(ActiveMQConnection* connection,
                                const Pointer<commands::SessionId>& sessionId,
                                const decaf::util::Properties& properties);

        virtual ~ActiveMQXASessionKernel();

    public:  // Override ActiveMQSessionKernel methods to make them XA Aware

        virtual bool isTransacted() const;

        virtual bool isAutoAcknowledge() const;

        virtual void doStartTransaction();

        virtual void commit();

        virtual void rollback();

    public:  // XASession overrides

        virtual cms::XAResource* getXAResource() const;

    };

}}}

#endif /* _ACTIVEMQ_CORE_KERNELS_ACTIVEMQXASESSIONKERNEL_H_ */
