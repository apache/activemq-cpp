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

#ifndef _DECAF_IO_DATAOUTPUT_H_
#define _DECAF_IO_DATAOUTPUT_H_

#include <decaf/util/Config.h>

#include <vector>
#include <string>

#include <decaf/io/IOException.h>
#include <decaf/io/EOFException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

namespace decaf {
namespace io {

    /**
     * The DataOutput interface provides for converting data from any of the C++ primitive types
     * to a series of bytes and writing these bytes to a binary stream. There is also a facility
     * for converting Strings into the Java standard modified UTF-8 format and writing the
     * resulting series of bytes.
     *
     * If a method in this interface encounters an error while writing it will throw an IOException.
     *
     * @see DataInput
     * @see DataOutputStream
     *
     * @since 1.0
     */
    class DECAF_API DataOutput {
    public:

        virtual ~DataOutput() {}

        /**
         * Writes a boolean to the underlying output stream as a 1-byte value. The
         * value true is written out as the value (byte)1; the value false
         * is written out as the value (byte)0. If no exception is thrown,
         * the counter written is incremented by 1.
         *
         * @param value
         *      The boolean to write as a byte (1=true, 0=false).
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeBoolean( bool value ) = 0;

        /**
         * Writes out a byte to the underlying output stream as a 1-byte
         * value. If no exception is thrown, the counter written is
         * incremented by 1.
         *
         * @param value
         *      The unsigned char value to write.
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeByte( unsigned char value ) = 0;

        /**
         * Writes a short to the underlying output stream as two bytes, high
         * byte first. If no exception is thrown, the counter written is
         * incremented by 2.
         *
         * @param value
         *      The signed short value to write.
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeShort( short value ) = 0;

        /**
         * Writes a unsigned short to the bytes message stream as a 2 byte value
         *
         * @param value
         *      The unsigned short to write to the stream.
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeUnsignedShort( unsigned short value ) = 0;

        /**
         * Writes out a char to the underlying output stream as a one byte
         * value If no exception is thrown, the counter written is
         * incremented by 1.
         *
         * @param value
         *      The signed char value to write.
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeChar( char value ) = 0;

        /**
         * Writes an int to the underlying output stream as four bytes, high
         * byte first. If no exception is thrown, the counter written is
         * incremented by 4.
         *
         * @param value
         *      The signed integer value to write.
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeInt( int value ) = 0;

        /**
         * Writes an 64 bit long to the underlying output stream as eight
         * bytes, high byte first. If no exception is thrown, the counter
         * written is incremented by 8.
         *
         * @param value
         *      The signed 64bit long value to write.
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeLong( long long value ) = 0;

        /**
         * Converts the float argument to an int using the floatToIntBits
         * method in class Float, and then writes that int value to the
         * underlying output stream as a 4-byte quantity, high byte first.
         * If no exception is thrown, the counter written is incremented
         * by 4.
         *
         * @param value
         *      The 32bit floating point value to write.
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeFloat( float value ) = 0;

        /**
         * Converts the double argument to a long using the doubleToLongBits
         * method in class Double, and then writes that long value to the
         * underlying output stream as an 8-byte quantity, high byte first.
         * If no exception is thrown, the counter written is incremented
         * by 8.
         *
         * @param value
         *      The 64bit double value to write.
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeDouble( double value ) = 0;

        /**
         * Writes out the string to the underlying output stream as a
         * sequence of bytes. Each character in the string is written out,
         * in sequence, by discarding its high eight bits. If no exception
         * is thrown, the counter written is incremented by the length of
         * value.  The value written does not include a trailing null as that
         * is not part of the sequence of bytes, if the null is needed, then use
         * the writeChars method.
         *
         * @param value
         *      The vector of bytes to write.
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeBytes( const std::string& value ) = 0;

        /**
         * Writes a string to the underlying output stream as a sequence of
         * characters. Each character is written to the data output stream
         * as if by the writeChar method. If no exception is thrown, the
         * counter written is incremented by the length of value.  The trailing
         * NULL character is written by this method.
         *
         * @param value
         *      The string value to write as raw bytes.
         *
         * @throws IOException if an I/O error is encountered.
         */
        virtual void writeChars( const std::string& value ) = 0;

        /**
         * Writes out the string to the underlying output stream as a modeified UTF-8
         * encoded sequence of bytes.  The first two bytes written are indicate its
         * encoded length followed by the rest of the string's characters encoded as
         * modified UTF-8.  The length represent the encoded length of the data not the
         * actual length of the string.
         *
         * @param value
         *        The string value value to write as modified UTF-8.
         *
         * @throws IOException if an I/O error is encountered.
         * @throws UTFDataFormatException if the encoded size if greater than 65535
         */
        virtual void writeUTF( const std::string& value ) = 0;

    };

}}

#endif /* _DECAF_IO_DATAOUTPUT_H_ */
