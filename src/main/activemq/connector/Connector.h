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
#ifndef _ACTIVEMQ_CONNECTOR_CONNECTOR_H_
#define _ACTIVEMQ_CONNECTOR_CONNECTOR_H_

#include <list>

#include <cms/Startable.h>
#include <cms/Closeable.h>
#include <cms/MessageListener.h>
#include <cms/ExceptionListener.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>
#include <cms/Session.h>
#include <cms/BytesMessage.h>
#include <cms/TextMessage.h>
#include <cms/MapMessage.h>

#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

#include <activemq/util/Config.h>
#include <activemq/transport/Transport.h>
#include <activemq/connector/SessionInfo.h>
#include <activemq/connector/ConsumerInfo.h>
#include <activemq/connector/ProducerInfo.h>
#include <activemq/connector/TransactionInfo.h>
#include <activemq/connector/ConsumerMessageListener.h>
#include <activemq/connector/ConnectorException.h>

namespace activemq{
namespace connector{

    // Forward declarations.
    class AMQCPP_API Connector : public cms::Startable,
                                 public cms::Closeable {
    protected:

        // Flags the state we are in for connection to broker.
        enum ConnectionState {
            CONNECTION_STATE_DISCONNECTED,
            CONNECTION_STATE_ERROR,
            CONNECTION_STATE_CONNECTING,
            CONNECTION_STATE_CONNECTED
        };

        // Flags to be applied when sending the Destination Info Command.
        enum DestinationActions {
            DESTINATION_ADD_OPERATION = 0,
            DESTINATION_REMOVE_OPERATION = 1
        };

    public:    // Connector Types

        enum AckType {
            ACK_TYPE_DELIVERED = 0,  // Message delivered but not consumed
            ACK_TYPE_POISON    = 1,  // Message could not be processed due to
                                     // poison pill but discard anyway
            ACK_TYPE_CONSUMED  = 2   // Message consumed, discard
        };

    public:

        virtual ~Connector() {}

        /**
         * Gets the Client Id for this connection, if this
         * connection has been closed, then this method returns ""
         * @return Client Id String
         */
        virtual std::string getClientId() const = 0;

        /**
         * Gets the Username for this connection, if this
         * connection has been closed, then this method returns ""
         * @return Username String
         */
        virtual std::string getUsername() const = 0;

        /**
         * Gets the Password for this connection, if this
         * connection has been closed, then this method returns ""
         * @return Password String
         */
        virtual std::string getPassword() const = 0;

        /**
         * Gets a reference to the Transport that this connection
         * is using.
         * @return reference to a transport
         * @throws InvalidStateException if the Transport is not set
         */
        virtual transport::Transport& getTransport() const
            throw ( decaf::lang::exceptions::InvalidStateException ) = 0;

        /**
         * Creates a Session Info object for this connector
         * @param ackMode Acknowledgement Mode of the Session
         * @returns Session Info Object
         * @throws ConnectorException
         */
        virtual SessionInfo* createSession(
            cms::Session::AcknowledgeMode ackMode )
                throw( ConnectorException ) = 0;

        /**
         * Create a Consumer for the given Session
         * @param destination Destination to Subscribe to.
         * @param session Session Information.
         * @param Message Selector String
         * @param should local messages be delivered back to this consumer
         * @return Consumer Information
         * @throws ConnectorException
         */
        virtual ConsumerInfo* createConsumer(
            const cms::Destination* destination,
            SessionInfo* session,
            const std::string& selector = "",
            bool noLocal = false )
                throw ( ConnectorException ) = 0;

        /**
         * Create a Durable Consumer for the given Session
         * @param topic Topic to Subscribe to.
         * @param session Session Information.
         * @param name name of the Durable Topic
         * @param selector Selector
         * @param noLocal if set, inhibits the delivery of messages
         *        published by its own connection
         * @return Consumer Information
         * @throws ConnectorException
         */
        virtual ConsumerInfo* createDurableConsumer(
            const cms::Topic* topic,
            SessionInfo* session,
            const std::string& name,
            const std::string& selector = "",
            bool noLocal = false )
                throw ( ConnectorException ) = 0;

        /**
         * Given a valid Consumer info Object that was previously created
         * by a call to <code>createConsumer</code>, the Consumer will be
         * registered with the Broker, and be placed in a state in which
         * it will now be able to receive messages.  All preperations
         * for message receipt should be done before calling this method.
         * @param consumer - ConsumerInfo of a consumer that isn't started
         * @throws ConnectorException
         */
        virtual void startConsumer( ConsumerInfo* consumer )
            throw ( ConnectorException ) = 0;

        /**
         * Create a Consumer for the given Session
         * @param destination Destination to Subscribe to.
         * @param session Session Information.
         * @return Producer Information
         * @throws ConnectorException
         */
        virtual ProducerInfo* createProducer(
            const cms::Destination* destination,
            SessionInfo* session )
                throw ( ConnectorException ) = 0;

        /**
         * Creates a Topic given a name and session info
         * @param name Topic Name
         * @param session Session Information
         * @return a newly created Topic Object
         * @throws ConnectorException
         */
        virtual cms::Topic* createTopic( const std::string& name,
                                         SessionInfo* session )
            throw ( ConnectorException ) = 0;

        /**
         * Creates a Queue given a name and session info
         * @param name Queue Name
         * @param session Session Information
         * @return a newly created Queue Object
         * @throws ConnectorException
         */
        virtual cms::Queue* createQueue( const std::string& name,
                                         SessionInfo* session )
            throw ( ConnectorException ) = 0;

        /**
         * Creates a Temporary Topic given a name and session info
         * @param session Session Information
         * @return a newly created Temporary Topic Object
         * @throws ConnectorException
         * @throws UnsupportedOperationException
         */
        virtual cms::TemporaryTopic* createTemporaryTopic(
            SessionInfo* session )
                throw ( ConnectorException, decaf::lang::exceptions::UnsupportedOperationException ) = 0;

        /**
         * Creates a Temporary Queue given a name and session info
         * @param session Session Information
         * @return a newly created Temporary Queue Object
         * @throws ConnectorException
         * @throws UnsupportedOperationException
         */
        virtual cms::TemporaryQueue* createTemporaryQueue(
            SessionInfo* session )
                throw ( ConnectorException, decaf::lang::exceptions::UnsupportedOperationException ) = 0;

        /**
         * Sends a Message
         * @param message The Message to send.
         * @param producerInfo Producer Info for the sender of this message
         * @throws ConnectorException
         */
        virtual void send( cms::Message* message, ProducerInfo* producerInfo )
            throw ( ConnectorException ) = 0;

        /**
         * Sends a set of Messages
         * @param messages List of Messages to send.
         * @param producerInfo Producer Info for the sender of this message
         * @throws ConnectorException
         */
        virtual void send( std::list<cms::Message*>& messages,
                           ProducerInfo* producerInfo)
            throw ( ConnectorException ) = 0;

        /**
         * Acknowledges a Message
         * @param session the Session that the message is linked to
         * @param consumer the Consumer that the message was linked to
         * @param message An ActiveMQMessage to Ack.
         * @param ackType the type of ack to perform
         * @throws ConnectorException
         */
        virtual void acknowledge( const SessionInfo* session,
                                  const ConsumerInfo* consumer,
                                  const cms::Message* message,
                                  AckType ackType = ACK_TYPE_CONSUMED)
            throw ( ConnectorException ) = 0;

        /**
         * Acknowledges a Message set, using the most efficient means possible
         * for the type of connector being used,
         * @param session the Session that the message is linked to
         * @param consumer the Consumer that the message was linked to
         * @param messages A set of ActiveMQMessages to Ack.
         * @param ackType the type of ack to perform
         * @throws ConnectorException
         */
        virtual void acknowledge( const SessionInfo* session,
                                  const ConsumerInfo* consumer,
                                  const std::list<const cms::Message*>& message,
                                  AckType ackType = ACK_TYPE_CONSUMED)
            throw ( ConnectorException ) = 0;

        /**
         * Starts a new Transaction.
         * @param Session Information
         * @throws ConnectorException
         */
        virtual TransactionInfo* startTransaction(
            SessionInfo* session )
                throw ( ConnectorException ) = 0;

        /**
         * Commits a Transaction.
         * @param transaction The Transaction information
         * @param session Session Information
         * @throws ConnectorException
         */
        virtual void commit( TransactionInfo* transaction,
                             SessionInfo* session )
            throw ( ConnectorException ) = 0;

        /**
         * Rolls back a Transaction.
         * @param transaction The Transaction information
         * @param session Session Information
         * @throws ConnectorException
         */
        virtual void rollback( TransactionInfo* transaction,
                               SessionInfo* session )
            throw ( ConnectorException ) = 0;

        /**
         * Creates a new Message.
         * @param session Session Information
         * @param transaction Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::Message* createMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException ) = 0;

        /**
         * Creates a new BytesMessage.
         * @param session Session Information
         * @param transaction Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::BytesMessage* createBytesMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException ) = 0;

        /**
         * Creates a new TextMessage.
         * @param session Session Information
         * @param transaction Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::TextMessage* createTextMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException ) = 0;

        /**
         * Creates a new MapMessage.
         * @param session Session Information
         * @param transaction Transaction Info for this Message
         * @throws ConnectorException
         * @throws UnsupportedOperationException
         */
        virtual cms::MapMessage* createMapMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException, decaf::lang::exceptions::UnsupportedOperationException ) = 0;

