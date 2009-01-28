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

#ifndef _ACTIVEMQ_TRANSPORT_CORRELATOR_RESPONSECORRELATORTEST_H_
#define _ACTIVEMQ_TRANSPORT_CORRELATOR_RESPONSECORRELATORTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/util/Config.h>
#include <activemq/commands/BaseCommand.h>
#include <activemq/transport/DefaultTransportListener.h>
#include <activemq/transport/correlator/ResponseCorrelator.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <queue>

namespace activemq{
namespace transport{
namespace correlator{

    class ResponseCorrelatorTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ResponseCorrelatorTest );
        CPPUNIT_TEST( testBasics );
        CPPUNIT_TEST( testOneway );
        CPPUNIT_TEST( testTransportException );
        CPPUNIT_TEST( testMultiRequests );
        CPPUNIT_TEST( testNarrow );
        CPPUNIT_TEST_SUITE_END();

    public:

        class MyCommand : public commands::BaseCommand{
        private:

            unsigned int commandId;

        public:

            virtual std::string toString() const{ return ""; }

            virtual unsigned char getDataStructureType() const { return 1; }

            virtual commands::Command* visit( activemq::state::CommandVisitor* visitor )
                throw( exceptions::ActiveMQException ) { return NULL; }

            virtual MyCommand* cloneDataStructure() const{
                MyCommand* command = new MyCommand;
                command->setCommandId( this->getCommandId() );
                command->setResponseRequired( this->isResponseRequired() );
                return command;
            }
        };

        class MyTransport
        :
            public Transport,
            public decaf::lang::Runnable{
        public:
            TransportListener* listener;
            decaf::lang::Thread* thread;
            decaf::util::concurrent::Mutex mutex;
            decaf::util::concurrent::Mutex startedMutex;
            bool done;
            std::queue<commands::Command*> requests;

        public:

            MyTransport(){
                listener = NULL;
                thread = NULL;
                done = false;
            }

            virtual ~MyTransport(){

                close();
            }

            virtual void oneway( commands::Command* command )
                throw(CommandIOException, decaf::lang::exceptions::UnsupportedOperationException)
            {
                synchronized( &mutex ){
                    requests.push( command );
                    mutex.notifyAll();
                }
            }

            virtual commands::Response* request( commands::Command* command AMQCPP_UNUSED)
                throw(CommandIOException, decaf::lang::exceptions::UnsupportedOperationException)
            {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                    __FILE__,
                    __LINE__,
                    "stuff" );
            }

            virtual commands::Response* request( commands::Command* command AMQCPP_UNUSED,
                                                 unsigned int timeout AMQCPP_UNUSED )
                throw(CommandIOException, decaf::lang::exceptions::UnsupportedOperationException)
            {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                    __FILE__,
                    __LINE__,
                    "stuff" );
            }

            virtual void setWireFormat( wireformat::WireFormat* wireFormat ) {

            }

            virtual void setTransportListener( TransportListener* listener ) {
                this->listener = listener;
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

            virtual commands::Response* createResponse( commands::Command* command ){

                commands::Response* resp = new commands::Response();
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

                                commands::Command* cmd = requests.front();
                                requests.pop();

                                // Only send a response if one is required.
                                commands::Response* resp = NULL;
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
                    if( listener ){
                        listener->onTransportException( this, ex );
                    }
                }
                catch( ... ){
                    if( listener ){
                        exceptions::ActiveMQException ex( __FILE__, __LINE__, "stuff" );
                        listener->onTransportException( this, ex );
                    }
                }
            }

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
                return true;
            }

            /**
             * Is the Transport Connected to its Broker.
             *
             * @returns true if a connection has been made.
             */
            virtual bool isConnected() const {
                return false;
            }

            /**
             * Has the Transport been shutdown and no longer usable.
             *
             * @returns true if the Transport
             */
            virtual bool isClosed() const {
                return false;
            }

        };

        class MyBrokenTransport : public MyTransport{
        public:

            MyBrokenTransport(){}
            virtual ~MyBrokenTransport(){}

            virtual commands::Response* createResponse(commands:: Command* command AMQCPP_UNUSED){
                throw exceptions::ActiveMQException( __FILE__, __LINE__,
                    "bad stuff" );
            }
        };

        class MyListener : public DefaultTransportListener {
        public:

            int exCount;
            std::set<int> commands;
            decaf::util::concurrent::Mutex mutex;

        public:

            MyListener(){
                exCount = 0;
            }
            virtual ~MyListener(){}
            virtual void onCommand( commands::Command* command ){

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
            commands::Response* resp;
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

#endif /*_ACTIVEMQ_TRANSPORT_CORRELATOR_RESPONSECORRELATORTEST_H_*/
