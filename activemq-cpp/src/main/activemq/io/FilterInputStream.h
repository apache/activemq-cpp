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
 
#ifndef _ACTIVEMQ_IO_FILTERINPUTSTREAM_H_
#define _ACTIVEMQ_IO_FILTERINPUTSTREAM_H_

#include <activemq/io/InputStream.h>
#include <activemq/io/IOException.h>
#include <activemq/concurrent/Mutex.h>

namespace activemq{
namespace io{

    /** 
     * A FilterInputStream contains some other input stream, which it uses 
     * as its basic source of data, possibly transforming the data along the 
     * way or providing additional functionality. The class FilterInputStream
     * itself simply overrides all methods of InputStream with versions 
     * that pass all requests to the contained input stream. Subclasses of
     * FilterInputStream  may further override some of these methods and may 
     * also provide additional methods and fields.
     */
    class FilterInputStream : public InputStream
    {
    protected:
    
        // The input stream to wrap
        InputStream* inputStream;
        
        // Synchronization object.
        concurrent::Mutex mutex;
        
        // Indicates if we own the wrapped stream
        bool own;

    public:
    
        /**
         * Constructor to create a wrapped InputStream
         * @param inputStream the stream to wrap and filter
         * @param own indicates if we own the stream object, defaults to false
         */ 
        FilterInputStream( InputStream* inputStream, bool own = false ) {
            this->inputStream = inputStream;
            this->own = own;
        }
        
        virtual ~FilterInputStream() {
            try {
                if( own == true ) delete inputStream;
            }
            AMQ_CATCH_NOTHROW( IOException )
            AMQ_CATCHALL_NOTHROW( )     
        }
        
        /**
         * Returns the number of bytes that can be read from this input stream 
         * without blocking.  This method simply performs in.available() and 
         * returns the result. 
         * @return the number of bytes available without blocking.
         */
        virtual int available() const throw ( IOException ) {
            try {
                return inputStream->available();
            }
            AMQ_CATCH_RETHROW( IOException )
            AMQ_CATCHALL_THROW( IOException )
        }

        /**
         * Reads the next byte of data from this input stream. The value byte 
         * is returned as an unsigned char in the range 0 to 255. If no byte is
         * available because the end of the stream has been reached, the value 
         * -1 is returned. This method blocks until input data is available, 
         * the end of the stream is detected, or an exception is thrown.
         * This method simply performs in.read() and returns the result. 
         * @return The next byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char read() throw ( IOException ) {
            try {
                return inputStream->read();
            }
            AMQ_CATCH_RETHROW( IOException )
            AMQ_CATCHALL_THROW( IOException )
        }
        
        /**
         * Reads up to len bytes of data from this input stream into an array 
         * of bytes. This method blocks until some input is available.
         * This method simply performs in.read(b, len) and returns the result. 
         * @param buffer (out) the target buffer.
         * @param bufferSize the size of the output buffer.
         * @return The number of bytes read or -1 if EOS is detected
         * @throws IOException thrown if an error occurs.
         */
        virtual int read( unsigned char* buffer, const int bufferSize ) 
            throw ( IOException )
        {
            try {
                return inputStream->read( buffer, bufferSize );
            }
            AMQ_CATCH_RETHROW( IOException )
            AMQ_CATCHALL_THROW( IOException )
        }
        
        /**
         * Close the Stream, the FilterOutputStream simply calls the close
         * method of the underlying stream
         * @throws CMSException
         */
        virtual void close() throw ( cms::CMSException ) {
            try {
                inputStream->close();
            }
            AMQ_CATCH_RETHROW( IOException )
            AMQ_CATCHALL_THROW( IOException )
        }

    public:  // Synchronizable
    
        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void lock() throw( exceptions::ActiveMQException ){
            mutex.lock();
        }
    
        /**
         * Unlocks the object.
         * @throws ActiveMQException
         */
        virtual void unlock() throw( exceptions::ActiveMQException ){ 
            mutex.unlock();
        }
        
        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void wait() throw( exceptions::ActiveMQException ){
            mutex.wait();
        }
    
        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param millisecs the time in millisecsonds to wait, or WAIT_INIFINITE
         * @throws ActiveMQException
         */
        virtual void wait( unsigned long millisecs ) throw( exceptions::ActiveMQException ){
            mutex.wait(millisecs);
        }

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void notify() throw( exceptions::ActiveMQException ){
            mutex.notify();
        }
        
        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void notifyAll() throw( exceptions::ActiveMQException ){
            mutex.notifyAll();
        }
    
    };

}}

#endif /*_ACTIVEMQ_IO_FILTERINPUTSTREAM_H_*/
