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
#include <activemq/commands/ActiveMQTempDestination.h>
#include <activemq/commands/SessionInfo.h>
#include <activemq/commands/ConsumerInfo.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/ProducerId.h>
#include <activemq/commands/TransactionId.h>
#include <activemq/core/Dispatcher.h>

#include <decaf/util/StlMap.h>
#include <decaf/util/StlQueue.h>
#include <decaf/util/Properties.h>

#include <string>
#include <memory>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;

    class ActiveMQTransactionContext;
    class ActiveMQConnection;
    class ActiveMQConsumer;
    class ActiveMQMessage;
    class ActiveMQProducer;
    class ActiveMQConsumer;
    class ActiveMQSessionExecutor;

    class AMQCPP_API ActiveMQSession : public cms::Session, public Dispatcher {
    private:

        typedef decaf::util::StlMap< Pointer<commands::ConsumerId>,
                                     ActiveMQConsumer*,
                                     commands::ConsumerId::COMPARATOR> ConsumersMap;

        typedef decaf::util::StlMap< Pointer<commands::ProducerId>,
                                     ActiveMQProducer*,
                                     commands::ProducerId::COMPARATOR> ProducersMap;

    private:

        /**
         * SessionInfo for this Session
         */
        Pointer<commands::SessionInfo> sessionInfo;

        /**
         * Transaction Management object
         */
        std::auto_ptr<ActiveMQTransactionContext> transaction;

        /**
         * Connection
         */
        ActiveMQConnection* connection;

        /**
         * Bool to indicate if this session was closed.
         */
        bool closed;

        /**
         * Map of consumers.
         */
        ConsumersMap consumers;

        /**
         * Map of producers.
         */
        ProducersMap producers;

        /**
         * Sends incoming messages to the registered consumers.
         */
        std::auto_ptr<ActiveMQSessionExecutor> executor;

        /**
         * This Sessions Acknowledgment mode.
         */
        cms::Session::AcknowledgeMode ackMode;

    public:

        ActiveMQSession( const Pointer<commands::SessionInfo>& sessionInfo,
                         cms::Session::AcknowledgeMode ackMode,
                         const decaf::util::Properties& properties,
                         ActiveMQConnection* connection );

        virtual ~ActiveMQSession();

        /**
         * Looks up a consumer of this Session by a Pointer to its Id.
         * @param id - a Pointer to a ConsumerId to match in the Map of Consumers.
         */
        ActiveMQConsumer* getConsumer( const decaf::lang::Pointer<commands::ConsumerId>& id );

        /**
         * Redispatches the given set of unconsumed messages to the consumers.
         * @param unconsumedMessages - unconsumed messages to be redelivered.
         */
        void redispatch( decaf::util::StlQueue<DispatchData>& unconsumedMessages );

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

        bool isAutoAcknowledge() const {
            return this->ackMode == cms::Session::AUTO_ACKNOWLEDGE;
        }
        bool isDupsOkAcknowledge() const {
            return this->ackMode == cms::Session::DUPS_OK_ACKNOWLEDGE;
        }
        bool isClientAcknowledge() const {
            return this->ackMode == cms::Session::CLIENT_ACKNOWLEDGE;
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
        virtual void dispatch( DispatchData& message );

    public:   // Implements Methods

        /**
         * Closes this session as well as any active child consumers or
         * producers.
         * @throws CMSException
         */
        virtual void close() throw ( cms::CMSException );

        /**
         * Commits all messages done in this transaction and releases any
         * locks currently held.
         * @throws CMSException
         */
        virtual void commit() throw ( cms::CMSException );

        /**
         * Rollsback all messages done in this transaction and releases any
         * locks currently held.
         * @throws CMSException
         */
        virtual void rollback() throw ( cms::CMSException );

        /**
         * Creates a MessageConsumer for the specified destination.
         * @param the Destination that this consumer receiving messages for.
         * @throws CMSException
         */
        virtual cms::MessageConsumer* createConsumer(
            const cms::Destination* destination )
                throw ( cms::CMSException );

        /**
         * Creates a MessageConsumer for the specified destination, using a
         * message selector.
         * @param the Destination that this consumer receiving messages for.
         * @param the Message Selector string to use for this destination
         * @throws CMSException
         */
        virtual cms::MessageConsumer* createConsumer(
            const cms::Destination* destination,
            const std::string& selector )
                throw ( cms::CMSException );
        /**
         * Creates a MessageConsumer for the specified destination, using a
         * message selector.
         * @param the Destination that this consumer receiving messages for.
         * @param the Message Selector string to use for this destination
         * @param if true, and the destination is a topic, inhibits the
         *        delivery of messages published by its own connection. The
         *        behavior for NoLocal is not specified if the destination is
         *        a queue.
         * @throws CMSException
         */
        virtual cms::MessageConsumer* createConsumer(
            const cms::Destination* destination,
            const std::string& selector,
            bool noLocal )
                throw ( cms::CMSException );

        /**
         * Creates a durable subscriber to the specified topic, using a
         * message selector
         * @param the topic to subscribe to
         * @param name used to identify the subscription
         * @param only messages matching the selector are received
         * @throws CMSException
         */
        virtual cms::MessageConsumer* createDurableConsumer(
            const cms::Topic* destination,
            const std::string& name,
            const std::string& selector,
            bool noLocal = false )
                throw ( cms::CMSException );

        /**
         * Creates a MessageProducer to send messages to the specified
         * destination.
         * @param the Destination to publish on
         * @throws CMSException
         */
        virtual cms::MessageProducer* createProducer(
            const cms::Destination* destination )
                throw ( cms::CMSException );

        /**
         * Creates a queue identity given a Queue name.
         * @param the name of the new Queue
         * @throws CMSException
         */
        virtual cms::Queue* createQueue( const std::string& queueName )
            throw ( cms::CMSException );

        /**
         * Creates a topic identity given a Queue name.
         * @param the name of the new Topic
         * @throws CMSException
         */
        virtual cms::Topic* createTopic( const std::string& topicName )
            throw ( cms::CMSException );

        /**
         * Creates a TemporaryQueue object.
         * @throws CMSException
         */
        virtual cms::TemporaryQueue* createTemporaryQueue()
            throw ( cms::CMSException );

        /**
         * Creates a TemporaryTopic object.
         * @throws CMSException
         */
        virtual cms::TemporaryTopic* createTemporaryTopic()
            throw ( cms::CMSException );

        /**
         * Creates a new Message
         * @throws CMSException
         */
        virtual cms::Message* createMessage()
            throw ( cms::CMSException );

        /**
         * Creates a BytesMessage
         * @throws CMSException
         */
        virtual cms::BytesMessage* createBytesMessage()
            throw ( cms::CMSException );

        /**
         * Creates a BytesMessage and sets the payload to the passed value
         * @param an array of bytes to set in the message
         * @param the size of the bytes array, or number of bytes to use
         * @throws CMSException
         */
        virtual cms::BytesMessage* createBytesMessage(
            const unsigned char* bytes,
            std::size_t bytesSize )
                throw ( cms::CMSException );

        /**
         * Creates a new TextMessage
         * @throws CMSException
         */
        virtual cms::TextMessage* createTextMessage()
            throw ( cms::CMSException );

        /**
         * Creates a new TextMessage and set the text to the value given
         * @param the initial text for the message
         * @throws CMSException
         */
        virtual cms::TextMessage* createTextMessage( const std::string& text )
            throw ( cms::CMSException );

        /**
         * Creates a new TextMessage
         * @throws CMSException
         */
        virtual cms::MapMessage* createMapMessage()
            throw ( cms::CMSException );

        /**
         * Returns the acknowledgment mode of the session.
         * @return the Sessions Acknowledge Mode
         */
        virtual cms::Session::AcknowledgeMode getAcknowledgeMode() const;

        /**
         * Gets if the Sessions is a Transacted Session
         * @return transacted true - false.
         */
        virtual bool isTransacted() const;

        /**
         * Unsubscribes a durable subscription that has been created by a
         * client.
         *
         * This method deletes the state being maintained on behalf of the
         * subscriber by its provider.  It is erroneous for a client to delete a
         * durable subscription while there is an active MessageConsumer or
         * Subscriber for the subscription, or while a consumed message is
         * part of a pending transaction or has not been acknowledged in the
         * session.
         * @param name the name used to identify this subscription
         * @throws CMSException
         */
        virtual void unsubscribe( const std::string& name )
            throw ( cms::CMSException );

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
         * @param prducer
         *        The sending Producer
         * @param usage
         *        Pointer to a Usage tracker which if set will be increased by the size
         *        of the given message.
         *
         * @throws CMSException
         */
        void send( cms::Message* message, ActiveMQProducer* producer, util::Usage* usage )
            throw ( cms::CMSException );

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
         * Sends a oneway message.
         * @param command The message to send.
         * @throws ActiveMQException if not currently connected, or
         * if the operation fails for any reason.
         */
        void oneway( Pointer<commands::Command> command )
            throw ( activemq::exceptions::ActiveMQException );

        /**
         * Sends a synchronous request and returns the response from the broker.
         * Converts any error responses into an exception.
         * @param command The request command.
         * @param timeout The time to wait for a response, default is zero or infinite.
         * @throws ActiveMQException thrown if an error response was received
         * from the broker, or if any other error occurred.
         */
        void syncRequest( Pointer<commands::Command> command, unsigned int timeout = 0 )
            throw ( activemq::exceptions::ActiveMQException );

        /**
         * Dispose of a Consumer from this session.  Removes it from the Connection
         * and clean up any resources associated with it.
         * @param consumerId - the Id of the Consumer to dispose.
         */
        void disposeOf( decaf::lang::Pointer<commands::ConsumerId> id )
            throw ( activemq::exceptions::ActiveMQException );

        /**
         * Dispose of a Producer from this session.  Removes it from the Connection
         * and clean up any resources associated with it.
         * @param consumerId - the Id of the Producer to dispose.
         */
        void disposeOf( decaf::lang::Pointer<commands::ProducerId> id )
            throw ( activemq::exceptions::ActiveMQException );

   private:

       // Checks for the closed state and throws if so.
       void checkClosed() const throw( exceptions::ActiveMQException );

       // Performs the work of creating and configuring a valid Consumer Info, this
       // can be used both by the normal createConsumer call and by a createDurableConsumer
       // call as well.  Caller owns the returned ConsumerInfo object.
       commands::ConsumerInfo* createConsumerInfo(
           const cms::Destination* destination )
               throw ( activemq::exceptions::ActiveMQException );

       // Using options from the Destination URI override any settings that are
       // defined for this consumer.
       void applyDestinationOptions( const Pointer<commands::ConsumerInfo>& info );

       // Send the Destination Creation Request to the Broker, alerting it
       // that we've created a new Temporary Destination.
       // @param tempDestination - The new Temporary Destination
       void createTemporaryDestination(
           commands::ActiveMQTempDestination* tempDestination )
               throw ( activemq::exceptions::ActiveMQException );

       // Send the Destination Destruction Request to the Broker, alerting
       // it that we've removed an existing Temporary Destination.
       // @param tempDestination - The Temporary Destination to remove
       void destroyTemporaryDestination(
           commands::ActiveMQTempDestination* tempDestination )
               throw ( activemq::exceptions::ActiveMQException );

       // Creates a new Temporary Destination name using the connection id
       // and a rolling count.
       // @returns a unique Temporary Destination name
       std::string createTemporaryDestinationName()
           throw ( activemq::exceptions::ActiveMQException );

       // Create a Transaction Id using the local context to create
       // the LocalTransactionId Command.
       // @returns a new TransactionId pointer, caller owns.
       commands::TransactionId* createLocalTransactionId()
           throw ( activemq::exceptions::ActiveMQException );

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQSESSION_H_*/
