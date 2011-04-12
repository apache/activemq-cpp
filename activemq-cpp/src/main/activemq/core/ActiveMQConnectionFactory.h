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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONFACTORY_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONFACTORY_H_

#include <activemq/util/Config.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>

#include <activemq/transport/Transport.h>

#include <decaf/net/URI.h>
#include <decaf/util/Properties.h>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;

    class ActiveMQConnection;
    class FactorySettings;
    class PrefetchPolicy;
    class RedeliveryPolicy;

    class AMQCPP_API ActiveMQConnectionFactory : public cms::ConnectionFactory {
    public:

        // Default Broker URI if none specified
        static const std::string DEFAULT_URI;

    private:

        // d-Pointer holding pre-configured factory settings
        FactorySettings* settings;

    private:

        ActiveMQConnectionFactory( const ActiveMQConnectionFactory& );
        ActiveMQConnectionFactory& operator= ( const ActiveMQConnectionFactory& );

    public:

        ActiveMQConnectionFactory();

        /**
         * Constructor
         * @param url the URI of the Broker we are connecting to.
         * @param username to authenticate with, defaults to ""
         * @param password to authenticate with, defaults to ""
         */
        ActiveMQConnectionFactory( const std::string& uri,
                                   const std::string& username = "",
                                   const std::string& password = "" );

        /**
         * Constructor
         * @param uri the URI of the Broker we are connecting to.
         * @param username to authenticate with, defaults to ""
         * @param password to authenticate with, defaults to ""
         */
        ActiveMQConnectionFactory( const decaf::net::URI& uri,
                                   const std::string& username = "",
                                   const std::string& password = "" );

        virtual ~ActiveMQConnectionFactory() throw();

        /**
         * Creates a connection with the default user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.
         * @returns a Connection Pointer
         * @throws CMSException
         */
        virtual cms::Connection* createConnection();

        /**
         * Creates a connection with the specified user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.  The username and password values passed here do not
         * change the defaults, subsequent calls to the parameterless
         * createConnection will continue to use the default values that
         * were set in the Constructor.
         * @param username to authenticate with
         * @param password to authenticate with
         * @returns a Connection Pointer
         * @throws CMSException
         */
        virtual cms::Connection* createConnection( const std::string& username,
                                                   const std::string& password );

        /**
         * Creates a connection with the specified user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.  The username and password values passed here do not
         * change the defaults, subsequent calls to the parameterless
         * createConnection will continue to use the default values that
         * were set in the Constructor.
         * @param username to authenticate with
         * @param password to authenticate with
         * @param clientId to assign to connection if "" then a random cleint
         *        Id is created for this connection.
         * @returns a Connection Pointer
         * @throws CMSException
         */
        virtual cms::Connection* createConnection( const std::string& username,
                                                   const std::string& password,
                                                   const std::string& clientId );

    public:   // Configuration Options

        /**
         * Sets the username that should be used when creating a new connection
         * @param username string
         */
        void setUsername( const std::string& username );

        /**
         * Gets the username that this factory will use when creating a new
         * connection instance.
         * @return username string, "" for default credentials
         */
        const std::string& getUsername() const;

        /**
         * Sets the password that should be used when creating a new connection
         * @param password string
         */
        void setPassword( const std::string& password );

        /**
         * Gets the password that this factory will use when creating a new
         * connection instance.
         * @return password string, "" for default credentials
         */
        const std::string& getPassword() const;

        /**
         * Gets the Configured Client Id.
         * @return the clientId.
         */
        std::string getClientId() const;

        /**
         * Sets the Client Id.
         * @param clientId - The new clientId value.
         */
        void setClientId( const std::string& clientId );

        /**
         * Sets the Broker URI that should be used when creating a new connection instance.
         *
         * @param brokerURI
         *      The string form of the Broker URI, this will be converted to a URI object.
         */
        void setBrokerURI( const std::string& uri );

        /**
         * Sets the Broker URI that should be used when creating a new connection instance.
         *
         * @param brokerURI
         *      The URI of the broker that this client will connect to.
         */
        void setBrokerURI( const decaf::net::URI& uri );

        /**
         * Gets the Broker URI that this factory will use when creating a new
         * connection instance.
         * @return brokerURI string
         */
        const decaf::net::URI& getBrokerURI() const;

        /**
         * Set an CMS ExceptionListener that will be set on eat connection once it has been
         * created.  The factory does not take ownership of this pointer, the client must ensure
         * that its lifetime is scoped to the connection that it is applied to.
         *
         * @param listener
         * 		The listener to set on the connection or NULL for no listener.
         */
        void setExceptionListener( cms::ExceptionListener* listener );

        /**
         * Returns the currently set ExceptionListener that will be set on any new Connection
         * instance that is created by this factory.
         *
         * @return a pointer to a CMS ExceptionListener instance or NULL if not set.
         */
        cms::ExceptionListener* getExceptionListener() const;

        /**
         * Sets the PrefetchPolicy instance that this factory should use when it creates
         * new Connection instances.  The PrefetchPolicy passed becomes the property of the
         * factory and will be deleted when the factory is destroyed.
         *
         * @param policy
         *      The new PrefetchPolicy that the ConnectionFactory should clone for Connections.
         */
        void setPrefetchPolicy( PrefetchPolicy* policy );

        /**
         * Gets the pointer to the current PrefetchPolicy that is in use by this ConnectionFactory.
         *
         * @returns a pointer to this objects PrefetchPolicy.
         */
        PrefetchPolicy* getPrefetchPolicy() const;

        /**
         * Sets the RedeliveryPolicy instance that this factory should use when it creates
         * new Connection instances.  The RedeliveryPolicy passed becomes the property of the
         * factory and will be deleted when the factory is destroyed.
         *
         * @param policy
         *      The new RedeliveryPolicy that the ConnectionFactory should clone for Connections.
         */
        void setRedeliveryPolicy( RedeliveryPolicy* policy );

        /**
         * Gets the pointer to the current RedeliveryPolicy that is in use by this ConnectionFactory.
         *
         * @returns a pointer to this objects RedeliveryPolicy.
         */
        RedeliveryPolicy* getRedeliveryPolicy() const;

        /**
         * @return The value of the dispatch asynchronously option sent to the broker.
         */
        bool isDispatchAsync() const;

        /**
         * Should messages be dispatched synchronously or asynchronously from the producer
         * thread for non-durable topics in the broker? For fast consumers set this to false.
         * For slow consumers set it to true so that dispatching will not block fast consumers. .
         *
         * @param value
         *        The value of the dispatch asynchronously option sent to the broker.
         */
        void setDispatchAsync( bool value );

        /**
         * Gets if the Connection should always send things Synchronously.
         *
         * @return true if sends should always be Synchronous.
         */
        bool isAlwaysSyncSend() const;

        /**
         * Sets if the Connection should always send things Synchronously.
         * @param value
         *        true if sends should always be Synchronous.
         */
        void setAlwaysSyncSend( bool value );

        /**
         * Gets if the useAsyncSend option is set
         * @returns true if on false if not.
         */
        bool isUseAsyncSend() const;

        /**
         * Sets the useAsyncSend option
         * @param value - true to activate, false to disable.
         */
        void setUseAsyncSend( bool value );

        /**
         * Gets if the Connection is configured for Message body compression.
         * @returns if the Message body will be Compressed or not.
         */
        bool isUseCompression() const;

        /**
         * Sets whether Message body compression is enabled.
         *
         * @param value
         *      Boolean indicating if Message body compression is enabled.
         */
        void setUseCompression( bool value );

        /**
         * Sets the Compression level used when Message body compression is enabled, a
         * value of -1 causes the Compression Library to use the default setting which
         * is a balance of speed and compression.  The range of compression levels is
         * [0..9] where 0 indicates best speed and 9 indicates best compression.
         *
         * @param value
         *      A signed int value that controls the compression level.
         */
        void setCompressionLevel( int value );

        /**
         * Gets the currently configured Compression level for Message bodies.
         *
         * @return the int value of the current compression level.
         */
        int getCompressionLevel() const;

        /**
         * Gets the assigned send timeout for this Connector
         * @return the send timeout configured in the connection uri
         */
        unsigned int getSendTimeout() const;

        /**
         * Sets the send timeout to use when sending Message objects, this will
         * cause all messages to be sent using a Synchronous request is non-zero.
         * @param timeout - The time to wait for a response.
         */
        void setSendTimeout( unsigned int timeout );

        /**
         * Gets the assigned close timeout for this Connector
         * @return the close timeout configured in the connection uri
         */
        unsigned int getCloseTimeout() const;

        /**
         * Sets the close timeout to use when sending the disconnect request.
         * @param timeout - The time to wait for a close message.
         */
        void setCloseTimeout( unsigned int timeout );

        /**
         * Gets the configured producer window size for Producers that are created
         * from this connector.  This only applies if there is no send timeout and the
         * producer is able to send asynchronously.
         * @return size in bytes of messages that this producer can produce before
         *         it must block and wait for ProducerAck messages to free resources.
         */
        unsigned int getProducerWindowSize() const;

        /**
         * Sets the size in Bytes of messages that a producer can send before it is blocked
         * to await a ProducerAck from the broker that frees enough memory to allow another
         * message to be sent.
         * @param windowSize - The size in bytes of the Producers memory window.
         */
        void setProducerWindowSize( unsigned int windowSize );

        /**
         * @returns true if the Connections that this factory creates should support the
         * message based priority settings.
         */
        bool isMessagePrioritySupported() const;

        /**
         * Set whether or not this factory should create Connection objects with the Message
         * priority support function enabled.
         *
         * @param value
         *      Boolean indicating if Message priority should be enabled.
         */
        void setMessagePrioritySupported( bool value );

    public:

        /**
         * Creates a connection with the specified user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly called.
         *
         * @param uri
         *      The URI of the Broker we are connecting to.
         * @param username
         *      The name of the user to authenticate with.
         * @param password
         *      The password for the user to authenticate with.
         * @param clientId
         *      The unique client id to assign to connection, defaults to "".
         *
         * @throw CMSException.
         */
        static cms::Connection* createConnection( const std::string& uri,
                                                  const std::string& username,
                                                  const std::string& password,
                                                  const std::string& clientId = "" );

    protected:

        /**
         * Create a new ActiveMQConnection instnace using the provided Transport and Properties.
         * Subclasses can override this to control the actual type of ActiveMQConnection that
         * is created.
         *
         * @param transport
         *      The Transport that the Connection should use to communicate with the Broker.
         * @param properties
         *      The Properties that are assigned to the new Connection instance.
         *
         * @returns a new ActiveMQConnection pointer instance.
         */
        virtual ActiveMQConnection* createActiveMQConnection( const Pointer<transport::Transport>& transport,
                                                              const Pointer<decaf::util::Properties>& properties );

    private:

        cms::Connection* doCreateConnection( const decaf::net::URI& uri,
                                             const std::string& username,
                                             const std::string& password,
                                             const std::string& clientId );

        void configureConnection( ActiveMQConnection* connection );

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTIONFACTORY_H_*/
