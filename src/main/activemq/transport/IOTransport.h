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

#ifndef _ACTIVEMQ_TRANSPORT_IOTRANSPORT_H_
#define _ACTIVEMQ_TRANSPORT_IOTRANSPORT_H_

#include <activemq/util/Config.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/TransportListener.h>
#include <activemq/commands/Command.h>
#include <activemq/commands/Response.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/wireformat/WireFormat.h>

#include <decaf/lang/Runnable.h>
#include <decaf/lang/Thread.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/util/logging/LoggerDefines.h>
#include <memory>

namespace activemq{
namespace transport{

    using decaf::lang::Pointer;
    using activemq::commands::Command;
    using activemq::commands::Response;

    /**
     * Implementation of the Transport interface that performs
     * marshaling of commands to IO streams.  This class does not
     * implement the request method, it only handles oneway messages.
     * A thread polls on the input stream for in-coming commands.  When
     * a command is received, the command listener is notified.  The
     * polling thread is not started until the start method is called.
     * The close method will close the associated streams.  Close can
     * be called explicitly by the user, but is also called in the
     * destructor.  Once this object has been closed, it cannot be
     * restarted.
     */
    class AMQCPP_API IOTransport : public Transport,
                                   public decaf::lang::Runnable {

        LOGDECAF_DECLARE( logger )

    private:

        /**
         * WireFormat instance to use to Encode / Decode.
         */
        Pointer<wireformat::WireFormat> wireFormat;

        /**
         * Listener of this transport.
         */
        TransportListener* listener;

        /**
         * The input stream for incoming commands.
         */
        decaf::io::DataInputStream* inputStream;

        /**
         * The output stream for out-going commands.
         */
        decaf::io::DataOutputStream* outputStream;

        /**
         * The polling thread.
         */
        Pointer<decaf::lang::Thread> thread;

        /**
         * Flag marking this transport as closed.
         */
        volatile bool closed;

    private:

        /**
         * Notify the exception listener
         * @param ex the exception to send
         */
        void fire( decaf::lang::Exception& ex );

        /**
         * Notify the command listener.
         * @param command the command the send
         */
        void fire( const Pointer<Command>& command );

    public:

        /**
         * Default Constructor
         */
        IOTransport();

        /**
         * Create an instance of this Transport and assign its WireFormat instance
         * at creation time.
         * @param wireFormat
         *        Data encoder / decoder to use when reading and writing.
         */
        IOTransport( const Pointer<wireformat::WireFormat>& wireFormat );

        virtual ~IOTransport();

        /**
         * Sends a one-way command.  Does not wait for any response from the
         * broker.
         * @param command the command to be sent.
         * @throws IOException if an exception occurs during writing of
         * the command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual void oneway( const Pointer<Command>& command )
            throw( decaf::io::IOException, decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Not supported by this class - throws an exception.
         * @param command the command to be sent.
         * @returns the response to the command sent.
         * @throws UnsupportedOperationException.
         */
        virtual Pointer<Response> request( const Pointer<Command>& command )
            throw( decaf::io::IOException, decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Not supported by this class - throws an exception.
         * @param command the command to be sent.
         * @param timeout the time to wait for a response.
         * @returns the response to the command sent.
         * @throws UnsupportedOperationException.
         */
        virtual Pointer<Response> request( const Pointer<Command>& command, unsigned int timeout )
            throw( decaf::io::IOException, decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Sets the WireFormat instance to use.
         *
         * @param wireFormat
         *      The WireFormat the object used to encode / decode commands.
         */
        virtual void setWireFormat( const Pointer<wireformat::WireFormat>& wireFormat ){
            this->wireFormat = wireFormat;
        }

        /**
         * Sets the observer of asynchronous exceptions from this transport.
         * @param listener the listener of transport events.
         */
        virtual void setTransportListener( TransportListener* listener ){
            this->listener = listener;
        }

        /**
         * Gets the observer of asynchronous exceptions from this transport.
         * @return The listener of transport events.
         */
        virtual TransportListener* getTransportListener() const {
            return this->listener;
        }

        /**
         * Sets the input stream for in-coming commands.
         * @param is The input stream.
         */
        virtual void setInputStream( decaf::io::DataInputStream* is ){
            this->inputStream = is;
        }

        /**
         * Sets the output stream for out-going commands.
         * @param os The output stream.
         */
        virtual void setOutputStream( decaf::io::DataOutputStream* os ){
            this->outputStream = os;
        }

        /**
         * Starts this transport object and creates the thread for
         * polling on the input stream for commands.  If this object
         * has been closed, throws an exception.  Before calling start,
         * the caller must set the IO streams and the reader and writer
         * objects.
         * @throws CMSException if an error occurs or if this transport
         * has already been closed.
         */
        virtual void start() throw( decaf::io::IOException );

        /**
         * Stops the Transport, terminating any threads and stopping all read
         * and write operations.
         *
         * @throw IOException if an error occurs while stopping the Transport.
         */
        virtual void stop() throw( decaf::io::IOException );

        /**
         * Stops the polling thread and closes the streams.  This can
         * be called explicitly, but is also called in the destructor. Once
         * this object has been closed, it cannot be restarted.
         * @throws IOException if errors occur.
         */
        virtual void close() throw( decaf::io::IOException );

        /**
         * Runs the polling thread.
         */
        virtual void run();

        /**
         * Narrows down a Chain of Transports to a specific Transport to allow a
         * higher level transport to skip intermediate Transports in certain
         * circumstances.
         *
         * @param typeId - The type_info of the Object we are searching for.
         *
         * @return the requested Object. or NULL if its not in this chain.
         */
        virtual Transport* narrow( const std::type_info& typeId ) {
            if( typeid( *this ) == typeId ) {
                return this;
            }

            return NULL;
        }

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
            return !this->closed;
        }

        /**
         * Has the Transport been shutdown and no longer usable.
         *
         * @returns true if the Transport
         */
        virtual bool isClosed() const {
            return this->closed;
        }

        /**
         * @return the remote address for this connection
         */
        virtual std::string getRemoteAddress() const {
            return "";
        }

        /**
         * reconnect to another location
         * @param uri
         * @throws IOException on failure of if not supported
         */
        virtual void reconnect( const decaf::net::URI& uri AMQCPP_UNUSED )
            throw( decaf::io::IOException ) {}

    };

}}

#endif /*_ACTIVEMQ_TRANSPORT_IOTRANSPORT_H_*/
