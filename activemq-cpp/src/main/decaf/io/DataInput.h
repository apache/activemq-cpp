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

#ifndef _DECAF_IO_DATAINPUT_H_
#define _DECAF_IO_DATAINPUT_H_

#include <decaf/util/Config.h>

#include <vector>
#include <string>

#include <decaf/io/IOException.h>
#include <decaf/io/EOFException.h>
#include <decaf/io/UTFDataFormatException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

namespace decaf {
namespace io {

    /**
     * The DataInput interface provides for reading bytes from a binary stream and reconstructing
     * from them data in any of the C++ primitive types. There is also a facility for reconstructing
     * Strings from data in the Java standard modified UTF-8 format.
     *
     * It is generally true of all the reading routines in this interface that if end of file is
     * reached before the desired number of bytes has been read, an EOFException is thrown. If any
     * byte cannot be read for any reason other than end of file, an IOException other than EOFException
     * is thrown. for example, an IOException may be thrown if the underlying input stream has been
     * closed.
     *
     * @see DataOutput
     * @see DataInputStream
     *
     * @since 1.0
     */
    class DECAF_API DataInput {
    public:

        virtual ~DataInput() {}

        /**
         * Reads in one byte and returns true if that byte is nonzero, false if that
         * byte is zero.
         *
         * @returns the boolean value of the read in byte (0=false, 1=true).
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual bool readBoolean() = 0;

        /**
         * Reads and returns one input byte. The byte is treated as a
         * signed value in the range -128 through 127, inclusive.
         *
         * @returns the 8-bit value read.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual char readByte() = 0;

        /**
         * Reads one input byte, zero-extends it to type int, and returns
         * the result, which is therefore in the range 0  through 255.
         *
         * @returns the 8 bit unsigned value read.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual unsigned char readUnsignedByte() = 0;

        /**
         * Reads an input char and returns the char value. A ascii char
         * is made up of one bytes.  This returns the same result as
         * <code>readByte</code>
         *
         * @returns the 8 bit char read.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual char readChar() = 0;

        /**
         * Reads eight input bytes and returns a double value. It does this
         * by first constructing a long long  value in exactly the manner of
         * the readlong  method, then converting this long  value to a double
         * in exactly the manner of the method Double::longBitsToDouble.
         *
         * @returns the double value read.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual double readDouble() = 0;

        /**
         * Reads four input bytes and returns a float value. It does this
         * by first constructing an int  value in exactly the manner of the
         * readInt  method, then converting this int  value to a float in
         * exactly the manner of the method Float::intBitsToFloat.
         *
         * @returns the float value read.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual float readFloat() = 0;

        /**
         * Reads four input bytes and returns an int value. Let a  be the
         * first byte read, b be the second byte, c be the third byte, and
         * d be the fourth byte. The value returned is:
         *
         *  (((a & 0xff) << 24) | ((b & 0xff) << 16) |
         *   ((c & 0xff) << 8) | (d & 0xff))
         *
         * @returns the int value read.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual int readInt() = 0;

        /**
         * Reads eight input bytes and returns a long value. Let a  be the
         * first byte read, b be the second byte, c be the third byte, d
         * be the fourth byte, e be the fifth byte, f  be the sixth byte,
         * g be the seventh byte, and h be the eighth byte. The value
         * returned is:
         *
         *  (((long)(a & 0xff) << 56) |
         *   ((long)(b & 0xff) << 48) |
         *   ((long)(c & 0xff) << 40) |
         *   ((long)(d & 0xff) << 32) |
         *   ((long)(e & 0xff) << 24) |
         *   ((long)(f & 0xff) << 16) |
         *   ((long)(g & 0xff) <<  8) |
         *   ((long)(h & 0xff)))
         *
         * @returns the 64 bit long long read.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual long long readLong() = 0;

        /**
         * Reads two input bytes and returns a short value. Let a  be the
         * first byte read and b  be the second byte. The value returned is:
         *
         *   (short)((a << 8) | (b & 0xff))
         *
         * @returns the 16 bit short value read.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual short readShort() = 0;

        /**
         * Reads two input bytes and returns an int value in the range 0
         * through 65535. Let a  be the first byte read and b  be the
         * second byte. The value returned is:
         *
         *   (((a & 0xff) << 8) | (b & 0xff))
         *
         * @returns the 16 bit unsigned short read.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual unsigned short readUnsignedShort() = 0;

        /**
         * Reads an NULL terminated ASCII string to the stream and returns the
         * string to the caller.
         *
         * @returns string object containing the string read.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         */
        virtual std::string readString() = 0;

