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
#include <activemq/concurrent/Runnable.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/connector/SessionInfo.h>
#include <activemq/util/Set.h>
#include <activemq/util/Queue.h>
#include <activemq/connector/ConnectorResourceListener.h>
#include <set>

namespace activemq{
namespace core{

    class ActiveMQTransaction;
    class ActiveMQConnection;
    class ActiveMQConsumer;
    class ActiveMQMessage;
    class ActiveMQProducer;
    class ActiveMQConsumer;

    class ActiveMQSession :
        public cms::Session,
        public concurrent::Runnable,
        public connector::ConnectorResourceListener
    {
    private:

        /**
         * SessionInfo for this Session
         */
        connector::SessionInfo* sessionInfo;

        /**
         * Transaction Management object
         */
        ActiveMQTransaction* transaction;

        /**
         * Connection
         */
        ActiveMQConnection* connection;

        /**
         * Bool to indicate if this session was closed.
         */
        bool closed;

        /**
         * The set of closable session resources;
         * This can consist of consumers and producers and sometimes
         * destination.
         */
        util::Set<cms::Closeable*> closableSessionResources;

        /**
         *  Thread to notif a listener if one is added
         */
        concurrent::Thread* asyncThread;

        /**
         * Is this Session using Async Sends.
         */
        bool useAsyncSend;

        /**
         * Outgoing Message Queue
         */
        util::Queue< std::pair<cms::Message*, ActiveMQProducer*> > msgQueue;

    public:

        ActiveMQSession( connector::SessionInfo* sessionInfo,
                         const util::Properties& properties,
                         ActiveMQConnection* connection );

        virtual ~ActiveMQSession();

    public:   // Implements Mehtods

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
         * Creates a BytesMessage and sets the paylod to the passed value
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
         * Returns the acknowledgement mode of the session.
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
         * Sends a message from the Producer specified
         * @param cms::Message pointer
         * @param Producer Information
         * @throws CMSException
         */
        virtual void send( cms::Message* message, ActiveMQProducer* producer )
            throw ( cms::CMSException );

        /**
         * Called to acknowledge the receipt of a message.
         * @param The consumer that received the message
         * @param The Message to acknowledge.
         * @throws CMSException
         */
        virtual void acknowledge( ActiveMQConsumer* consumer,
                                  ActiveMQMessage* message )
            throw ( cms::CMSException );

        /**
         * This method gets any registered exception listener of this sessions
         * connection and returns it.  Mainly intended for use by the objects
         * that this session creates so that they can notify the client of
         * exceptions that occur in the context of another thread.
         * @returns cms::ExceptionListener pointer or NULL
         */
        virtual cms::ExceptionListener* getExceptionListener();

        /**
         * Gets the Session Information object for this session, if the
         * session is closed than this returns null
         * @return SessionInfo Pointer
         */
        virtual connector::SessionInfo* getSessionInfo() {
            return sessionInfo;
        }

    protected:   // ConnectorResourceListener

        /**
         * When a Connector Resouce is closed it will notify any registered
         * Listeners of its close so that they can take the appropriate
         * action.
         * @param resource - The ConnectorResource that was closed.
         */
        virtual void onConnectorResourceClosed(
            const connector::ConnectorResource* resource ) throw ( cms::CMSException );

    protected:

        /**
         * Run method that is called from the Thread class when this object
         * is registered with a Thread and started.  This function reads from
         * the outgoing message queue and dispatches calls to the connector that
         * is registered with this class.
         */
        virtual void run();

        /**
         * Starts the message processing thread to receive messages
         * asynchronously.  This thread is started when setMessageListener
         * is invoked, which means that the caller is choosing to use this
         * consumer asynchronously instead of synchronously (receive).
         */
        void startThread() throw ( exceptions::ActiveMQException );

        /**
         * Stops the asynchronous message processing thread if it's started.
         */
        void stopThread() throw ( exceptions::ActiveMQException );

        /**
         * Purges all messages currently in the queue.  This can be as a
         * result of a rollback, or of the consumer being shutdown.
         */
        virtual void purgeMessages() throw ( exceptions::ActiveMQException );

        /**
         * Given a ConnectorResource pointer, this method will add it to the map
         * of closeable resources that this connection must close on shutdown
         * and register itself as a ConnectorResourceListener so that it
         * can be told when the resouce has been closed by someone else
         * and remove it from its map of closeable resources.
         * @param resource - ConnectorResouce to monitor, if NULL no action
         *                   is taken and no exception is thrown.
         */
        virtual void checkConnectorResource(
            connector::ConnectorResource* resource );

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQSESSION_H_*/
