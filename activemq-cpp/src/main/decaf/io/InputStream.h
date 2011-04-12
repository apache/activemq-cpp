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

#ifndef _DECAF_IO_INPUTSTREAM_H_
#define _DECAF_IO_INPUTSTREAM_H_

#include <decaf/io/IOException.h>
#include <decaf/io/Closeable.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace io{

    /**
     * A base class that must be implemented by all classes wishing to provide a
     * class that reads in a stream of bytes
     *
     * @since 1.0
     */
    class DECAF_API InputStream : public Closeable,
                                  virtual public util::concurrent::Synchronizable {
    private:

        // Synchronization object.
        util::concurrent::Mutex mutex;

    private:

        InputStream( const InputStream& );
        InputStream& operator= ( const InputStream& );

    public:

        InputStream();

        virtual ~InputStream();

        /**
         * Closes the InputStream freeing any resources that might have been acquired
         * during the lifetime of this stream.
         *
         * The default implementation of this method does nothing.
         *
         * @throws IOException if an I/O error occurs while closing the InputStream.
         */
        virtual void close();

        /**
         * Marks the current position in the stream A subsequent call to the
         * reset method repositions this stream at the last marked position so
         * that subsequent reads re-read the same bytes.
         *
         * If a stream instance reports that marks are supported then the stream
         * will ensure that the same bytes can be read again after the reset method
         * is called so long the readLimit is not reached.
         *
         * Calling mark on a closed stream instance should have no effect.
         *
         * The default implementation of this method does nothing.
         *
         * @param readLimit
         *      The max bytes read before marked position is invalid.
         */
        virtual void mark( int readLimit );

        /**
         * Repositions this stream to the position at the time the mark method was
         * last called on this input stream.
         *
         * If the method markSupported returns true, then:
         *   * If the method mark has not been called since the stream was created,
         *     or the number of bytes read from the stream since mark was last called
         * 	   is larger than the argument to mark at that last call, then an
         *     IOException might be thrown.
         *   * If such an IOException is not thrown, then the stream is reset to a
         *     state such that all the bytes read since the most recent call to mark
         *     (or since the start of the file, if mark has not been called) will be
         *     resupplied to subsequent callers of the read method, followed by any
         *     bytes that otherwise would have been the next input data as of the
         *     time of the call to reset.
         *
         * If the method markSupported returns false, then:
         *   * The call to reset may throw an IOException.
         *   * If an IOException is not thrown, then the stream is reset to a fixed
         *     state that depends on the particular type of the input stream and how
         *     it was created. The bytes that will be supplied to subsequent callers
         *     of the read method depend on the particular type of the input stream.
         *
         * The default implementation of this method throws an IOException.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void reset();

        /**
         * Determines if this input stream supports the mark and reset methods.
         * Whether or not mark and reset are supported is an invariant property of
         * a particular input stream instance.
         *
         * The default implementation of this method returns false.
         *
         * @returns true if this stream instance supports marks
         */
        virtual bool markSupported() const {
            return false;
        }

        /**
         * Indicates the number of bytes available.  The default implementation of this
         * methods returns 0.  Classes that override this method may return the total
         * number of bytes that are currently available for reading and others may simply
         * return a value of one indicating that there is some data avaiable.  The caller
         * should view the result of this method as an absolute.
         *
         * The default implementation of this method returns zero.
         *
         * @return the number of bytes available on this input stream.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual int available() const {
            return 0;
        }

        /**
         * Reads a single byte from the buffer.  The value byte is returned as an int in the
         * range 0 to 255. If no byte is available because the end of the stream has been reached,
         * the value -1 is returned. This method blocks until input data is available, the end of
         * the stream is detected, or an exception is thrown.
         *
         * The default implementation of this method calls the internal virtual method doReadByte
         * which is a pure virtual method and must be overridden by all subclasses.
         *
         * @return The next byte or -1 if the end of stream is reached.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual int read();

        /**
         * Reads up to size bytes of data from the input stream into an array of bytes. An
         * attempt is made to read as many as size bytes, but a smaller number may be read.
         * The number of bytes actually read is returned as an integer.
         *
         * This method blocks until input data is available, end of file is detected, or
         * an exception is thrown.
         *
         * If size is zero, then no bytes are read and 0 is returned; otherwise, there is an
         * attempt to read at least one byte. If no byte is available because the stream is
         * at end of file, the value -1 is returned; otherwise, at least one byte is read and
         * stored into b.
         *
         * This method called the protected virtual method doReadArray which by default is the
         * same as calling read( buffer, size, 0, size ).  Subclasses can customize the behavior
         * of this method by overriding the doReadArray method to provide a better performing
         * read operation.
         *
         * @param buffer
         *      The target buffer to write the read in data to.
         * @param size
         *      The size in bytes of the target buffer.
         *
         * @return The number of bytes read or -1 if EOF is detected
         *
         * @throws IOException if an I/O error occurs.
         * @throws NullPointerException if buffer passed is NULL.
         */
        virtual int read( unsigned char* buffer, int size );

        /**
         * Reads up to length bytes of data from the input stream into an array of bytes. An
         * attempt is made to read as many as length bytes, but a smaller number may be read.
         * The number of bytes actually read is returned as an integer.
         *
         * This method blocks until input data is available, end of file is detected, or
         * an exception is thrown.
         *
         * If length is zero, then no bytes are read and 0 is returned; otherwise, there is an
         * attempt to read at least one byte. If no byte is available because the stream is
         * at end of file, the value -1 is returned; otherwise, at least one byte is read and
         * stored into b.
         *
         * The first byte read is stored into element b[off], the next one into b[off+1], and
         * so on. The number of bytes read is, at most, equal to length. Let k be the number of
         * bytes actually read; these bytes will be stored in elements b[off] through b[off+k-1],
         * leaving elements b[offset+k] through b[offset+length-1] unaffected.
         *
         * In every case, elements b[0] through b[offset] and elements b[offset+length] through
         * b[b.length-1] are unaffected.
         *
         * This method called the protected virtual method doReadArrayBounded which simply
         * calls the method doReadByte() repeatedly. If the first such call results in an IOException,
         * that exception is returned. If any subsequent call to doReadByte() results in a IOException,
         * the exception is caught and treated as if it were end of file; the bytes read up to that
         * point are stored into the buffer and the number of bytes read before the exception occurred
         * is returned. The default implementation of this method blocks until the requested amount of
         * input data has been read, end of file is detected, or an exception is thrown. Subclasses
         * are encouraged to provide a more efficient implementation of this method.
         *
         * @param buffer
         *      The target buffer to write the read in data to.
         * @param size
         *      The size in bytes of the target buffer.
         * @param offset
         *      The position in the buffer to start inserting the read in data.
         * @param length
         *      The maximum number of bytes that should be read into buffer.
         *
         * @return The number of bytes read or -1 if EOF is detected
         *
         * @throws IOException if an I/O error occurs.
         * @throws NullPointerException if buffer passed is NULL.
         * @throws IndexOutOfBoundsException if length > size - offset.
         */
        virtual int read( unsigned char* buffer, int size, int offset, int length );

        /**
         * Skips over and discards n bytes of data from this input stream. The skip
         * method may, for a variety of reasons, end up skipping over some smaller
         * number of bytes, possibly 0. This may result from any of a number of
         * conditions; reaching end of file before n bytes have been skipped is
         * only one possibility. The actual number of bytes skipped is returned.
         *
         * The skip method of InputStream creates a byte array and then repeatedly
         * reads into it until num bytes have been read or the end of the stream has
         * been reached. Subclasses are encouraged to provide a more efficient
         * implementation of this method.
         *
         * @param num
         *       The number of bytes to skip.
         *
         * @returns total bytes skipped
         *
         * @throws IOException if an I/O error occurs.
         * @throws UnsupportedOperationException if the concrete stream class does
         *         not support skipping bytes.
         */
        virtual long long skip( long long num );

        /**
         * Output a String representation of this object.
         *
         * The default version of this method just prints the Class Name.
         *
         * @return a string representation of the object.
         */
        virtual std::string toString() const;

    protected:  // Virtual doRead methods that can be overridden to customize subclasses.

        virtual int doReadByte() = 0;

        virtual int doReadArray( unsigned char* buffer, int size );

        virtual int doReadArrayBounded( unsigned char* buffer, int size, int offset, int length );

    public:  // Synchronizable

        virtual void lock() {
            mutex.lock();
        }

        virtual bool tryLock() {
            return mutex.tryLock();
        }

        virtual void unlock() {
            mutex.unlock();
        }

        virtual void wait() {
            mutex.wait();
        }

        virtual void wait( long long millisecs ) {
            mutex.wait( millisecs );
        }

        virtual void wait( long long millisecs, int nanos ) {
            mutex.wait( millisecs, nanos );
        }

        virtual void notify() {
            mutex.notify();
        }

        virtual void notifyAll() {
            mutex.notifyAll();
        }

    };

}}

#endif /*_DECAF_IO_INPUTSTREAM_H_*/
