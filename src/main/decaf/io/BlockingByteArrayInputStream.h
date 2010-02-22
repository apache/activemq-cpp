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
#include <vector>

namespace decaf{
namespace io{

    /**
     * This is a blocking version of a byte buffer stream.  Read operations
     * block until the requested data becomes available in the internal
     * buffer via a call to setByteArray.
     */
    class DECAF_API BlockingByteArrayInputStream : public InputStream {
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
         * Indicates the number of bytes available to be read without
         * blocking.
         * @return the data available in the internal buffer.
         * @throws IOException if an error occurs.
         */
        virtual std::size_t available() const throw ( decaf::io::IOException );

        /**
         * Closes the target input stream.
         * @throws IOException if an error occurs.
         */
        virtual void close() throw ( decaf::io::IOException );

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

    protected:

        virtual int doReadByte() throw ( IOException );

        virtual int doReadArrayBounded( unsigned char* buffer, std::size_t size,
                                        std::size_t offset, std::size_t length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::IndexOutOfBoundsException,
                    decaf::lang::exceptions::NullPointerException );

    };

}}

#endif /*_DECAF_IO_BLOCKINGBYTEARRAYINPUTSTREAM_H_*/
