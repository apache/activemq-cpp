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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQXACONNECTIONFACTORY_H_
#define _ACTIVEMQ_CORE_ACTIVEMQXACONNECTIONFACTORY_H_

#include <activemq/util/Config.h>

#include <cms/XAConnectionFactory.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <decaf/net/URI.h>
#include <decaf/util/Properties.h>

namespace activemq {
namespace core {

    using decaf::lang::Pointer;

    class AMQCPP_API ActiveMQXAConnectionFactory : public cms::XAConnectionFactory,
                                                   public ActiveMQConnectionFactory {
    public:

        ActiveMQXAConnectionFactory();

        /**
         * Constructor
         * @param uri the URI of the Broker we are connecting to.
         * @param username to authenticate with, defaults to ""
         * @param password to authenticate with, defaults to ""
         */
        ActiveMQXAConnectionFactory( const std::string& uri,
                                     const std::string& username = "",
                                     const std::string& password = "" );

        /**
         * Constructor
         * @param uri the URI of the Broker we are connecting to.
         * @param username to authenticate with, defaults to ""
         * @param password to authenticate with, defaults to ""
         */
        ActiveMQXAConnectionFactory( const decaf::net::URI& uri,
                                     const std::string& username = "",
                                     const std::string& password = "" );

        virtual ~ActiveMQXAConnectionFactory() throw();

        virtual cms::XAConnection* createXAConnection();

        virtual cms::XAConnection* createXAConnection( const std::string& userName,
                                                       const std::string& password );

    protected:

        virtual ActiveMQConnection* createActiveMQConnection( const Pointer<transport::Transport>& transport,
                                                              const Pointer<decaf::util::Properties>& properties );

    };

}}

#endif /* _ACTIVEMQ_CORE_ACTIVEMQXACONNECTIONFACTORY_H_ */
