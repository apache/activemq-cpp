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

#ifndef ACTIVEMQ_COMMANDS_IOTRANSPORTTEST_H_
#define ACTIVEMQ_COMMANDS_IOTRANSPORTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/transport/IOTransport.h>
#include <activemq/transport/CommandListener.h>
#include <activemq/transport/CommandReader.h>
#include <activemq/transport/CommandWriter.h>
#include <activemq/transport/Command.h>
#include <activemq/transport/TransportExceptionListener.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <activemq/io/BlockingByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/Mutex.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace transport{

    class IOTransportTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( IOTransportTest );
        CPPUNIT_TEST( testStartClose );
        CPPUNIT_TEST( testRead );
        CPPUNIT_TEST( testWrite );
        CPPUNIT_TEST( testException );
        CPPUNIT_TEST_SUITE_END();

    public:

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

        class MyCommandListener : public CommandListener{
        public:
            MyCommandListener(){}
            virtual ~MyCommandListener(){}

            std::string str;
            virtual void onCommand( Command* command ){
                const MyCommand* cmd = dynamic_cast<const MyCommand*>(command);
                str += cmd->c;
                delete command;
            }
        };

        class MyCommandReader : public CommandReader{
        private:

            /**
             * The target input stream.
             */
            io::InputStream* inputStream;

        public:
            MyCommandReader(){ throwException = false; }
            virtual ~MyCommandReader(){}

            bool throwException;

            virtual void setInputStream(io::InputStream* is){
                inputStream = is;
            }

            virtual io::InputStream* getInputStream(void){
                return inputStream;
            }

            virtual Command* readCommand( void ) throw (CommandIOException){

                try{
                    if( throwException ){
                        throw CommandIOException();
                    }

                    synchronized( inputStream ){
                        MyCommand* command = new MyCommand();
                        try{
                            command->c = inputStream->read();
                        } catch( exceptions::ActiveMQException& ex ){

                            // Free the memory.
                            delete command;

                            ex.setMark( __FILE__, __LINE__ );
                            throw ex;
                        }

                        return command;
                    }

                    assert(false);
                    return NULL;
                }catch( exceptions::ActiveMQException& ex ){
                    CommandIOException cx( ex );
                    cx.setMark( __FILE__, __LINE__ );
                    throw cx;
                }
            }

            virtual std::size_t read(unsigned char* buffer AMQCPP_UNUSED,
                             std::size_t count AMQCPP_UNUSED)
                throw( io::IOException ) {
                return 0;
            }

            virtual unsigned char readByte() throw(io::IOException) {
                return 0;
            }
        };

        class MyCommandWriter : public CommandWriter{
        private:

            /**
             * Target output stream.
             */
            io::OutputStream* outputStream;

        public:
            virtual ~MyCommandWriter(){}

            virtual void setOutputStream(io::OutputStream* os){
                outputStream = os;
            }

            virtual io::OutputStream* getOutputStream(void){
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
                }catch( exceptions::ActiveMQException& ex ){
                    CommandIOException cx( ex );
                    cx.setMark( __FILE__, __LINE__ );
                    throw cx;
                }
            }

            virtual void write( const unsigned char* buffer AMQCPP_UNUSED,
                                std::size_t count AMQCPP_UNUSED)
                throw(io::IOException) {}

            virtual void writeByte(unsigned char v AMQCPP_UNUSED) throw(io::IOException) {}
        };

        class MyExceptionListener : public TransportExceptionListener{
        public:

            Transport* transport;
            concurrent::Mutex mutex;

            MyExceptionListener(){
                transport = NULL;
            }
            virtual ~MyExceptionListener(){}

            virtual void onTransportException( Transport* source,
                        const exceptions::ActiveMQException& ex AMQCPP_UNUSED){
                transport = source;

                synchronized(&mutex)
                {
                   mutex.notify();
                }
            }
        };

    public:

        virtual ~IOTransportTest(){}

        // This will just test that we can start and stop the
        // transport without any exceptions.
        void testStartClose(){

            io::BlockingByteArrayInputStream is;
            io::ByteArrayOutputStream os;
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

            decaf::lang::Thread::sleep( 50 );

            transport.close();
        }

        void testRead(){

            io::BlockingByteArrayInputStream is;
            io::ByteArrayOutputStream os;
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

            decaf::lang::Thread::sleep( 10 );

            unsigned char buffer[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
            try{
                synchronized( &is ){
                    is.setByteArray( buffer, 10 );
                }
            }catch( exceptions::ActiveMQException& ex ){
                ex.setMark( __FILE__, __LINE__ );
            }

            decaf::lang::Thread::sleep( 100 );

            CPPUNIT_ASSERT( listener.str == "1234567890" );

            transport.close();
        }

        void testWrite(){

            io::BlockingByteArrayInputStream is;
            io::ByteArrayOutputStream os;
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

            const unsigned char* bytes = os.getByteArray();
            std::size_t size = os.getByteArraySize();
            CPPUNIT_ASSERT( size >= 5 );
            CPPUNIT_ASSERT( bytes[0] == '1' );
            CPPUNIT_ASSERT( bytes[1] == '2' );
            CPPUNIT_ASSERT( bytes[2] == '3' );
            CPPUNIT_ASSERT( bytes[3] == '4' );
            CPPUNIT_ASSERT( bytes[4] == '5' );

            transport.close();
        }

        void testException(){

            io::BlockingByteArrayInputStream is;
            io::ByteArrayOutputStream os;
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
            }catch( exceptions::ActiveMQException& ex ){
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
    };

}}

#endif /*ACTIVEMQ_COMMANDS_IOTRANSPORTTEST_H_*/
