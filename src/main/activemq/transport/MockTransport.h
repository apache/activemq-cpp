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

#ifndef ACTIVEMQ_TANSPORT_MOCKTRANSPORT_H_
#define ACTIVEMQ_TANSPORT_MOCKTRANSPORT_H_

#include <activemq/transport/Transport.h>
#include <activemq/concurrent/Concurrent.h>
#include <activemq/transport/CommandListener.h>
#include <activemq/transport/TransportExceptionListener.h>
#include <activemq/transport/CommandIOException.h>
#include <activemq/concurrent/Concurrent.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/concurrent/Thread.h>
#include <activemq/util/Config.h>
#include <activemq/util/Queue.h>
#include <activemq/concurrent/CountDownLatch.h>

#include <cms/Message.h>

#include <map>
#include <set>

namespace activemq{
namespace transport{

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
    class MockTransport : public Transport{
    public:

        /**
         * Interface for all Protocols to implement that defines the behavior
         * of the Broker in response to messages of that protocol.
         */
        class ResponseBuilder{
        private:

            typedef std::multimap< std::string, long long > ConsumersMap;

            // Map of destination names to consumer Ids, this should be filled in
            // by the implemented ResponseBuilder by calling registerConsumer and
            // unregisterConsumer whenever a Consumer is created and destroyed.
            ConsumersMap consumersMap;

        protected:

            /**
             * Called by a derived class whenever a consumer is created
             * @param destination - String name of Destination
             * @param consumerId - unique Id of the consumer.
             */
            void registerConsumer( const std::string& destination,
                                   long long consumerId ) {
                consumersMap.insert( std::make_pair( destination, consumerId ) );
            }

            /**
             * Called by a dervied class when a consumer is unsubscribed.
             * @param destination - String name of the Destination
             * @param consumerId - unique Id of the consumer.
             */
            void unregisterConsumer( const std::string& destination,
                                     long long consumerId ) {
                ConsumersMap::iterator iter =
                    consumersMap.lower_bound( destination );

                for( ; iter != consumersMap.upper_bound( destination ); ++iter ){
                    if( iter-> second == consumerId ) {
                        consumersMap.erase( iter );
                        return;
                    }
                }
            }

        public:

            virtual ~ResponseBuilder(){}

            /**
             * Checks if the named Destination has any registered consumers
             * @param destination - Name of the Destination in question
             * @returns true if there are any consumers on this destination
             */
            bool hasConsumers( const std::string& destination ) const {
                return consumersMap.find( destination ) != consumersMap.end();
            }

            /**
             * Gets the Set of consumer ids that are registered for the passed in
             * destination name.
             * @param destination - String name of the Destination
             * @returns set of Consumer Ids listening on this destination
             */
            std::set<long long> getConsumers( const std::string& destination ) const {

                std::set<long long> consumerIds;
                ConsumersMap::const_iterator iter =
                    consumersMap.lower_bound( destination );

                for( ; iter != consumersMap.upper_bound( destination ); ++iter ){
                    consumerIds.insert( iter->second );
                }

                return consumerIds;
            }

            /**
             * Given a Command, check if it requires a response and return the
             * appropriate Response that the Broker would send for this Command
             * @param command - The command to build a response for
             * @return A Reponse object pointer, or NULL if no response.
             */
            virtual Response* buildResponse( const Command* command ) = 0;

            /**
             * When called the ResponseBuilder must construct all the
             * Responses or Asynchronous commands that would be sent to
             * this client by the Broker upon receipt of the passed command.
             * @param command - The Command being sent to the Broker.
             * @param queue - Queue of Command sent back from the broker.
             */
            virtual void buildIncomingCommands(
                const Command* cmd, util::Queue<Command*>& queue ) = 0;

            /**
             * When called the ResponseBuilder must return a Command Object that
             * corresponed to an incomming cms::Message sent to the Consumer
             * specified by consumerId.  This new message should be a new instance
             * on the passed message created with all the data necessary to route
             * the message to the consumer.
             * @param cms::Message - the Message to create the dispatched version from
             * @param consumerId - the Id of the Consumer that is to reveice the message
             * @returns new Command object that will be routed back into the transport.
             */
            virtual Command* buildDisptachedMessage( const cms::Message* message,
                                                     long long consumerId ) = 0;

            /**
             * Called to allow the response builder to maintain the internal list
             * of consumers by checking if the passed command is a register or
             * unregister of a consumer and add or remove it from the internal map
             * as needed.  If the passed command is not either of these actions
             * the responseBuilder should do nothing.
             * @param command - Transport Command to check for consumer add / remove
             */
            virtual void maintainConsumers( const Command* command ) = 0;

        };

