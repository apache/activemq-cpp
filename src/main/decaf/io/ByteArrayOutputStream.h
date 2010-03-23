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

#ifndef _DECAF_IO_BYTEARRAYOUTPUTSTREAM_H_
#define _DECAF_IO_BYTEARRAYOUTPUTSTREAM_H_

#include <decaf/io/OutputStream.h>
#include <vector>

namespace decaf{
namespace io{

    class DECAF_API ByteArrayOutputStream : public OutputStream {
    private:

        /**
         * Default buffer to use, if none provided.
         */
        std::vector<unsigned char> defaultBuffer;

        /**
         * Reference to the buffer being used by this stream.
         */
        std::vector<unsigned char>* activeBuffer;

    private:

        ByteArrayOutputStream( const ByteArrayOutputStream& );
        ByteArrayOutputStream& operator= ( const ByteArrayOutputStream& );

    public:

        /**
         * Default Constructor - uses a default internal buffer
         */
        ByteArrayOutputStream();

        /**
         * Uses the given buffer as the target.  Calls setBuffer.
         * @param buffer the target buffer.
         */
        ByteArrayOutputStream( std::vector<unsigned char>& buffer );

        virtual ~ByteArrayOutputStream();

        /**
         * Sets the internal buffer.  This input stream will wrap around
         * the given buffer and all writes will be performed directly on
         * the buffer.  This object does not retain control of the buffer's
         * lifetime however - this is the job of the caller.
         * @param buffer The target buffer.
         */
        virtual void setBuffer( std::vector<unsigned char>& buffer );

        /**
         * Get a snapshot of the data
         * @return pointer to the data
         */
        virtual const unsigned char* toByteArray() const;

        /**
         * Get a snapshot of the data
         * @return reference to the underlying data as a const std::vector<unsigned char>&
         */
        virtual const std::vector<unsigned char>& toByteArrayRef() const;

        /**
         * Get the Size of the Internal Buffer
         * @return size of the internal buffer
         */
        virtual long long size() const;

        /**
         * Clear current Stream contents
         * @throws IOException
         */
        virtual void reset() throw ( IOException );

        /**
         * Converts the bytes in the buffer into a standard C++ string
         * @returns a string containing the bytes in the buffer
         */
        virtual std::string toString() const;

        /**
         * Writes the complete contents of this byte array output stream to the
         * specified output stream argument, as if by calling the output
         * stream's write method using out.write( buf, 0, count ).
         */
        void writeTo( OutputStream* out ) const
            throw ( decaf::io::IOException, decaf::lang::exceptions::NullPointerException );

    protected:

        virtual void doWriteByte( unsigned char value )
           throw ( decaf::io::IOException );

        virtual void doWriteArrayBounded( const unsigned char* buffer, int size, int offset, int length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IndexOutOfBoundsException );

    };

}}

#endif /*_DECAF_IO_BYTEARRAYOUTPUTSTREAM_H_*/
