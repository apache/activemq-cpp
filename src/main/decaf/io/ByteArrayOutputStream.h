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

#ifndef _DECAF_IO_BYTEARRAYOUTPUTSTREAM_H_
#define _DECAF_IO_BYTEARRAYOUTPUTSTREAM_H_

#include <decaf/util/Config.h>

#include <decaf/io/OutputStream.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

#include <utility>

namespace decaf{
namespace io{

    class DECAF_API ByteArrayOutputStream : public OutputStream {
    private:

        /**
         * The internal buffer used to hold written bytes.
         */
        unsigned char* buffer;

        /**
         * Current Size of the buffer.
         */
        int bufferSize;

        /**
         * The number of bytes currently held in the buffer.
         */
        int count;

    private:

        ByteArrayOutputStream( const ByteArrayOutputStream& );
        ByteArrayOutputStream& operator= ( const ByteArrayOutputStream& );

    public:

        /**
         * Default Constructor - uses a default internal buffer of 32 bytes, the size
         * increases as the need for more room arises.
         */
        ByteArrayOutputStream();

        /**
         * Creates a ByteArrayOutputStream with an internal buffer allocated with the
         * given size.
         *
         * @param bufferSize
         *      The size to use for the internal buffer.
         *
         * @throw IllegalArgumentException if the size is less than or equal to zero.
         */
        ByteArrayOutputStream( int bufferSize );

        virtual ~ByteArrayOutputStream();

        /**
         * Creates a newly allocated byte array. Its size is the current size of this output
         * stream and the valid contents of the buffer have been copied into it.  The newly
         * allocated array and its size are returned inside an STL pair structure, the caller
         * is responsible for freeing the returned array.
         *
         * @return an STL pair containing the copied array and its size.
         */
        std::pair<unsigned char*, int> toByteArray() const;

        /**
         * Gets the current count of bytes written into this ByteArrayOutputStream.
         *
         * @return the number of valid bytes contained in the ByteArrayOutputStream.
         */
        long long size() const;

        /**
         * Clear current Stream contents
         * @throws IOException
         */
        virtual void reset();

        /**
         * Converts the bytes in the buffer into a standard C++ string
         * @returns a string containing the bytes in the buffer
         */
        virtual std::string toString() const;

        /**
         * Writes the complete contents of this byte array output stream to the
         * specified output stream argument, as if by calling the output
         * stream's write method using out.write( buf, 0, count ).
         */
        void writeTo( OutputStream* out ) const;

    protected:

        virtual void doWriteByte( unsigned char value );

        virtual void doWriteArrayBounded( const unsigned char* buffer, int size, int offset, int length );

    private:

        // Expands the buffer if there's not enough room for the needed length.
        void checkExpandSize( int needed );

    };

}}

#endif /*_DECAF_IO_BYTEARRAYOUTPUTSTREAM_H_*/
