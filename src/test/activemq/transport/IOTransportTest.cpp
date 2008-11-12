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
#include <activemq/transport/CommandListener.h>
#include <activemq/transport/CommandReader.h>
#include <activemq/transport/CommandWriter.h>
#include <activemq/transport/Command.h>
#include <activemq/transport/TransportExceptionListener.h>
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

////////////////////////////////////////////////////////////////////////////////
class MyCommand : public Command{
public:
    MyCommand(){ c = 0; }
    virtual ~MyCommand(){}

    char c;

    virtual void setCommandId( int id AMQCPP_UNUSED){}
    virtual int getCommandId() const{ return 0; }

    virtual void setResponseRequired( const bool required AMQCPP_UNUSED){}
    virtual bool isResponseRequired() const{ return false; }
    virtual std::string toString() const{ return ""; }
    virtual Command* cloneCommand() const{
        MyCommand* command = new MyCommand;
        command->c = c;
        return command;
    }
};

////////////////////////////////////////////////////////////////////////////////
class MyCommandListener : public CommandListener{
private:

    decaf::util::concurrent::CountDownLatch latch;

public:

    MyCommandListener() : latch(1) {}
    MyCommandListener( unsigned int num ) : latch( num ) {}

    virtual ~MyCommandListener(){}

    virtual void await() {
        latch.await();
    }

    std::string str;
    virtual void onCommand( Command* command ){
        const MyCommand* cmd = dynamic_cast<const MyCommand*>(command);
        str += cmd->c;
        delete command;
        latch.countDown();
    }
};

////////////////////////////////////////////////////////////////////////////////
class MyCommandReader : public CommandReader{
private:

    /**
     * The target input stream.
     */
    decaf::io::InputStream* inputStream;

public:

    MyCommandReader(){ throwException = false; }
    virtual ~MyCommandReader(){}

    bool throwException;

    virtual void setInputStream(decaf::io::InputStream* is){
        inputStream = is;
    }

    virtual decaf::io::InputStream* getInputStream(void){
        return inputStream;
    }

    virtual Command* readCommand( void ) throw ( CommandIOException ){

        try{
            if( throwException ){
                throw CommandIOException();
            }

            decaf::util::Random randGen;

            synchronized( inputStream ){
                MyCommand* command = new MyCommand();
                try{

                    // Throw a little uncertainty into the test.
                    unsigned int randWait = randGen.nextInt( 50 );
                    decaf::lang::Thread::sleep( randWait );

                    command->c = inputStream->read();

                } catch( decaf::lang::Exception& ex ){

                    // Free the memory.
                    delete command;

                    ex.setMark( __FILE__, __LINE__ );
                    throw CommandIOException();
                } catch( ... ) {
                    // Free the memory.
                    delete command;

                    throw CommandIOException( __FILE__, __LINE__, "Catch all" );
                }

                return command;
            }

            assert(false);
            return NULL;

        }catch( decaf::lang::Exception& ex ){
            CommandIOException cx;
            cx.setMark( __FILE__, __LINE__ );
            throw cx;
        }
        catch( ... ){
            CommandIOException cx;
            cx.setMark( __FILE__, __LINE__ );
            throw cx;
        }
    }

    virtual std::size_t read(unsigned char* buffer AMQCPP_UNUSED,
                     std::size_t count AMQCPP_UNUSED)
        throw( decaf::io::IOException ) {
        return 0;
    }

    virtual unsigned char readByte() throw(decaf::io::IOException) {
        return 0;
    }
};

////////////////////////////////////////////////////////////////////////////////
class MyCommandWriter : public CommandWriter{
private:

    /**
     * Target output stream.
     */
    decaf::io::OutputStream* outputStream;

public:

    virtual ~MyCommandWriter(){}

    virtual void setOutputStream(decaf::io::OutputStream* os){
        outputStream = os;
    }

    virtual decaf::io::OutputStream* getOutputStream(void){
        return outputStream;
    }

    virtual void writeCommand( Command* command )
        throw (CommandIOException)
    {
        try{
            synchronized( outputStream ){

                const MyCommand* m =
                    dynamic_cast<const MyCommand*>(command);
                outputStream->write( m->c );
            }
        }catch( decaf::lang::Exception& ex ){
            ex.setMark( __FILE__, __LINE__ );
            throw CommandIOException( ex );
        }
        catch(...){
            throw CommandIOException( __FILE__, __LINE__, "writeCommand");
        }
    }

