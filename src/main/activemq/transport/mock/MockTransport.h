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

#ifndef _ACTIVEMQ_TANSPORT_MOCK_MOCKTRANSPORT_H_
#define _ACTIVEMQ_TANSPORT_MOCK_MOCKTRANSPORT_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/TransportListener.h>
#include <activemq/transport/DefaultTransportListener.h>
#include <activemq/transport/CommandIOException.h>
#include <activemq/wireformat/WireFormat.h>

#include <decaf/lang/Thread.h>
#include <decaf/util/StlQueue.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/concurrent/CountDownLatch.h>

#include <cms/Message.h>

#include <map>
#include <set>

namespace activemq{
namespace transport{
namespace mock{

    using decaf::lang::Pointer;
    using activemq::commands::Command;
    using activemq::commands::Response;

    /**
     * The MockTransport defines a base level Transport class that is intended to
     * be used in place of an a regular protocol Transport suck as TCP.  This
     * Transport assumes that it is the base Transport in the Transports stack, and
     * destroys any Transports that are passed to it in its constructor.
     *
     * This Transport defines an Interface ResponseBuilder which must be implemented
     * by any protocol for which the Transport is used to Emulate.  The Transport
     * hands off all outbound commands to the ResponseBuilder for processing, it is
     * up to the builder to create appropriate responses and schedule any asynchronous
     * messages that might result from a message sent to the Broker.
     */
    class AMQCPP_API MockTransport : public Transport{
    public:

        /**
         * Interface for all Protocols to implement that defines the behavior
         * of the Broker in response to messages of that protocol.
         */
        class AMQCPP_API ResponseBuilder {
        public:

            virtual ~ResponseBuilder() {}

            /**
             * Given a Command, check if it requires a response and return the
             * appropriate Response that the Broker would send for this Command
             * @param command - The command to build a response for
             * @return A Response object pointer, or NULL if no response.
             */
            virtual Pointer<Response> buildResponse( const Pointer<Command>& command ) = 0;

            /**
             * When called the ResponseBuilder must construct all the
             * Responses or Asynchronous commands that would be sent to
             * this client by the Broker upon receipt of the passed command.
             * @param command - The Command being sent to the Broker.
             * @param queue - Queue of Command sent back from the broker.
             */
            virtual void buildIncomingCommands(
                const Pointer<Command>& command,
                decaf::util::StlQueue< Pointer<Command> >& queue ) = 0;

        };

        /**
         * Listens for Commands sent from the MockTransport.  This class
         * processes all outbound commands and sends responses that are
         * constructed by calling the Protocol provided ResponseBuilder
         * and getting a set of Commands to send back into the MockTransport
         * as incoming Commands and Responses.
         */
        class InternalCommandListener :
            public DefaultTransportListener,
            public decaf::lang::Thread {

        private:

            MockTransport* transport;
            Pointer<ResponseBuilder> responseBuilder;
            bool done;
            decaf::util::concurrent::CountDownLatch startedLatch;
            decaf::util::StlQueue< Pointer<Command> > inboundQueue;

        public:

            InternalCommandListener() : startedLatch(1) {
                transport = NULL;
                done = false;

                this->start();
                startedLatch.await();
            }

            virtual ~InternalCommandListener() {
                done = true;
                synchronized( &inboundQueue ) {
                    inboundQueue.notifyAll();
                }
                this->join();

                inboundQueue.clear();
            }

            void setTransport( MockTransport* transport ){
                this->transport = transport;
            }

            void setResponseBuilder( const Pointer<ResponseBuilder>& responseBuilder ) {
                this->responseBuilder = responseBuilder;
            }

            virtual void onCommand( const Pointer<Command>& command ) {
                synchronized( &inboundQueue )
                {
                    // Create a response now before the caller has a
                    // chance to destroy the command.
                    responseBuilder->buildIncomingCommands( command, inboundQueue );

                    // Wake up the thread, messages are dispatched from there.
                    inboundQueue.notifyAll();
                }
            }

            void run() {
                try {

                    synchronized( &inboundQueue ) {

                        while( !done ) {
                            startedLatch.countDown();

                            while( inboundQueue.empty() && !done ){
                                inboundQueue.wait();
                            }

                            if( done || transport == NULL ) {
                                continue;
                            }

                            // If we created a response then send it.
                            while( !inboundQueue.empty() ) {
                                transport->fireCommand( inboundQueue.pop() );
                            }
                        }
                    }
                }
                AMQ_CATCHALL_NOTHROW()
            }
        };

