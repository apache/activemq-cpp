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
#include <activemq/core/Dispatcher.h>
#include <activemq/core/RedeliveryPolicy.h>
#include <activemq/core/MessageDispatchChannel.h>

#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/concurrent/Mutex.h>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;
    using decaf::util::concurrent::atomic::AtomicBoolean;

    class ActiveMQSession;
    class ActiveMQConsumerMembers;

    class AMQCPP_API ActiveMQConsumer : public cms::MessageConsumer,
                                        public Dispatcher
    {
    private:

        /**
         * Internal Class that holds Members of this class, allows for changes without API breakage.
         */
        ActiveMQConsumerMembers* internal;

        /**
         * The ActiveMQSession that owns this class instance.
         */
        ActiveMQSession* session;

        /**
         * The ConsumerInfo object for this class instance.
         */
        Pointer<commands::ConsumerInfo> consumerInfo;

    private:

        ActiveMQConsumer( const ActiveMQConsumer& );
        ActiveMQConsumer& operator= ( const ActiveMQConsumer& );

    public:

        /**
         * Constructor
         */
        ActiveMQConsumer( ActiveMQSession* session,
                          const Pointer<commands::ConsumerId>& id,
                          const Pointer<commands::ActiveMQDestination>& destination,
                          const std::string& name,
                          const std::string& selector,
                          int prefetch,
                          int maxPendingMessageCount,
                          bool noLocal,
                          bool browser,
                          bool dispatchAsync,
                          cms::MessageListener* listener );

        virtual ~ActiveMQConsumer() throw();

    public:  // Interface Implementation

        virtual void start();

        virtual void stop();

        virtual void close();

        virtual cms::Message* receive();

        virtual cms::Message* receive( int millisecs );

        virtual cms::Message* receiveNoWait();

        virtual void setMessageListener( cms::MessageListener* listener );

        virtual cms::MessageListener* getMessageListener() const;

        virtual std::string getMessageSelector() const;

        virtual void acknowledge( const Pointer<commands::MessageDispatch>& dispatch );

    public:  // Dispatcher Methods

        virtual void dispatch( const Pointer<MessageDispatch>& message );

    public:  // ActiveMQConsumer Methods

        /**
         * Method called to acknowledge all messages that have been received so far.
         *
         * @throw CMSException if an error occurs while ack'ing the message.
         */
        void acknowledge();

        /**
         * Called to Commit the current set of messages in this Transaction
         *
         * @throw ActiveMQException if an error occurs while performing the operation.
         */
        void commit();

        /**
         * Called to Roll back the current set of messages in this Transaction
         *
         * @throw ActiveMQException if an error occurs while performing the operation.
         */
        void rollback();

        /**
         * Performs the actual close operation on this consumer
         *
         * @throw ActiveMQException if an error occurs while performing the operation.
         */
        void doClose();

        /**
         * Cleans up this objects internal resources.
         *
         * @throw ActiveMQException if an error occurs while performing the operation.
         */
        void dispose();

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
         * Has this Consumer Transaction Synchronization been added to the transaction
         * @return true if the synchronization has been added.
         */
        bool isSynchronizationRegistered() const ;

        /**
         * Sets the Synchronization Registered state of this consumer.
         * @param value - true if registered false otherwise.
         */
        void setSynchronizationRegistered( bool value );

        /**
         * Deliver any pending messages to the registered MessageListener if there
         * is one, return true if not all dispatched, or false if no listener or all
         * pending messages have been dispatched.
         */
        bool iterate();

        /**
         * Forces this consumer to send all pending acks to the broker.
         *
         * @throw ActiveMQException if an error occurs while performing the operation.
         */
        void deliverAcks();

        /**
         * Called on a Failover to clear any pending messages.
         */
        void clearMessagesInProgress();

        /**
         * Signals that a Failure occurred and that anything in-progress in the
         * consumer should be cleared.
         */
        void inProgressClearRequired();

        /**
         * Gets the currently set Last Delivered Sequence Id
         *
         * @returns long long containing the sequence id of the last delivered Message.
         */
        long long getLastDeliveredSequenceId() const;

        /**
         * Sets the value of the Last Delivered Sequence Id
         *
         * @param value
         *      The new value to assign to the Last Delivered Sequence Id property.
         */
        void setLastDeliveredSequenceId( long long value );

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
        void setRedeliveryPolicy( RedeliveryPolicy* policy );

        /**
         * Gets a pointer to this Consumer's Redelivery Policy object, the Consumer
         * retains ownership of this pointer so the caller should not delete it.
         *
         * @returns a Pointer to a RedeliveryPolicy that is in use by this Consumer.
         */
        RedeliveryPolicy* getRedeliveryPolicy() const;

        /**
         * Sets the Exception that has caused this Consumer to be in a failed state.
         *
         * @param error
         *      The error that is to be thrown when a Receive call is made.
         */
        void setFailureError( decaf::lang::Exception* error );

        /**
         * Gets the error that caused this Consumer to be in a Failed state, or NULL if
         * there is no Error.
         *
         * @returns pointer to the error that faulted this Consumer or NULL.
         */
        decaf::lang::Exception* getFailureError() const;

    protected:

        /**
         * Used by synchronous receive methods to wait for messages to come in.
         * @param timeout - The maximum number of milliseconds to wait before
         * returning.
         *
         * If -1, it will block until a messages is received or this consumer
         * is closed.
         * If 0, will not block at all.  If > 0, will wait at a maximum the
         * specified number of milliseconds before returning.
         * @return the message, if received within the allotted time.
         * Otherwise NULL.
         *
         * @throws InvalidStateException if this consumer is closed upon
         *         entering this method.
         */
        Pointer<MessageDispatch> dequeue( long long timeout );

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

        // Using options from the Destination URI override any settings that are
        // defined for this consumer.
        void applyDestinationOptions( const Pointer<commands::ConsumerInfo>& info );

        // If supported sends a message pull request to the service provider asking
        // for the delivery of a new message.  This is used in the case where the
        // service provider has been configured with a zero prefetch or is only
        // capable of delivering messages on a pull basis.  No request is made if
        // there are already messages in the unconsumed queue since there's no need
        // for a server round-trip in that instance.
        void sendPullRequest( long long timeout );

        // Checks for the closed state and throws if so.
        void checkClosed() const;

        // Sends an ack as needed in order to keep them coming in if the current
        // ack mode allows the consumer to receive up to the prefetch limit before
        // an real ack is sent.
        void ackLater( const Pointer<commands::MessageDispatch>& message, int ackType );

        // Create an Ack Message that acks all messages that have been delivered so far.
        Pointer<commands::MessageAck> makeAckForAllDeliveredMessages( int type );

        // Should Acks be sent on each dispatched message
        bool isAutoAcknowledgeEach() const;

        // Can Acks be batched for less network overhead.
        bool isAutoAcknowledgeBatch() const;

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONSUMER_H_*/
