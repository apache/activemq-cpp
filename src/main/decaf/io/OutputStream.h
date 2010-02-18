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

#ifndef _DECAF_IO_OUTPUTSTREAM_H
#define _DECAF_IO_OUTPUTSTREAM_H

#include <decaf/io/Closeable.h>
#include <decaf/io/Flushable.h>
#include <decaf/io/IOException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/Config.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

namespace decaf{
namespace io{

    /**
     * Base interface for any class that wants to represent an output stream of bytes.
     *
     * @since 1.0
     */
    class DECAF_API OutputStream : public Closeable,
                                   public Flushable,
                                   public util::concurrent::Synchronizable
    {
    public:

        virtual ~OutputStream() {}

        /**
         * Writes a single byte to the output stream.
         *
         * @param c
         *      The byte to write to the sink.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void write( unsigned char c ) throw ( IOException ) = 0;

        /**
         * Writes an array of bytes to the output stream.  The entire contents of
         * the given vector are written to the output stream.
         *
         * @param buffer
         *      The vector of bytes to write.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void write( const std::vector<unsigned char>& buffer )
            throw ( IOException ) = 0;

        /**
         * Writes an array of bytes to the output stream in order starting at buffer[offset]
         * and proceeding until the number of bytes specified by the length argument are
         * written or an error occurs.
         *
         * @param buffer
         *      The array of bytes to write.
         * @param size
         *      The size of the buffer array passed.
         * @param offset
         *      The position to start writing in buffer.
         * @param length
         *      The number of bytes from the buffer to be written.
         *
         * @throws IOException if an I/O error occurs.
         * @throws NullPointerException thrown if buffer is Null.
         * @throws IndexOutOfBoundsException if the offset + length > size.
         */
        virtual void write( const unsigned char* buffer, std::size_t size,
                            std::size_t offset, std::size_t length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IndexOutOfBoundsException ) = 0;

    };

}}

#endif /*_DECAF_IO_OUTPUTSTREAM_H*/
