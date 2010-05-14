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

#ifndef _ACTIVEMQ_TRANSPORT_TCP_SSLTRANSPORT_H_
#define _ACTIVEMQ_TRANSPORT_TCP_SSLTRANSPORT_H_

#include <activemq/util/Config.h>

#include <activemq/transport/tcp/TcpTransport.h>

namespace activemq {
namespace transport {
namespace tcp {

    /**
     * Transport for connecting to a Broker using an SSL Socket.  This transport simply
     * wraps the TcpTransport and provides the TcpTransport an SSL based Socket pointer
     * allowing the core TcpTransport logic to be reused.
     *
     * @since 3.2.0
     */
    class AMQCPP_API SslTransport : public TcpTransport {
    private:

        SslTransport( const SslTransport& );
        SslTransport& operator= ( const SslTransport& );

    public:

        /**
         * Creates a new instance of the SslTransport, the transport will not attempt to
         * connect to a remote host until the connect method is called.
         *
         * @param next the next transport in the chain
         */
        SslTransport( const Pointer<Transport>& next );

        virtual ~SslTransport();

    protected:

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::Socket* createSocket();

        /**
         * {@inheritDoc}
         */
        virtual void configureSocket( decaf::net::Socket* socket, decaf::util::Properties& properties );


    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_TCP_SSLTRANSPORT_H_ */
