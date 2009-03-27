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

#ifndef _DECAF_IO_BUFFEREDOUTPUTSTREAM_H_
#define _DECAF_IO_BUFFEREDOUTPUTSTREAM_H_

#include <decaf/io/FilterOutputStream.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace decaf{
namespace io{

    /**
     * Wrapper around another output stream that buffers
     * output before writing to the target output stream.
     */
    class DECAF_API BufferedOutputStream : public FilterOutputStream
    {
    private:

        /**
         * The internal buffer.
         */
        unsigned char* buffer;

        /**
         * The size of the internal buffer.
         */
        std::size_t bufferSize;

        /**
         * The current head of the buffer.
         */
        std::size_t head;

        /**
         * The current tail of the buffer.
         */
        std::size_t tail;

    public:

        /**
         * Constructor.
         * @param stream the target output stream.
         */
        BufferedOutputStream( OutputStream* stream, bool own = false );

        /**
         * Constructor
         * @param stream the target output stream.
         * @param bufSize the size for the internal buffer.
         */
        BufferedOutputStream( OutputStream* stream,
                              std::size_t bufSize,
                              bool own = false )
            throw ( lang::exceptions::IllegalArgumentException );

        virtual ~BufferedOutputStream();

        /**
         * Writes a single byte to the output stream.
         * @param c the byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( unsigned char c ) throw ( IOException );

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The bytes to write.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const std::vector<unsigned char>& buffer )
            throw ( IOException );

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
            throw ( IOException, lang::exceptions::NullPointerException );

        /**
         * Invokes flush on the target output stream.
         * @throws IOException thrown if an error occurs.
         */
        virtual void flush() throw ( IOException );

        /**
         * Invokes close on the target output stream.
         * @throws CMSException thrown if an error occurs.
         */
        void close() throw( lang::Exception );

   private:

        /**
         * Initializes the internal structures.
         */
        void init( std::size_t bufSize );

        /**
         * Writes the contents of the buffer to the output stream.
         */
        void emptyBuffer() throw ( IOException );

   };

}}

#endif /*_DECAF_IO_BUFFEREDOUTPUTSTREAM_H_*/
