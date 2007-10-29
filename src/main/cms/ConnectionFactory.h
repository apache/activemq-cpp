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
#include <cms/Connection.h>
#include <cms/CMSException.h>

#include <string>

namespace cms
{

    /**
     * Defines the interface for a factory that creates connection objects
     */
    class CMS_API ConnectionFactory
    {
    public:

        virtual ~ConnectionFactory() {}

        /**
         * Creates a connection with the default user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.
         *
         * @return Pointer to a connection object, caller owns the pointer
         * @throws CMSException
         */
        virtual Connection* createConnection() throw ( CMSException ) = 0;

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
         *      to authenticate with
         * @param password
         *      to authenticate with
         * @returns a Connection Pointer
         * @throws CMSException
         */
        virtual cms::Connection* createConnection( const std::string& username,
                                                   const std::string& password )
            throw ( cms::CMSException ) = 0;

        /**
         * Creates a connection with the specified user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.  The username and password values passed here do not
         * change the defaults, subsequent calls to the parameterless
         * createConnection will continue to use the default values that
         * were set in the Constructor.
         *
         * @param username
         *      to authenticate with
         * @param password
         *      to authenticate with
         * @param clientId
         *      to assign to connection if "" then a random client Id is
         *      created for this connection.
         * @returns a Connection Pointer
         * @throws CMSException
         */
        virtual cms::Connection* createConnection( const std::string& username,
                                                   const std::string& password,
                                                   const std::string& clientId )
            throw ( cms::CMSException ) = 0;

    public:

        /**
         * Static method that is used to create a provider specfic connection
         * factory.  The provider implements this method in their library and
         * returns an instance of a ConnectionFactory dervied object.
         * @param brokerURI - the address to use to connect to the broker.
         * @returns Provider specific ConnectionFactory
         * @throws CMSException if and error occurs.
         */
        static ConnectionFactory* createCMSConnectionFactory( const std::string& brokerURI )
            throw ( cms::CMSException );

    };

}

#endif /*_CMS_CONNECTIONFACTORY_H_*/
