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
 
#ifndef ACTIVEMQ_TRANSPORT_RESPONSECORRELATOR_H_
#define ACTIVEMQ_TRANSPORT_RESPONSECORRELATOR_H_

#include <activemq/transport/TransportFilter.h>
#include <activemq/transport/FutureResponse.h>
#include <activemq/transport/Command.h>
#include <activemq/transport/ExceptionResponse.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/concurrent/Concurrent.h>
#include <map>
#include <stdio.h>

namespace activemq{
namespace transport{
  
    /**
     * This type of transport filter is responsible for correlating
     * asynchronous responses with requests.  Non-response messages
     * are simply sent directly to the CommandListener.  It owns
     * the transport that it
     */
    class ResponseCorrelator : public TransportFilter 
    {
    private:        

        /**
         * The next command id for sent commands.
         */
        unsigned int nextCommandId;        
        
        /**
         * Map of request ids to future response objects.
         */
        std::map<unsigned int, FutureResponse*> requestMap;
        
        /**
         * Maximum amount of time in milliseconds to wait for a response.
         */
        unsigned long maxResponseWaitTime;
        
        /**
         * Sync object for accessing the next command id variable.
         */
        concurrent::Mutex commandIdMutex;
        
        /**
         * Sync object for accessing the request map.
         */
        concurrent::Mutex mapMutex;
        
        /**
         * Flag to indicate the closed state.
         */
        bool closed;
       
    private:
    
        /**
         * Returns the next available command id.
         */
        unsigned int getNextCommandId() throw ( exceptions::ActiveMQException );
         
    public:
  
        /**
         * Constructor.
         * @param next the next transport in the chain
         * @param own indicates if this transport owns the next
         */
        ResponseCorrelator( Transport* next, const bool own = true );
        
        virtual ~ResponseCorrelator();
        
        /**
         * Gets the maximum wait time for a response in milliseconds.
         * @return max time that a response can take
         */
        virtual unsigned long getMaxResponseWaitTime() const;
        
        /**
         * Sets the maximum wait time for a response in milliseconds.
         * @param milliseconds the max time that a response can take.
         */
        virtual void setMaxResponseWaitTime( const unsigned long milliseconds );
        
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
            throw( CommandIOException, exceptions::UnsupportedOperationException );
        
        /**
         * Sends the given request to the server and waits for the response.
         * @param command The request to send.
         * @return the response from the server.  This may be of type ExceptionResponse
         * in the case of a distributed error that occurs at the broker.
         * @throws CommandIOException if an error occurs with the request.
         */
        virtual Response* request( Command* command ) 
            throw( CommandIOException, exceptions::UnsupportedOperationException );
        
        /**
         * This is called in the context of the nested transport's
         * reading thread.  In the case of a response object,
         * updates the request map and notifies those waiting on the
         * response.  Non-response messages are just delegated to
         * the command listener.
         * @param command the received from the nested transport.
         */
        virtual void onCommand( Command* command );
        
        /**
         * Assigns the command listener for non-response commands.
         * @param listener the listener.
         */
        virtual void setCommandListener( CommandListener* listener );
        
        /**
         * Sets the observer of asynchronous exceptions from this transport.
         * @param listener the listener of transport exceptions.
         */
        virtual void setTransportExceptionListener( 
            TransportExceptionListener* listener );
        
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
        
    };
    
}}

#endif /*ACTIVEMQ_TRANSPORT_RESPONSECORRELATOR_H_*/
