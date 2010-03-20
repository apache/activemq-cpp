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

#ifndef _DECAF_IO_DATAOUTPUTSTREAM_H_
#define _DECAF_IO_DATAOUTPUTSTREAM_H_

#include <decaf/io/FilterOutputStream.h>
#include <decaf/io/IOException.h>
#include <decaf/io/UTFDataFormatException.h>
#include <string>

namespace decaf{
namespace io{

    /**
     * A data output stream lets an application write primitive Java data
     * types to an output stream in a portable way. An application can then
     * use a data input stream to read the data back in.
     */
    class DECAF_API DataOutputStream : public FilterOutputStream {
    protected:

        // The number of bytes written to the data output stream so far.
        long long written;

        // Buffer used for storing byte values to write to the stream
        unsigned char buffer[8];

    public:

        /**
         * Creates a new data output stream to write data to the specified
         * underlying output stream.
         * @param outputStream a stream to wrap with this one.
         * @param own true if this objects owns the stream that it wraps.
         */
        DataOutputStream( OutputStream* outputStream, bool own = false );

        virtual ~DataOutputStream();

        /**
         * Returns the current value of the counter written, the number of
         * bytes written to this data output stream so far. If the counter
         * overflows, it will be wrapped to decaf::lang::Long::MAX_VALUE.
         *
         * @return the value of the written field.
         */
        virtual long long size() const {
            return written;
        }

        /**
         * {@inheritDoc}
         */
        virtual void writeBoolean( bool value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
       virtual void writeByte( unsigned char value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual void writeShort( short value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual void writeUnsignedShort( unsigned short value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual void writeChar( char value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual void writeInt( int value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual void writeLong( long long value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual void writeFloat( float value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual void writeDouble( double value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual void writeBytes( const std::string& value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual void writeChars( const std::string& value ) throw ( IOException );

        /**
         * {@inheritDoc}
         */
        virtual void writeUTF( const std::string& value )
            throw ( IOException, UTFDataFormatException );

    protected:

        virtual void doWriteByte( unsigned char value ) throw ( decaf::io::IOException );

        virtual void doWriteArrayBounded( const unsigned char* buffer, int size, int offset, int length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IndexOutOfBoundsException );

    private:

        // Determine the encoded length of a string when written as modified UTF-8
        unsigned int countUTFLength( const std::string& value );

    };

}}

#endif /*_DECAF_IO_DATAOUTPUTSTREAM_H_*/
