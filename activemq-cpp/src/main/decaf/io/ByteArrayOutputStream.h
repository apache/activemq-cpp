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

#ifndef _DECAF_IO_BYTEARRAYOUTPUTSTREAM_H_
#define _DECAF_IO_BYTEARRAYOUTPUTSTREAM_H_

#include <decaf/io/OutputStream.h>
#include <decaf/util/concurrent/Mutex.h>
#include <vector>

namespace decaf{
namespace io{

    class DECAF_API ByteArrayOutputStream : public OutputStream {
    private:

        /**
         * Default buffer to use, if none provided.
         */
        std::vector<unsigned char> defaultBuffer;

        /**
         * Reference to the buffer being used by this stream.
         */
        std::vector<unsigned char>* activeBuffer;

        /**
         * Synchronization object.
         */
        util::concurrent::Mutex mutex;

    public:

        /**
         * Default Constructor - uses a default internal buffer
         */
        ByteArrayOutputStream();

        /**
         * Uses the given buffer as the target.  Calls setBuffer.
         * @param buffer the target buffer.
         */
        ByteArrayOutputStream( std::vector<unsigned char>& buffer );

        /**
         * Destructor
         */
        virtual ~ByteArrayOutputStream() {};

        /**
         * Sets the internal buffer.  This input stream will wrap around
         * the given buffer and all writes will be performed directly on
         * the buffer.  This object does not retain control of the buffer's
         * lifetime however - this is the job of the caller.
         * @param buffer The target buffer.
         */
        virtual void setBuffer( std::vector<unsigned char>& buffer );

        /**
         * Get a snapshot of the data
         * @return pointer to the data
         */
        virtual const unsigned char* toByteArray() const {
            if( activeBuffer->size() == 0 ){
                return NULL;
            }

            return &(*activeBuffer)[0];
        }

        /**
         * Get the Size of the Internal Buffer
         * @return size of the internal buffer
         */
        virtual std::size_t size() const {
            return activeBuffer->size();
        }

        /**
         * Writes a single byte to the output stream.
         * @param c the byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( unsigned char c )
           throw ( IOException );

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The bytes to write.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const std::vector<unsigned char>& buffer )
            throw ( IOException );

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The array of bytes to write.
         * @param offset, the position to start writing in buffer.
         * @param len The number of bytes from the buffer to be written.
         * @throws IOException thrown if an error occurs.
         * @throws NullPointerException thrown if buffer is Null.
         */
        virtual void write( const unsigned char* buffer,
                            std::size_t offset,
                            std::size_t len )
            throw ( IOException, lang::exceptions::NullPointerException );

        /**
         * Invokes flush on the target output stream, has no affect.
         * @throws IOException
         */
        virtual void flush() throw ( IOException ){ /* do nothing */ }

        /**
         * Clear current Stream contents
         * @throws IOException
         */
        virtual void reset() throw ( IOException );

        /**
         * Invokes close on the target output stream.
         * @throws CMSException
         */
        void close() throw( lang::Exception ){ /* do nothing */ }

        /**
         * Converts the bytes in the buffer into a standard C++ string
         * @returns a string contianing the bytes in the buffer
         */
        std::string toString() const;

        /**
         * Writes the complete contents of this byte array output stream to the
         * specified output stream argument, as if by calling the output
         * stream's write method using out.write( buf, 0, count ).
         */
        void writeTo( OutputStream* out ) const
            throw ( IOException, lang::exceptions::NullPointerException );

    public:

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void lock() throw( lang::Exception ){
            mutex.lock();
        }

        /**
         * Unlocks the object.
         * @throws Exception
         */
        virtual void unlock() throw( lang::Exception ){
            mutex.unlock();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void wait() throw( lang::Exception ){
            mutex.wait();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param time in millisecsonds to wait, or WAIT_INIFINITE
         * @throws Exception
         */
        virtual void wait( unsigned long millisecs ) throw( lang::Exception ){
            mutex.wait(millisecs);
        }

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notify() throw( lang::Exception ){
            mutex.notify();
        }

        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notifyAll() throw( lang::Exception ){
            mutex.notifyAll();
        }
    };

}}

#endif /*_DECAF_IO_BYTEARRAYOUTPUTSTREAM_H_*/
