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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONNECTION_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONNECTION_H_

#include <cms/Connection.h>
#include <cms/ExceptionListener.h>
#include <activemq/util/Config.h>
#include <activemq/core/ActiveMQConnectionData.h>
#include <activemq/core/ActiveMQMessage.h>
#include <activemq/core/Dispatcher.h>
#include <activemq/connector/ConsumerMessageListener.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/util/Properties.h>
#include <decaf/util/Map.h>
#include <decaf/util/Set.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

#include <string>

namespace activemq{

    namespace connector {
        class ConsumerInfo;
    }

namespace core{

    class ActiveMQSession;
    class ActiveMQConsumer;

    /**
     * Concrete connection used for all connectors to the
     * ActiveMQ broker.
     */
    class AMQCPP_API ActiveMQConnection :
        public cms::Connection,
        public connector::ConsumerMessageListener,
        public cms::ExceptionListener
    {
    private:

        /**
         * the registered exception listener
         */
        cms::ExceptionListener* exceptionListener;

        /**
         * All the data that is used to connect this Connection
         */
        ActiveMQConnectionData* connectionData;

        /**
         * Indicates if this Connection is started
         */
        bool started;

        /**
         * Indicates that this connection has been closed, it is no longer
         * usable after this becomes true
         */
        bool closed;

        /**
         * Map of message dispatchers indexed by consumer id.
         */
        decaf::util::Map< long long, Dispatcher* > dispatchers;

        /**
         * Maintain the set of all active sessions.
         */
        decaf::util::Set<ActiveMQSession*> activeSessions;

    public:

        /**
         * Constructor
         * @param Pointer to an ActiveMQConnectionData object, owned here
         */
        ActiveMQConnection( ActiveMQConnectionData* connectionData );

        virtual ~ActiveMQConnection();

        /**
         * Removes the session resources for the given session
         * instance.
         * @param session The session to be unregistered from this connection.
         */
        virtual void removeSession( ActiveMQSession* session ) throw ( cms::CMSException );

        /**
         * Adds a dispatcher for a consumer.
         * @param consumer - The consumer for which to register a dispatcher.
         * @param dispatcher - The dispatcher to handle incoming messages for the consumer.
         */
        virtual void addDispatcher( connector::ConsumerInfo* consumer, Dispatcher* dispatcher );

        /**
         * Removes the dispatcher for a consumer.
         * @param consumer - The consumer for which to remove the dispatcher.
         */
        virtual void removeDispatcher( const connector::ConsumerInfo* consumer );

        /**
         * If supported sends a message pull request to the service provider asking
         * for the delivery of a new message.  This is used in the case where the
         * service provider has been configured with a zero prefectch or is only
         * capable of delivering messages on a pull basis.
         * @param consumer - the ConsumerInfo for the requesting Consumer.
         * @param timeout - the time that the client is willing to wait.
         */
        virtual void sendPullRequest( const connector::ConsumerInfo* consumer, long long timeout )
            throw ( exceptions::ActiveMQException );

        /**
         * Checks if this connection has been closed
         * @return true if the connection is closed
         */
        bool isClosed() const {
            return this->closed;
        }

        /**
         * Requests that the Broker removes the given Destination.  Calling this
         * method implies that the client is finished with the Destination and that
         * no other messages will be sent or received for the given Destination.  The
         * Broker frees all resources it has associated with this Destination.
         *
         * @param destination
         *        The Destination the Broker will be requested to remove.
         *
         * @throws NullPointerException
         *         If the passed Destination is Null
         * @throws IllegalStateException
         *         If the connection is closed.
         * @throws UnsupportedOperationException
         *         If the wire format in use does not support this operation.
         * @throws ActiveMQException
         *         If any other error occurs during the attempt to destroy the destination.
         */
        virtual void destroyDestination( const cms::Destination* destination )
            throw( decaf::lang::exceptions::NullPointerException,
                   decaf::lang::exceptions::IllegalStateException,
                   decaf::lang::exceptions::UnsupportedOperationException,
                   activemq::exceptions::ActiveMQException );

    public:   // Connection Interface Methods

        /**
         * Creates a new Session to work for this Connection
         * @throws CMSException
         */
        virtual cms::Session* createSession() throw ( cms::CMSException );

        /**
         * Creates a new Session to work for this Connection using the
         * specified acknowledgment mode
         * @param ackMode the Acknowledgment Mode to use.
         * @throws CMSException
         */
        virtual cms::Session* createSession( cms::Session::AcknowledgeMode ackMode )
            throw ( cms::CMSException );

        /**
         * Get the Client Id for this session
         * @return string version of Client Id
         */
        virtual std::string getClientID() const;

        /**
         * Retrieves the Connection Data object for this object.
         * @return pointer to a connection data object.
         */
        virtual ActiveMQConnectionData* getConnectionData(){
            return connectionData;
        }

        /**
         * Gets the registered Exception Listener for this connection
         * @return pointer to an exception listener or NULL
         */
        virtual cms::ExceptionListener* getExceptionListener() const{
            return exceptionListener; };

        /**
         * Sets the registered Exception Listener for this connection
         * @param listener pointer to and <code>ExceptionListener</code>
         */
        virtual void setExceptionListener( cms::ExceptionListener* listener ){
            exceptionListener = listener;
        };

        /**
         * Closes this connection as well as any Sessions
         * created from it (and those Sessions' consumers and
         * producers).
         * @throws CMSException
         */
        virtual void close() throw ( cms::CMSException );

        /**
         * Starts or (restarts) a connections delivery of incoming messages
         * @throws CMSException
         */
        virtual void start() throw ( cms::CMSException );

        /**
         * Stop the flow of incoming messages
         * @throws CMSException
         */
        virtual void stop() throw ( cms::CMSException );

    public:     // ExceptionListener interface methods

        /**
         * Called when an exception occurs.  Once notified of an exception
         * the caller should no longer use the resource that generated the
         * exception.
         * @param Exception Object that occurred.
         */
        virtual void onException( const cms::CMSException& ex );

    public:     // ConsumerMessageListener interface methods

        /**
         * Called to dispatch a message to a particular consumer.
         * @param consumer the target consumer of the dispatch.
         * @param message the message to be dispatched.
         * @param own If true, it is the responsibility of the callee
         * to destroy the message object.  Otherwise, the callee must NOT
         * destroy it.
         *
         */
        virtual void onConsumerMessage( connector::ConsumerInfo* consumer,
                                        core::ActiveMQMessage* message );

    public:

        /**
         * Notify the exception listener
         * @param ex the exception to fire
         */
        void fire( exceptions::ActiveMQException& ex ) {
            if( exceptionListener != NULL ) {
                try {
                    exceptionListener->onException( ex );
                }
                catch(...){}
            }
        }

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTION_H_*/