        /**
         * Listens for Commands sent from the MockTransport.  This class
         * processes all outbound commands and sends responses that are
         * constructed by calling the Protocol provided ResponseBuilder
         * and getting a set of Commands to send back into the MockTransport
         * as imcoming Commands and Responses.
         */
        class InternalCommandListener :
            public CommandListener,
            public concurrent::Thread
        {
        private:

            MockTransport* transport;
            ResponseBuilder* responseBuilder;
            bool done;
            concurrent::CountDownLatch startedLatch;
            util::Queue<Command*> inboundQueue;

        public:

            InternalCommandListener(void) : startedLatch(1) {
                transport = NULL;
                responseBuilder = NULL;
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

                while( !inboundQueue.empty() ) {
                    delete inboundQueue.pop();
                }
            }

            void setTransport( MockTransport* transport ){
                this->transport = transport;
            }

            void setResponseBuilder( ResponseBuilder* responseBuilder ) {
                this->responseBuilder = responseBuilder;
            }

            virtual void onCommand( Command* command ) {
                synchronized( &inboundQueue )
                {
                    // Create a response now before the caller has a
                    // chance to destroy the command.
                    responseBuilder->buildIncomingCommands( command, inboundQueue );

                    // Allow the ResponseBuilder to add or remove consumers
                    responseBuilder->maintainConsumers( command );

                    // Chech for message loop, outgoing messages get sent in to
                    // consumers on the destination they are sent to.
                    cms::Message* message = dynamic_cast<cms::Message*>( command );
                    if( message != NULL ) {
                        std::string destination =
                            message->getCMSDestination()->toProviderString();

                        if( responseBuilder->hasConsumers( destination ) ) {

                            std::set<long long> consumers =
                                responseBuilder->getConsumers( destination );
                            std::set<long long>::const_iterator iter = consumers.begin();

                            for(; iter != consumers.end(); ++iter ) {
                                Command* dispatch =
                                    responseBuilder->buildDisptachedMessage(
                                        message, *iter );

                                if( dispatch != NULL ) {
                                    inboundQueue.push( dispatch );
                                }
                            }
                        }
                    }

                    // Wake up the thread, messages are dispatched from there.
                    inboundQueue.notifyAll();
                }
            }

            void run(void) {
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

        ResponseBuilder* responseBuilder;
        CommandListener* commandListener;
        CommandListener* outgoingCommandListener;
        TransportExceptionListener* exceptionListener;
        unsigned int nextCommandId;
        concurrent::Mutex commandIdMutex;
        bool own;
        InternalCommandListener internalListener;
        static MockTransport* instance;

    public:

        MockTransport( ResponseBuilder* responseBuilder ,
                       bool own = false );

        virtual ~MockTransport();

        static MockTransport* getInstance() {
            return instance;
        }

        /**
         * Sets the ResponseBuilder that this class uses to create Responses to
         * Commands sent.  These are either real Response Objects, or Commands that
         * would have been sent Asynchronously be the Broker.
         * @param responseBuilder - The ResponseBuilder to use from now on.
         */
        void setResponseBuilder( ResponseBuilder* responseBuilder ){
            this->responseBuilder = responseBuilder;
        }

        virtual void oneway( Command* command )
            throw(CommandIOException, exceptions::UnsupportedOperationException);

        virtual Response* request( Command* command )
            throw(CommandIOException,
                  exceptions::UnsupportedOperationException);

        virtual void setCommandListener( CommandListener* listener ){
            this->commandListener = listener;
        }

        /**
         * Sets a Command Listener that gets notified for every command that would
         * have been sent by this transport to the Broker, this allows a client
         * to verify that its messages are making it to the wire.
         * @param listener - The CommandListener to notify for each message
         */
        virtual void setOutgoingCommandListener( CommandListener* listener ){
            outgoingCommandListener = listener;
        }

        // Not impl is needed in this transport for these methods.
        virtual void setCommandReader( CommandReader* reader AMQCPP_UNUSED){}
        virtual void setCommandWriter( CommandWriter* writer AMQCPP_UNUSED){}

        virtual void setTransportExceptionListener(
            TransportExceptionListener* listener )
        {
            this->exceptionListener = listener;
        }

        /**
         * Fires a Command back through this transport to its registered
         * CommandListener if there is one.
         * @param command - Command to send to the Listener.
         */
        virtual void fireCommand( Command* command ){
            if( commandListener != NULL ){
                commandListener->onCommand( command );
            }
        }

        /**
         * Fires a Exception back through this transport to its registered
         * ExceptionListener if there is one.
         * @param command - Command to send to the Listener.
         */
        virtual void fireException( const exceptions::ActiveMQException& ex ){
            if( exceptionListener != NULL ){
                exceptionListener->onTransportException( this, ex );
            }
        }

        // No Impl needed for this Transport.
        virtual void start() throw (cms::CMSException){}
        virtual void close() throw (cms::CMSException){}

    protected:

        unsigned int getNextCommandId() throw( exceptions::ActiveMQException );

    };

}}

#endif /*ACTIVEMQ_TANSPORT_MOCKTRANSPORT_H_*/
