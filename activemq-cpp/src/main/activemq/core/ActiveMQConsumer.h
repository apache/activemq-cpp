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
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/commands/ConsumerInfo.h>
#include <activemq/commands/MessageAck.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <activemq/core/ActiveMQTransactionContext.h>
#include <activemq/core/Dispatcher.h>

#include <decaf/util/concurrent/atomic/AtomicReference.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/StlQueue.h>
#include <decaf/util/concurrent/Mutex.h>
#include <memory>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;
    using decaf::util::concurrent::atomic::AtomicReference;
    using decaf::util::concurrent::atomic::AtomicBoolean;

    class ActiveMQSession;

    class AMQCPP_API ActiveMQConsumer :
        public cms::MessageConsumer,
        public ActiveMQAckHandler,
        public Dispatcher
    {
    private:

        /**
         * The session that owns this Consumer
         */
        ActiveMQSession* session;

        /**
         * The Transaction Context, null if not in a Transacted Session.
         */
        Pointer<ActiveMQTransactionContext> transaction;

        /**
         * The Consumer info for this Consumer
         */
        Pointer<commands::ConsumerInfo> consumerInfo;

        /**
         * The Message Listener for this Consumer
         */
        AtomicReference<cms::MessageListener> listener;

        /**
         * Is the consumer currently delivering acks.
         */
        AtomicBoolean deliveringAcks;

        /**
         * Queue of unconsumed messages.
         */
        decaf::util::StlQueue<DispatchData> unconsumedMessages;

        /**
         * Queue of consumed messages.
         */
        decaf::util::StlQueue< decaf::lang::Pointer<commands::Message> > dispatchedMessages;

        /**
         * The last delivered message's BrokerSequenceId.
         */
        long long lastDeliveredSequenceId;

        /**
         * Next Ack to go out.
         */
        Pointer<commands::MessageAck> pendingAck;

        /**
         * How many message's have been delivered so far since the last Ack was sent.
         */
        int deliveredCounter;

        /**
         * How big to grow the ack window next time.
         */
        int additionalWindowSize;

        /**
         * Time to wait before restarting delivery of rollback messages.
         */
        long redeliveryDelay;

        /**
         * Has the Synchronization been added for this transaction
         */
        volatile bool synchronizationRegistered;

        /**
         * Boolean that indicates if the consumer has been closed
         */
        bool closed;

    public:

        /**
         * Constructor
         */
        ActiveMQConsumer( const Pointer<commands::ConsumerInfo>& consumerInfo,
                          ActiveMQSession* session,
                          const Pointer<ActiveMQTransactionContext>& transaction );

        virtual ~ActiveMQConsumer();

    public:  // Interface Implementation

        /**
         * Closes the Consumer.  This will return all allocated resources
         * and purge any outstanding messages.  This method will block if
         * there is a call to receive in progress, or a dispatch to a
         * MessageListener in place
         * @throws CMSException
         */
        virtual void close() throw ( cms::CMSException );

        /**
         * Synchronously Receive a Message
         * @return new message
         * @throws CMSException
         */
        virtual cms::Message* receive() throw ( cms::CMSException );

        /**
         * Synchronously Receive a Message, time out after defined interval.
         * Returns null if nothing read.
         * @param millisecs the time in milliseconds to wait before returning
         * @return new message or null on timeout
         * @throws CMSException
         */
        virtual cms::Message* receive( int millisecs ) throw ( cms::CMSException );

        /**
         * Receive a Message, does not wait if there isn't a new message
         * to read, returns NULL if nothing read.
         * @return new message
         * @throws CMSException
         */
        virtual cms::Message* receiveNoWait() throw ( cms::CMSException );

        /**
         * Sets the MessageListener that this class will send notifs on
         * @param listener MessageListener interface pointer
         */
        virtual void setMessageListener( cms::MessageListener* listener );

        /**
         * Gets the MessageListener that this class will send notifs on
         * @param MessageListener interface pointer
         */
        virtual cms::MessageListener* getMessageListener() const {
            return this->listener.get();
        }

        /**
         * Gets this message consumer's message selector expression.
         * @return This Consumer's selector expression or "".
         * @throws cms::CMSException
         */
        virtual std::string getMessageSelector() const
            throw ( cms::CMSException );

        /**
         * Method called to acknowledge the message passed, called from a message
         * when the mode is client ack.
         * @param message the Message to Acknowledge
         * @throw CMSException
         */
        virtual void acknowledgeMessage( const commands::Message* message )
            throw ( cms::CMSException );

    public:  // Dispatcher Methods

        /**
         * Called asynchronously by the session to dispatch a message.
         * @param message object pointer
         */
        virtual void dispatch( DispatchData& message );

    public:  // ActiveMQConsumer Methods

        /**
         * Method called to acknowledge all messages that have been received so far.
         * @throw CMSException
         */
        void acknowledge() throw ( cms::CMSException );

        /**
         * Called to Commit the current set of messages in this Transaction
         * @throw ActiveMQException
         */
        void commit() throw ( exceptions::ActiveMQException );

        /**
         * Called to Roll back the current set of messages in this Transaction
         * @throw ActiveMQException
         */
        void rollback() throw ( exceptions::ActiveMQException );

        /**
         * Performs the actual close operation on this consumer
         * @throw ActiveMQException
         */
        void doClose() throw ( exceptions::ActiveMQException );

        /**
         * Get the Consumer information for this consumer
         * @return Reference to a Consumer Info Object
         */
        const commands::ConsumerInfo& getConsumerInfo() const {
            this->checkClosed();
            return *( this->consumerInfo );
        }

        /**
         * Get the Consumer Id for this consumer
         * @return Reference to a Consumer Id Object
         */
        const commands::ConsumerId& getConsumerId() const {
            this->checkClosed();
            return *( this->consumerInfo->getConsumerId() );
        }

        /**
         * @returns if this Consumer has been closed.
         */
        bool isClosed() const {
            return this->closed;
        }

        /**
         * Has this Consumer Transaction Synchronization been added to the transaction
         * @return true if the synchronization has been added.
         */
        bool issynchronizationRegistered() const {
            return this->synchronizationRegistered;
        }

        /**
         * Sets the Synchronization Registered state of this consumer.
         * @param value - true if registered false otherwise.
         */
        void setSynchronizationRegistered( bool value ) {
            this->synchronizationRegistered = value;
        }

    protected:

        /**
         * Purges all messages currently in the queue.  This can be as a
         * result of a rollback, or of the consumer being shutdown.
         */
        void purgeMessages() throw ( exceptions::ActiveMQException );

        /**
         * Used by synchronous receive methods to wait for messages to come in.
         * @param timeout - The maximum number of milliseconds to wait before
         * returning.
         * If -1, it will block until a messages is received or this consumer
         * is closed.
         * If 0, will not block at all.  If > 0, will wait at a maximum the
         * specified number of milliseconds before returning.
         * @return the message, if received within the allotted time.
         * Otherwise NULL.
         * @throws InvalidStateException if this consumer is closed upon
         * entering this method.
         */
        Pointer<commands::Message> dequeue( int timeout )
            throw ( cms::CMSException );

        /**
         * Pre-consume processing
         * @param message - the message being consumed.
         */
        void beforeMessageIsConsumed(
            const Pointer<commands::Message>& message );

        /**
         * Post-consume processing
         * @param message - the consumed message
         * @param messageExpired - flag indicating if the message has expired.
         */
        void afterMessageIsConsumed(
            const Pointer<commands::Message>& message, bool messageExpired );

    private:

        /**
         * If supported sends a message pull request to the service provider asking
         * for the delivery of a new message.  This is used in the case where the
         * service provider has been configured with a zero prefetch or is only
         * capable of delivering messages on a pull basis.  No request is made if
         * there are already messages in the unconsumed queue since there's no need
         * for a server round-trip in that instance.
         * @param timeout - the time that the client is willing to wait.
         */
        void sendPullRequest( long long timeout )
            throw ( exceptions::ActiveMQException );

        // Checks for the closed state and throws if so.
        void checkClosed() const throw( exceptions::ActiveMQException );

        // Sends an ack as needed in order to keep them coming in if the current
        // ack mode allows the consumer to receive up to the prefetch limit before
        // an real ack is sent.
        void ackLater( const Pointer<commands::Message>& message, int ackType )
            throw ( exceptions::ActiveMQException );

        // Delivers all pending acks before a consumer is closed
        void deliverAcks() throw ( exceptions::ActiveMQException );

        // Create an Ack Message that acks all messages that have been delivered so far.
        Pointer<commands::MessageAck> makeAckForAllDeliveredMessages( int type );

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONSUMER_H_*/
