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
         * The currently marked position or begin() of activeBuffer.
         */
        std::vector<unsigned char>::const_iterator markpos;

    public:

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
         *
         * @throws NullPointerException if the buffer is Null.
         * @throws IllegalArguementException if the bufferSize is negative.
         */
        ByteArrayInputStream( const unsigned char* buffer, long long bufferSize )
            throw( decaf::lang::exceptions::NullPointerException,
                   decaf::lang::exceptions::IllegalArgumentException );

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
        virtual void setBuffer( const std::vector<unsigned char>& buffer );

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
        virtual void setByteArray( const unsigned char* buffer, long long bufferSize )
            throw( decaf::lang::exceptions::NullPointerException,
                   decaf::lang::exceptions::IllegalArgumentException );

        /**
         * {@inheritDoc}
         */
        virtual int available() const throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual long long skip( long long num )
            throw ( io::IOException, lang::exceptions::UnsupportedOperationException );

        /**
         * {@inheritDoc}
         */
        virtual void mark( int readLimit );

        /**
         * {@inheritDoc}
         */
        virtual void reset() throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual bool markSupported() const{ return true; }

    protected:

        virtual int doReadByte() throw ( IOException );

        virtual int doReadArrayBounded( unsigned char* buffer, int size, int offset, int length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::IndexOutOfBoundsException,
                    decaf::lang::exceptions::NullPointerException );

    };

}}

#endif /*_DECAF_IO_BYTEARRAYINPUTSTREAM_H_*/
