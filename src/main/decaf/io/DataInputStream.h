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

#ifndef _DECAF_IO_DATAINPUTSTREAM_H_
#define _DECAF_IO_DATAINPUTSTREAM_H_

#include <decaf/io/FilterInputStream.h>
#include <decaf/io/IOException.h>
#include <decaf/io/EOFException.h>
#include <decaf/io/UTFDataFormatException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

namespace decaf{
namespace io{

    /**
     * A data input stream lets an application read primitive Java data
     * types from an underlying input stream in a machine-independent way.
     * An application uses a data output stream to write data that can
     * later be read by a data input stream.
     *
     * Due to the lack of garbage collection in C++ a design decision was
     * made to add a boolean parameter to the constructor indicating if the
     * wrapped <code>InputStream</code> is owned by this object.  That way
     * creation of the underlying stream can occur in a Java like way. Ex:
     *
     *  DataInputStream os = new DataInputStream( new InputStream(), true )
     */
    class DECAF_API DataInputStream : public FilterInputStream
    {
    public:

        /**
         * Creates a DataInputStream that uses the specified underlying
         * InputStream.
         * @param inputStream the InputStream instance to wrap.
         * @param own, indicates if this class owns the wrapped string
         * defaults to false.
         */
        DataInputStream( InputStream* inputStream, bool own = false );

        virtual ~DataInputStream();

        /**
         * Reads some number of bytes from the contained input stream and
         * stores them into the buffer array b. The number of bytes actually
         * read is returned as an integer. This method blocks until input
         * data is available, end of file is detected, or an exception is
         * thrown.
         * <p>
         * If the length of buffer is zero, then no bytes are read and 0 is
         * returned; otherwise, there is an attempt to read at least one
         * byte. If no byte is available because the stream is at end of
         * file, the value -1 is returned; otherwise, at least one byte is
         * read and stored into buffer.
         * <p>
         * The first byte read is stored into element buffer[0], the next one
         * into buffer[1], and so on. The number of bytes read is, at most,
         * equal to the length of buffer. Let k be the number of bytes actually
         * read; these bytes will be stored in elements b[0] through b[k-1],
         * leaving elements buffer[k] through buffer[buffer.length-1]
         * unaffected.
         * <p>
         * If the first byte cannot be read for any reason other than end
         * of file, then an IOException is thrown. In particular, an
         * IOException is thrown if the input stream has been closed.
         * <p>
         * The read( buffer ) method has the same effect as:
         *      read( buffer, 0, b.length )
         * @param buffer - byte array to insert read data into
         * @returns the total number of bytes read, or -1 if there is no
         *          more data because the stream is EOF.
         * @throws IOException
         */
        virtual int read( std::vector< unsigned char >& buffer )
            throw ( io::IOException );

        /**
         * Reads up to len bytes of data from the contained input stream
         * into an array of bytes. An attempt is made to read as many as
         * len bytes, but a smaller number may be read, possibly zero. The
         * number of bytes actually read is returned as an integer.
         * <p>
         * This method blocks until input data is available, end of file is
         * detected, or an exception is thrown.
         * <p>
         * If buffer is null, a NullPointerException is thrown.
         * <p>
         * If off is negative, or len is negative then an
         * IndexOutOfBoundsException is thrown, if off + len is greater that
         * the allocated length of the array, an IOException will result
         * depending on the platform and compiler settings.
         * <p>
         * If len is zero, then no bytes are read and 0 is returned;
         * otherwise, there is an attempt to read at least one byte. If no
         * byte is available because the stream is at end of file, the
         * value -1 is returned; otherwise, at least one byte is read and
         * stored into buffer.
         * <p>
         * The first byte read is stored into element b[off], the next one
         * into buffer[off+1], and so on. The number of bytes read is, at most,
         * equal to len. Let k be the number of bytes actually read; these
         * bytes will be stored in elements buffer[off] through buffer[off+k-1],
         * leaving elements buffer[off+k] through buffer[off+len-1] unaffected.
         * <p>
         * In every case, elements buffer[0] through buffer[off] and elements
         * buffer[off+len] through buffer[buffer.length-1] are unaffected.
         * <p>
         * If the first byte cannot be read for any reason other than end of
         * file, then an IOException is thrown. In particular, an IOException
         * is thrown if the input stream has been closed.
         * @param buffer - byte array to insert read data into
         * @param offset - location in buffer to start writing
         * @param length - number of bytes to read
         * @returns the total number of bytes read, or -1 if there is no
         *          more data because the stream is EOF.
         * @throws IOException
         * @throws NullPointerException if the buffer is null
         */
        virtual int read( unsigned char* buffer,
                          std::size_t offset,
                          std::size_t length )
            throw ( io::IOException,
                    lang::exceptions::NullPointerException );

