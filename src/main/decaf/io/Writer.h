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
#ifndef _DECAF_IO_WRITER_H
#define _DECAF_IO_WRITER_H

#include <string>
#include <vector>
#include <decaf/io/IOException.h>
#include <decaf/io/Closeable.h>
#include <decaf/io/Flushable.h>
#include <decaf/lang/Appendable.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

namespace decaf{
namespace io{

    /*
     * Abstract class for writing to character streams. The only methods that a
     * subclass must implement are write( char*, size_t, size_t ), flush(), and
     * close().  Most subclasses, however, will override some of the methods
     * defined here in order to provide higher efficiency, additional functionality,
     * or both.
     *
     * @since 1.0
     */
    class DECAF_API Writer : public decaf::io::Closeable,
                             public decaf::io::Flushable,
                             public decaf::lang::Appendable {
    public:

        virtual ~Writer(){};

        /**
         * Writes an single byte char value.
         *
         * @param v
         *      The value to be written.
         *
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( char v ) throw( decaf::io::IOException );

        /**
         * Writes an array of Chars
         *
         * @param buffer
         *      The array to be written.
         *
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const std::vector<char>& buffer )
            throw( decaf::io::IOException );

        /**
         * Writes a byte array to the output stream.
         *
         * @param buffer
         *      The byte array to write (cannot be NULL).
         * @param offset
         *      The position in the array to start writing from.
         * @param length
         *      The number of bytes in the array to write.
         *
         * @throws IOException if an I/O error occurs.
         * @throws NullPointerException if buffer is NULL.
         */
        virtual void write( const char* buffer, std::size_t offset, std::size_t length )
            throw( decaf::io::IOException, decaf::lang::exceptions::NullPointerException ) = 0;

        /**
         * Writes a string
         *
         * @param str
         *      The string to be written.
         *
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const std::string& str ) throw( decaf::io::IOException );

        /**
         * Writes a string
         *
         * @param str
         *      The string to be written.
         * @param offset
         *      The position in the array to start writing from.
         * @param length
         *      The number of bytes in the array to write.
         *
         * @throws IOException thrown if an error occurs.
         * @throws IndexOutOfBoundsException if offset+length is greater than the string length.
         */
        virtual void write( const std::string& str, std::size_t offset, std::size_t length )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual decaf::lang::Appendable& append( char value ) throw( decaf::io::IOException );

        virtual decaf::lang::Appendable& append( const decaf::lang::CharSequence* csq )
            throw ( decaf::io::IOException );

        virtual decaf::lang::Appendable& append( const decaf::lang::CharSequence* csq,
                                                 std::size_t start, std::size_t end )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::IndexOutOfBoundsException );

    };

}}

#endif /*_DECAF_IO_WRITER_H*/
