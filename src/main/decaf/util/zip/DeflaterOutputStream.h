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

#ifndef _DECAF_UTIL_ZIP_DEFLATEROUTPUTSTREAM_H_
#define _DECAF_UTIL_ZIP_DEFLATEROUTPUTSTREAM_H_

#include <decaf/util/Config.h>

#include <decaf/io/FilterOutputStream.h>
#include <decaf/io/IOException.h>
#include <decaf/util/zip/Deflater.h>

#include <vector>

namespace decaf {
namespace util {
namespace zip {

    /**
     * Provides a FilterOutputStream instance that compresses the data before writing it
     * to the wrapped OutputStream.
     *
     * @since 1.0
     */
    class DECAF_API DeflaterOutputStream : public decaf::io::FilterOutputStream {
    protected:

        /**
         * The Deflater for this stream.
         */
        Deflater* deflater;

        /**
         * The Buffer to use for
         */
        std::vector<unsigned char> buf;

        bool ownDeflater;
        bool isDone;

        static const std::size_t DEFAULT_BUFFER_SIZE;

    public:

        /**
         * Creates a new DeflateOutputStream with a Default Deflater and buffer size.
         *
         * @param outputStream
         *      The OutputStream instance to wrap.
         * @param own
         *      Should this filter take ownership of the OutputStream pointer (default is false).
         */
        DeflaterOutputStream( decaf::io::OutputStream* outputStream, bool own = false );

        /**
         * Creates a new DeflateOutputStream with a user supplied Deflater and a default buffer size.
         * When the user supplied a Deflater instance the DeflaterOutpotStream does not take ownership
         * of the Deflater pointer, the caller is still responsible for deleting the Deflater.
         *
         * @param outputStream
         *      The OutputStream instance to wrap.
         * @param deflater
         *      The user supplied Deflater to use for compression. (
         * @param own
         *      Should this filter take ownership of the OutputStream pointer (default is false).
         *
         * @throws NullPointerException if the Deflater given is NULL.
         */
        DeflaterOutputStream( decaf::io::OutputStream* outputStream, Deflater* deflater, bool own = false );

        /**
         * Creates a new DeflateOutputStream with a user supplied Deflater and specified buffer size.
         * When the user supplied a Deflater instance the DeflaterOutpotStream does not take ownership
         * of the Deflater pointer, the caller is still responsible for deleting the Deflater.
         *
         * @param outputStream
         *      The OutputStream instance to wrap.
         * @param deflater
         *      The user supplied Deflater to use for compression.
         * @param bufferSize
         *      The size of the input buffer.
         * @param own
         *      Should this filter take ownership of the OutputStream pointer (default is false).
         *
         * @throws NullPointerException if the Deflater given is NULL.
         * @throws IllegalArgumentException if bufferSize is 0.
         */
        DeflaterOutputStream( decaf::io::OutputStream* outputStream, Deflater* deflater,
                              int bufferSize, bool own = false );

        virtual ~DeflaterOutputStream();

        /**
         * Finishes writing any remaining data to the wrapped OutputStream but does not close
         * it upon completion.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void finish() throw ( decaf::io::IOException );

        /**
         * {@inheritDoc}
         *
         * Finishes writing any remaining data to the OutputStream then closes the stream.
         */
        virtual void close() throw ( decaf::io::IOException );

    protected:

        virtual void doWriteByte( unsigned char value ) throw ( decaf::io::IOException );

        virtual void doWriteArrayBounded( const unsigned char* buffer, int size,
                                          int offset, int length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IndexOutOfBoundsException );

    protected:

        /**
         * Writes a buffers worth of compressed data to the wrapped OutputStream.
         */
        virtual void deflate() throw( decaf::io::IOException );

    };

}}}

#endif /* _DECAF_UTIL_ZIP_DEFLATEROUTPUTSTREAM_H_ */
