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
#ifndef _CMS_CONNECTIONFACTORY_H_
#define _CMS_CONNECTIONFACTORY_H_

#include <cms/Config.h>
#include <cms/CMSException.h>

#include <string>

namespace cms{

    class Connection;
    class ExceptionListener;
    class MessageTransformer;

    /**
     * Defines the interface for a factory that creates connection objects, the Connection
     * objects returned implement the CMS Connection interface and hide the CMS Provider
     * specific implementation details behind that interface.  A Client creates a new
     * ConnectionFactory either directly by instantiating the provider specific implementation
     * of the factory or by using the static method <code>createCMSConnectionFactory</code>
     * which all providers are required to implement.
     *
     * @since 1.0
     */
    class CMS_API ConnectionFactory {
    public:

        virtual ~ConnectionFactory();

        /**
         * Creates a connection with the default user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.
         *
         * @return A pointer to a connection object, caller owns the pointer and
         *         is responsible for closing the connection and deleting the instance.
         *
         * @throws CMSException if an internal error occurs while creating the Connection.
         */
        virtual cms::Connection* createConnection() = 0;

        /**
         * Creates a connection with the default specified identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.  The username and password values passed here do not
         * change the defaults, subsequent calls to the parameterless
         * createConnection will continue to use the default values that
         * were set in the Constructor.
         *
         * @param username
         *      The user name used to authenticate with the Provider.
         * @param password
         *      The password used to authenticate with the Provider.
         *
         * @return A pointer to a connection object, caller owns the pointer and
         *         is responsible for closing the connection and deleting the instance.
         * @throws CMSException if an internal error occurs while creating the Connection.
         */
        virtual cms::Connection* createConnection(const std::string& username, const std::string& password) = 0;

        /**
         * Creates a connection with the specified user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.  The user name and password values passed here do not
         * change the defaults, subsequent calls to the parameterless
         * createConnection will continue to use the default values that
         * were set in the Constructor.
         *
         * @param username
         *      The user name used to authenticate with the Provider.
         * @param password
         *      The password used to authenticate with the Provider.
         * @param clientId
         *      The Client Id assigned to connection.  If the id is the empty string ("")
         *      then a random client Id is created for this connection.
         *
         * @return A pointer to a connection object, caller owns the pointer and
         *         is responsible for closing the connection and deleting the instance.
         * @throws CMSException if an internal error occurs while creating the Connection.
         */
        virtual cms::Connection* createConnection(const std::string& username, const std::string& password, const std::string& clientId) = 0;

        /**
         * Set an ExceptionListener instance that is passed on to all Connection objects created from
         * this ConnectionFactory
         *
         * @param transformer
         *      Pointer to the cms::ExceptionListener to set on all newly created Connection objects/
         */
        virtual void setExceptionListener(cms::ExceptionListener* listener) = 0;

        /**
         * Gets the currently configured ExceptionListener for this ConnectionFactory.
         *
         * The CMS code never takes ownership of the ExceptionListener pointer which implies that
         * the client code must ensure that the object remains valid for the lifetime of the CMS
         * object to which the ExceptionListener has been assigned.
         *
         * @returns the pointer to the currently set cms::ExceptionListener.
         */
        virtual cms::ExceptionListener* getExceptionListener() const = 0;

        /**
         * Set an MessageTransformer instance that is passed on to all Connection objects created from
         * this ConnectionFactory
         *
         * The CMS code never takes ownership of the MessageTransformer pointer which implies that
         * the client code must ensure that the object remains valid for the lifetime of the CMS
         * object to which the MessageTransformer has been assigned.
         *
         * @param transformer
         *      Pointer to the cms::MessageTransformer to set on all newly created Connection objects.
         */
        virtual void setMessageTransformer(cms::MessageTransformer* transformer) = 0;

        /**
         * Gets the currently configured MessageTransformer for this ConnectionFactory.
         *
         * @returns the pointer to the currently set cms::MessageTransformer.
         */
        virtual cms::MessageTransformer* getMessageTransformer() const = 0;

    public:

        /**
         * Static method that is used to create a provider specific connection
         * factory.  The provider implements this method in their library and
         * returns an instance of a ConnectionFactory derived object.  Clients can
         * use this method to remain abstracted from the specific CMS implementation
         * being used.
         *
         * @param brokerURI
         *      The remote address to use to connect to the Provider.
         *
         * @return A pointer to a provider specific implementation of the ConnectionFactory
         *         interface, the caller is responsible for deleting this resource.
         *
         * @throws CMSException if an internal error occurs while creating the ConnectionFactory.
         */
        static cms::ConnectionFactory* createCMSConnectionFactory(const std::string& brokerURI);

    };

}

#endif /*_CMS_CONNECTIONFACTORY_H_*/
