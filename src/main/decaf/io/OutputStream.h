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
#include <decaf/io/IOException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/NullPointerException.h>

namespace decaf{
namespace io{

    /**
     * Base interface for an output stream.
     */
    class DECAF_API OutputStream : public Closeable,
                                   public util::concurrent::Synchronizable
    {
    public:

        virtual ~OutputStream(){}

        /**
         * Writes a single byte to the output stream.
         * @param c the byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( unsigned char c ) throw ( IOException ) = 0;

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The bytes to write.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const std::vector<unsigned char>& buffer )
            throw ( IOException ) = 0;

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The array of bytes to write.
         * @param offset, the position to start writing in buffer.
         * @param len The number of bytes from the buffer to be written.
         * @throws IOException thrown if an error occurs.
         * @throws NullPointerException thrown if buffer is Null.
         */
        virtual void write( const unsigned char* buffer,
                            std::size_t offset,
                            std::size_t len )
            throw ( IOException, lang::exceptions::NullPointerException ) = 0;

        /**
         * Flushes any pending writes in this output stream.
         * @throws IOException
         */
        virtual void flush() throw ( IOException ) = 0;

    };

}}

#endif /*_DECAF_IO_OUTPUTSTREAM_H*/
