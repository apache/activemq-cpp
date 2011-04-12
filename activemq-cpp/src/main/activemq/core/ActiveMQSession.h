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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQSESSION_H_
#define _ACTIVEMQ_CORE_ACTIVEMQSESSION_H_

#include <cms/Session.h>
#include <cms/ExceptionListener.h>

#include <activemq/util/Config.h>
#include <activemq/util/Usage.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQTransactionContext.h>
#include <activemq/commands/ActiveMQTempDestination.h>
#include <activemq/commands/Response.h>
#include <activemq/commands/SessionInfo.h>
#include <activemq/commands/ConsumerInfo.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/ProducerId.h>
#include <activemq/commands/TransactionId.h>
#include <activemq/core/Dispatcher.h>
#include <activemq/core/MessageDispatchChannel.h>
#include <activemq/util/LongSequenceGenerator.h>
#include <activemq/threads/Scheduler.h>

#include <decaf/lang/Pointer.h>
#include <decaf/util/StlMap.h>
#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/CopyOnWriteArrayList.h>

#include <string>
#include <memory>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;
    using decaf::util::concurrent::atomic::AtomicBoolean;

    class SessionConfig;
    class ActiveMQConnection;
    class ActiveMQConsumer;
    class ActiveMQMessage;
    class ActiveMQProducer;
    class ActiveMQConsumer;
    class ActiveMQSessionExecutor;

    class AMQCPP_API ActiveMQSession : public virtual cms::Session, public Dispatcher {
    private:

        typedef decaf::util::StlMap< Pointer<commands::ConsumerId>,
                                     ActiveMQConsumer*,
                                     commands::ConsumerId::COMPARATOR> ConsumersMap;

        friend class ActiveMQSessionExecutor;

    protected:

        SessionConfig* config;

        /**
         * SessionInfo for this Session
         */
        Pointer<commands::SessionInfo> sessionInfo;

        /**
         * Transaction Management object
         */
        Pointer<ActiveMQTransactionContext> transaction;

        /**
         * Connection
         */
        ActiveMQConnection* connection;

        /**
         * Map of consumers.
         */
        ConsumersMap consumers;

        /**
         * Indicates that this connection has been closed, it is no longer
         * usable after this becomes true
         */
        AtomicBoolean closed;

        /**
         * Sends incoming messages to the registered consumers.
         */
        std::auto_ptr<ActiveMQSessionExecutor> executor;

        /**
         * This Sessions Acknowledgment mode.
         */
        cms::Session::AcknowledgeMode ackMode;

        /**
         * Next available Producer Id
         */
        util::LongSequenceGenerator producerIds;

        /**
         * Next available Producer Sequence Id
         */
        util::LongSequenceGenerator producerSequenceIds;

        /**
         * Next available Consumer Id
         */
        util::LongSequenceGenerator consumerIds;

        /**
         * Last Delivered Sequence Id
         */
        long long lastDeliveredSequenceId;

    private:

        ActiveMQSession( const ActiveMQSession& );
        ActiveMQSession& operator= ( const ActiveMQSession& );

    public:

        ActiveMQSession( ActiveMQConnection* connection,
                         const Pointer<commands::SessionId>& id,
                         cms::Session::AcknowledgeMode ackMode,
                         const decaf::util::Properties& properties );

        virtual ~ActiveMQSession() throw();

        /**
         * Redispatches the given set of unconsumed messages to the consumers.
         * @param unconsumedMessages - unconsumed messages to be redelivered.
         */
        virtual void redispatch( MessageDispatchChannel& unconsumedMessages );

        /**
         * Stops asynchronous message delivery.
         */
        void start();

        /**
         * Starts asynchronous message delivery.
         */
        void stop();

        /**
         * Indicates whether or not the session is currently in the started
         * state.
         */
        bool isStarted() const;

        virtual bool isAutoAcknowledge() const {
            return this->ackMode == cms::Session::AUTO_ACKNOWLEDGE;
        }

        virtual bool isDupsOkAcknowledge() const {
            return this->ackMode == cms::Session::DUPS_OK_ACKNOWLEDGE;
        }

        virtual bool isClientAcknowledge() const {
            return this->ackMode == cms::Session::CLIENT_ACKNOWLEDGE;
        }

        virtual bool isIndividualAcknowledge() const {
            return this->ackMode == cms::Session::INDIVIDUAL_ACKNOWLEDGE;
        }

        /**
         * Fires the given exception to the exception listener of the connection
         */
        void fire( const exceptions::ActiveMQException& ex );

    public:  // Methods from ActiveMQMessageDispatcher

        /**
         * Dispatches a message to a particular consumer.
         * @param message - the message to be dispatched
         */
        virtual void dispatch( const Pointer<MessageDispatch>& message );

    public:   // Implements Methods

        virtual void close();

        virtual void commit();

        virtual void rollback();

        virtual void recover();

        virtual cms::MessageConsumer* createConsumer( const cms::Destination* destination );

        virtual cms::MessageConsumer* createConsumer( const cms::Destination* destination,
                                                      const std::string& selector );

        virtual cms::MessageConsumer* createConsumer( const cms::Destination* destination,
                                                      const std::string& selector,
                                                      bool noLocal );

        virtual cms::MessageConsumer* createDurableConsumer( const cms::Topic* destination,
                                                             const std::string& name,
                                                             const std::string& selector,
                                                             bool noLocal = false );

        virtual cms::MessageProducer* createProducer( const cms::Destination* destination );

        virtual cms::QueueBrowser* createBrowser( const cms::Queue* queue );

        virtual cms::QueueBrowser* createBrowser( const cms::Queue* queue, const std::string& selector );

        virtual cms::Queue* createQueue( const std::string& queueName );

        virtual cms::Topic* createTopic( const std::string& topicName );

        virtual cms::TemporaryQueue* createTemporaryQueue();

        virtual cms::TemporaryTopic* createTemporaryTopic();

        virtual cms::Message* createMessage();

        virtual cms::BytesMessage* createBytesMessage();

        virtual cms::BytesMessage* createBytesMessage( const unsigned char* bytes, int bytesSize );

        virtual cms::StreamMessage* createStreamMessage();

        virtual cms::TextMessage* createTextMessage();

        virtual cms::TextMessage* createTextMessage( const std::string& text );

        virtual cms::MapMessage* createMapMessage();

        virtual cms::Session::AcknowledgeMode getAcknowledgeMode() const;

        virtual bool isTransacted() const;

        virtual void unsubscribe( const std::string& name );

   public:   // ActiveMQSession specific Methods

        /**
         * Sends a message from the Producer specified using this session's connection
         * the message will be sent using the best available means depending on the
         * configuration of the connection.
         * <p>
         * Asynchronous sends will be chosen if at all possible.
         *
         * @param message
         *        The message to send to the broker.
         * @param producer
         *        The sending Producer
         * @param usage
         *        Pointer to a Usage tracker which if set will be increased by the size
         *        of the given message.
         *
         * @throws CMSException
         */
        void send( cms::Message* message, ActiveMQProducer* producer, util::Usage* usage );

        /**
         * This method gets any registered exception listener of this sessions
         * connection and returns it.  Mainly intended for use by the objects
         * that this session creates so that they can notify the client of
         * exceptions that occur in the context of another thread.
         * @returns cms::ExceptionListener pointer or NULL
         */
        cms::ExceptionListener* getExceptionListener();

        /**
         * Gets the Session Information object for this session, if the
         * session is closed than this method throws an exception.
         * @return SessionInfo Reference
         */
        const commands::SessionInfo& getSessionInfo() const {
            this->checkClosed();
            return *( this->sessionInfo );
        }

        /**
         * Gets the Session Id object for this session, if the session
         * is closed than this method throws an exception.
         * @return SessionId Reference
         */
        const commands::SessionId& getSessionId() const {
            this->checkClosed();
            return *( this->sessionInfo->getSessionId() );
        }

        /**
         * Gets the ActiveMQConnection that is associated with this session.
         */
        ActiveMQConnection* getConnection() const {
            return this->connection;
        }

        /**
         * Gets a Pointer to this Session's Scheduler instance
         */
        Pointer<threads::Scheduler> getScheduler() const;

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

        /**
         * Sends a Command to the broker without requesting any Response be returned.
         * .
         * @param command
         *      The message to send to the Broker.
         *
         * @throws ActiveMQException if not currently connected, or if the
         *         operation fails for any reason.
         */
        void oneway( Pointer<commands::Command> command );

        /**
         * Sends a synchronous request and returns the response from the broker.
         * Converts any error responses into an exception.
         *
         * @param command
         *      The command to send to the broker.
         * @param timeout
         *      The time to wait for a response, default is zero or infinite.
         *
         * @returns Pointer to a Response object that the broker has returned for the Command sent.
         *
         * @throws ActiveMQException thrown if an error response was received
         *         from the broker, or if any other error occurred.
         */
        Pointer<commands::Response> syncRequest( Pointer<commands::Command> command, unsigned int timeout = 0 );

        /**
         * Adds a MessageConsumer to this session registering it with the Connection and store
         * a reference to it so the session can ensure that all resources are closed when
         * the session is closed.
         *
         * @param consumer
         *      The ActiveMQConsumer instance to add to this session.
         *
         * @throw ActiveMQException if an internal error occurs.
         */
        void addConsumer( ActiveMQConsumer* consumer );

        /**
         * Dispose of a MessageConsumer from this session.  Removes it from the Connection
         * and clean up any resources associated with it.
         *
         * @param consumerId
         *      The ConsumerId of the MessageConsumer to remove from this Session.
         *
         * @throw ActiveMQException if an internal error occurs.
         */
        void removeConsumer( const Pointer<commands::ConsumerId>& consumerId );

        /**
         * Adds a MessageProducer to this session registering it with the Connection and store
         * a reference to it so the session can ensure that all resources are closed when
         * the session is closed.
         *
         * @param consumer
         *      The ActiveMQProducer instance to add to this session.
         *
         * @throw ActiveMQException if an internal error occurs.
         */
        void addProducer( ActiveMQProducer* producer );

        /**
         * Dispose of a MessageProducer from this session.  Removes it from the Connection
         * and clean up any resources associated with it.
         *
         * @param producerId
         *      The ProducerId of the MessageProducer to remove from this session.
         *
         * @throw ActiveMQException if an internal error occurs.
         */
        void removeProducer( ActiveMQProducer* producer );

        /**
         * Starts if not already start a Transaction for this Session.  If the session
         * is not a Transacted Session then an exception is thrown.  If a transaction is
         * already in progress then this method has no effect.
         *
         * @throw ActiveMQException if this is not a Transacted Session.
         */
        virtual void doStartTransaction();

        /**
         * Gets the Pointer to this Session's TransactionContext
         *
         * @return a Pointer to this Session's TransactionContext
         */
        Pointer<ActiveMQTransactionContext> getTransactionContext() {
            return this->transaction;
        }

        /**
         * Request that the Session inform all its consumers to Acknowledge all Message's
         * that have been received so far.
         */
        void acknowledge();

        /**
         * Request that this Session inform all of its consumers to deliver their pending
         * acks.
         */
        void deliverAcks();

        /**
         * Request that this Session inform all of its consumers to clear all messages that
         * are currently in progress.
         */
        void clearMessagesInProgress();

        /**
         * Causes the Session to wakeup its executer and ensure all messages are dispatched.
         */
        void wakeup();

        /**
         * Get the Next available Consumer Id
         * @return the next id in the sequence.
         */
        Pointer<commands::ConsumerId> getNextConsumerId();

        /**
         * Get the Next available Producer Id
         * @return the next id in the sequence.
         */
        Pointer<commands::ProducerId> getNextProducerId();

        /**
         * Performs the actual Session close operations.  This method is meant for use
         * by ActiveMQConnection, the connection object calls this when it has been
         * closed to skip some of the extraneous processing done by the client level
         * close method.
         */
        void doClose();

        /**
         * Cleans up the Session object's resources without attempting to send the
         * Remove command to the broker, this can be called from ActiveMQConnection when
         * it knows that the transport is down and the doClose method would throw an
         * exception when it attempt to send the Remove Command.
         */
        void dispose();

   private:

       /**
        * Get the Next available Producer Sequence Id
        * @return the next id in the sequence.
        */
       long long getNextProducerSequenceId() {
           return this->producerSequenceIds.getNextSequenceId();
       }

       // Checks for the closed state and throws if so.
       void checkClosed() const;

       // Send the Destination Creation Request to the Broker, alerting it
       // that we've created a new Temporary Destination.
       // @param tempDestination - The new Temporary Destination
       void createTemporaryDestination( commands::ActiveMQTempDestination* tempDestination );

       // Send the Destination Destruction Request to the Broker, alerting
       // it that we've removed an existing Temporary Destination.
       // @param tempDestination - The Temporary Destination to remove
       void destroyTemporaryDestination( commands::ActiveMQTempDestination* tempDestination );

       // Creates a new Temporary Destination name using the connection id
       // and a rolling count.
       // @returns a unique Temporary Destination name
       std::string createTemporaryDestinationName();

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQSESSION_H_*/
