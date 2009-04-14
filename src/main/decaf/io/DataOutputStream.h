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
    class DECAF_API DataOutputStream : public FilterOutputStream
    {
    protected:

        // The number of bytes written to the data output stream so far.
        std::size_t written;

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
         * overflows, it will be wrapped to Integer.MAX_VALUE.
         * @return the value of the written field.
         */
        virtual std::size_t size() const {
            return written;
        }

        /**
         * Writes a single byte to the output stream.  If no exception is
         * thrown, the counter written is incremented by 1.
         * @param c the byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( unsigned char c ) throw ( IOException );

        /**
         * Writes an array of bytes to the output stream.  the counter
         * written is incremented by len.
         * @param buffer The array of bytes to write.
         * @param offset the position in buffer to start writing from.
         * @param len The number of bytes from the buffer to be written.
         * @throws IOException thrown if an error occurs.
         * @throws NullPointerException if buffer is Null
         */
        virtual void write( const unsigned char* buffer,
                            std::size_t offset,
                            std::size_t len )
            throw ( IOException, lang::exceptions::NullPointerException );

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The bytes to write.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const std::vector<unsigned char>& buffer )
            throw ( IOException );

        /**
         * Writes a boolean to the underlying output stream as a 1-byte value. The
         * value true is written out as the value (byte)1; the value false
         * is written out as the value (byte)0. If no exception is thrown,
         * the counter written is incremented by 1.
         * @param value the boolean to write.
         * @throws IOException
         *
         */
        virtual void writeBoolean( bool value ) throw ( IOException );

        /**
         * Writes out a byte to the underlying output stream as a 1-byte
         * value. If no exception is thrown, the counter written is
         * incremented by 1.
         * @param value the unsigned char value to write.
         * @throws IOException
         */
        virtual void writeByte( unsigned char value ) throw ( IOException );

        /**
         * Writes a short to the underlying output stream as two bytes, high
         * byte first. If no exception is thrown, the counter written is
         * incremented by 2.
         * @param value the value to write.
         * @throws IOException
         */
        virtual void writeShort( short value ) throw ( IOException );

        /**
         * Writes a unsigned short to the bytes message stream as a 2 byte value
         * @param value - unsigned short to write to the stream
         * @throws IOException
         */
        virtual void writeUnsignedShort( unsigned short value ) throw ( IOException );

        /**
         * Writes out a char to the underlying output stream as a one byte
         * value If no exception is thrown, the counter written is
         * incremented by 1.
         * @param value the value to write.
         * @throws IOException
         */
        virtual void writeChar( char value ) throw ( IOException );

        /**
         * Writes an int to the underlying output stream as four bytes, high
         * byte first. If no exception is thrown, the counter written is
         * incremented by 4.
         * @param value the value to write.
         * @throws IOException
         */
        virtual void writeInt( int value ) throw ( IOException );

        /**
         * Writes an 64 bit long to the underlying output stream as eight
         * bytes, high byte first. If no exception is thrown, the counter
         * written is incremented by 8.
         * @param value the value to write.
         * @throws IOException
         */
        virtual void writeLong( long long value ) throw ( IOException );

        /**
         * Converts the float argument to an int using the floatToIntBits
         * method in class Float, and then writes that int value to the
         * underlying output stream as a 4-byte quantity, high byte first.
         * If no exception is thrown, the counter written is incremented
         * by 4.
         * @param value the value to write.
         * @throws IOException
         */
        virtual void writeFloat( float value ) throw ( IOException );

        /**
         * Converts the double argument to a long using the doubleToLongBits
         * method in class Double, and then writes that long value to the
         * underlying output stream as an 8-byte quantity, high byte first.
         * If no exception is thrown, the counter written is incremented
         * by 8.
         * @param value the value to write.
         * @throws IOException
         */
        virtual void writeDouble( double value ) throw ( IOException );

        /**
         * Writes out the string to the underlying output stream as a
         * sequence of bytes. Each character in the string is written out,
         * in sequence, by discarding its high eight bits. If no exception
         * is thrown, the counter written is incremented by the length of
         * value.  The value written does not include a trailing null as that
         * is not part of the sequence of bytes, if the null is needed, then use
         * the writeChars method.
         * @param value the value to write.
         * @throws IOException
         */
        virtual void writeBytes( const std::string& value ) throw ( IOException );

        /**
         * Writes a string to the underlying output stream as a sequence of
         * characters. Each character is written to the data output stream
         * as if by the writeChar method. If no exception is thrown, the
         * counter written is incremented by the length of value.  The trailing
         * NULL character is written by this method.
         * @param value the value to write.
         * @throws IOException
         */
        virtual void writeChars( const std::string& value ) throw ( IOException );

        /**
         * Writes out the string to the underlying output stream as a modeified UTF-8
         * encoded sequence of bytes.  The first two bytes written are indicate its
         * encoded length followed by the rest of the string's characters encoded as
         * modified UTF-8.  The length represent the encoded length of the data not the
         * actual length of the string.
         *
         * @param value
         *        the value to write.
         *
         * @throws IOException - on a write error
         * @throws UTFDataFormatException - if encoded size if greater than 65535
         */
        virtual void writeUTF( const std::string& value )
            throw ( IOException, UTFDataFormatException );

    private:

        // Determine the encoded length of a string when written as modified UTF-8
        unsigned int countUTFLength( const std::string& value );

    };

}}

#endif /*_DECAF_IO_DATAOUTPUTSTREAM_H_*/
