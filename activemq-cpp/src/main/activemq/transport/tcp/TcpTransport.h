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

#ifndef _ACTIVEMQ_TRANSPORT_TCP_TCPTRANSPORT_H_
#define _ACTIVEMQ_TRANSPORT_TCP_TCPTRANSPORT_H_

#include <activemq/io/LoggingInputStream.h>
#include <activemq/io/LoggingOutputStream.h>
#include <activemq/util/Config.h>
#include <activemq/transport/TransportFilter.h>
#include <decaf/net/Socket.h>
#include <decaf/net/URI.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/Pointer.h>
#include <decaf/io/BufferedInputStream.h>
#include <decaf/io/BufferedOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <memory>

namespace activemq{
namespace transport{
namespace tcp{

    using decaf::lang::Pointer;

    /**
     * Implements a TCP/IP based transport filter, this transport
     * is meant to wrap an instance of an IOTransport.  The lower
     * level transport should take care of managing stream reads
     * and writes.
     */
    class AMQCPP_API TcpTransport : public TransportFilter {
    private:

        /**
         * has close been called.
         */
        bool closed;

        /**
         * Socket that this Transport Communicates with
         */
        std::auto_ptr<decaf::net::Socket> socket;

        /**
         * Input Stream for Reading in Messages
         */
        std::auto_ptr<decaf::io::DataInputStream> dataInputStream;

        /**
         * Output Stream for Writing out Messages.
         */
        std::auto_ptr<decaf::io::DataOutputStream> dataOutputStream;

    public:

        /**
         * Constructor
         * @param properties the configuration properties for this transport
         * @param next the next transport in the chain
         */
        TcpTransport( const decaf::util::Properties& properties,
                      const Pointer<Transport>& next );

        /**
         * Constructor
         * @param uri - The URI containing the host to connect to.
         * @param properties the configuration properties for this transport
         * @param next the next transport in the chain
         */
        TcpTransport( const decaf::net::URI& uri,
                      const decaf::util::Properties& properties,
                      const Pointer<Transport>& next );

        virtual ~TcpTransport();

        /**
         * Delegates to the superclass and then closes the socket.
         * @throws IOException if errors occur.
         */
        virtual void close() throw( decaf::io::IOException );

        /**
         * Is this Transport fault tolerant, meaning that it will reconnect to
         * a broker on disconnect.
         *
         * @returns true if the Transport is fault tolerant.
         */
        virtual bool isFaultTolerant() const {
            return false;
        }

        /**
         * Is the Transport Connected to its Broker.
         *
         * @returns true if a connection has been made.
         */
        virtual bool isConnected() const {
            if( this->socket.get() != NULL ) {
                return this->socket->isConnected();
            }

            return false;
        }

        /**
         * Has the Transport been shutdown and no longer usable.
         *
         * @returns true if the Transport
         */
        virtual bool isClosed() const {
            return this->closed;
        }

    private:

        void initialize( const decaf::net::URI& uri,
                         const decaf::util::Properties& properties );

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_TCP_TCPTRANSPORT_H_*/
