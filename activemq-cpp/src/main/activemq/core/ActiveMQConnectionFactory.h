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

namespace activemq{
namespace core{

    class FactorySettings;

    class AMQCPP_API ActiveMQConnectionFactory : public cms::ConnectionFactory {
    private:

        // d-Pointer holding pre-configured factory settings
        FactorySettings* settings;

    public:

        ActiveMQConnectionFactory();

        /**
         * Constructor
         * @param url the URL of the Broker we are connecting to.
         * @param username to authenticate with, defaults to ""
         * @param password to authenticate with, defaults to ""
         */
        ActiveMQConnectionFactory( const std::string& url,
                                   const std::string& username = "",
                                   const std::string& password = "" );

        virtual ~ActiveMQConnectionFactory() {}

        /**
         * Creates a connection with the default user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.
         * @returns a Connection Pointer
         * @throws CMSException
         */
        virtual cms::Connection* createConnection()
            throw ( cms::CMSException );

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
                                                   const std::string& password )
            throw ( cms::CMSException );

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
                                                   const std::string& clientId )
            throw ( cms::CMSException );

        /**
         * Sets the username that should be used when creating a new connection
         * @param username string
         */
        virtual void setUsername( const std::string& username );

        /**
         * Gets the username that this factory will use when creating a new
         * connection instance.
         * @return username string, "" for default credentials
         */
        virtual const std::string& getUsername() const;

        /**
         * Sets the password that should be used when creating a new connection
         * @param password string
         */
        virtual void setPassword( const std::string& password );

        /**
         * Gets the password that this factory will use when creating a new
         * connection instance.
         * @return password string, "" for default credentials
         */
        virtual const std::string& getPassword() const;

        /**
         * Sets the Broker URL that should be used when creating a new
         * connection instance
         * @param brokerURL string
         */
        virtual void setBrokerURL( const std::string& brokerURL );

        /**
         * Gets the Broker URL that this factory will use when creating a new
         * connection instance.
         * @return brokerURL string
         */
        virtual const std::string& getBrokerURL() const;

        /**
         * Set an CMS ExceptionListener that will be set on eat connection once it has been
         * created.  The factory des not take ownership of this pointer, the client must ensure
         * that its lifetime is scoped to the connection that it is applied to.
         *
         * @param listener
         * 		The listener to set on the connection or NULL for no listener.
         */
        virtual void setExceptionListener( cms::ExceptionListener* listener );

        /**
         * Returns the currently set ExceptionListener that will be set on any new Connection
         * instance that is created by this factory.
         *
         * @return a pointer to a CMS ExceptionListener instance or NULL if not set.
         */
        virtual cms::ExceptionListener* getExceptionListener() const;

    public:

        /**
         * Creates a connection with the specified user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly called.
         * @param url the URL of the Broker we are connecting to.
         * @param username to authenticate with
         * @param password to authenticate with
         * @param clientId to assign to connection, defaults to ""
         * @throw CMSException.
         */
        static cms::Connection* createConnection( const std::string& url,
                                                  const std::string& username,
                                                  const std::string& password,
                                                  const std::string& clientId = "" )
            throw ( cms::CMSException );

    public:

        virtual cms::Connection* doCreateConnection( const std::string& url,
                                                     const std::string& username,
                                                     const std::string& password,
                                                     const std::string& clientId )
            throw ( cms::CMSException );

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTIONFACTORY_H_*/
