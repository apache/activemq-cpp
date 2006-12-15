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

#ifndef _ACTIVEMQ_IO_BLOCKINGBYTEARRAYINPUTSTREAM_H_
#define _ACTIVEMQ_IO_BLOCKINGBYTEARRAYINPUTSTREAM_H_

#include <activemq/io/InputStream.h>
#include <activemq/concurrent/Mutex.h>
#include <vector>

namespace activemq{
namespace io{

    /**
     * This is a blocking version of a byte buffer stream.  Read operations
     * block until the requested data becomes available in the internal
     * buffer via a call to setByteArray.
     */
    class BlockingByteArrayInputStream : public InputStream
    {
    private:
         
        /** 
         * Default buffer to use, if none provided.
         */
        std::vector<unsigned char> buffer;
        
        /**
         * iterator to current position in buffer.
         */
        std::vector<unsigned char>::const_iterator pos;

        /**
         * Synchronization object.
         */
        concurrent::Mutex mutex;
        
        /**
         * Indicates that this stream is in the process of shutting
         * down.
         */
        bool closing;
      
    public:

        /**
         * Default Constructor - uses a default internal buffer
         */
        BlockingByteArrayInputStream();
        
        /**
         * Constructor that initializes the internal buffer. 
         * @see setByteArray.
         */
        BlockingByteArrayInputStream( const unsigned char* buffer,
                                      int bufferSize );

        /**
         * Destructor
         */
        virtual ~BlockingByteArrayInputStream();

        /**
         * Sets the data that this reader uses.  Replaces any existing
         * data and resets the read index to the beginning of the buffer.
         * When this method is called, it notifies any other threads that
         * data is now available to be read.
         * @param buffer The new data to be copied to the internal buffer.
         * @param bufferSize The size of the new buffer.
         */
        virtual void setByteArray( const unsigned char* buffer,
            int bufferSize );
            
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
         * @param time in millisecsonds to wait, or WAIT_INIFINITE
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
      
        /**
         * Indicates the number of bytes available to be read without
         * blocking.
         * @return the data available in the internal buffer.
         * @throws IOException if an error occurs.
         */
        virtual int available() const throw (IOException){
            return std::distance( pos, buffer.end() );
        }
        
        /**
         * Reads a single byte from the buffer.  This operation will
         * block until data has been added to the buffer via a call
         * to setByteArray.
         * @return the next byte.
         * @throws IOException if an error occurs.
         */
        virtual unsigned char read() throw (IOException);
        
        /**
         * Reads an array of bytes from the buffer.  If the desired amount
         * of data is not currently available, this operation
         * will block until the appropriate amount of data is available
         * in the buffer via a call to setByteArray.
         * @param buffer (out) the target buffer
         * @param bufferSize the size of the output buffer.
         * @return the number of bytes read.
         * @throws IOException f an error occurs.
         */
        virtual int read( unsigned char* buffer, int bufferSize )
            throw (IOException);
            
        /**
         * Closes the target input stream.
         * @throws IOException if an error occurs.
         */
        virtual void close() throw (cms::CMSException);

   };

}}

#endif /*_ACTIVEMQ_IO_BLOCKINGBYTEARRAYINPUTSTREAM_H_*/
