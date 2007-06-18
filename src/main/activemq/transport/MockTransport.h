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
#include <activemq/concurrent/CountDownLatch.h>

namespace activemq{
namespace transport{

    class MockTransport : public Transport{

    public:

        class ResponseBuilder{
        public:
            virtual ~ResponseBuilder(){}

            virtual Response* buildResponse( const Command* cmd ) = 0;
            virtual Command* buildIncomingCommand( const Command* cmd ) = 0;
        };

        class InternalCommandListener :
            public CommandListener,
            public concurrent::Thread
        {
        private:

            MockTransport* transport;
            ResponseBuilder* responseBuilder;
            concurrent::Mutex mutex;
            Command* command;
            Command* response;
            bool done;
            concurrent::CountDownLatch startedLatch;

        public:

            InternalCommandListener(void) : startedLatch(1) {
                command = NULL;
                response = NULL;
                transport = NULL;
                responseBuilder = NULL;
                done = false;

                this->start();
                startedLatch.await();
            }

            virtual ~InternalCommandListener() {
                done = true;
                synchronized( &mutex )
                {
                    mutex.notifyAll();
                }
                this->join();

                delete response;
            }

            void setTransport( MockTransport* transport ){
                this->transport = transport;
            }

            void setResponseBuilder( ResponseBuilder* responseBuilder ) {
                this->responseBuilder = responseBuilder;
            }

            virtual void onCommand( Command* command )
            {
                synchronized( &mutex )
                {
                    this->command = command;
                    // Create a response now before the caller has a
                    // chance to destroy the command.
                    this->response =
                        responseBuilder->buildIncomingCommand( command );

                    mutex.notifyAll();
                }
            }

            void run(void)
            {
                try
                {
                    synchronized( &mutex )
                    {
                        while( !done )
                        {
                            startedLatch.countDown();
                            mutex.wait();

                            if( command == NULL )
                            {
                                continue;
                            }

                            // If we created a response then send it.
                            if( response != NULL && transport != NULL )
                            {
                                transport->fireCommand( this->response );
                            }

                            this->response = NULL;
                            this->command = NULL;
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
        InternalCommandListener defaultListener;
        static MockTransport* instance;

    public:

        static MockTransport* getInstance() {
            return instance;
        }

        MockTransport( ResponseBuilder* responseBuilder ,
                       bool own = false,
                       bool useDefOutgoing = true );

        virtual ~MockTransport();

        void setResponseBuilder( ResponseBuilder* responseBuilder ){
            this->responseBuilder = responseBuilder;
        }

        unsigned int getNextCommandId() throw (exceptions::ActiveMQException);

        virtual void oneway( Command* command )
                throw(CommandIOException, exceptions::UnsupportedOperationException);

        virtual Response* request( Command* command )
            throw(CommandIOException,
                  exceptions::UnsupportedOperationException);

        virtual void setCommandListener( CommandListener* listener ){
            this->commandListener = listener;
        }

        virtual void setOutgoingCommandListener( CommandListener* listener ){
            outgoingCommandListener = listener;
        }

        virtual void setCommandReader( CommandReader* reader AMQCPP_UNUSED){}

        virtual void setCommandWriter( CommandWriter* writer AMQCPP_UNUSED){}

        virtual void setTransportExceptionListener(
            TransportExceptionListener* listener )
        {
            this->exceptionListener = listener;
        }

        virtual void fireCommand( Command* cmd ){
            if( commandListener != NULL ){
                commandListener->onCommand( cmd );
            }
        }

        virtual void fireException( const exceptions::ActiveMQException& ex ){
            if( exceptionListener != NULL ){
                exceptionListener->onTransportException( this, ex );
            }
        }

        virtual void start() throw (cms::CMSException){}
        virtual void close() throw (cms::CMSException){}
    };

}}

#endif /*ACTIVEMQ_TANSPORT_MOCKTRANSPORT_H_*/
