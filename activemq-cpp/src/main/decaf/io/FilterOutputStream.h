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

#ifndef _DECAF_IO_FILTEROUTPUTSTREAM_H_
#define _DECAF_IO_FILTEROUTPUTSTREAM_H_

#include <decaf/io/OutputStream.h>
#include <decaf/io/IOException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

namespace decaf{
namespace io{

    /**
     * This class is the superclass of all classes that filter output
     * streams. These streams sit on top of an already existing output
     * stream (the underlying output stream) which it uses as its basic
     * sink of data, but possibly transforming the data along the way or
     * providing additional functionality.
     *
     * The class FilterOutputStream itself simply overrides all methods of
     * OutputStream with versions that pass all requests to the underlying
     * output stream. Subclasses of FilterOutputStream may further override
     * some of these methods as well as provide additional methods and
     * fields.
     *
     * Due to the lack of garbage collection in C++ a design decision was
     * made to add a boolean parameter to the constructor indicating if the
     * wrapped <code>InputStream</code> is owned by this object.  That way
     * creation of the underlying stream can occur in a Java like way. Ex:
     *
     *  DataOutputStream os = new DataOutputStream( new OutputStream(), true )
     */
    class DECAF_API FilterOutputStream : public OutputStream {
    protected:

        // The output Stream to wrap
        OutputStream* outputStream;

        // Indicates if we own the wrapped stream
        bool own;

        // Indicates that this stream was closed
        volatile bool closed;

    private:

        FilterOutputStream( const FilterOutputStream& );
        FilterOutputStream& operator= ( const FilterOutputStream& );

    public:

        /**
         * Constructor, creates a wrapped output stream
         * @param outputStream the OutputStream to wrap
         * @param own If true, this object will control the lifetime of the
         * output stream that it encapsulates.
         */
        FilterOutputStream( OutputStream* outputStream, bool own = false );

        virtual ~FilterOutputStream();

        /**
         * {@inheritDoc}
         *
         * The flush method of FilterOutputStream calls the flush method of its
         * underlying output stream.
         */
        virtual void flush();

        /**
         * {@inheritDoc}
         *
         * The close method of FilterOutputStream calls its flush method, and then
         * calls the close method of its underlying output stream.
         */
        virtual void close();

        /**
         * {@inheritDoc}
         *
         * The toString method of FilterOutputStream calls the toString method of its
         * underlying output stream.
         */
        virtual std::string toString() const;

    protected:

        virtual void doWriteByte( unsigned char value );

        virtual void doWriteArray( const unsigned char* buffer, int size );

        virtual void doWriteArrayBounded( const unsigned char* buffer, int size, int offset, int length );

    protected:

        /**
         * @returns true if this stream has been closed.
         */
        virtual bool isClosed() const;

    };

}}

#endif /*_DECAF_IO_FILTEROUTPUTSTREAM_H_*/
