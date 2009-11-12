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
#include <activemq/commands/MessageDispatch.h>
#include <activemq/core/ActiveMQTransactionContext.h>
#include <activemq/core/Dispatcher.h>
#include <activemq/core/MessageDispatchChannel.h>

#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/StlQueue.h>
#include <decaf/util/concurrent/Mutex.h>
#include <memory>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;
    using decaf::util::concurrent::atomic::AtomicBoolean;
    using activemq::core::MessageDispatchChannel;

    class ActiveMQSession;

    class AMQCPP_API ActiveMQConsumer : public cms::MessageConsumer,
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
        cms::MessageListener* listener;

        /**
         * Mutex to Protect access to the listener during delivery.
         */
        decaf::util::concurrent::Mutex listenerMutex;

        /**
         * Is the consumer currently delivering acks.
         */
        AtomicBoolean deliveringAcks;

        /**
         * Has this Consumer been started yet.
         */
        AtomicBoolean started;

        /**
         * Queue of unconsumed messages.
         */
        MessageDispatchChannel unconsumedMessages;

        /**
         * Queue of consumed messages.
         */
        decaf::util::StlQueue< decaf::lang::Pointer<commands::MessageDispatch> > dispatchedMessages;

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
        long long redeliveryDelay;

        /**
         * Has the Synchronization been added for this transaction
         */
        volatile bool synchronizationRegistered;

        /**
         * Boolean indicating if in progress messages should be cleared.
         */
        bool clearDispatchList;

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
         * Starts the Consumer if not already started and not closed. A consumer
         * will no deliver messages until started.
         */
        virtual void start();

        /**
         * Stops a Consumer, the Consumer will not deliver any messages that are
         * dispatched to it until it is started again.  A Closed Consumer is also a
         * stopped consumer.
         */
        virtual void stop();

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
        virtual void setMessageListener( cms::MessageListener* listener ) throw ( cms::CMSException );

        /**
         * Gets the MessageListener that this class will send events to.
         * @return the currently registered MessageListener interface pointer.
         */
        virtual cms::MessageListener* getMessageListener() const throw ( cms::CMSException ) {
            return this->listener;
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
        virtual void acknowledge( const Pointer<commands::MessageDispatch>& dispatch )
            throw ( cms::CMSException );

    public:  // Dispatcher Methods

        /**
         * Called asynchronously by the session to dispatch a message.
         * @param message dispatch object pointer
         */
        virtual void dispatch( const Pointer<MessageDispatch>& message );

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
            return this->unconsumedMessages.isClosed();
        }

        /**
         * Has this Consumer Transaction Synchronization been added to the transaction
         * @return true if the synchronization has been added.
         */
        bool isSynchronizationRegistered() const {
            return this->synchronizationRegistered;
        }

        /**
         * Sets the Synchronization Registered state of this consumer.
         * @param value - true if registered false otherwise.
         */
        void setSynchronizationRegistered( bool value ) {
            this->synchronizationRegistered = value;
        }

        /**
         * Deliver any pending messages to the registered MessageListener if there
         * is one, return true if not all dispatched, or false if no listener or all
         * pending messages have been dispatched.
         */
        bool iterate();

        /**
         * Forces this consumer to send all pending acks to the broker.
         */
        void deliverAcks() throw ( exceptions::ActiveMQException );

        /**
         * Called on a Failover to clear any pending messages.
         */
        void clearMessagesInProgress();

        /**
         * Gets the currently set Last Delivered Sequence Id
         *
         * @returns long long containing the sequence id of the last delivered Message.
         */
        long long getLastDeliveredSequenceId() const {
            return this->lastDeliveredSequenceId;
        }

        /**
         * Sets the value of the Last Delivered Sequence Id
         *
         * @param value
         *      The new value to assign to the Last Delivered Sequence Id property.
         */
        void setLastDeliveredSequenceId( long long value ) {
            this->lastDeliveredSequenceId = value;
        }

    protected:

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
        Pointer<MessageDispatch> dequeue( long long timeout )
            throw ( cms::CMSException );

        /**
         * Pre-consume processing
         * @param dispatch - the message being consumed.
         */
        void beforeMessageIsConsumed(
            const Pointer<commands::MessageDispatch>& dispatch );

        /**
         * Post-consume processing
         * @param dispatch - the consumed message
         * @param messageExpired - flag indicating if the message has expired.
         */
        void afterMessageIsConsumed(
            const Pointer<commands::MessageDispatch>& dispatch, bool messageExpired );

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
        void ackLater( const Pointer<commands::MessageDispatch>& message, int ackType )
            throw ( exceptions::ActiveMQException );

        // Create an Ack Message that acks all messages that have been delivered so far.
        Pointer<commands::MessageAck> makeAckForAllDeliveredMessages( int type );

        // Should Acks be sent on each dispatched message
        bool isAutoAcknowledgeEach() const;

        // Can Acks be batched for less network overhead.
        bool isAutoAcknowledgeBatch() const;

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONSUMER_H_*/
