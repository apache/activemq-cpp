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

#ifndef ACTIVEMQ_TRANSPORT_FILTERS_RESPONSECORRELATORTEST_H_
#define ACTIVEMQ_TRANSPORT_FILTERS_RESPONSECORRELATORTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/transport/filters/ResponseCorrelator.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <activemq/util/Config.h>
#include <queue>

namespace activemq{
namespace transport{
namespace filters{

    class ResponseCorrelatorTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ResponseCorrelatorTest );
        CPPUNIT_TEST( testBasics );
        CPPUNIT_TEST( testOneway );
        CPPUNIT_TEST( testTransportException );
        CPPUNIT_TEST( testMultiRequests );
        CPPUNIT_TEST( testNarrow );
        CPPUNIT_TEST_SUITE_END();

    public:

        class MyCommand : public Command{
        private:

            unsigned int commandId;
            bool responseRequired;

        public:

            virtual void setCommandId( int id ){
                commandId = id;
            }
            virtual int getCommandId() const{
                return commandId;
            }

            virtual void setResponseRequired( const bool required ){
                responseRequired = required;
            }
            virtual bool isResponseRequired() const{
                return responseRequired;
            }

            virtual std::string toString() const{ return ""; }

            virtual Command* cloneCommand() const{
                MyCommand* command = new MyCommand;
                command->commandId = commandId;
                command->responseRequired = responseRequired;
                return command;
            }
        };

        class MyResponse : public Response{
        private:

            unsigned int commandId;
            bool responseRequired;
            unsigned int corrId;

        public:

            virtual void setCommandId( int id ){
                commandId = id;
            }
            virtual int getCommandId() const{
                return commandId;
            }

            virtual void setResponseRequired( const bool required ){
                responseRequired = required;
            }
            virtual bool isResponseRequired() const{
                return responseRequired;
            }

            virtual int getCorrelationId() const{
                return corrId;
            }
            virtual void setCorrelationId( int corrId ){
                this->corrId = corrId;
            }

            virtual std::string toString() const{ return ""; }

            virtual Command* cloneCommand() const{
                MyResponse* command = new MyResponse;
                command->commandId = commandId;
                command->responseRequired = responseRequired;
                command->corrId = corrId;
                return command;
            }
        };

        class MyTransport
        :
            public Transport,
            public decaf::lang::Runnable{
        public:
            CommandReader* reader;
            CommandWriter* writer;
            CommandListener* listener;
            TransportExceptionListener* exListener;
            decaf::lang::Thread* thread;
            decaf::util::concurrent::Mutex mutex;
            decaf::util::concurrent::Mutex startedMutex;
            bool done;
            std::queue<Command*> requests;

        public:

            MyTransport(){
                reader = NULL;
                writer = NULL;
                listener = NULL;
                exListener = NULL;
                thread = NULL;
                done = false;
            }

            virtual ~MyTransport(){

                close();
            }

            virtual void oneway( Command* command )
                throw(CommandIOException, decaf::lang::exceptions::UnsupportedOperationException)
            {
                synchronized( &mutex ){
                    requests.push( command );
                    mutex.notifyAll();
                }
            }

            virtual Response* request( Command* command AMQCPP_UNUSED)
                throw(CommandIOException, decaf::lang::exceptions::UnsupportedOperationException)
            {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                    __FILE__,
                    __LINE__,
                    "stuff" );
            }

            virtual Response* request( Command* command AMQCPP_UNUSED,
                                       unsigned int timeout AMQCPP_UNUSED )
                throw(CommandIOException, decaf::lang::exceptions::UnsupportedOperationException)
            {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                    __FILE__,
                    __LINE__,
                    "stuff" );
            }

            virtual void setCommandListener( CommandListener* listener ){
                this->listener = listener;
            }

            virtual void setCommandReader( CommandReader* reader ){
                this->reader = reader;
            }

            virtual void setCommandWriter( CommandWriter* writer ){
                this->writer = writer;
            }