    private:

        Pointer<ResponseBuilder> responseBuilder;
        Pointer<wireformat::WireFormat> wireFormat;
        TransportListener* outgoingListener;
        TransportListener* listener;
        decaf::util::concurrent::atomic::AtomicInteger nextCommandId;
        InternalCommandListener internalListener;
        static MockTransport* instance;

    public:

        MockTransport( const Pointer<wireformat::WireFormat>& wireFormat,
                       const Pointer<ResponseBuilder>& responseBuilder );

        virtual ~MockTransport() {}

        static MockTransport* getInstance() {
            return instance;
        }

        /**
         * Sets the ResponseBuilder that this class uses to create Responses to
         * Commands sent.  These are either real Response Objects, or Commands that
         * would have been sent Asynchronously be the Broker.
         * @param responseBuilder - The ResponseBuilder to use from now on.
         */
        void setResponseBuilder( const Pointer<ResponseBuilder>& responseBuilder ){
            this->responseBuilder = responseBuilder;
        }

        virtual void oneway( const Pointer<Command>& command )
            throw(CommandIOException, decaf::lang::exceptions::UnsupportedOperationException);

        virtual Pointer<Response> request( const Pointer<Command>& command )
            throw( CommandIOException,
                   decaf::lang::exceptions::UnsupportedOperationException);


        virtual Pointer<Response> request( const Pointer<Command>&, unsigned int timeout )
            throw( CommandIOException,
                   decaf::lang::exceptions::UnsupportedOperationException);

        /**
         * Sets a Listener that gets notified for every command that would
         * have been sent by this transport to the Broker, this allows a client
         * to verify that its messages are making it to the wire.
         * @param listener - The CommandListener to notify for each message
         */
        virtual void setOutgoingListener( TransportListener* listener ){
            outgoingListener = listener;
        }

        /**
         * Sets the WireFormat instance to use.
         * @param WireFormat the object used to encode / decode commands.
         */
        virtual void setWireFormat( const Pointer<wireformat::WireFormat>& wireFormat AMQCPP_UNUSED ) {}

        virtual void setTransportListener( TransportListener* listener ) {
            this->listener = listener;
        }

        /**
         * Fires a Command back through this transport to its registered
         * CommandListener if there is one.
         * @param command - Command to send to the Listener.
         */
        virtual void fireCommand( const Pointer<Command>& command ){
            if( listener != NULL ){
                listener->onCommand( command );
            }
        }

        /**
         * Fires a Exception back through this transport to its registered
         * ExceptionListener if there is one.
         * @param command - Command to send to the Listener.
         */
        virtual void fireException( const exceptions::ActiveMQException& ex ){
            if( listener != NULL ){
                listener->onException( ex );
            }
        }

        // No Impl needed for this Transport.
        virtual void start() throw (cms::CMSException){}
        virtual void close() throw (cms::CMSException){}

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
            return true;
        }

        /**
         * Has the Transport been shutdown and no longer usable.
         *
         * @returns true if the Transport
         */
        virtual bool isClosed() const {
            return false;
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
        virtual void reconnect( const decaf::net::URI& uri )
            throw( decaf::io::IOException ) {}

    };

}}}

#endif /*_ACTIVEMQ_TANSPORT_MOCK_MOCKTRANSPORT_H_*/
