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
        int bufferSize;
        int pos;

    private:

        PushbackInputStream( const PushbackInputStream& );
        PushbackInputStream& operator= ( const PushbackInputStream& );

    public:

        /**
         * Creates a PushbackInputStream  and saves its argument, the input stream in, for later
         * use. Initially, there is no pushed-back byte.
         *
         * @param stream
         *      The InputStream instance to wrap.
         * @param
         *      Boolean value indicating if this FilterInputStream owns the wrapped stream.
         */
        PushbackInputStream( InputStream* stream, bool own = false );

        /**
         * Creates a PushbackInputStream  and saves its argument, the input stream in, for later
         * use. Initially, there is no pushed-back byte.
         *
         * @param stream
         *      The InputStream instance to wrap.
         * @param bufSize
         *      The number of byte to allocate for pushback into this stream.
         * @param
         *      Boolean value indicating if this FilterInputStream owns the wrapped stream.
         *
         * @throws IllegalArgumentException if the bufSize argument is < zero.
         */
        PushbackInputStream( InputStream* stream, int bufSize, bool own = false );

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
        void unread( unsigned char value );

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
         * @throws IndexOutOfBoundsException if the size value given is negative.
         * @throws IOException if there is not enough space in the pushback buffer or this stream
         *         has already been closed.
         */
        void unread( const unsigned char* buffer, int size );

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
        void unread( const unsigned char* buffer, int size, int offset, int length );

        /**
         * {@inheritDoc}
         *
         * Returns the sum of the number of pushed back bytes if any and the amount of bytes
         * available in the underlying stream via a call to available.
         */
        virtual int available() const;

        /**
         * {@inheritDoc}
         *
         * This method first skips bytes in the local pushed back buffer before attempting to
         * complete the request by calling the underlying stream skip method with the remainder
         * of bytes that needs to be skipped.
         */
        virtual long long skip( long long num );

        /**
         * Does nothing except throw an IOException.
         *
         * {@inheritDoc}
         */
        virtual void mark( int readLimit );

        /**
         * Does nothing except throw an IOException.
         *
         * {@inheritDoc}
         */
        virtual void reset();

        /**
         * Does nothing except throw an IOException.
         *
         * {@inheritDoc}
         */
        virtual bool markSupported() const {
            return false;
        }

    protected:

        virtual int doReadByte();

        virtual int doReadArrayBounded( unsigned char* buffer, int size, int offset, int length );

    };

}}

#endif /* _DECAF_IO_PUSHBACKINPUTSTREAM_H_ */