            virtual void setTransportExceptionListener(
                TransportExceptionListener* listener )
            {
                this->exListener = listener;
            }

            virtual void start() throw( cms::CMSException ){
                close();

                done = false;

                thread = new decaf::lang::Thread( this );
                thread->start();
            }

            virtual void close() throw( cms::CMSException ){

                done = true;

                if( thread != NULL ){
                    synchronized( &mutex ){
                        mutex.notifyAll();
                    }
                    thread->join();
                    delete thread;
                    thread = NULL;
                }
            }

            virtual Response* createResponse( Command* command ){

                MyResponse* resp = new MyResponse();
                resp->setCorrelationId( command->getCommandId() );
                resp->setResponseRequired( false );
                return resp;
            }

            virtual void run(){

                try{

                    synchronized(&startedMutex)
                    {
                       startedMutex.notifyAll();
                    }

                    synchronized( &mutex ){

                        while( !done ){

                            if( requests.empty() ){
                                mutex.wait();
                            }else{

                                Command* cmd = requests.front();
                                requests.pop();

                                // Only send a response if one is required.
                                Response* resp = NULL;
                                if( cmd->isResponseRequired() ){
                                    resp = createResponse( cmd );
                                }

                                mutex.unlock();

                                // Send both the response and the original
                                // command back to the correlator.
                                if( listener != NULL ){
                                    if( resp != NULL ){
                                        listener->onCommand( resp );
                                    }
                                    listener->onCommand( cmd );
                                }

                                mutex.lock();
                            }
                        }
                    }
                }catch( exceptions::ActiveMQException& ex ){
                    if( exListener ){
                        exListener->onTransportException( this, ex );
                    }
                }
                catch( ... ){
                    if( exListener ){
                        exceptions::ActiveMQException ex( __FILE__, __LINE__, "stuff" );
                        exListener->onTransportException( this, ex );
                    }
                }
            }

            virtual Transport* narrow( const std::type_info& typeId ) {
                if( typeid( *this ) == typeId ) {
                    return this;
                }

                return NULL;
            }
        };

        class MyBrokenTransport : public MyTransport{
        public:

            MyBrokenTransport(){}
            virtual ~MyBrokenTransport(){}

            virtual Response* createResponse( Command* command AMQCPP_UNUSED){
                throw exceptions::ActiveMQException( __FILE__, __LINE__,
                    "bad stuff" );
            }
        };

        class MyListener
        :
            public CommandListener,
            public TransportExceptionListener{

        public:

            int exCount;
            std::set<int> commands;
            decaf::util::concurrent::Mutex mutex;

        public:

            MyListener(){
                exCount = 0;
            }
            virtual ~MyListener(){}
            virtual void onCommand( Command* command ){

                synchronized( &mutex ){
                    commands.insert( command->getCommandId() );

                    mutex.notify();
                }
            }

            virtual void onTransportException(
                Transport* source AMQCPP_UNUSED,
                const decaf::lang::Exception& ex AMQCPP_UNUSED)
            {
                synchronized( &mutex ){
                    exCount++;
                }
            }
        };

        class RequestThread : public decaf::lang::Thread{
        public:

            Transport* transport;
            MyCommand cmd;
            Response* resp;
        public:

            RequestThread(){
                transport = NULL;
                resp = NULL;
            }
            virtual ~RequestThread(){
                join();

                if( resp != NULL ){
                    delete resp;
                    resp = NULL;
                }
            }

            void setTransport( Transport* transport ){
                this->transport = transport;
            }

            void run(){

                try{
                    resp = transport->request(&cmd);
                }catch( ... ){
                    CPPUNIT_ASSERT( false );
                }
            }
        };

    public:

        virtual ~ResponseCorrelatorTest(){}

        void testBasics();
        void testOneway();
        void testTransportException();
        void testMultiRequests();
        void testNarrow();

    };

}}}

#endif /*ACTIVEMQ_TRANSPORT_FILTERS_RESPONSECORRELATORTEST_H_*/
