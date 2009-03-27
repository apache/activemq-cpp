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

#ifndef _DECAF_IO_BYTEARRAYINPUTSTREAM_H_
#define _DECAF_IO_BYTEARRAYINPUTSTREAM_H_

#include <decaf/io/InputStream.h>
#include <decaf/util/concurrent/Mutex.h>
#include <vector>
#include <algorithm>

namespace decaf{
namespace io{

    /**
     * Simple implementation of InputStream that wraps around an STL Vector
     * std::vector<unsigned char>.
     *
     * Closing a ByteArrayInputStream has no effect. The methods in this class can be
     * called after the stream has been closed without generating an IOException.
     */
    class DECAF_API ByteArrayInputStream : public InputStream {
    private:

        /**
         * Default buffer to use, if none provided.
         */
        std::vector<unsigned char> defaultBuffer;

        /**
         * Reference to the buffer being used by this stream.
         */
        const std::vector<unsigned char>* activeBuffer;

        /**
         * iterator to current position in buffer.
         */
        std::vector<unsigned char>::const_iterator pos;

        /**
         * Synchronization object.
         */
        util::concurrent::Mutex mutex;

        /**
         * The currently marked position or begin() of activeBuffer.
         */
        std::vector<unsigned char>::const_iterator markpos;

    public:

        /**
         * Default Constructor
         */
        ByteArrayInputStream();

        /**
         * Creates the input stream and calls setBuffer with the
         * specified buffer object.
         * @param buffer The buffer to be used.
         */
        ByteArrayInputStream( const std::vector<unsigned char>& buffer );

        /**
         * Constructor
         * @param buffer initial byte array to use to read from
         * @param bufferSize the size of the buffer
         */
        ByteArrayInputStream( const unsigned char* buffer,
                              std::size_t bufferSize );

        virtual ~ByteArrayInputStream();

        /**
         * Sets the internal buffer.  The input stream will wrap around
         * this buffer and will perform all read operations on it.  The
         * position will be reinitialized to the beginning of the specified
         * buffer.  This class will not own the given buffer - it is the
         * caller's responsibility to free the memory of the given buffer
         * as appropriate.
         * @param buffer The buffer to be used.
         */
        virtual void setBuffer( const std::vector<unsigned char>& buffer );

        /**
         * Sets the data that this reader uses, replaces any existing
         * data and resets to beginning of the buffer.
         * @param buffer initial byte array to use to read from
         * @param bufferSize the size of the buffer
         */
        virtual void setByteArray( const unsigned char* buffer,
                                   std::size_t bufferSize );

        /**
         * Indcates the number of bytes avaialable.
         * @return The number of bytes until the end of the internal buffer.
         */
        virtual std::size_t available() const throw ( IOException ) {
            if( activeBuffer == NULL ){
                throw IOException(
                    __FILE__, __LINE__,
                    "buffer has not been initialized");
            }

            return std::distance( pos, activeBuffer->end() );
        }

        /**
         * Reads a single byte from the buffer.
         * @return The next byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char read() throw ( IOException );

        /**
         * Reads an array of bytes from the buffer.
         * @param buffer (out) the target buffer.
         * @param offset the position in the buffer to start reading from.
         * @param bufferSize the size of the output buffer.
         * @return The number of bytes read.
         * @throws IOException thrown if an error occurs.
         */
        virtual int read( unsigned char* buffer,
                          std::size_t offset,
                          std::size_t bufferSize )
            throw ( IOException, lang::exceptions::NullPointerException );

        /**
         * Closes the target input stream.
         * @throws IOException thrown if an error occurs.
         */
        virtual void close() throw( lang::Exception ){ /* do nothing */ }

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

        /**
         * Marks the current position in the stream A subsequent call to the
         * reset method repositions this stream at the last marked position so
         * that subsequent reads re-read the same bytes.
         *
         * If a stream instance reports that marks are supported then the stream
         * will ensure that the same bytes can be read again after the reset method
         * is called so long the readLimit is not reached.
         * @param readLimit - max bytes read before marked position is invalid.
         */
        virtual void mark( int readLimit DECAF_UNUSED ) {
            // the reset point is now the marked position until a new byte buffer
            // is set on this stream.
            this->markpos = pos;
        }

        /**
         * Resets the read index to the beginning of the byte array, unless mark
         * has been called and the markLimit has not been exceeded, in which case
         * the stream is reset to the marked position.
         */
        virtual void reset() throw ( IOException );

        /**
         * Determines if this input stream supports the mark and reset methods.
         * Whether or not mark and reset are supported is an invariant property of
         * a particular input stream instance.
         * @returns true if this stream instance supports marks
         */
        virtual bool markSupported() const{ return true; }

    protected:

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
         * @param millisecs the time in millisecsonds to wait, or
         * WAIT_INIFINITE
         * @throws Exception
         */
        virtual void wait( unsigned long millisecs ) throw( lang::Exception ){
            mutex.wait( millisecs );
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

#endif /*_DECAF_IO_BYTEARRAYINPUTSTREAM_H_*/