        /**
         * Reads one input byte and returns true if that byte is nonzero,
         * false if that byte is zero.
         * @returns the boolean value read.
         * @throws IOException
         * @throws EOFException
         */
        virtual bool readBoolean()
            throw( io::IOException, io::EOFException );

        /**
         * Reads and returns one input byte. The byte is treated as a
         * signed value in the range -128 through 127, inclusive.
         * @returns the 8-bit value read.
         * @throws IOException
         * @throws EOFException
         */
        virtual char readByte()
            throw ( io::IOException, io::EOFException );

        /**
         * Reads one input byte, zero-extends it to type int, and returns
         * the result, which is therefore in the range 0  through 255.
         * @returns the 8 bit unsigned value read
         * @throws IOException
         * @throws EOFException
         */
        virtual unsigned char readUnsignedByte()
            throw ( io::IOException, io::EOFException );

        /**
         * Reads an input char and returns the char value. A ascii char
         * is made up of one bytes.  This returns the same result as
         * <code>readByte</code>
         * @returns the 8 bit char read
         * @throws IOException
         * @throws EOFException
         */
        virtual char readChar() throw ( io::IOException, io::EOFException );

        /**
         * Reads eight input bytes and returns a double value. It does this
         * by first constructing a long long  value in exactly the manner of
         * the readlong  method, then converting this long  value to a double
         * in exactly the manner of the method Double.longBitsToDouble.
         * @returns the double value read
         * @throws IOException
         * @throws EOFException
         */
        virtual double readDouble()
            throw ( io::IOException, io::EOFException );

        /**
         * Reads four input bytes and returns a float value. It does this
         * by first constructing an int  value in exactly the manner of the
         * readInt  method, then converting this int  value to a float in
         * exactly the manner of the method Float.intBitsToFloat.
         * @returns the float value read
         * @throws IOException
         * @throws EOFException
         */
        virtual float readFloat() throw ( io::IOException, io::EOFException );

        /**
         * Reads four input bytes and returns an int value. Let a  be the
         * first byte read, b be the second byte, c be the third byte, and
         * d be the fourth byte. The value returned is: <p>
         *  (((a & 0xff) << 24) | ((b & 0xff) << 16) |
         *   ((c & 0xff) << 8) | (d & 0xff))
         * @returns the int value read
         * @throws IOException
         * @throws EOFException
         */
        virtual int readInt() throw ( io::IOException, io::EOFException );

        /**
         * Reads eight input bytes and returns a long value. Let a  be the
         * first byte read, b be the second byte, c be the third byte, d
         * be the fourth byte, e be the fifth byte, f  be the sixth byte,
         * g be the seventh byte, and h be the eighth byte. The value
         * returned is:
         *  (((long)(a & 0xff) << 56) |
         *   ((long)(b & 0xff) << 48) |
         *   ((long)(c & 0xff) << 40) |
         *   ((long)(d & 0xff) << 32) |
         *   ((long)(e & 0xff) << 24) |
         *   ((long)(f & 0xff) << 16) |
         *   ((long)(g & 0xff) <<  8) |
         *   ((long)(h & 0xff)))
         * @returns the 64 bit long long read
         * @throws IOException
         * @throws EOFException
         */
        virtual long long readLong()
            throw ( io::IOException, io::EOFException );

        /**
         * Reads two input bytes and returns a short value. Let a  be the
         * first byte read and b  be the second byte. The value returned is:
         *   (short)((a << 8) | (b & 0xff))
         * @returns the 16 bit short value read
         * @throws IOException
         * @throws EOFException
         */
        virtual short readShort() throw ( io::IOException, io::EOFException );

