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

#ifndef _DECAF_IO_BUFFEREDINPUTSTREAM_H_
#define _DECAF_IO_BUFFEREDINPUTSTREAM_H_

#include <decaf/io/FilterInputStream.h>
#include <assert.h>

namespace decaf{
namespace io{

    /**
     * A wrapper around another input stream that performs
     * a buffered read, where it reads more data than it needs
     * in order to reduce the number of io operations on the
     * input stream.
     */
    class BufferedInputStream : public FilterInputStream
    {
    private:

        /**
         * The internal buffer.
         */
        unsigned char* buffer;

        /**
         * The buffer size.
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
         * Constructor
         * @param stream The target input stream.
         * @param own indicates if we own the stream object, defaults to false
         */
        BufferedInputStream( InputStream* stream, bool own = false );

        /**
         * Constructor
         * @param stream the target input stream
         * @param bufferSize the size for the internal buffer.
         * @param own indicates if we own the stream object, defaults to false.
         */
        BufferedInputStream( InputStream* stream,
                             std::size_t bufferSize,
                             bool own = false);

        virtual ~BufferedInputStream();

        /**
         * Indcates the number of bytes avaialable.
         * @return the sum of the amount of data avalable
         * in the buffer and the data available on the target
         * input stream.
         */
        virtual std::size_t available() const throw ( IOException ) {
            return ( tail - head ) + inputStream->available();
        }

        /**
         * Reads a single byte from the buffer.  Blocks until
         * the data is available.
         * @return The next byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char read() throw ( IOException );

        /**
         * Reads an array of bytes from the buffer.  Blocks
         * until the requested number of bytes are available.
         * @param buffer (out) the target buffer.
         * @param bufferSize the size of the output buffer.
         * @return The number of bytes read.
         * @throws IOException thrown if an error occurs.
         */
        virtual std::size_t read( unsigned char* buffer, std::size_t bufferSize )
            throw ( IOException );

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
        virtual std::size_t skip( std::size_t num )
            throw ( io::IOException, exceptions::UnsupportedOperationException );

    private:

        /**
         * Initializes the internal structures.
         * @param size of buffer to allocate
         */
        void init( std::size_t bufferSize );

        /**
         * Populates the buffer with as much data as possible
         * from the target input stream.
         * @throws CMSException
         */
        void bufferData() throw ( IOException );

        /**
         * Returns the number of bytes that are currently unused
         * in the buffer.
         */
        std::size_t getUnusedBytes() const{
            return bufferSize - tail;
        }

        /**
         * Returns the current tail position of the buffer.
         */
        unsigned char* getTail(){
            return buffer + tail;
        }

        /**
         * Initializes the head and tail indicies to the beginning
         * of the buffer.
         */
        void clear(){
            head = tail = 0;
        }

        /**
         * Inidicates whether or not the buffer is empty.
         */
        bool isEmpty() const{
            return head == tail;
        }

        /**
         * Clears the buffer if there is no data remaining.
         */
        void normalizeBuffer(){
            if( isEmpty() ){
                clear();
            }
        }

    };

}}

#endif /*_DECAF_IO_BUFFEREDINPUTSTREAM_H_*/
