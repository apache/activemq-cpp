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

#include <activemq/util/Config.h>
#include <activemq/transport/TransportFilter.h>
#include <decaf/net/Socket.h>
#include <decaf/net/URI.h>
#include <decaf/util/Properties.h>
#include <activemq/io/LoggingInputStream.h>
#include <activemq/io/LoggingOutputStream.h>
#include <decaf/io/BufferedInputStream.h>
#include <decaf/io/BufferedOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>

namespace activemq{
namespace transport{
namespace tcp{

    /**
     * Implements a TCP/IP based transport filter, this transport
     * is meant to wrap an instance of an IOTransport.  The lower
     * level transport should take care of managing stream reads
     * and writes.
     */
    class AMQCPP_API TcpTransport : public TransportFilter {
    private:

        /**
         * Socket that this Transport Communicates with
         */
        decaf::net::Socket* socket;

        /**
         * Input Stream for Reading in Messages
         */
        decaf::io::DataInputStream* dataInputStream;

        /**
         * Output Stream for Writing out Messages.
         */
        decaf::io::DataOutputStream* dataOutputStream;

    public:

        /**
         * Constructor
         * @param properties the configuration properties for this transport
         * @param next the next transport in the chain
         * @param own indicates if this transport owns the next.
         */
        TcpTransport( const decaf::util::Properties& properties,
                      Transport* next,
                      const bool own = true );

        /**
         * Constructor
         * @param uri - The URI containing the host to connect to.
         * @param properties the configuration properties for this transport
         * @param next the next transport in the chain
         * @param own indicates if this transport owns the next.
         */
        TcpTransport( const decaf::net::URI& uri,
                      const decaf::util::Properties& properties,
                      Transport* next,
                      const bool own = true );

        virtual ~TcpTransport();

        /**
         * Delegates to the superclass and then closes the socket.
         * @throws CMSException if errors occur.
         */
        virtual void close() throw( cms::CMSException );

    private:

        void initialize( const decaf::net::URI& uri,
                         const decaf::util::Properties& properties );

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_TCP_TCPTRANSPORT_H_*/
