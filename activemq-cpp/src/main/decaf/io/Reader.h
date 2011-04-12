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
#ifndef _DECAF_IO_READER_H
#define _DECAF_IO_READER_H

#include <string>
#include <decaf/lang/Readable.h>
#include <decaf/io/Closeable.h>
#include <decaf/io/IOException.h>
#include <decaf/io/InputStream.h>

#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

namespace decaf{
namespace io{

    /*
     * Abstract class for reading character streams. The only methods that a subclass
     * must implement are read( char*, size_t, size_t ) and close(). Most subclasses,
     * however, will override some of the methods defined here in order to provide
     * higher efficiency, additional functionality, or both.
     *
     * @since 1.0
     */
    class DECAF_API Reader : public virtual decaf::io::Closeable,
                             public virtual decaf::lang::Readable {
    private:

        Reader( const Reader& );
        Reader& operator= ( const Reader& );

    protected:

        Reader();

    public:

        virtual ~Reader();

        /**
         * Marks the present position in the stream. Subsequent calls to reset() will attempt
         * to reposition the stream to this point. Not all character-input streams support
         * the mark() operation.
         *
         * @param readAheadLimit
         *      Limit on the number of characters that may be read while still preserving the
         *      mark. After reading this many characters, attempting to reset the stream may fail.
         *
         * @throw IOException if an I/O error occurs, or the stream does not support mark.
         */
        virtual void mark( int readAheadLimit );

        /**
         * Tells whether this stream supports the mark() operation. The default implementation
         * always returns false. Subclasses should override this method.
         *
         * @return true if and only if this stream supports the mark operation.
         */
        virtual bool markSupported() const {
            return false;
        }

        /**
         * Tells whether this stream is ready to be read.
         *
         * @return True if the next read() is guaranteed not to block for input, false otherwise.
         *         Note that returning false does not guarantee that the next read will block.
         *
         * @throw IOException if an I/O error occurs.
         */
        virtual bool ready() const;

        /**
         * Resets the stream. If the stream has been marked, then attempt to reposition it at the
         * mark. If the stream has not been marked, then attempt to reset it in some way appropriate
         * to the particular stream, for example by repositioning it to its starting point. Not all
         * character-input streams support the reset() operation, and some support reset() without
         * supporting mark().
         *
         * @throw IOException if an I/O error occurs.
         */
        virtual void reset();

        /**
         * Skips characters. This method will block until some characters are available, an I/O error
         * occurs, or the end of the stream is reached.
         *
         * @param count
         *      The number of character to skip.
         *
         * @return the number of Character actually skipped.
         *
         * @throw IOException if an I/O error occurs.
         */
        virtual long long skip( long long count );

        /**
         * Reads characters into an array. This method will block until some input is available,
         * an I/O error occurs, or the end of the stream is reached.
         *
         * @param buffer
         *       The buffer to read characters into.
         *
         * @return The number of characters read, or -1 if the end of the stream has been reached
         *
         * @throws IOException thrown if an I/O error occurs.
         */
        virtual int read( std::vector<char>& buffer );

        /**
         * Reads characters into an array, the method will attempt to read as much data as the size
         * of the array. This method will block until some input is available, an I/O error occurs,
         * or the end of the stream is reached.
         *
         * @param buffer
         *      The target char buffer.
         * @param size
         *      The size in bytes of the target buffer.
         *
         * @return The number of bytes read or -1 if the end of stream is reached.
         *
         * @throws IOException thrown if an I/O error occurs.
         * @throws NullPointerException if buffer is NULL.
         */
        virtual int read( char* buffer, int size );

        /**
         * Reads characters into a portion of an array. This method will block until some input
         * is available, an I/O error occurs, or the end of the stream is reached.
         *
         * @param buffer
         *      The target char buffer.
         * @param size
         *      The size in bytes of the target buffer.
         * @param offset
         *      The position in the buffer to start filling.
         * @param length
         *      The maximum number of bytes to read.
         *
         * @return The number of bytes read or -1 if the end of stream is reached.
         *
         * @throws IOException thrown if an I/O error occurs.
         * @throws NullPointerException if buffer is NULL.
         * @throws IndexOutOfBoundsException if the offset + length is greater than the array size.
         */
        virtual int read( char* buffer, int size, int offset, int length );

        /**
         * Reads a single character. This method will block until a character is available,
         * an I/O error occurs, or the end of the stream is reached.
         *
         * Subclasses that intend to support efficient single-character input should override
         * this method.
         *
         * @return The character read, as an integer in the range 0 to 65535 (0x00-0xffff),
         *         or -1 if the end of the stream has been reached.
         *
         * @throws IOException thrown if an I/O error occurs.
         */
        virtual int read();

        virtual int read( decaf::nio::CharBuffer* charBuffer );

    protected:

        /**
         * Override this method to customize the functionality of the method
         * read( unsigned char* buffer, int size, int offset, int length ).
         *
         * All subclasses must override this method to provide the basic Reader
         * functionality.
         */
        virtual int doReadArrayBounded( char* buffer, int size, int offset, int length ) = 0;

    protected:

        /**
         * Override this method to customize the functionality of the method
         * read( std::vector<char>& buffer ).
         */
        virtual int doReadVector( std::vector<char>& buffer );

        /**
         * Override this method to customize the functionality of the method
         * read( char* buffer, std::size_t length ).
         */
        virtual int doReadArray( char* buffer, int length );

        /**
         * Override this method to customize the functionality of the method
         * read().
         */
        virtual int doReadChar();

        /**
         * Override this method to customize the functionality of the method
         * read( CharBuffer* charBuffer ).
         */
        virtual int doReadCharBuffer( decaf::nio::CharBuffer* charBuffer );

    };

}}

#endif /*_DECAF_IO_READER_H*/