    virtual void write( const unsigned char* buffer AMQCPP_UNUSED,
                        std::size_t count AMQCPP_UNUSED )
        throw(decaf::io::IOException) {}

    virtual void writeByte( unsigned char v AMQCPP_UNUSED )
        throw( decaf::io::IOException ) {}
};

////////////////////////////////////////////////////////////////////////////////
class MyExceptionListener : public TransportExceptionListener{
public:

    Transport* transport;
    decaf::util::concurrent::Mutex mutex;

    MyExceptionListener(){
        transport = NULL;
    }
    virtual ~MyExceptionListener(){}

    virtual void onTransportException( Transport* source,
                const decaf::lang::Exception& ex AMQCPP_UNUSED){
        transport = source;

        synchronized( &mutex )
        {
           mutex.notify();
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
// This will just test that we can start and stop the
// transport without any exceptions.
void IOTransportTest::testStartClose(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    MyCommandListener listener;
    MyCommandReader reader;
    MyCommandWriter writer;
    MyExceptionListener exListener;
    IOTransport transport;
    transport.setCommandListener( &listener );
    transport.setCommandReader( &reader );
    transport.setCommandWriter( &writer );
    transport.setInputStream( &is );
    transport.setOutputStream( &os );
    transport.setTransportExceptionListener( &exListener );

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

    for( int i = 0; i < 50; ++i ) {
        MyCommandListener listener;
        MyCommandReader reader;
        MyCommandWriter writer;
        MyExceptionListener exListener;

        IOTransport transport;
        transport.setCommandListener( &listener );
        transport.setCommandReader( &reader );
        transport.setCommandWriter( &writer );
        transport.setInputStream( &bis );
        transport.setOutputStream( &bos );
        transport.setTransportExceptionListener( &exListener );

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
    MyCommandListener listener(10);
    MyCommandReader reader;
    MyCommandWriter writer;
    MyExceptionListener exListener;
    IOTransport transport;
    transport.setCommandListener( &listener );
    transport.setCommandReader( &reader );
    transport.setCommandWriter( &writer );
    transport.setInputStream( &is );
    transport.setOutputStream( &os );
    transport.setTransportExceptionListener( &exListener );

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
    MyCommandListener listener;
    MyCommandReader reader;
    MyCommandWriter writer;
    MyExceptionListener exListener;
    IOTransport transport;
    transport.setCommandListener( &listener );
    transport.setCommandReader( &reader );
    transport.setCommandWriter( &writer );
    transport.setInputStream( &is );
    transport.setOutputStream( &os );
    transport.setTransportExceptionListener( &exListener );

    transport.start();

    MyCommand cmd;
    cmd.c = '1';
    transport.oneway( &cmd );
    cmd.c = '2';
    transport.oneway( &cmd );
    cmd.c = '3';
    transport.oneway( &cmd );
    cmd.c = '4';
    transport.oneway( &cmd );
    cmd.c = '5';
    transport.oneway( &cmd );

    const unsigned char* bytes = os.toByteArray();
    std::size_t size = os.size();
    CPPUNIT_ASSERT( size >= 5 );
    CPPUNIT_ASSERT( bytes[0] == '1' );
    CPPUNIT_ASSERT( bytes[1] == '2' );
    CPPUNIT_ASSERT( bytes[2] == '3' );
    CPPUNIT_ASSERT( bytes[3] == '4' );
    CPPUNIT_ASSERT( bytes[4] == '5' );

    transport.close();
}

////////////////////////////////////////////////////////////////////////////////
void IOTransportTest::testException(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    MyCommandListener listener;
    MyCommandReader reader;
    MyCommandWriter writer;
    MyExceptionListener exListener;
    IOTransport transport;
    transport.setCommandListener( &listener );
    transport.setCommandReader( &reader );
    reader.throwException = true;
    transport.setCommandWriter( &writer );
    transport.setInputStream( &is );
    transport.setOutputStream( &os );
    transport.setTransportExceptionListener( &exListener );

    unsigned char buffer[1] = { '1' };
    try{
        synchronized( &is ){
            is.setByteArray( buffer, 1);
        }
    }catch( decaf::lang::Exception& ex ){
        ex.setMark(__FILE__, __LINE__ );
    }

    transport.start();

    synchronized(&exListener.mutex)
    {
       if(exListener.transport != &transport)
       {
          exListener.mutex.wait(1000);
       }
    }

    CPPUNIT_ASSERT( exListener.transport == &transport );

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
