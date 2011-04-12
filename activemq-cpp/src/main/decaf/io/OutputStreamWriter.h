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

#ifndef _DECAF_IO_OUTPUTSTREAMWRITER_H_
#define _DECAF_IO_OUTPUTSTREAMWRITER_H_

#include <decaf/util/Config.h>
#include <decaf/io/Writer.h>

namespace decaf {
namespace io {

    class OutputStream;

    /**
     * A class for turning a character stream into a byte stream.
     *
     * @see InputStreamReader
     *
     * @since 1.0
     */
    class DECAF_API OutputStreamWriter : public Writer {
    private:

        // Pointer to the Stream this Writer writes its data to.
        OutputStream* stream;

        // Does this class own the output stream.
        bool own;

        // Closed flag
        volatile bool closed;

    private:

        OutputStreamWriter( const OutputStreamWriter& );
        OutputStreamWriter& operator= ( const OutputStreamWriter& );

    public:

        /**
         * Creates a new OutputStreamWriter.
         *
         * @param stream
         *      The OutputStream to wrap. (cannot be NULL).
         * @param own
         *      Indicates whether this instance own the given OutputStream.
         *      If true then the OutputStream is destroyed when this class is.
         *
         * @throws NullPointerException if the stream is NULL.
         */
        OutputStreamWriter( OutputStream* stream, bool own = false );

        virtual ~OutputStreamWriter();

        virtual void close();

        virtual void flush();

    protected:

        virtual void doWriteArrayBounded( const char* buffer, int size, int offset, int length );

        // Used to check state and throw error when closed.
        virtual void checkClosed() const;

    };

}}

#endif /* _DECAF_IO_OUTPUTSTREAMWRITER_H_ */
