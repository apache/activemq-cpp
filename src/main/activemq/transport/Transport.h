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

#ifndef ACTIVEMQ_TRANSPORT_TRANSPORT_H_
#define ACTIVEMQ_TRANSPORT_TRANSPORT_H_

#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>
#include <decaf/io/IOException.h>
#include <decaf/io/Closeable.h>
#include <decaf/util/List.h>
#include <decaf/net/URI.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <activemq/util/Config.h>
#include <activemq/commands/Command.h>
#include <activemq/commands/Response.h>
#include <typeinfo>

namespace activemq{
namespace wireformat{
    class WireFormat;
}
namespace transport{

    // Forward declarations.
    class TransportListener;

    using decaf::lang::Pointer;
    using activemq::commands::Command;
    using activemq::commands::Response;

    /**
     * Interface for a transport layer for command objects.  Callers can
     * send oneway messages or make synchronous requests.  Non-response
     * messages will be delivered to the specified listener object upon
     * receipt.  A user of the Transport can set an exception listener
     * to be notified of errors that occurs in Threads that the Transport
     * layer runs.  Transports should be given an instance of a WireFormat
     * object when created so that they can turn the built in Commands to /
     * from the required wire format encoding.
     */
    class AMQCPP_API Transport : public decaf::io::Closeable {
    public:

        virtual ~Transport() {}

        /**
         * Starts the Transport, the send methods of a Transport will throw an exception
         * if used before the Transport is started.
         *
         * @throw IOException if and error occurs while starting the Transport.
         */
        virtual void start() = 0;

        /**
         * Stops the Transport.
         *
         * @throw IOException if an error occurs while stopping the transport.
         */
        virtual void stop() = 0;

        /**
         * Sends a one-way command.  Does not wait for any response from the
         * broker.
         *
         * @param command
         *      The command to be sent.
         *
         * @throws IOException if an exception occurs during writing of the command.
         * @throws UnsupportedOperationException if this method is not implemented
         *         by this transport.
         */
        virtual void oneway( const Pointer<Command>& command ) = 0;

        /**
         * Sends the given command to the broker and then waits for the response.
         *
         * @param command the command to be sent.
         *
         * @return the response from the broker.
         *
         * @throws IOException if an exception occurs during the read of the command.
         * @throws UnsupportedOperationException if this method is not implemented
         *         by this transport.
         */
        virtual Pointer<Response> request( const Pointer<Command>& command ) = 0;

        /**
         * Sends the given command to the broker and then waits for the response.
         *
         * @param command
         *      The command to be sent.
         * @param timeout
         *      The time to wait for this response.
         *
         * @return the response from the broker.
         *
         * @throws IOException if an exception occurs during the read of the command.
         * @throws UnsupportedOperationException if this method is not implemented
         *         by this transport.
         */
        virtual Pointer<Response> request( const Pointer<Command>& command, unsigned int timeout ) = 0;

        /**
         * Sets the WireFormat instance to use.
         * @param wireFormat
         *      The WireFormat the object used to encode / decode commands.
         */
        virtual void setWireFormat( const Pointer<wireformat::WireFormat>& wireFormat ) = 0;

        /**
         * Sets the observer of asynchronous events from this transport.
         * @param listener the listener of transport events.
         */
        virtual void setTransportListener( TransportListener* listener ) = 0;

        /**
         * Gets the observer of asynchronous events from this transport.
         * @return the listener of transport events.
         */
        virtual TransportListener* getTransportListener() const = 0;

        /**
         * Narrows down a Chain of Transports to a specific Transport to allow a
         * higher level transport to skip intermediate Transports in certain
         * circumstances.
         *
         * @param typeId - The type_info of the Object we are searching for.
         *
         * @return the requested Object. or NULL if its not in this chain.
         */
        virtual Transport* narrow( const std::type_info& typeId ) = 0;

        /**
         * Is this Transport fault tolerant, meaning that it will reconnect to
         * a broker on disconnect.
         *
         * @returns true if the Transport is fault tolerant.
         */
        virtual bool isFaultTolerant() const = 0;

        /**
         * Is the Transport Connected to its Broker.
         *
         * @returns true if a connection has been made.
         */
        virtual bool isConnected() const = 0;

        /**
         * Has the Transport been shutdown and no longer usable.
         *
         * @returns true if the Transport
         */
        virtual bool isClosed() const = 0;

        /**
         * @return true if reconnect is supported.
         */
        virtual bool isReconnectSupported() const = 0;

        /**
         * @return true if updating uris is supported.
         */
        virtual bool isUpdateURIsSupported() const = 0;

        /**
         * @return the remote address for this connection
         */
        virtual std::string getRemoteAddress() const = 0;

        /**
         * reconnect to another location
         *
         * @param uri
         *      The new URI to connect this Transport to.
         *
         * @throws IOException on failure or if reconnect is not supported.
         */
        virtual void reconnect( const decaf::net::URI& uri ) = 0;

        /**
         * Updates the set of URIs the Transport can connect to.  If the Transport
         * doesn't support updating its URIs then an IOException is thrown.
         *
         * @param rebalance
         *      Indicates if a forced reconnection should be performed as a result of the update.
         * @param uris
         *      The new list of URIs that can be used for connection.
         *
         * @throws IOException if an error occurs or updates aren't supported.
         */
        virtual void updateURIs( bool rebalance, const decaf::util::List<decaf::net::URI>& uris ) = 0;

    };

}}

#endif /*_ACTIVEMQ_TRANSPORT_TRANSPORT_H_*/
