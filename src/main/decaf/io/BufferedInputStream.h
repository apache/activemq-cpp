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

#include <decaf/util/Config.h>
#include <decaf/io/FilterInputStream.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace decaf{
namespace io{

    /**
     * A wrapper around another input stream that performs
     * a buffered read, where it reads more data than it needs
     * in order to reduce the number of io operations on the
     * input stream.
     */
    class DECAF_API BufferedInputStream : public FilterInputStream {
    private:

        int pos;
        int count;
        int markLimit;
        int markPos;
        int bufferSize;
        unsigned char* buff;

        // Used to swap the active buffer with so it can be safely deleted later.
        unsigned char* deleteBuff;

    private:

        BufferedInputStream( const BufferedInputStream& );
        BufferedInputStream& operator= ( const BufferedInputStream& );

    public:

        /**
         * Constructor
         *
         * @param stream
         *      The target input stream to buffer.
         * @param own
         *      Indicates if we own the stream object, defaults to false.
         */
        BufferedInputStream( InputStream* stream, bool own = false );

        /**
         * Constructor
         *
         * @param stream
         *      The target input stream to buffer.
         * @param bufferSize
         *      The size in bytes to allocate for the internal buffer.
         * @param own
         *      Indicates if we own the stream object, defaults to false.
         *
         * @throws IllegalArgumentException is the size is zero or negative.
         */
        BufferedInputStream( InputStream* stream, int bufferSize, bool own = false )
            throw ( lang::exceptions::IllegalArgumentException );

        virtual ~BufferedInputStream();

        /**
         * {@inheritDoc}
         */
        virtual int available() const throw ( decaf::io::IOException );

        /**
         * {@inheritDoc}
         */
        virtual void close() throw( decaf::io::IOException );

        /**
         * {@inheritDoc}
         */
        virtual long long skip( long long num )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * {@inheritDoc}
         */
        virtual void mark( int readLimit );

        /**
         * {@inheritDoc}
         */
        virtual void reset() throw ( decaf::io::IOException );

        /**
         * {@inheritDoc}
         */
        virtual bool markSupported() const{ return true; }

    protected:

        virtual int doReadByte() throw ( decaf::io::IOException );

        virtual int doReadArrayBounded( unsigned char* buffer, int size, int offset, int length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::IndexOutOfBoundsException,
                    decaf::lang::exceptions::NullPointerException );

    private:

        int bufferData( InputStream* stream, unsigned char*& buffer )
            throw ( decaf::io::IOException );

    };

}}

#endif /*_DECAF_IO_BUFFEREDINPUTSTREAM_H_*/
