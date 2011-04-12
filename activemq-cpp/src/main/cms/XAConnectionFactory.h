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

#ifndef _CMS_XACONNECTIONFACTORY_H_
#define _CMS_XACONNECTIONFACTORY_H_

#include <cms/Config.h>

#include <cms/XAConnection.h>
#include <cms/XAException.h>

namespace cms {

    /**
     * The XAConnectionFactory interface is specialized interface that defines an
     * ConnectionFactory that creates Connection instance that will participate in
     * XA Transactions.
     *
     * Some application provide support for grouping XA capable resource use into
     * a distributed transaction (optional). To include CMS API transactions in a
     * XA transaction, an application requires a XA aware library.  A CMS provider
     * exposes its XA support using an XAConnectionFactory object, which an application
     * uses to create XAConnection objects.
     *
     * The XAConnectionFactory interface is optional. CMS providers are not required
     * to support this interface. This interface is for use by CMS providers to support
     * transactional environments. Client programs are strongly encouraged to use the
     * transactional support available in their environment, rather than use these
     * XA interfaces directly.
     *
     * @since 2.3
     */
    class CMS_API XAConnectionFactory {
    public:

        virtual ~XAConnectionFactory();

        /**
         * Creates an XAConnection with the default user name and password. The connection is created
         * in stopped mode just as the standard Connection object is created from the ConnectionFactory.
         * No messages will be delivered until the Connection.start method is explicitly called.
         *
         * @returns a new XAConnectionFactory instance, the caller owns the returned pointer.
         *
         * @throws CMSException if an internal error occurs while creating the Connection.
         * @throws CMSSecurityException if the client authentication fails because the user name or
         *                              password are invalid.
         */
        virtual XAConnection* createXAConnection() = 0;

        /**
         * Creates an XA connection with the specified user name and password. The connection is
         * created in stopped mode just as the standard ConnectionFactory creates a new Connection.
         * No messages will be delivered until the Connection.start method is explicitly called.
         *
         * @returns a new XAConnectionFactory instance, the caller owns the returned pointer.
         *
         * @throws CMSException if an internal error occurs while creating the Connection.
         * @throws CMSSecurityException if the client authentication fails because the user name or
         *                              password are invalid.
         */
        virtual XAConnection* createXAConnection( const std::string& userName,
                                                  const std::string& password ) = 0;
    public:

        /**
         * Static method that is used to create a provider specific XA Connection
         * factory.  The provider implements this method in their library and
         * returns an instance of a XAConnectionFactory derived object.  Clients can
         * use this method to remain abstracted from the specific CMS implementation
         * being used.
         *
         * The XA interfaces are optional in CMS however if a provider chooses to omit
         * them it should still override this method and throw an UnsupportedOperationException
         * to indicate that it doesn't provide this functionality.
         *
         * @param brokerURI
         *      The remote address to use to connect to the Provider.
         *
         * @return A pointer to a provider specific implementation of the XAConnectionFactory
         *         interface, the caller is responsible for deleting this resource.
         *
         * @throws CMSException if an internal error occurs while creating the XAConnectionFactory.
         * @throws UnsupportedOperationException if the provider does not support the XA API.
         */
        static XAConnectionFactory* createCMSXAConnectionFactory( const std::string& brokerURI );

    };

}

#endif /* _CMS_XACONNECTIONFACTORY_H_ */
