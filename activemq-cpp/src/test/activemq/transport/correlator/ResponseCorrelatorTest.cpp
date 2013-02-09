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

#include "ResponseCorrelatorTest.h"

#include <activemq/util/Config.h>
#include <activemq/commands/BaseCommand.h>
#include <activemq/wireformat/WireFormat.h>
#include <activemq/transport/DefaultTransportListener.h>
#include <activemq/transport/correlator/ResponseCorrelator.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <queue>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::correlator;
using namespace decaf::io;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace transport{
namespace correlator{

    class MyCommand : public commands::BaseCommand{
    private:

        unsigned int commandId;

    public:

        MyCommand() : commandId() {}
        virtual ~MyCommand() {}

        virtual std::string toString() const{ return ""; }

        virtual unsigned char getDataStructureType() const { return 1; }

        virtual decaf::lang::Pointer<commands::Command> visit( activemq::state::CommandVisitor* visitor )
        { return decaf::lang::Pointer<commands::Command>(); }

        virtual MyCommand* cloneDataStructure() const{
            MyCommand* command = new MyCommand;
            command->setCommandId( this->getCommandId() );
            command->setResponseRequired( this->isResponseRequired() );
            return command;
        }
    };

    class MyTransport : public Transport, public decaf::lang::Runnable{
    public:

        TransportListener* listener;
        decaf::lang::Thread* thread;
        decaf::util::concurrent::Mutex mutex;
        decaf::util::concurrent::Mutex startedMutex;
        bool done;
        std::queue< Pointer<commands::Command> > requests;

    private:

        MyTransport(const MyTransport&);
        MyTransport& operator= (const MyTransport&);

    public:

        MyTransport() :
            listener(NULL), thread(NULL), mutex(), startedMutex(), done(false), requests() {
        }

        virtual ~MyTransport(){
            close();
        }

        virtual void oneway( const Pointer<Command> command )
        {
            synchronized( &mutex ){
                requests.push( command );
                mutex.notifyAll();
            }
        }

        virtual Pointer<FutureResponse> asyncRequest(const Pointer<Command> command,
                                                     const Pointer<ResponseCallback> responseCallback) {
            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "stuff" );
        }

        virtual Pointer<Response> request( const Pointer<Command> command AMQCPP_UNUSED ) {
            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "stuff" );
        }

        virtual Pointer<Response> request( const Pointer<Command> command AMQCPP_UNUSED,
                                           unsigned int timeout AMQCPP_UNUSED ) {
            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "stuff" );
        }

        virtual Pointer<wireformat::WireFormat> getWireFormat() const {
            return Pointer<wireformat::WireFormat>();
        }

        virtual void setWireFormat(
            const Pointer<wireformat::WireFormat> wireFormat AMQCPP_UNUSED ) {}

        virtual void setTransportListener( TransportListener* listener ) {
            this->listener = listener;
        }

        virtual TransportListener* getTransportListener() const {
            return this->listener;
        }

        virtual void start() {
            close();

            done = false;
            thread = new decaf::lang::Thread( this );
            thread->start();
        }

        virtual void stop() {
        }

        virtual void close() {

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

        virtual Pointer<Response> createResponse( const Pointer<Command> command ){

            Pointer<Response> resp( new commands::Response() );
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

                            Pointer<Command> cmd = requests.front();
                            requests.pop();

                            // Only send a response if one is required.
                            Pointer<Response> resp;
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
                    listener->onException( ex );
                }
            }
            catch( ... ){
                if( listener ){
                    exceptions::ActiveMQException ex( __FILE__, __LINE__, "stuff" );
                    listener->onException( ex );
                }
            }
        }

        virtual Transport* narrow( const std::type_info& typeId ) {
            if( typeid( *this ) == typeId ) {
                return this;
            }

            return NULL;
        }

        virtual bool isFaultTolerant() const {
            return true;
        }

        virtual bool isConnected() const {
            return false;
        }

        virtual bool isClosed() const {
            return false;
        }

        virtual std::string getRemoteAddress() const {
            return "";
        }

        virtual void reconnect( const decaf::net::URI& uri ) {}

        virtual bool isReconnectSupported() const {
            return false;
        }

        virtual bool isUpdateURIsSupported() const {
            return false;
        }

        virtual void updateURIs( bool rebalance AMQCPP_UNUSED,
                                 const decaf::util::List<decaf::net::URI>& uris AMQCPP_UNUSED ) {
            throw decaf::io::IOException();
        }

    };

    class MyBrokenTransport : public MyTransport{
    public:

        MyBrokenTransport(){}
        virtual ~MyBrokenTransport(){}

        virtual Pointer<Response> createResponse( const Pointer<Command> command ){
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

        MyListener() : exCount(0), commands(), mutex() {}
        virtual ~MyListener(){}

        virtual void onCommand( const Pointer<Command> command ){

            synchronized( &mutex ){
                commands.insert( command->getCommandId() );

                mutex.notify();
            }
        }

        virtual void onException(
            const decaf::lang::Exception& ex AMQCPP_UNUSED)
        {
            synchronized( &mutex ){
                exCount++;
            }
        }

    };

    class RequestThread : public decaf::lang::Thread {
    public:

        Transport* transport;
        Pointer<MyCommand> cmd;
        Pointer<Response> resp;

    private:

        RequestThread(const RequestThread&);
        RequestThread& operator= (const RequestThread&);

    public:

        RequestThread() : transport(NULL), cmd(new MyCommand()), resp() {}

        virtual ~RequestThread() {}

        void setTransport(Transport* transport) {
            this->transport = transport;
        }

        void run() {

            try{
                resp = transport->request(cmd);
                Thread::sleep(10);
            }catch( ... ){
                CPPUNIT_ASSERT( false );
            }
        }
    };

}}}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelatorTest::testBasics() {

    MyListener listener;
    Pointer<MyTransport> transport( new MyTransport() );
    ResponseCorrelator correlator( transport );
    correlator.setTransportListener( &listener );
    CPPUNIT_ASSERT( transport->listener == &correlator );

    // Give the thread a little time to get up and running.
    synchronized( &(transport->startedMutex) ) {
        // Start the transport.
        correlator.start();
        transport->startedMutex.wait();
    }

    // Send one request.
    Pointer<MyCommand> cmd( new MyCommand );
    Pointer<Response> resp = correlator.request( cmd );
    CPPUNIT_ASSERT( resp != NULL );
    CPPUNIT_ASSERT( resp->getCorrelationId() == cmd->getCommandId() );

    // Wait to get the message back asynchronously.
    decaf::lang::Thread::sleep( 100 );

    // Since our transport relays our original command back at us as a
    // non-response message, check to make sure we received it and that
    // it is the original command.
    CPPUNIT_ASSERT( listener.commands.size() == 1 );
    CPPUNIT_ASSERT( listener.exCount == 0 );

    correlator.close();
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelatorTest::testOneway(){

    MyListener listener;
    Pointer<MyTransport> transport( new MyTransport() );
    ResponseCorrelator correlator( transport );
    correlator.setTransportListener( &listener );
    CPPUNIT_ASSERT( transport->listener == &correlator );

    // Give the thread a little time to get up and running.
    synchronized( &(transport->startedMutex) ) {

        // Start the transport.
        correlator.start();

        transport->startedMutex.wait();
    }

    // Send many oneway request (we'll get them back asynchronously).
    const unsigned int numCommands = 1000;
    for( unsigned int ix = 0; ix < numCommands; ++ix ) {
        Pointer<MyCommand> command( new MyCommand() );
        correlator.oneway( command );
    }

    // Give the thread a little time to get all the messages back.
    decaf::lang::Thread::sleep( 500 );

    // Make sure we got them all back.
    CPPUNIT_ASSERT( listener.commands.size() == numCommands );
    CPPUNIT_ASSERT( listener.exCount == 0 );

    correlator.close();
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelatorTest::testTransportException(){

    MyListener listener;
    Pointer<MyBrokenTransport> transport( new MyBrokenTransport() );
    ResponseCorrelator correlator( transport );
    correlator.setTransportListener( &listener );
    CPPUNIT_ASSERT( transport->listener == &correlator );

    // Give the thread a little time to get up and running.
    synchronized( &(transport->startedMutex) ) {
        // Start the transport.
        correlator.start();

        transport->startedMutex.wait();
    }

    // Send one request.
    Pointer<MyCommand> cmd( new MyCommand );
    try{
        correlator.request( cmd, 1000 );
    }catch( IOException& ex ){
        CPPUNIT_ASSERT(false);
    }

    // Wait to make sure we get the asynchronous message back.
    decaf::lang::Thread::sleep( 200 );

    // Since our transport relays our original command back at us as a
    // non-response message, check to make sure we received it and that
    // it is the original command.
    CPPUNIT_ASSERT( listener.commands.size() == 0 );
    CPPUNIT_ASSERT( listener.exCount == 1 );

    correlator.close();
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelatorTest::testMultiRequests(){

    MyListener listener;
    Pointer<MyTransport> transport( new MyTransport() );
    ResponseCorrelator correlator( transport );
    correlator.setTransportListener( &listener );
    CPPUNIT_ASSERT( transport->listener == &correlator );

    // Start the transport.
    correlator.start();

    // Make sure the start command got down to the thread.
    CPPUNIT_ASSERT( transport->thread != NULL );

    // Give the thread a little time to get up and running.
    synchronized( &(transport->startedMutex) ) {
        transport->startedMutex.wait(500);
    }

    // Start all the requester threads.
    const unsigned int numRequests = 20;
    RequestThread requesters[numRequests];
    for( unsigned int ix=0; ix<numRequests; ++ix ){
        requesters[ix].setTransport( &correlator );
        requesters[ix].start();
    }

    // Make sure we got all the responses and that they were all
    // what we expected.
    for( unsigned int ix=0; ix<numRequests; ++ix ){
        requesters[ix].join();
        CPPUNIT_ASSERT( requesters[ix].resp != NULL );
        CPPUNIT_ASSERT( requesters[ix].cmd->getCommandId() ==
                        requesters[ix].resp->getCorrelationId() );
    }

    decaf::lang::Thread::sleep( 60 );
    synchronized( &listener.mutex )
    {
        unsigned int count = 0;

        while( listener.commands.size() != numRequests )
        {
            listener.mutex.wait( 75 );

            ++count;

            if( count == numRequests ) {
                break;
            }
        }
    }

    // Since our transport relays our original command back at us as a
    // non-response message, check to make sure we received it and that
    // it is the original command.
    CPPUNIT_ASSERT( listener.commands.size() == numRequests );
    CPPUNIT_ASSERT( listener.exCount == 0 );

    correlator.close();
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelatorTest::testNarrow(){

    Pointer<MyTransport> transport( new MyTransport() );
    ResponseCorrelator correlator( transport );

    Transport* narrowed = correlator.narrow( typeid( *transport ) );
    CPPUNIT_ASSERT( narrowed == transport );

    narrowed = correlator.narrow( typeid( std::string() ) );
    CPPUNIT_ASSERT( narrowed == NULL );

    narrowed = correlator.narrow( typeid( MyTransport ) );
    CPPUNIT_ASSERT( narrowed == transport );

    narrowed = correlator.narrow( typeid( transport::correlator::ResponseCorrelator ) );
    CPPUNIT_ASSERT( narrowed == &correlator );

    narrowed = correlator.narrow( typeid( correlator ) );
    CPPUNIT_ASSERT( narrowed == &correlator );
}
