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

#ifndef _DECAF_IO_BLOCKINGBYTEARRAYINPUTSTREAM_H_
#define _DECAF_IO_BLOCKINGBYTEARRAYINPUTSTREAM_H_

#include <decaf/io/InputStream.h>
#include <decaf/util/concurrent/Mutex.h>
#include <vector>

namespace decaf{
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
        util::concurrent::Mutex mutex;

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
                                      std::size_t bufferSize );

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
            std::size_t bufferSize );

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

        /**
         * Indicates the number of bytes available to be read without
         * blocking.
         * @return the data available in the internal buffer.
         * @throws IOException if an error occurs.
         */
        virtual std::size_t available() const throw ( IOException ){
            return std::distance( pos, buffer.end() );
        }

        /**
         * Reads a single byte from the buffer.  This operation will
         * block until data has been added to the buffer via a call
         * to setByteArray.
         * @return the next byte.
         * @throws IOException if an error occurs.
         */
        virtual unsigned char read() throw ( IOException );

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
        virtual std::size_t read( unsigned char* buffer, std::size_t bufferSize )
            throw ( IOException );

        /**
         * Closes the target input stream.
         * @throws IOException if an error occurs.
         */
        virtual void close() throw ( lang::Exception );

        /**
         * Skips over and discards n bytes of data from this input stream. The
         * skip method may, for a variety of reasons, end up skipping over some
         * smaller number of bytes, possibly 0. This may result from any of a
         * number of conditions; reaching end of file before n bytes have been
         * skipped is only one possibility. The actual number of bytes skipped
         * is returned. If n is negative, no bytes are skipped.
         * <p>
         * The skip method of InputStream creates a byte array and then
         * repeatedly reads into it until n bytes have been read or the end
         * of the stream has been reached. Subclasses are encouraged to
         * provide a more efficient implementation of this method.
         * @param num - the number of bytes to skip
         * @returns total butes skipped
         * @throws IOException if an error occurs
         */
        virtual std::size_t skip( std::size_t num )
            throw ( io::IOException, lang::exceptions::UnsupportedOperationException );

   };

}}

#endif /*_DECAF_IO_BLOCKINGBYTEARRAYINPUTSTREAM_H_*/
