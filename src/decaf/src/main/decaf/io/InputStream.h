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

#ifndef _DECAF_IO_INPUTSTREAM_H_
#define _DECAF_IO_INPUTSTREAM_H_

#include <decaf/io/IOException.h>
#include <decaf/io/Closeable.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

namespace decaf{
namespace io{

    /**
     * Base interface for an input stream.
     */
    class InputStream : public Closeable,
                        public util::concurrent::Synchronizable
    {
    public:

        virtual ~InputStream(){}

        /**
         * Indcates the number of bytes avaialable.
         * @return the number of bytes available on this input stream.
         * @throws IOException if an error occurs.
         */
        virtual std::size_t available() const throw ( IOException ) = 0;

        /**
         * Reads a single byte from the buffer.  Blocks until
         * data is available.
         * @return The next byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char read() throw ( IOException ) = 0;

        /**
         * Reads an array of bytes from the buffer.  Blocks until
         * the requested number of bytes are available.
         * @param buffer (out) the target buffer.
         * @param bufferSize the size of the output buffer.
         * @return The number of bytes read or -1 if EOF is detected
         * @throws IOException thrown if an error occurs.
         */
        virtual std::size_t read( unsigned char* buffer, std::size_t bufferSize )
            throw ( IOException ) = 0;

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
        virtual std::size_t skip( std::size_t num ) throw ( io::IOException, lang::exceptions::UnsupportedOperationException ) = 0;

    };

}}

#endif /*_DECAF_IO_INPUTSTREAM_H_*/
