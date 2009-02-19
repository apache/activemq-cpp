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

#ifndef ACTIVEMQ_TRANSPORT_TRANSPORTFILTER_H_
#define ACTIVEMQ_TRANSPORT_TRANSPORTFILTER_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/Transport.h>
#include <activemq/commands/Command.h>
#include <activemq/transport/TransportListener.h>
#include <decaf/lang/Pointer.h>
#include <typeinfo>

namespace activemq{
namespace transport{

    using decaf::lang::Pointer;
    using activemq::commands::Command;
    using activemq::commands::Response;

    /**
     * A filter on the transport layer.  Transport
     * filters implement the Transport interface and
     * optionally delegate calls to another Transport object.
     */
    class AMQCPP_API TransportFilter : public Transport,
                                       public TransportListener {
    protected:

        /**
         * The transport that this filter wraps around.
         */
        Pointer<Transport> next;

        /**
         * Listener of this transport.
         */
        TransportListener* listener;

    protected:

        /**
         * Notify the exception listener
         * @param ex - the exception to send to listeners
         */
        void fire( const decaf::lang::Exception& ex ){

            if( listener != NULL ){
                try{
                    listener->onTransportException( this, ex );
                }catch( ... ){}
            }
        }

        /**
         * Notify the command listener.
         * @param command - the command to send to the listener
         */
        void fire( const Pointer<Command>& command ){
            try{
                if( listener != NULL ){
                    listener->onCommand( command );
                }
            }catch( ... ){}
        }

    public:

        /**
         * Constructor.
         * @param next - the next Transport in the chain
         * @param own - true if this filter owns the next and should delete it
         */
        TransportFilter( const Pointer<Transport>& next );

        virtual ~TransportFilter() {}

        /**
         * Event handler for the receipt of a command.
         * @param command - the received command object.
         */
        virtual void onCommand( const Pointer<Command>& command ){
            fire( command );
        }

        /**
         * Event handler for an exception from a command transport.
         * @param source The source of the exception
         * @param ex The exception.
         */
        virtual void onTransportException( Transport* source,
                                           const decaf::lang::Exception& ex );

        /**
         * The transport has suffered an interruption from which it hopes to recover
         */
        virtual void transportInterrupted() {}

        /**
         * The transport has resumed after an interruption
         */
        virtual void transportResumed() {}

        /**
         * Sends a one-way command.  Does not wait for any response from the
         * broker.
         * @param command the command to be sent.
         * @throws CommandIOException if an exception occurs during writing of
         * the command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual void oneway( const Pointer<Command>& command )
            throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ){

            next->oneway( command );
        }

        /**
         * Not supported by this class - throws an exception.
         * @param command the command that is sent as a request
         * @throws CommandIOException
         * @throws UnsupportedOperationException.
         */
        virtual Pointer<Response> request( const Pointer<Command>& command )
            throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ){

            return next->request( command );
        }

        /**
         * Not supported by this class - throws an exception.
         * @param command - The command that is sent as a request
         * @param timeout - The the time to wait for a response.
         * @throws CommandIOException
         * @throws UnsupportedOperationException.
         */
        virtual Pointer<Response> request( const Pointer<Command>& command, unsigned int timeout )
            throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ){

            return next->request( command, timeout );
        }

        /**
         * Sets the observer of asynchronous exceptions from this transport.
         * @param listener the listener of transport exceptions.
         */
        virtual void setTransportListener( TransportListener* listener ){
            this->listener = listener;
        }

        /**
         * Sets the WireFormat instance to use.
         * @param WireFormat the object used to encode / decode commands.
         */
        virtual void setWireFormat( const Pointer<wireformat::WireFormat>& wireFormat ) {
            next->setWireFormat( wireFormat );
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
        virtual void start() throw( cms::CMSException ) {

            if( listener == NULL ){
                throw exceptions::ActiveMQException( __FILE__, __LINE__,
                    "exceptionListener is invalid" );
            }

            // Start the delegate transport object.
            next->start();
        }

        /**
         * Stops the polling thread and closes the streams.  This can
         * be called explicitly, but is also called in the destructor. Once
         * this object has been closed, it cannot be restarted.
         * @throws CMSException if errors occur.
         */
        virtual void close() throw( cms::CMSException ){
            next->close();
            next.reset( NULL );
        }

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
            } else if( this->next != NULL ) {
                return this->next->narrow( typeId );
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
            return next->isFaultTolerant();
        }

        /**
         * Is the Transport Connected to its Broker.
         *
         * @returns true if a connection has been made.
         */
        virtual bool isConnected() const {
            return next->isConnected();
        }

        /**
         * Has the Transport been shutdown and no longer usable.
         *
         * @returns true if the Transport
         */
        virtual bool isClosed() const {
            return next->isClosed();
        }

    };

}}

#endif /*ACTIVEMQ_TRANSPORT_TRANSPORTFILTER_H_*/