        /**
         * Reads the next line of text from the input stream. It reads successive bytes, converting
         * each byte to an ASCII char separately, until it encounters a line terminator or end of
         * file; the characters read are then returned as a standard String. Note that because this
         * method processes bytes, it does not support input of the full Unicode character set.
         *
         * If end of file is encountered before even one byte can be read, then an empty string is
         * returned. Otherwise, each byte that is read is converted to type char.  If the character
         * '\n' is encountered, it is discarded and reading ceases. If the character '\r' is
         * encountered, it is discarded and, if the following byte converts to the character '\n',
         * then that is discarded also; reading then ceases. If end of file is encountered before
         * either of the characters '\n' and '\r' is encountered, reading ceases. Once reading has
         * ceased, a String is returned that contains all the characters read and not discarded,
         * taken in order.
         *
         * @return the next line of text read from the input stream or empty string if at EOF.
         *
         * @throws IOException if an I/O Error occurs.
         */
        virtual std::string readLine() = 0;

        /**
         * Reads a modified UTF-8 encoded string in ASCII format and returns it,
         * this is only useful if you know for sure that the string that is to be read
         * was a string that contained all ASCII values (0-255), if so this method will
         * throw a UTFFormatException.  This method reads String value written from a
         * Java DataOutputStream and assumes that the length prefix the precedes the
         * encoded UTF-8 bytes is an unsigned short, which implies that the String will
         * be no longer than 65535 characters.
         *
         * @returns The decoded string read from stream.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         * @throws UTFDataFormatException if the bytes are not valid modified UTF-8 values.
         */
        virtual std::string readUTF() = 0;

        /**
         * Reads some bytes from an input stream and stores them into the
         * buffer array buffer. The number of bytes read is equal to the length
         * of buffer.
         *
         * This method blocks until one of the following conditions occurs:
         *    * buffer's size bytes of input data are available, in which case
         *      a normal return is made.
         *    * End of file is detected, in which case an EOFException is
         *      thrown.
         *    * An I/O error occurs, in which case an IOException other than
         *      EOFException is thrown.
         *
         * If buffer size is zero, then no bytes are read. Otherwise, the
         * first byte read is stored into element b[0], the next one into
         * buffer[1], and so on. If an exception is thrown from this method,
         * then it may be that some but not all bytes of buffer have been
         * updated with data from the input stream.
         *
         * @param buffer
         *      The byte array to insert read data into.
         * @param size
         *      The size in bytes of the given byte buffer.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         * @throws IndexOutOfBoundsException if the size value is negative.
         */
        virtual void readFully( unsigned char* buffer, int size ) = 0;

        /**
         * Reads length bytes from an input stream.
         *
         * This method blocks until one of the following conditions occurs:
         *    * length bytes of input data are available, in which case a
         *      normal return is made.
         *    * End of file is detected, in which case an EOFException is
         *      thrown.
         *    * An I/O error occurs, in which case an IOException other
         *      than EOFException is thrown.
         *
         * If buffer is NULL, a NullPointerException is thrown. If offset+length
         * is greater than the length of the array buffer, then an IndexOutOfBoundsException
         * is thrown. If length is zero, then no bytes are read. Otherwise, the first
         * byte read is stored into element buffer[off], the next one into buffer[offset+1],
         * and so on. The number of bytes read is, at most, equal to length.
         *
         * @param buffer
         *      The byte array to insert read data into.
         * @param size
         *      The size in bytes of the given byte buffer.
         * @param offset
         *      The location in buffer to start writing.
         * @param length
         *      The number of bytes to read from the buffer.
         *
         * @throws IOException if an I/O Error occurs.
         * @throws EOFException if the end of input is reached.
         * @throws NullPointerException if the buffer is NULL.
         * @throws IndexOutOfBoundsException if the offset + length > size, or an int param is negative.
         */
        virtual void readFully( unsigned char* buffer, int size, int offset, int length ) = 0;

        /**
         * Makes an attempt to skip over n bytes of data from the input stream,
         * discarding the skipped bytes. However, it may skip over some smaller
         * number of bytes, possibly zero. This may result from any of a number
         * of conditions; reaching end of file before n bytes have been skipped
         * is only one possibility. This method never throws an EOFException.
         * The actual number of bytes skipped is returned.
         *
         * @param num
         *      The number of bytes to skip over.
         *
         * @return the total number of bytes skipped.
         *
         * @throws IOException if an I/O Error occurs.
         */
        virtual long long skipBytes( long long num ) = 0;

    };

}}

#endif /* _DECAF_IO_DATAINPUT_H_ */
