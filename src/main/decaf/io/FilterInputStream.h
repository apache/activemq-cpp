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

#ifndef _DECAF_IO_FILTERINPUTSTREAM_H_
#define _DECAF_IO_FILTERINPUTSTREAM_H_

#include <decaf/io/InputStream.h>
#include <decaf/io/IOException.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

namespace decaf{
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
    class DECAF_API FilterInputStream : public InputStream
    {
    protected:

        // The input stream to wrap
        InputStream* inputStream;

        // Synchronization object.
        util::concurrent::Mutex mutex;

        // Indicates if we own the wrapped stream
        bool own;

        // Indicates that this stream was closed
        volatile bool closed;

    public:

        /**
         * Constructor to create a wrapped InputStream
         * @param inputStream the stream to wrap and filter
         * @param own indicates if we own the stream object, defaults to false
         */
        FilterInputStream( InputStream* inputStream, bool own = false ) {
            this->inputStream = inputStream;
            this->own = own;
            this->closed = false;
        }

        virtual ~FilterInputStream() {
            try {
                this->close();

                if( own ) {
                    delete inputStream;
                }
                inputStream = NULL;
            }
            DECAF_CATCH_NOTHROW( IOException )
            DECAF_CATCHALL_NOTHROW( )
        }

        /**
         * Returns the number of bytes that can be read from this input stream
         * without blocking.  This method simply performs in.available() and
         * returns the result.
         * @return the number of bytes available without blocking.
         */
        virtual std::size_t available() const throw ( IOException ) {
            try {

                if( isClosed() ) {
                    throw IOException(
                        __FILE__, __LINE__,
                        "FilterInputStream::available - Stream is closed" );
                }

                return inputStream->available();
            }
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCHALL_THROW( IOException )
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

                if( isClosed() ) {
                    throw IOException(
                        __FILE__, __LINE__,
                        "FilterInputStream::read - Stream is closed" );
                }

                return inputStream->read();
            }
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCHALL_THROW( IOException )
        }

        /**
         * Reads up to len bytes of data from this input stream into an array
         * of bytes. This method blocks until some input is available.
         * This method simply performs in.read(b, len) and returns the result.
         * @param buffer (out) the target buffer.
         * @param offset the position to start reading in the passed buffer.
         * @param bufferSize the size of the output buffer.
         * @return The number of bytes read or -1 if EOF is detected
         * @throws IOException thrown if an error occurs.
         * @throws NullPointerException
         */
        virtual int read( unsigned char* buffer,
                          std::size_t offset,
                          std::size_t bufferSize )
            throw ( IOException,
                    lang::exceptions::NullPointerException )
        {
            try {

                if( isClosed() ) {
                    throw IOException(
                        __FILE__, __LINE__,
                        "FilterInputStream::read - Stream is closed" );
                }

                return inputStream->read( buffer, offset, bufferSize );
            }
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCH_RETHROW( lang::exceptions::NullPointerException )
            DECAF_CATCHALL_THROW( IOException )
        }

        /**
         * Close the Stream, the FilterOutputStream simply calls the close
         * method of the underlying stream
         * @throws Exception
         */
        virtual void close() throw ( lang::Exception ) {
            try {
                if( !closed && inputStream != NULL ) {
                    inputStream->close();
                }
                this->closed = true;
            }
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCHALL_THROW( IOException )
        }

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
        virtual std::size_t skip( std::size_t num ) throw ( io::IOException, lang::exceptions::UnsupportedOperationException ) {
            try {

                if( isClosed() ) {
                    throw IOException(
                        __FILE__, __LINE__,
                        "FilterInputStream::skip - Stream is closed" );
                }

                return inputStream->skip( num );
            }
            DECAF_CATCH_RETHROW( lang::exceptions::UnsupportedOperationException )
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCHALL_THROW( IOException )
        }

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
        virtual void mark( int readLimit ) {
            try {

                if( !isClosed() ) {
                    inputStream->mark( readLimit );
                }
            }
            DECAF_CATCHALL_NOTHROW()
        }

        /**
         * Repositions this stream to the position at the time the mark method was
         * last called on this input stream.
         *
         * If the method markSupported returns true, then:
         *   * If the method mark has not been called since the stream was created,
         *     or the number of bytes read from the stream since mark was last called
         * 	   is larger than the argument to mark at that last call, then an
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
        virtual void reset() throw ( IOException ) {
            try {

                if( isClosed() ) {
                    throw IOException(
                        __FILE__, __LINE__,
                        "FilterInputStream::skip - Stream is closed" );
                }

                return inputStream->reset();
            }
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCHALL_THROW( IOException )
        }

        /**
         * Determines if this input stream supports the mark and reset methods.
         * Whether or not mark and reset are supported is an invariant property of
         * a particular input stream instance.
         * @returns true if this stream instance supports marks
         */
        virtual bool markSupported() const {
            try {
                if( !isClosed() ) {
                    return inputStream->markSupported();
                }
            }
            DECAF_CATCHALL_NOTHROW()
            return false;
        }

    public:  // Synchronizable

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
         * @param millisecs the time in millisecsonds to wait, or WAIT_INIFINITE
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

    protected:

        /**
         * @returns true if this stream has been closed.
         */
        virtual bool isClosed() const {
            return this->closed || inputStream == NULL;
        }

    };

}}

#endif /*_DECAF_IO_FILTERINPUTSTREAM_H_*/
