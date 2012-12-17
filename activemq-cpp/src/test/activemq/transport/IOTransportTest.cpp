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

#include "IOTransportTest.h"

#include <activemq/transport/IOTransport.h>
#include <activemq/transport/TransportListener.h>
#include <activemq/wireformat/WireFormat.h>
#include <activemq/commands/BaseCommand.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/io/BufferedInputStream.h>
#include <decaf/io/BufferedOutputStream.h>
#include <decaf/io/BlockingByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Exception.h>
#include <decaf/util/Random.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace decaf::io;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
class MyCommand : public commands::BaseCommand {
public:

    MyCommand() : c(0) {}
    virtual ~MyCommand(){}

    char c;

    virtual unsigned char getDataStructureType() const { return 1; }

    virtual decaf::lang::Pointer<commands::Command> visit( activemq::state::CommandVisitor* visitor ) {
        return decaf::lang::Pointer<commands::Command>();
    }

    virtual std::string toString() const{ return ""; }
    virtual MyCommand* cloneDataStructure() const{
        MyCommand* command = new MyCommand;
        command->c = c;
        return command;
    }
};

////////////////////////////////////////////////////////////////////////////////
class MyWireFormat : public wireformat::WireFormat {
public:

    MyWireFormat() : throwException(false) {}
    virtual ~MyWireFormat(){}

    bool throwException;

    virtual void setVersion( int version ) {}

    virtual int getVersion() const { return 0; }

    virtual bool inReceive() const { return false; }

    virtual bool hasNegotiator() const { return false; }

    virtual Pointer<Transport> createNegotiator(
        const Pointer<transport::Transport> transport ) {

        return Pointer<wireformat::WireFormatNegotiator>();
    }

