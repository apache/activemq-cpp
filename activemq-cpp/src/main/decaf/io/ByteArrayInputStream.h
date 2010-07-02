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

namespace decaf{
namespace io{

    /**
     * A ByteArrayInputStream contains an internal buffer that contains bytes that may be read from
     * the stream. An internal counter keeps track of the next byte to be supplied by the read method.
     * The ByteArrayInputStream never copies the supplied buffers, only points to them, therefore the
     * caller must ensure that the supplied buffer remain in scope, or is not deleted before this
     * ByteArrayInputStream is freed.  If the own argument of one of the constructors that accepts an
     * array pointer is set to true than the ByteArrayInputStream instance will take ownership of the
     * supplied pointer and delete it when that instance is destroyed.
     *
     * Closing a ByteArrayInputStream has no effect. The methods in this class can be called after
     * the stream has been closed without generating an IOException.
     *
     * @since 1.0
     */
    class DECAF_API ByteArrayInputStream : public InputStream {
    private:

        /**
         * An array of bytes that was provided by the creator of the stream. Elements buffer[0]
         * through buffer[count-1] are the only bytes that can ever be read from the stream; element
         * buffer[pos] is the next byte to be read.
         */
        const unsigned char* buffer;

        /**
         * The Size of the input buffer.
         */
        int size;

        /**
         * Does this object own the supplied pointer.
         */
        bool own;

        /**
         * The index one greater than the last valid character in the input stream buffer. This
         * value should always be nonnegative and not larger than the length of buffer. It is one
         * greater than the position of the last byte within buffer that can ever be read from the
         * input stream buffer.
         */
        int count;

        /**
         * The index of the next character to read from the input stream buffer. This value should
         * always be nonnegative and not larger than the value of count. The next byte to be read
         * from the input stream buffer will be buffer[pos].
         */
        int pos;

        /**
         * The currently marked position in the stream. ByteArrayInputStream objects are marked at
         * position zero by default when constructed. They may be marked at another position within
         * the buffer by the mark() method. The current buffer position is set to this point by the
         * reset() method.
         *
         * If no mark has been set, then the value of mark is the offset passed to the constructor
         * (or 0 if the offset was not supplied).
         */
        int markpos;

    private:

        ByteArrayInputStream( const ByteArrayInputStream& );
        ByteArrayInputStream& operator= ( const ByteArrayInputStream& );

    public:

        /**
         * Creates an ByteArrayInputStream with an empty input buffer, the buffer can be
         * initialized with a call to setByteArray.
         */
        ByteArrayInputStream();

        /**
         * Creates the input stream and calls setBuffer with the
         * specified buffer object.
         *
         * @param buffer
         *      The buffer to be used.
         */
        ByteArrayInputStream( const std::vector<unsigned char>& buffer );

        /**
         * Create an instance of the ByteArrayInputStream with the given buffer as
         * the source of input for all read operations.
         *
         * @param buffer
         *      The initial byte array to use to read from.
         * @param bufferSize
         *      The size of the buffer.
         * @param own
         *      Indicates if this object should take ownership of the array, default is false.
         *
         * @throws NullPointerException if the buffer is Null.
         * @throws IllegalArguementException if the bufferSize is negative.
         */
        ByteArrayInputStream( const unsigned char* buffer, int bufferSize, bool own = false );

        /**
         * Create an instance of the ByteArrayInputStream with the given buffer as
         * the source of input for all read operations.
         *
         * @param buffer
         *      The initial byte array to use to read from.
         * @param bufferSize
         *      The size of the buffer.
         * @param offset
         *      The offset into the buffer to begin reading from.
         * @param length
         *      The number of bytes to read past the offset.
         * @param own
         *      Indicates if this object should take ownership of the array, default is false.
         *
         * @throws NullPointerException if the buffer is Null.
         * @throws IllegalArguementException if the bufferSize is negative.
         */
        ByteArrayInputStream( const unsigned char* buffer, int bufferSize, int offset, int length, bool own = false );

        virtual ~ByteArrayInputStream();

        /**
         * Sets the internal buffer.  The input stream will wrap around
         * this buffer and will perform all read operations on it.  The
         * position will be reinitialized to the beginning of the specified
         * buffer.  This class will not own the given buffer - it is the
         * caller's responsibility to free the memory of the given buffer
         * as appropriate.
         *
         * @param buffer
         *      The buffer to be used.
         */
        virtual void setByteArray( const std::vector<unsigned char>& buffer );

        /**
         * Sets the data that this reader uses, replaces any existing
         * data and resets to beginning of the buffer.
         *
         * @param buffer
         *      The initial byte array to use to read from.
         * @param bufferSize
         *      The size of the buffer.
         *
         * @throws NullPointerException if the buffer is Null.
         * @throws IllegalArguementException if the bufferSize is negative.
         */
        virtual void setByteArray( const unsigned char* buffer, int bufferSize );

        /**
         * Sets the data that this reader uses, replaces any existing
         * data and resets to beginning of the buffer.
         *
         * @param buffer
         *      The initial byte array to use to read from.
         * @param bufferSize
         *      The size of the buffer.
         * @param offset
         *      The offset into the buffer to begin reading from.
         * @param length
         *      The number of bytes to read past the offset.
         *
         * @throws NullPointerException if the buffer is Null.
         * @throws IllegalArguementException if the bufferSize is negative.
         */
        virtual void setByteArray( const unsigned char* buffer, int bufferSize, int offset, int length );

        /**
         * {@inheritDoc}
         */
        virtual int available() const;

        /**
         * {@inheritDoc}
         */
        virtual long long skip( long long num );

        /**
         * {@inheritDoc}
         */
        virtual void mark( int readLimit );

        /**
         * {@inheritDoc}
         */
        virtual void reset();

        /**
         * {@inheritDoc}
         */
        virtual bool markSupported() const{ return true; }

    protected:

        virtual int doReadByte();

        virtual int doReadArrayBounded( unsigned char* buffer, int size, int offset, int length );

    };

}}

#endif /*_DECAF_IO_BYTEARRAYINPUTSTREAM_H_*/
