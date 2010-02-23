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

#ifndef _DECAF_IO_PUSHBACKINPUTSTREAM_H_
#define _DECAF_IO_PUSHBACKINPUTSTREAM_H_

#include <decaf/util/Config.h>

#include <decaf/io/InputStream.h>
#include <decaf/io/FilterInputStream.h>

namespace decaf {
namespace io {

    /**
     * A PushbackInputStream adds functionality to another input stream, namely the ability to
     * "push back" or "unread" one byte. This is useful in situations where it is convenient for
     * a fragment of code to read an indefinite number of data bytes that are delimited by a
     * particular byte value; after reading the terminating byte, the code fragment can "unread"
     * it, so that the next read operation on the input stream will reread the byte that was
     * pushed back. For example, bytes representing the characters constituting an identifier
     * might be terminated by a byte representing an operator character; a method whose job
     * is to read just an identifier can read until it sees the operator and then push the
     * operator back to be re-read.
     *
     * @since 1.0
     */
    class PushbackInputStream : public FilterInputStream {
    private:

        unsigned char* buffer;
        std::size_t bufferSize;
        std::size_t pos;

    public:

        /**
         *
         */
        PushbackInputStream( InputStream* stream, bool own = false );

        /**
         *
         */
        PushbackInputStream( InputStream* stream, std::size_t bufSize, bool own = false );

        virtual ~PushbackInputStream();

        /**
         * Pushes back the given byte, the byte is copied to the front of the pushback buffer, future
         * calls to read start reading from the beginning of these pushed back byte.
         *
         * @param value
         *      The byte that is to be placed at the front of the push back buffer.
         *
         * @throws IOException if there is not enough space in the pushback buffer or this stream
         *         has already been closed.
         */
        void unread( unsigned char value )
            throw( decaf::io::IOException );

        /**
         * Pushes back the given array of bytes, the bytes are copied to the front of the pushback
         * buffer, future calls to read start reading from the beginning of these pushed back bytes.
         *
         * @param buffer
         *      The bytes to copy to the front of push back buffer.
         * @param size
         *      The size of the array to be copied.
         *
         * @throws NullPointerException if the buffer passed is NULL.
         * @throws IOException if there is not enough space in the pushback buffer or this stream
         *         has already been closed.
         */
        void unread( const unsigned char* buffer, std::size_t size )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::NullPointerException );

        /**
         * Pushes back the given array of bytes, the bytes are copied to the front of the pushback
         * buffer, future calls to read start reading from the beginning of these pushed back bytes.
         *
         * @param buffer
         *      The bytes to copy to the front of push back buffer.
         * @param size
         *      The size of the array to be copied.
         * @param offset
         *      The position in the buffer to start copying from.
         * @param length
         *      The number of bytes to push back from the passed buffer.
         *
         * @throws NullPointerException if the buffer passed is NULL.
         * @throws IndexOutOfBoundsException if the offset + length is greater than the buffer size.
         * @throws IOException if there is not enough space in the pushback buffer or this stream
         *         has already been closed.
         */
        void unread( const unsigned char* buffer, std::size_t size, std::size_t offset, std::size_t length )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NullPointerException );

        /**
         * {@inheritdoc}
         *
         * Returns the sum of the number of pushed back bytes if any and the amount of bytes
         * available in the underlying stream via a call to available.
         */
        virtual std::size_t available() const throw ( decaf::io::IOException );

        /**
         * {@inheritDoc}
         *
         * This method first skips bytes in the local pushed back buffer before attempting to
         * complete the request by calling the underlying stream skip method with the remainder
         * of bytes that needs to be skipped.
         */
        virtual std::size_t skip( std::size_t num )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * {@inheritDoc}
         *
         * Does nothing for this class.
         */
        virtual void mark( int readLimit );

        /**
         * {@inheritDoc}
         *
         * Does nothing except throw an IOException.
         */
        virtual void reset() throw ( decaf::io::IOException );

        /**
         * {@inheritDoc}
         *
         * Always returns false for this class.
         */
        virtual bool markSupported() const {
            return false;
        }

    protected:

        virtual int doReadByte() throw ( decaf::io::IOException );

        virtual int doReadArrayBounded( unsigned char* buffer, std::size_t size,
                                        std::size_t offset, std::size_t length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::IndexOutOfBoundsException,
                    decaf::lang::exceptions::NullPointerException );

    };

}}

#endif /* _DECAF_IO_PUSHBACKINPUTSTREAM_H_ */
