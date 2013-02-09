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

namespace activemq {
namespace transport {
namespace tcp {

    using decaf::lang::Pointer;

    class TcpTransportImpl;

    /**
     * Implements a TCP/IP based transport filter, this transport is meant to
     * wrap an instance of an IOTransport.  The lower level transport should take
     * care of managing stream reads and writes.
     */
    class AMQCPP_API TcpTransport: public TransportFilter {
    private:

        TcpTransportImpl* impl;

    private:

        TcpTransport(const TcpTransport&);
        TcpTransport& operator=(const TcpTransport&);

    public:

        /**
         * Creates a new instance of a TcpTransport, the transport is left unconnected
         * and is in a unusable state until the connect method is called.
         *
         * @param next
         *      The next transport in the chain
         * @param location
         *      The URI of the host this transport is to connect to.
         */
        TcpTransport(const Pointer<Transport> next, const decaf::net::URI& location);

        virtual ~TcpTransport();

        void setConnectTimeout(int soConnectTimeout);
        int getConnectTimeout() const;

        void setOutputBufferSize(int outputBufferSize);
        int getOutputBufferSize() const;

        void setInputBufferSize(int inputBufferSize);
        int getInputBufferSize() const;

        void setTrace(bool trace);
        bool isTrace() const;

        void setLinger(int soLinger);
        int getLinger() const;

        void setKeepAlive(bool soKeepAlive);
        bool isKeepAlive() const;

        void setReceiveBufferSize(int soReceiveBufferSize);
        int getReceiveBufferSize() const;

        void setSendBufferSize(int soSendBufferSize);
        int getSendBufferSize() const;

        void setTcpNoDelay(bool tcpNoDelay);
        bool isTcpNoDelay() const;

    public: // Transport Methods

        virtual bool isFaultTolerant() const {
            return false;
        }

        virtual bool isConnected() const;

    protected:

        virtual void beforeNextIsStarted();

        virtual void afterNextIsStopped();

        virtual void doClose();

        /**
         * Creates a Socket and configures it before attempting to connect to the location specified
         * by the URI passed in.
         */
        void connect();

        /**
         * Create an unconnected Socket instance to be used by the transport to communicate
         * with the broker.
         *
         * @return a newly created unconnected Socket instance.
         *
         * @throw IOException if there is an error while creating the unconnected Socket.
         */
        virtual decaf::net::Socket* createSocket();

        /**
         * Using options from configuration URI, configure the socket options before the
         * Socket instance is connected to the Server.  Subclasses can override this option
         * to set more configuration options, they should called the base class version to
         * allow the default set of Socket options to also be configured.
         *
         * @param socket
         *      The Socket instance to configure using options from the given Properties.
         *
         * @throw NullPointerException if the Socket instance is null.
         * @throw IllegalArgumentException if the socket instance is not handled by the class.
         * @throw SocketException if there is an error while setting one of the Socket options.
         */
        virtual void configureSocket(decaf::net::Socket* socket);

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_TCP_TCPTRANSPORT_H_*/
