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

#ifndef _DECAF_INTERNAL_IO_STANDARDINPUTSTREAM_H_
#define _DECAF_INTERNAL_IO_STANDARDINPUTSTREAM_H_

#include <decaf/util/Config.h>
#include <decaf/io/InputStream.h>
#include <decaf/util/concurrent/Mutex.h>

namespace decaf {
namespace internal {
namespace io {

    class DECAF_API StandardInputStream : public decaf::io::InputStream {
    private:

        /**
         * Synchronization object.
         */
        util::concurrent::Mutex mutex;

    public:

        StandardInputStream();
        virtual ~StandardInputStream();

        /**
         * Indicates the number of bytes available.
         * @return The number of bytes until the end of the internal buffer.
         */
        virtual std::size_t available() const throw ( decaf::io::IOException );

        /**
         * Reads a single byte from the buffer.
         * @return The next byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char read() throw ( decaf::io::IOException );

        /**
         * Reads an array of bytes from the buffer.
         *
         * @param buffer (out) the target buffer.
         * @param offset the position in the buffer to start reading from.
         * @param bufferSize the size of the output buffer.
         *
         * @return The number of bytes read.
         *
         * @throws decaf::io::IOException thrown if an error occurs.
         * @throws decaf::lang::exceptions::NullPointerException if buffer is null.
         */
        virtual int read( unsigned char* buffer,
                          std::size_t offset,
                          std::size_t bufferSize )
            throw ( decaf::io::IOException, decaf::lang::exceptions::NullPointerException );

        /**
         * Closes the target input stream.
         * @throws decaf::io::IOException thrown if an error occurs.
         */
        virtual void close() throw( lang::Exception ) {}

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
         * @returns total bytes skipped
         *
         * @throws decaf::io::IOException if an error occurs
         * @throws decaf::lang::exceptions::UnsupportedOperationException
         *         If skip is not supported.
         */
        virtual std::size_t skip( std::size_t num )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::UnsupportedOperationException );

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
        virtual void mark( int readLimit DECAF_UNUSED ) {}

        /**
         * Repositions this stream to the position at the time the mark method was
         * last called on this input stream.
         *
         * If the method markSupported returns true, then:
         *   * If the method mark has not been called since the stream was created,
         *     or the number of bytes read from the stream since mark was last called
         *     is larger than the argument to mark at that last call, then an
         *     IOException might be thrown.
         *   * If such an IOException is not thrown, then the stream is reset to a
         *     state such that all the bytes read since the most recent call to mark
         *     (or since the start of the file, if mark has not been called) will be
         *     resupplied to subsequent callers of the read method, followed by any
         *     bytes that otherwise would have been the next input data as of the
         *     time of the call to reset.
         * If the method markSupported returns false, then:
         *   * The call to reset may throw an IOException.
         *   * If an IOException is not thrown, then the stream is reset to a fixed
         *     state that depends on the particular type of the input stream and how
         *     it was created. The bytes that will be supplied to subsequent callers
         *     of the read method depend on the particular type of the input stream.
         * @throws IOException
         */
        virtual void reset() throw ( decaf::io::IOException );

        /**
         * Determines if this input stream supports the mark and reset methods.
         * Whether or not mark and reset are supported is an invariant property of
         * a particular input stream instance.
         * @returns true if this stream instance supports marks
         */
        virtual bool markSupported() const{ return false; }

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
         * @param millisecs the time in milliseconds to wait, or
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

}}}

#endif /* _DECAF_INTERNAL_IO_STANDARDINPUTSTREAM_H_ */