        /**
         * Unsubscribe from a givenDurable Subscription
         * @param name name of the Subscription
         * @throws ConnectorException
         * @throws UnsupportedOperationException
         */
        virtual void unsubscribe( const std::string&  )
            throw ( ConnectorException, decaf::lang::exceptions::UnsupportedOperationException ) = 0;

        /**
         * Closes the given connector resource, caller must still delete
         * the resource once its been closed.
         * @param resource the resource to be closed
         * @throws ConnectorException
         */
        virtual void closeResource( ConnectorResource* resource )
            throw ( ConnectorException ) = 0;

        /**
         * Sets the listener of consumer messages.
         * @param listener the ConsumerMessageListener observer.
         */
        virtual void setConsumerMessageListener(
            ConsumerMessageListener* listener ) = 0;

        /**
         * Sets the Listner of exceptions for this connector
         * @param listener the ExceptionListener observer.
         */
        virtual void setExceptionListener(
            cms::ExceptionListener* listener ) = 0;

        /**
         * Checks if this connector supports pull of a new mesage from the service
         * provider, if so then the user can call pullMessage() on the Connector
         * to try and get a new message added to the receive queue.
         * @returns true if the caller can use pullMessage without an exception
         */
        virtual bool isMessagePullSupported() const = 0;

        /**
         * Pulls a message from the the service provider that this Connector is
         * associated with. This could be because the service has a prefetch
         * policy that is set to zero and therefore requires each message to
         * be pulled from the server to the client via a poll.
         *
         * @param info - the consumer info for the consumer to pull for
         * @param timeout - the time that the caller is going to wait for new messages
         * @throw ConnectorException if a communications error occurs
         * @throw UnsupportedOperationException if the connector can't pull
         */
        virtual void pullMessage( const connector::ConsumerInfo* info, long long timeout )
            throw ( ConnectorException, decaf::lang::exceptions::UnsupportedOperationException ) = 0;

        /**
         * Requests that the Broker remove a Destination, destroying all resources that
         * have been associated with it.  The Destination is removed and does not become
         * valid again until a client creates a new Destination with that name again and
         * sends a message that is bound to it.
         *
         * @param destination
         *        The Destination to Remove.
         *
         * @throw ConnectorException if a communications error occurs
         *
         * @throw UnsupportedOperationException if the connector can't pull
         */
        virtual void destroyDestination( const cms::Destination* destination )
            throw ( ConnectorException, decaf::lang::exceptions::UnsupportedOperationException ) = 0;

    };

}}

#endif /*_ACTIVEMQ_CONNECTOR_CONNECTOR_H_*/