    virtual Pointer<commands::Command> unmarshal( const activemq::transport::Transport* transport AMQCPP_UNUSED,
                                                  decaf::io::DataInputStream* inputStream ) {

        try{
            if( throwException ){
                throw IOException();
            }

            if( inputStream == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "InputStream passed is Null" );
            }

            decaf::util::Random randGen;

            synchronized( inputStream ){

                Pointer<MyCommand> command( new MyCommand() );
                try{

                    // Throw a little uncertainty into the test.
                    unsigned int randWait = randGen.nextInt( 50 );
                    decaf::lang::Thread::sleep( randWait );

                    command->c = inputStream->readByte();

                } catch( decaf::lang::Exception& ex ){
                    ex.setMark( __FILE__, __LINE__ );
                    throw IOException();
                } catch( ... ) {
                    throw IOException( __FILE__, __LINE__, "Catch all" );
                }

                return command;
            }

            CPPUNIT_ASSERT( false );
            return Pointer<Command>();

        }catch( decaf::lang::Exception& ex ){
            IOException cx;
            cx.setMark( __FILE__, __LINE__ );
            throw cx;
        }
        catch( ... ){
            IOException cx;
            cx.setMark( __FILE__, __LINE__ );
            throw cx;
        }
    }

    virtual void marshal( const Pointer<commands::Command> command,
                          const activemq::transport::Transport* transport AMQCPP_UNUSED,
                          decaf::io::DataOutputStream* outputStream )
    {
        try{

            synchronized( outputStream ){

                const MyCommand* m =
                    dynamic_cast<const MyCommand*>(command.get());
                outputStream->write( m->c );
            }

        }catch( decaf::lang::Exception& ex ){
            ex.setMark( __FILE__, __LINE__ );
            throw IOException( ex );
        }
        catch(...){
            throw IOException( __FILE__, __LINE__, "writeCommand");
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
class MyTransportListener : public TransportListener{
private:

    decaf::util::concurrent::CountDownLatch latch;

public:

    decaf::util::concurrent::Mutex mutex;
    bool caughtOne;
    std::string str;

    MyTransportListener() : latch(1), mutex(), caughtOne(false), str() {}
    MyTransportListener(unsigned int num) : latch(num), mutex(), caughtOne(false), str() {}
    virtual ~MyTransportListener(){}

    virtual void await() {
        latch.await();
    }

    virtual void onCommand( const Pointer<commands::Command> command ){
        const MyCommand* cmd = dynamic_cast<const MyCommand*>(command.get());
        str += cmd->c;
        latch.countDown();
    }

    virtual void onException( const decaf::lang::Exception& ex AMQCPP_UNUSED){

        this->caughtOne = true;
        synchronized( &mutex )
        {
           mutex.notify();
        }
    }

    /**
     * The transport has suffered an interruption from which it hopes to recover
     */
    virtual void transportInterrupted() {}

    /**
     * The transport has resumed after an interruption
     */
    virtual void transportResumed() {}
};

////////////////////////////////////////////////////////////////////////////////
// This will just test that we can start and stop the
// transport without any exceptions.
void IOTransportTest::testStartClose(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    decaf::io::DataInputStream input( &is );
    decaf::io::DataOutputStream output( &os );
    MyTransportListener listener;
    Pointer<MyWireFormat> wireFormat( new MyWireFormat() );
    IOTransport transport( wireFormat );
    transport.setTransportListener( &listener );
    transport.setInputStream( &input );
    transport.setOutputStream( &output );

    transport.start();
    decaf::lang::Thread::sleep( 60 );
    transport.close();
}

////////////////////////////////////////////////////////////////////////////////
void IOTransportTest::testStressTransportStartClose(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    decaf::io::BufferedInputStream bis( &is );
    decaf::io::BufferedOutputStream bos( &os );
    decaf::io::DataInputStream input( &bis );
    decaf::io::DataOutputStream output( &bos );

    Pointer<MyWireFormat> wireFormat( new MyWireFormat() );
    MyTransportListener listener;

    for( int i = 0; i < 50; ++i ) {

        IOTransport transport;
        transport.setWireFormat( wireFormat );
        transport.setTransportListener( &listener );
        transport.setInputStream( &input );
        transport.setOutputStream( &output );

        transport.start();
        unsigned char buffer[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
        try{
            synchronized( &is ){
                is.setByteArray( buffer, 10 );
            }
        }catch( decaf::lang::Exception& ex ){
            ex.setMark( __FILE__, __LINE__ );
        }
        decaf::lang::Thread::yield();
        transport.close();
    }
}

////////////////////////////////////////////////////////////////////////////////
void IOTransportTest::testRead(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    decaf::io::DataInputStream input( &is );
    decaf::io::DataOutputStream output( &os );

    Pointer<MyWireFormat> wireFormat( new MyWireFormat() );
    MyTransportListener listener(10);
    IOTransport transport;
    transport.setInputStream( &input );
    transport.setOutputStream( &output );
    transport.setTransportListener( &listener );
    transport.setWireFormat( wireFormat );

    transport.start();

    decaf::lang::Thread::sleep( 10 );

    unsigned char buffer[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
    try{
        synchronized( &is ){
            is.setByteArray( buffer, 10 );
        }
    }catch( decaf::lang::Exception& ex ){
        ex.setMark( __FILE__, __LINE__ );
    }

    listener.await();

    CPPUNIT_ASSERT( listener.str == "1234567890" );

    transport.close();
}

////////////////////////////////////////////////////////////////////////////////
void IOTransportTest::testWrite(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    decaf::io::DataInputStream input( &is );
    decaf::io::DataOutputStream output( &os );

    Pointer<MyWireFormat> wireFormat( new MyWireFormat() );
    MyTransportListener listener;
    IOTransport transport;
    transport.setInputStream( &input );
    transport.setOutputStream( &output );
    transport.setTransportListener( &listener );
    transport.setWireFormat( wireFormat );

    transport.start();

    Pointer<MyCommand> cmd( new MyCommand() );
    cmd->c = '1';
    transport.oneway( cmd );
    cmd->c = '2';
    transport.oneway( cmd );
    cmd->c = '3';
    transport.oneway( cmd );
    cmd->c = '4';
    transport.oneway( cmd );
    cmd->c = '5';
    transport.oneway( cmd );

    std::pair<const unsigned char*, int> array = os.toByteArray();
    const unsigned char* bytes = array.first;
    std::size_t size = array.second;
    CPPUNIT_ASSERT( size >= 5 );
    CPPUNIT_ASSERT( bytes[0] == '1' );
    CPPUNIT_ASSERT( bytes[1] == '2' );
    CPPUNIT_ASSERT( bytes[2] == '3' );
    CPPUNIT_ASSERT( bytes[3] == '4' );
    CPPUNIT_ASSERT( bytes[4] == '5' );

    delete [] array.first;

    transport.close();
}

////////////////////////////////////////////////////////////////////////////////
void IOTransportTest::testException(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    decaf::io::DataInputStream input( &is );
    decaf::io::DataOutputStream output( &os );

    Pointer<MyWireFormat> wireFormat( new MyWireFormat() );
    MyTransportListener listener;
    IOTransport transport;
    wireFormat->throwException = true;
    transport.setInputStream( &input );
    transport.setOutputStream( &output );
    transport.setTransportListener( &listener );
    transport.setWireFormat( wireFormat );

    unsigned char buffer[1] = { '1' };
    try{
        synchronized( &is ){
            is.setByteArray( buffer, 1);
        }
    }catch( decaf::lang::Exception& ex ){
        ex.setMark(__FILE__, __LINE__ );
    }

    transport.start();

    synchronized(&listener.mutex) {
        if( !listener.caughtOne ) {
            listener.mutex.wait(1000);
         }
    }

    transport.close();
}

////////////////////////////////////////////////////////////////////////////////
void IOTransportTest::testNarrow(){

    IOTransport transport;

    Transport* narrowed = transport.narrow( typeid( transport ) );
    CPPUNIT_ASSERT( narrowed == &transport );

    narrowed = transport.narrow( typeid( std::string() ) );
    CPPUNIT_ASSERT( narrowed == NULL );

    narrowed = transport.narrow( typeid( transport::IOTransport ) );
    CPPUNIT_ASSERT( narrowed == &transport );

}
