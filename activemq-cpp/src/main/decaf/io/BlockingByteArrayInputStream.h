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

#ifndef _DECAF_IO_BLOCKINGBYTEARRAYINPUTSTREAM_H_
#define _DECAF_IO_BLOCKINGBYTEARRAYINPUTSTREAM_H_

#include <decaf/io/InputStream.h>
#include <vector>

namespace decaf{
namespace io{

    /**
     * This is a blocking version of a byte buffer stream.  Read operations
     * block until the requested data becomes available in the internal
     * buffer via a call to setByteArray.
     */
    class DECAF_API BlockingByteArrayInputStream : public InputStream {
    private:

        /**
         * Default buffer to use, if none provided.
         */
        std::vector<unsigned char> buffer;

        /**
         * iterator to current position in buffer.
         */
        std::vector<unsigned char>::const_iterator pos;

        /**
         * Indicates that this stream is in the process of shutting
         * down.
         */
        bool closing;

    private:

        BlockingByteArrayInputStream( const BlockingByteArrayInputStream& );
        BlockingByteArrayInputStream& operator= ( const BlockingByteArrayInputStream& );

    public:

        /**
         * Default Constructor - uses a default internal buffer
         */
        BlockingByteArrayInputStream();

        /**
         * Constructor that initializes the internal buffer.
         * @see setByteArray.
         */
        BlockingByteArrayInputStream( const unsigned char* buffer, int bufferSize );

        virtual ~BlockingByteArrayInputStream();

        /**
         * {@inheritDoc}
         */
        virtual void setByteArray( const unsigned char* buffer, int bufferSize );

        /**
         * {@inheritDoc}
         */
        virtual int available() const;

        /**
         * {@inheritDoc}
         */
        virtual void close();

        /**
         * {@inheritDoc}
         */
        virtual long long skip( long long num );

    protected:

        virtual int doReadByte();

        virtual int doReadArrayBounded( unsigned char* buffer, int size, int offset, int length );

    };

}}

#endif /*_DECAF_IO_BLOCKINGBYTEARRAYINPUTSTREAM_H_*/
