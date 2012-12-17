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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONSUMER_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONSUMER_H_

#include <cms/MessageConsumer.h>
#include <cms/MessageListener.h>
#include <cms/Message.h>
#include <cms/CMSException.h>

#include <activemq/util/Config.h>
#include <activemq/core/kernels/ActiveMQConsumerKernel.h>
#include <activemq/commands/ConsumerInfo.h>
#include <activemq/core/RedeliveryPolicy.h>

#include <decaf/lang/Pointer.h>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;

    class ActiveMQSession;
    class ActiveMQConsumerData;

    class AMQCPP_API ActiveMQConsumer : public cms::MessageConsumer {
    private:

        ActiveMQConsumerData* config;

    private:

        ActiveMQConsumer( const ActiveMQConsumer& );
        ActiveMQConsumer& operator= ( const ActiveMQConsumer& );

    public:

        /**
         * Create a new ActiveMQConsumer that contains the pointer to the Kernel
         * that implement the real MessageConsumer functionality.
         *
         * @param ActiveMQConsumerKernel
         *      This Consumer's functionality kernel.
         */
        ActiveMQConsumer(const Pointer<activemq::core::kernels::ActiveMQConsumerKernel>& kernel);

        virtual ~ActiveMQConsumer();

    public:  // Interface Implementation for cms::MessageConsumer

        virtual void start();

        virtual void stop();

        virtual void close();

        virtual cms::Message* receive();

        virtual cms::Message* receive(int millisecs);

        virtual cms::Message* receiveNoWait();

        virtual void setMessageListener(cms::MessageListener* listener);

        virtual cms::MessageListener* getMessageListener() const;

        virtual void setMessageAvailableListener(cms::MessageAvailableListener* listener);

        virtual cms::MessageAvailableListener* getMessageAvailableListener() const;

        virtual std::string getMessageSelector() const;

        virtual void setMessageTransformer(cms::MessageTransformer* transformer);

        virtual cms::MessageTransformer* getMessageTransformer() const;

    public:

        /**
         * Get the Consumer information for this consumer
         * @return Reference to a Consumer Info Object
         */
        const Pointer<commands::ConsumerInfo>& getConsumerInfo() const;

        /**
         * Get the Consumer Id for this consumer
         * @return Reference to a Consumer Id Object
         */
        const Pointer<commands::ConsumerId>& getConsumerId() const;

        /**
         * @returns if this Consumer has been closed.
         */
        bool isClosed() const;

        /**
         * @returns the number of Message's this consumer is waiting to Dispatch.
         */
        int getMessageAvailableCount() const;

        /**
         * Sets the RedeliveryPolicy this Consumer should use when a rollback is
         * performed on a transacted Consumer.  The Consumer takes ownership of the
         * passed pointer.  The Consumer's redelivery policy can never be null, a
         * call to this method with a NULL pointer is ignored.
         *
         * @param policy
         *      Pointer to a Redelivery Policy object that his Consumer will use.
         */
        void setRedeliveryPolicy(RedeliveryPolicy* policy);

        /**
         * Gets a pointer to this Consumer's Redelivery Policy object, the Consumer
         * retains ownership of this pointer so the caller should not delete it.
         *
         * @returns a Pointer to a RedeliveryPolicy that is in use by this Consumer.
         */
        RedeliveryPolicy* getRedeliveryPolicy() const;

        /**
         * Gets the error that caused this Consumer to be in a Failed state, or NULL if
         * there is no Error.
         *
         * @returns pointer to the error that faulted this Consumer or NULL.
         */
        decaf::lang::Exception* getFailureError() const;

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONSUMER_H_*/
