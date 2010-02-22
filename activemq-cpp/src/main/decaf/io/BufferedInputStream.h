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

#ifndef _DECAF_IO_BUFFEREDINPUTSTREAM_H_
#define _DECAF_IO_BUFFEREDINPUTSTREAM_H_

#include <decaf/util/Config.h>
#include <decaf/io/FilterInputStream.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace decaf{
namespace io{

    class StreamBuffer;

    /**
     * A wrapper around another input stream that performs
     * a buffered read, where it reads more data than it needs
     * in order to reduce the number of io operations on the
     * input stream.
     */
    class DECAF_API BufferedInputStream : public FilterInputStream {
    private:

        StreamBuffer* buffer;

    public:

        /**
         * Constructor
         *
         * @param stream
         *      The target input stream to buffer.
         * @param own
         *      Indicates if we own the stream object, defaults to false.
         */
        BufferedInputStream( InputStream* stream, bool own = false );

        /**
         * Constructor
         *
         * @param stream
         *      The target input stream to buffer.
         * @param bufferSize
         *      The size in bytes to allocate for the internal buffer.
         * @param own
         *      Indicates if we own the stream object, defaults to false.
         *
         * @throws IllegalArgumentException is the size is zero.
         */
        BufferedInputStream( InputStream* stream, std::size_t bufferSize, bool own = false )
            throw ( lang::exceptions::IllegalArgumentException );

        virtual ~BufferedInputStream();

        /**
         * Indicates the number of bytes available.
         * @return the sum of the amount of data available
         * in the buffer and the data available on the target
         * input stream.
         */
        virtual std::size_t available() const throw ( decaf::io::IOException );

        /**
         * Close this BufferedInputStream. This implementation closes the target
         * stream and releases any resources associated with it.
         * @throws IOException If an error occurs attempting to close this stream.
         */
        virtual void close() throw( decaf::io::IOException );

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
         *
         * @param readLimit
         *      max bytes read before marked position is invalid.
         */
        virtual void mark( int readLimit );

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
        virtual void reset() throw ( decaf::io::IOException );

        /**
         * Determines if this input stream supports the mark and reset methods.
         * Whether or not mark and reset are supported is an invariant property of
         * a particular input stream instance.
         * @returns true if this stream instance supports marks
         */
        virtual bool markSupported() const{ return true; }

    protected:

        virtual int doReadByte() throw ( decaf::io::IOException );

        virtual int doReadArrayBounded( unsigned char* buffer, std::size_t size,
                                        std::size_t offset, std::size_t length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::IndexOutOfBoundsException,
                    decaf::lang::exceptions::NullPointerException );

    private:

        int bufferData( InputStream* stream ) throw ( decaf::io::IOException );

    };

}}

#endif /*_DECAF_IO_BUFFEREDINPUTSTREAM_H_*/
