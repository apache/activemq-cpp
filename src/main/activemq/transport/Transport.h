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
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <activemq/util/Config.h>
#include <activemq/transport/CommandIOException.h>
#include <activemq/transport/Command.h>
#include <activemq/transport/Response.h>
#include <cms/Startable.h>
#include <cms/Closeable.h>
#include <typeinfo>

namespace activemq{
namespace transport{

    // Forward declarations.
    class CommandListener;
    class CommandReader;
    class CommandWriter;
    class TransportExceptionListener;

    /**
     * Interface for a transport layer for command objects.  Callers can
     * send oneway messages or make synchronous requests.  Non-response
     * messages will be delivered to the specified listener object upon
     * receipt.  A user of the Transport can set an exception listener
     * to be notified of errors that occurs in Threads that the Transport
     * layer runs.  Since a Transport doesn't know the Wire Format of the
     * Commands it reads and writes, its up to the managing object to
     * provide object(s) that implement the CommandReader and CommandWriter
     * interfaces.
     */
    class AMQCPP_API Transport : public cms::Startable,
                                 public cms::Closeable {
    public:

        virtual ~Transport() {}

        /**
         * Sends a one-way command.  Does not wait for any response from the
         * broker.
         * @param command the command to be sent.
         * @throws CommandIOException if an exception occurs during writing of
         * the command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual void oneway( Command* command )
            throw( CommandIOException,
                   decaf::lang::exceptions::UnsupportedOperationException ) = 0;

        /**
         * Sends the given command to the broker and then waits for the response.
         * @param command the command to be sent.
         * @return the response from the broker.
         * @throws CommandIOException if an exception occurs during the read of the
         * command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual Response* request( Command* command )
            throw( CommandIOException,
                    decaf::lang::exceptions::UnsupportedOperationException ) = 0;

        /**
         * Sends the given command to the broker and then waits for the response.
         * @param command - The command to be sent.
         * @param timeout - The time to wait for this response.
         * @return the response from the broker.
         * @throws CommandIOException if an exception occurs during the read of the
         * command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual Response* request( Command* command, unsigned int timeout )
            throw( CommandIOException,
                    decaf::lang::exceptions::UnsupportedOperationException ) = 0;

        /**
         * Assigns the command listener for non-response commands.
         * @param listener the listener.
         */
        virtual void setCommandListener( CommandListener* listener ) = 0;

        /**
         * Sets the command reader.
         * @param reader the object that will be used for reading command objects.
         */
        virtual void setCommandReader( CommandReader* reader ) = 0;

        /**
         * Sets the command writer.
         * @param writer the object that will be used for writing command objects.
         */
        virtual void setCommandWriter( CommandWriter* writer ) = 0;

        /**
         * Sets the observer of asynchronous exceptions from this transport.
         * @param listener the listener of transport exceptions.
         */
        virtual void setTransportExceptionListener(
            TransportExceptionListener* listener ) = 0;

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

    };

}}

#endif /*_ACTIVEMQ_TRANSPORT_TRANSPORT_H_*/
