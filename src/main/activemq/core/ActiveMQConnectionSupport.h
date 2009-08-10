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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONSUPPORT_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONSUPPORT_H_

#include <cms/ExceptionListener.h>

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/TransportListener.h>
#include <activemq/util/LongSequenceGenerator.h>

#include <decaf/util/Properties.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/Pointer.h>

#include <memory>

namespace activemq {
namespace core {

    using decaf::lang::Pointer;

    class AMQCPP_API ActiveMQConnectionSupport :
        public transport::TransportListener
    {
    private:

        // Properties used to configure this connection.
        Pointer<decaf::util::Properties> properties;

        // Transport we are using
        Pointer<transport::Transport> transport;

        /**
         * Boolean indicating that we are to always send message Synchronously.
         * This overrides the sending on non-persistent messages or transacted
         * messages asynchronously, also fully overrides the useAsyncSend flag.
         */
        bool alwaysSyncSend;

        /**
         * Boolean indicating that we are to send any messages that we would normally
         * send synchronously using an asynchronous send.  Normally we send all the
         * persistent messages not in a transaction synchronously and all others are
         * sent asynchronously.  Only applied though is alwaysSyncSend is false.
         */
        bool useAsyncSend;

        /**
         * Send Timeout, forces all messages to be sent Synchronously.
         */
        unsigned int sendTimeout;

        /**
         * Close Timeout, time to wait for a Closed message from the broker before
         * giving up and just shutting down the connection.
         */
        unsigned int closeTimeout;

        /**
         * Producer Window Size, amount of memory that can be used before the producer
         * blocks and waits for ProducerAck messages.
         */
        unsigned int producerWindowSize;

        /**
         * The configured User Name
         */
        std::string username;

        /**
         * The configured Password
         */
        std::string password;

        /**
         * The configured Client Id
         */
        std::string clientId;

        /**
         * Next available Session Id.
         */
        util::LongSequenceGenerator sessionIds;

        /**
         * Next Temporary Destination Id
         */
        util::LongSequenceGenerator tempDestinationIds;

        /**
         * Next Local Transaction Id
         */
        util::LongSequenceGenerator localTransactionIds;

    public:

        /**
         * Creates an instance of the ActiveMQConnectionSupport class, the
         * most common properties for a connection are pulled from the
         * properties instance or are set to defaults.
         *
         * @param transport
         *        The Transport that this Connection will use for sending Commands
         *        to the Broker.
         * @param properties
         *        The URI configured properties for this connection.
         */
        ActiveMQConnectionSupport( const Pointer<transport::Transport>& transport,
                                   const Pointer<decaf::util::Properties>& properties );

        virtual ~ActiveMQConnectionSupport();

        /**
         * Starts the Transport, this should initiate the connection between
         * this client and the Transports endpoint.
         * @throws Exception
         */
        virtual void startupTransport() throw( decaf::lang::Exception );

        /**
         * Closes this object and deallocates the appropriate resources.
         * The object is generally no longer usable after calling close.
         * @throws Exception
         */
        virtual void shutdownTransport() throw( decaf::lang::Exception );

        /**
         * Gets the Properties object that this Config object was initialized with.
         * @returns a const reference to the Connection Config.
         */
        const decaf::util::Properties& getProperties() const {
            return *( this->properties.get() );
        }

        /**
         * Gets the Transport Configured for this Connection.
         * @return the configured transport
         */
        transport::Transport& getTransport() const {
            return *( this->transport.get() );
        }

        /**
         * Gets if the Connection should always send things Synchronously.
         * @return true if sends should always be Synchronous.
         */
        bool isAlwaysSyncSend() const {
            return this->alwaysSyncSend;
        }

        /**
         * Sets if the Connection should always send things Synchronously.
         * @param value
         *        true if sends should always be Synchronous.
         */
        void setAlwaysSyncSend( bool value ) {
            this->alwaysSyncSend = value;
        }

        /**
         * Gets if the useAsyncSend option is set
         * @returns true if on false if not.
         */
        bool isUseAsyncSend() const {
            return this->useAsyncSend;
        }

        /**
         * Sets the useAsyncSend option
         * @param value - true to activate, false to disable.
         */
        void setUseAsyncSend( bool value ) {
            this->useAsyncSend = value;
        }

        /**
         * Gets the assigned send timeout for this Connector
         * @return the send timeout configured in the connection uri
         */
        unsigned int getSendTimeout() const {
            return this->sendTimeout;
        }

        /**
         * Sets the send timeout to use when sending Message objects, this will
         * cause all messages to be sent using a Synchronous request is non-zero.
         * @param timeout - The time to wait for a response.
         */
        void setSendTimeout( unsigned int timeout ) {
            this->sendTimeout = timeout;
        }

        /**
         * Gets the assigned close timeout for this Connector
         * @return the close timeout configured in the connection uri
         */
        unsigned int getCloseTimeout() const {
            return this->closeTimeout;
        }

        /**
         * Sets the close timeout to use when sending the disconnect request.
         * @param timeout - The time to wait for a close message.
         */
        void setCloseTimeout( unsigned int timeout ) {
            this->closeTimeout = timeout;
        }

        /**
         * Gets the configured producer window size for Producers that are created
         * from this connector.  This only applies if there is no send timeout and the
         * producer is able to send asynchronously.
         * @return size in bytes of messages that this producer can produce before
         *         it must block and wait for ProducerAck messages to free resources.
         */
        unsigned int getProducerWindowSize() const {
            return this->producerWindowSize;
        }

        /**
         * Sets the size in Bytes of messages that a producer can send before it is blocked
         * to await a ProducerAck from the broker that frees enough memory to allow another
         * message to be sent.
         * @param windowSize - The size in bytes of the Producers memory window.
         */
        void setProducerWindowSize( unsigned int windowSize ) {
            this->producerWindowSize = windowSize;
        }

        /**
         * Gets the Configured Username.
         * @return the username.
         */
        std::string getUsername() const {
            return this->username;
        }

        /**
         * Sets the Username.
         * @param username - The new username value.
         */
        void setUsername( const std::string& username ) {
            this->username = username;
        }

        /**
         * Gets the Configured Password.
         * @return the password.
         */
        std::string getPassword() const {
            return this->password;
        }

        /**
         * Sets the Password.
         * @param password - The new password value.
         */
        void setPassword( const std::string& password ) {
            this->password = password;
        }

        /**
         * Gets the Configured Client Id.
         * @return the clientId.
         */
        std::string getClientId() const {
            return this->clientId;
        }

        /**
         * Sets the Client Id.
         * @param clientId - The new clientId value.
         */
        void setClientId( const std::string& clientId ) {
            this->clientId = clientId;
        }

        /**
         * Get the Next available Session Id.
         * @return the next id in the sequence.
         */
        long long getNextSessionId() {
            return this->sessionIds.getNextSequenceId();
        }

        /**
         * Get the Next Temporary Destination Id
         * @return the next id in the sequence.
         */
        long long getNextTempDestinationId() {
            return this->tempDestinationIds.getNextSequenceId();
        }

        /**
         * Get the Next Temporary Destination Id
         * @return the next id in the sequence.
         */
        long long getNextLocalTransactionId() {
            return this->localTransactionIds.getNextSequenceId();
        }

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTIONSUPPORT_H_*/