        /**
         * Reads two input bytes and returns an int value in the range 0
         * through 65535. Let a  be the first byte read and b  be the
         * second byte. The value returned is:
         *   (((a & 0xff) << 8) | (b & 0xff))
         * @returns the 16 bit unsigned short read
         * @throws IOException
         * @throws EOFException
         */
        virtual unsigned short readUnsignedShort()
            throw ( io::IOException, io::EOFException );

        /**
         * Reads an null terminated ASCII string to the stream and returns the
         * string to the caller.
         * @returns string object containing the string read.
         * @throws IOException
         * @throws EOFException
         */
        virtual std::string readString()
            throw ( io::IOException, io::EOFException );

        /**
         * Reads a UTF8 encoded string in ASCII format and returns it, this is
         * only useful if you know for sure that the string that is to be read
         * was a string that contained all ascii values, and not uncide chars.
         * @returns string read from stream.
         * @throws IOException
         * @throws EOFException
         * @throws UTFDataFormatException
         */
        virtual std::string readUTF()
            throw ( io::IOException, io::EOFException, io::UTFDataFormatException );

        /**
         * Reads some bytes from an input stream and stores them into the
         * buffer array buffer. The number of bytes read is equal to the length
         * of buffer.<p>
         * This method blocks until one of the following conditions occurs:
         *    * buffer.size() bytes of input data are available, in which case
         *      a normal return is made.
         *    * End of file is detected, in which case an EOFException is
         *      thrown.
         *    * An I/O error occurs, in which case an IOException other than
         *      EOFException is thrown.
         * <p>
         * If buffer.size() is zero, then no bytes are read. Otherwise, the
         * first byte read is stored into element b[0], the next one into
         * buffer[1], and so on. If an exception is thrown from this method,
         * then it may be that some but not all bytes of buffer have been
         * updated with data from the input stream.
         * @param buffer - vector of char that is read to its size()
         * @throws IOException
         * @throws EOFException
         */
        virtual void readFully( std::vector< unsigned char >& buffer )
            throw ( io::IOException, io::EOFException );

        /**
         * Reads length bytes from an input stream.
         * <p>
         * This method blocks until one of the following conditions occurs:
         *    * length bytes of input data are available, in which case a
         *      normal return is made.
         *    * End of file is detected, in which case an EOFException is
         *      thrown.
         *    * An I/O error occurs, in which case an IOException other
         *      than EOFException is thrown.
         * <p>
         * If buffer is null, a NullPointerException is thrown. If offset is
         * negative, or len is negative, or offset+length is greater than the
         * length of the array buffer, then an IndexOutOfBoundsException is
         * thrown. If len is zero, then no bytes are read. Otherwise, the
         * first byte read is stored into element buffer[off], the next one into
         * buffer[offset+1], and so on. The number of bytes read is, at most,
         * equal to len.
         * @param buffer - byte array to insert read data into
         * @param offset - location in buffer to start writing
         * @param length - number of bytes to read
         * @throws IOException
         * @throws EOFException
         * @throws NullPointerException if the buffer is null
         */
        virtual void readFully( unsigned char* buffer,
                                std::size_t offset,
                                std::size_t length )
            throw ( io::IOException,
                    io::EOFException,
                    lang::exceptions::NullPointerException );

        /**
         * Makes an attempt to skip over n bytes of data from the input
         * stream, discarding the skipped bytes. However, it may skip over
         * some smaller number of bytes, possibly zero. This may result from
         * any of a number of conditions; reaching end of file before n
         * bytes have been skipped is only one possibility. This method
         * never throws an EOFException. The actual number of bytes skipped
         * is returned.
         * @param num - number of bytes to skip
         * @return the total number of bytes skipped
         */
        virtual std::size_t skip( std::size_t num )
            throw( io::IOException,
                   lang::exceptions::UnsupportedOperationException );

    private:

        // Used internally to reliable get data from the underlying stream
        inline void readAllData( unsigned char* buffer,
                                 std::size_t length )
            throw ( io::IOException,
                    io::EOFException ) {

            std::size_t n = 0;
            do{
                int count = inputStream->read( &buffer[n], 0, length - n );
                if( count == -1 ) {
                    throw EOFException(
                        __FILE__, __LINE__,
                        "DataInputStream::readLong - Reached EOF" );
                }
                n += count;
            } while( n < length );
        }

    };

}}

#endif /*_DECAF_IO_DATAINPUTSTREAM_H_*/
