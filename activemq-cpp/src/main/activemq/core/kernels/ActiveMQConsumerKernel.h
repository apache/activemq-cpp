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
#ifndef _ACTIVEMQ_CORE_KERNELS_ACTIVEMQCONSUMERKERNEL_H_
#define _ACTIVEMQ_CORE_KERNELS_ACTIVEMQCONSUMERKERNEL_H_

#include <cms/MessageConsumer.h>
#include <cms/MessageListener.h>
#include <cms/MessageAvailableListener.h>
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

namespace activemq {
namespace core {
namespace kernels {

    using decaf::lang::Pointer;
    using decaf::util::concurrent::atomic::AtomicBoolean;

    class ActiveMQSessionKernel;
    class ActiveMQConsumerKernelConfig;

    class AMQCPP_API ActiveMQConsumerKernel : public cms::MessageConsumer, public Dispatcher {
    private:

        /**
         * Internal Class that holds Members of this class, allows for changes without API breakage.
         */
        ActiveMQConsumerKernelConfig* internal;

        /**
         * The ActiveMQSession that owns this class instance.
         */
        ActiveMQSessionKernel* session;

        /**
         * The ConsumerInfo object for this class instance.
         */
        Pointer<commands::ConsumerInfo> consumerInfo;

    private:

        ActiveMQConsumerKernel(const ActiveMQConsumerKernel&);
        ActiveMQConsumerKernel& operator=(const ActiveMQConsumerKernel&);

    public:

        ActiveMQConsumerKernel(ActiveMQSessionKernel* session,
                               const Pointer<commands::ConsumerId>& id,
                               const Pointer<commands::ActiveMQDestination>& destination,
                               const std::string& name,
                               const std::string& selector,
                               int prefetch,
                               int maxPendingMessageCount,
                               bool noLocal,
                               bool browser,
                               bool dispatchAsync,
                               cms::MessageListener* listener);

        virtual ~ActiveMQConsumerKernel();

    public:  // Interface Implementation

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

    public:  // Dispatcher Methods

        virtual void dispatch( const Pointer<MessageDispatch>& message );

        virtual int getHashCode() const;

    public:  // ActiveMQConsumerKernel Methods

        /**
         * Method called to acknowledge all messages that have been received so far.
         *
         * @throw CMSException if an error occurs while ack'ing the message.
         */
        void acknowledge();

        /**
         * Method called to acknowledge the Message contained in the given MessageDispatch
         *
         * @throw CMSException if an error occurs while ack'ing the message.
         */
        void acknowledge(Pointer<commands::MessageDispatch> dispatch);

        /**
         * Method called to acknowledge the Message contained in the given MessageDispatch
         *
         * @throw CMSException if an error occurs while ack'ing the message.
         */
        void acknowledge(Pointer<commands::MessageDispatch> dispatch, int ackType);

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
        void setSynchronizationRegistered(bool value);

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
         * Will Message's in a transaction be acknowledged using the Individual Acknowledge mode.
         *
         * @return true if individual transacted acknowledge is enabled.
         */
        bool isTransactedIndividualAck() const;

        /**
         * Set if Message's in a transaction be acknowledged using the Individual Acknowledge mode.
         *
         * @param value
         *      True if individual transacted acknowledge is enabled.
         */
        void setTransactedIndividualAck(bool value);

        /**
         * Returns the delay after a failover before Message redelivery starts.
         *
         * @returns time in milliseconds to wait after failover.
         */
        long long setFailoverRedeliveryWaitPeriod() const;

        /**
         * Sets the time in milliseconds to delay after failover before starting
         * message redelivery.
         *
         * @param value
         *      Time in milliseconds to delay after failover for redelivery start.
         */
        void setFailoverRedeliveryWaitPeriod(long long value);

        /**
         * Sets the value of the Last Delivered Sequence Id
         *
         * @param value
         *      The new value to assign to the Last Delivered Sequence Id property.
         */
        void setLastDeliveredSequenceId(long long value);

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
         * Sets the Exception that has caused this Consumer to be in a failed state.
         *
         * @param error
         *      The error that is to be thrown when a Receive call is made.
         */
        void setFailureError(decaf::lang::Exception* error);

        /**
         * Gets the error that caused this Consumer to be in a Failed state, or NULL if
         * there is no Error.
         *
         * @returns pointer to the error that faulted this Consumer or NULL.
         */
        decaf::lang::Exception* getFailureError() const;

        /**
         * Sets the current prefetch size for the consumer as indicated by a Broker
         * ConsumerControl command.
         */
        void setPrefetchSize(int prefetchSize);

        /**
         * Checks if the given destination is the Destination that this Consumer is subscribed to.
         *
         * @return true if the consumer is subscribed to the given destination.
         */
        bool isInUse(Pointer<commands::ActiveMQDestination> destination) const;

        /**
         * Time in Milliseconds before an automatic acknowledge is done for any outstanding
         * delivered Messages.  A value less than one means no task is scheduled.
         *
         * @returns time in milliseconds for the scheduled ack task.
         */
        long long getOptimizedAckScheduledAckInterval() const;

        /**
         * Sets the time in Milliseconds to schedule an automatic acknowledge of outstanding
         * messages when optimize acknowledge is enabled.  A value less than one means disable
         * any scheduled tasks.
         *
         * @param value
         *      The time interval to send scheduled acks.
         */
        void setOptimizedAckScheduledAckInterval(long long value);

        /**
         * @returns true if this consumer is using optimize acknowledge mode.
         */
        bool isOptimizeAcknowledge() const;

        /**
         * Enable or disable optimized acknowledge for this consumer.
         *
         * @param value
         *      True if optimize acknowledge is enabled, false otherwise.
         */
        void setOptimizeAcknowledge(bool value);

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
        Pointer<MessageDispatch> dequeue(long long timeout);

        /**
         * Pre-consume processing
         * @param dispatch - the message being consumed.
         */
        void beforeMessageIsConsumed(Pointer<commands::MessageDispatch> dispatch);

        /**
         * Post-consume processing
         * @param dispatch - the consumed message
         * @param messageExpired - flag indicating if the message has expired.
         */
        void afterMessageIsConsumed(Pointer<commands::MessageDispatch> dispatch, bool messageExpired);

    private:

        Pointer<cms::Message> createCMSMessage(Pointer<commands::MessageDispatch> dispatch);

        void applyDestinationOptions(Pointer<commands::ConsumerInfo> info);

        void sendPullRequest(long long timeout);

        void checkClosed() const;

        void checkMessageListener() const;

        void ackLater(Pointer<commands::MessageDispatch> message, int ackType);

        void immediateIndividualTransactedAck(Pointer<commands::MessageDispatch> dispatch);

        Pointer<commands::MessageAck> makeAckForAllDeliveredMessages(int type);

        bool isAutoAcknowledgeEach() const;

        bool isAutoAcknowledgeBatch() const;

        void registerSync();

        void clearDispatchList();

    };

}}}

#endif /* _ACTIVEMQ_CORE_KERNELS_ACTIVEMQCONSUMERKERNEL_H_ */
