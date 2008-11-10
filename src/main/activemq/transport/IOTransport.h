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

#ifndef _ACTIVEMQ_TRANSPORT_IOTRANSPORT_H_
#define _ACTIVEMQ_TRANSPORT_IOTRANSPORT_H_

#include <activemq/util/Config.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/TransportExceptionListener.h>
#include <activemq/transport/CommandListener.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/Command.h>

#include <decaf/lang/Runnable.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/logging/LoggerDefines.h>

namespace activemq{
namespace transport{

    /**
     * Implementation of the Transport interface that performs
     * marshalling of commands to IO streams.  This class does not
     * implement the request method, it only handles oneway messages.
     * A thread polls on the input stream for in-coming commands.  When
     * a command is received, the command listener is notified.  The
     * polling thread is not started until the start method is called.
     * The close method will close the associated streams.  Close can
     * be called explicitly by the user, but is also called in the
     * destructor.  Once this object has been closed, it cannot be
     * restarted.
     */
    class AMQCPP_API IOTransport : public Transport,
                                   public decaf::lang::Runnable {

        LOGDECAF_DECLARE( logger )

    private:

        /**
         * Listener to incoming commands.
         */
        CommandListener* listener;

        /**
         * Reads commands from the input stream.
         */
        CommandReader* reader;

        /**
         * Writes commands to the output stream.
         */
        CommandWriter* writer;

        /**
         * Listener of exceptions from this transport.
         */
        TransportExceptionListener* exceptionListener;

        /**
         * The input stream for incoming commands.
         */
        decaf::io::InputStream* inputStream;

        /**
         * The output stream for out-going commands.
         */
        decaf::io::OutputStream* outputStream;

        /**
         * The polling thread.
         */
        decaf::lang::Thread* thread;

        /**
         * Flag marking this transport as closed.
         */
        volatile bool closed;

    private:

        /**
         * Notify the excpetion listener
         * @param ex the exception to send
         */
        void fire( decaf::lang::Exception& ex ){

            if( exceptionListener != NULL ){

                try{
                    exceptionListener->onTransportException( this, ex );
                }catch( ... ){}
            }
        }

        /**
         * Notify the command listener.
         * @param command the command the send
         */
        void fire( Command* command ){

            try{
                // Since the listener is responsible for freeing the memory,
                // if there is no listener - free the command here.  Also if
                // we have been closed then we don't deliver any messages that
                // might have snuck in while we where closing.
                if( listener == NULL || closed == true ){
                    delete command;
                    return;
                }

                listener->onCommand( command );

            }catch( ... ){
                try{
                    delete command;
                } catch( ... ) {}
            }
        }

    public:

        IOTransport();
        virtual ~IOTransport();

        /**
         * Sends a one-way command.  Does not wait for any response from the
         * broker.
         * @param command the command to be sent.
         * @throws CommandIOException if an exception occurs during writing of
         * the command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual void oneway( Command* command )
            throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Not supported by this class - throws an exception.
         * @param command the command to be sent.
         * @returns the response to the command sent.
         * @throws UnsupportedOperationException.
         */
        virtual Response* request( Command* command )
            throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Not supported by this class - throws an exception.
         * @param command the command to be sent.
         * @param timeout the time to wait for a response.
         * @returns the response to the command sent.
         * @throws UnsupportedOperationException.
         */
        virtual Response* request( Command* command, unsigned int timeout )
            throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Assigns the command listener for non-response commands.
         * @param listener the listener.
         */
        virtual void setCommandListener( CommandListener* listener ){
            this->listener = listener;
        }

        /**
         * Sets the command reader.
         * @param reader the object that will be used for reading command objects.
         */
        virtual void setCommandReader( CommandReader* reader ){
            this->reader = reader;
        }

        /**
         * Sets the command writer.
         * @param writer the object that will be used for writing command objects.
         */
        virtual void setCommandWriter( CommandWriter* writer ){
            this->writer = writer;
        }

        /**
         * Sets the observer of asynchronous exceptions from this transport.
         * @param listener the listener of transport exceptions.
         */
        virtual void setTransportExceptionListener( TransportExceptionListener* listener ){
            this->exceptionListener = listener;
        }

        /**
         * Sets the input stream for in-coming commands.
         * @param is The input stream.
         */
        virtual void setInputStream( decaf::io::InputStream* is ){
            this->inputStream = is;
        }

        /**
         * Sets the output stream for out-going commands.
         * @param os The output stream.
         */
        virtual void setOutputStream( decaf::io::OutputStream* os ){
            this->outputStream = os;
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
        virtual void start() throw( cms::CMSException );

        /**
         * Stops the polling thread and closes the streams.  This can
         * be called explicitly, but is also called in the destructor. Once
         * this object has been closed, it cannot be restarted.
         * @throws CMSException if errors occur.
         */
        virtual void close() throw( cms::CMSException );

        /**
         * Runs the polling thread.
         */
        virtual void run();

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
    };

}}

#endif /*_ACTIVEMQ_TRANSPORT_IOTRANSPORT_H_*/
