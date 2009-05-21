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

#ifndef _CMS_CONNECTION_H_
#define _CMS_CONNECTION_H_

#include <cms/Config.h>
#include <cms/Startable.h>
#include <cms/Stoppable.h>
#include <cms/Closeable.h>
#include <cms/Session.h>
#include <cms/ConnectionMetaData.h>

namespace cms{

    class ExceptionListener;

    /**
     * The client's connection to its provider.
     * <p>
     * Connections support concurrent use.
     * <p>
     * A connection serves several purposes:
     *  - It encapsulates an open connection with a JMS provider. It typically represents
     *    an open TCP/IP socket between a client and the service provider software.
     *  - Its creation is where client authentication takes place.
     *  - It can specify a unique client identifier.
     *  - It provides a ConnectionMetaData object.
     *  - It supports an optional ExceptionListener object.
     *
     * Because the creation of a connection involves setting up authentication and communication,
     * a connection is a relatively heavy-weight object. Most clients will do all their messaging
     * with a single connection. Other more advanced applications may use several connections.
     * The CMS API does not architect a reason for using multiple connections; however, there
     * may be operational reasons for doing so.
     * <p>
     * A CMS client typically creates a connection, one or more sessions, and a number of message
     * producers and consumers. When a connection is created, it is in stopped mode. That means
     * that no messages are being delivered.
     * <p>
     * It is typical to leave the connection in stopped mode until setup is complete (that is,
     * until all message consumers have been created). At that point, the client calls the
     * connection's start method, and messages begin arriving at the connection's consumers.
     * This setup convention minimizes any client confusion that may result from asynchronous
     * message delivery while the client is still in the process of setting itself up.
     * <p>
     * A connection can be started immediately, and the setup can be done afterwards. Clients
     * that do this must be prepared to handle asynchronous message delivery while they are still
     * in the process of setting up.
     * <p>
     * A message producer can send messages while a connection is stopped.
     *
     * @since 1.0
     */
    class CMS_API Connection : public Startable,
                               public Stoppable,
                               public Closeable
    {
    public:

        virtual ~Connection() {}

        /**
         * Closes this connection as well as any Sessions
         * created from it (and those Sessions' consumers and
         * producers).
         *
         * @throws CMSException
         */
        virtual void close() throw( CMSException ) = 0;

        /**
         * Gets the metadata for this connection.
         *
         * @returns the connection MetaData pointer ( caller does not own it ).
         *
         * @throws CMSException
         *         if the provider fails to get the connection metadata for this connection.
         *
         * @see ConnectionMetaData
         * @since 2.0
         */
        virtual const ConnectionMetaData* getMetaData() const throw( CMSException ) = 0;

        /**
         * Creates an AUTO_ACKNOWLEDGE Session.
         *
         * @throws CMSException
         */
        virtual Session* createSession() throw ( CMSException ) = 0;

        /**
         * Creates a new Session to work for this Connection using the
         * specified acknowledgment mode
         *
         * @param ackMode
         *        the Acknowledgment Mode to use.
         * @throws CMSException
         */
        virtual Session* createSession( Session::AcknowledgeMode ackMode )
            throw ( CMSException ) = 0;

        /**
         * Get the Client Id for this session
         *
         * @return Client Id String
         */
        virtual std::string getClientID() const = 0;

        /**
         * Gets the registered Exception Listener for this connection
         *
         * @return pointer to an exception listener or NULL
         */
        virtual ExceptionListener* getExceptionListener() const = 0;

        /**
         * Sets the registered Exception Listener for this connection
         *
         * @param listener
         *        pointer to and <code>ExceptionListener</code>
         */
        virtual void setExceptionListener( ExceptionListener* listener ) = 0;

    };

}

#endif /*_CMS_CONNECTION_H_*/
