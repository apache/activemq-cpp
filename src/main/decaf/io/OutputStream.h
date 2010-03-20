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

#ifndef _DECAF_IO_OUTPUTSTREAM_H
#define _DECAF_IO_OUTPUTSTREAM_H

#include <decaf/io/Closeable.h>
#include <decaf/io/Flushable.h>
#include <decaf/io/IOException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Config.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

namespace decaf{
namespace io{

    /**
     * Base interface for any class that wants to represent an output stream of bytes.
     *
     * @since 1.0
     */
    class DECAF_API OutputStream : public Closeable,
                                   public Flushable,
                                   public util::concurrent::Synchronizable
    {
    private:

        // Synchronization object.
        util::concurrent::Mutex mutex;

    public:

        virtual ~OutputStream();

        /**
         * {@inheritDoc}
         *
         * The default implementation of this method does nothing.
         */
        virtual void close() throw( decaf::io::IOException );

        /**
         * {@inheritDoc}
         *
         * The default implementation of this method does nothing.
         */
        virtual void flush() throw( decaf::io::IOException );

        /**
         * Writes a single byte to the output stream.
         *
         * The default implementation of this method calls the pure virtual method
         * doWriteByte which must be implemented by any subclass of the OutputStream.
         *
         * @param c
         *      The byte to write to the sink.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void write( unsigned char c ) throw ( decaf::io::IOException );

        /**
         * Writes an array of bytes to the output stream.  The entire contents of
         * the given vector are written to the output stream.
         *
         * The default implementation of this method simply calls the doWriteArray which
         * writes the contents of the array using the doWriteByte method repeatedly.  It is
         * recommended that a subclass override doWriteArray to provide more performant
         * means of writing the array.
         *
         * @param buffer
         *      The vector of bytes to write.
         * @param size
         *      The size of the buffer passed.
         *
         * @throws IOException if an I/O error occurs.
         * @throws NullPointerException thrown if buffer is Null.
         * @throws IndexOutOfBoundsException if size value is negative.
         */
        virtual void write( const unsigned char* buffer, int size )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes an array of bytes to the output stream in order starting at buffer[offset]
         * and proceeding until the number of bytes specified by the length argument are
         * written or an error occurs.
         *
         * The default implementation of this method simply calls the doWriteArrayBounded method
         * which writes the contents of the array using the doWriteByte method repeatedly.  It is
         * recommended that a subclass override doWriteArrayBounded to provide more performant
         * means of writing the array.
         *
         * @param buffer
         *      The array of bytes to write.
         * @param size
         *      The size of the buffer array passed.
         * @param offset
         *      The position to start writing in buffer.
         * @param length
         *      The number of bytes from the buffer to be written.
         *
         * @throws IOException if an I/O error occurs.
         * @throws NullPointerException thrown if buffer is Null.
         * @throws IndexOutOfBoundsException if the offset + length > size. or one of the
         *         parameters is negative.
         */
        virtual void write( const unsigned char* buffer, int size, int offset, int length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Output a String representation of this object.
         *
         * The default version of this method just prints the Class Name.
         *
         * @return a string representation of the object.
         */
        virtual std::string toString() const;

    protected:

        virtual void doWriteByte( unsigned char value )
            throw( decaf::io::IOException ) = 0;

        virtual void doWriteArray( const unsigned char* buffer, int size )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual void doWriteArrayBounded( const unsigned char* buffer, int size, int offset, int length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IndexOutOfBoundsException );

    public:  // Synchronizable

        virtual void lock() throw( decaf::lang::exceptions::RuntimeException ) {
            mutex.lock();
        }

        virtual bool tryLock() throw( decaf::lang::exceptions::RuntimeException ) {
            return mutex.tryLock();
        }

        virtual void unlock() throw( decaf::lang::exceptions::RuntimeException ) {
            mutex.unlock();
        }

        virtual void wait() throw( decaf::lang::exceptions::RuntimeException,
                                   decaf::lang::exceptions::IllegalMonitorStateException,
                                   decaf::lang::exceptions::InterruptedException ) {

            mutex.wait();
        }

        virtual void wait( long long millisecs )
            throw( decaf::lang::exceptions::RuntimeException,
                   decaf::lang::exceptions::IllegalMonitorStateException,
                   decaf::lang::exceptions::InterruptedException ) {

            mutex.wait( millisecs );
        }

        virtual void wait( long long millisecs, int nanos )
            throw( decaf::lang::exceptions::RuntimeException,
                   decaf::lang::exceptions::IllegalArgumentException,
                   decaf::lang::exceptions::IllegalMonitorStateException,
                   decaf::lang::exceptions::InterruptedException ) {

            mutex.wait( millisecs, nanos );
        }

        virtual void notify() throw( decaf::lang::exceptions::RuntimeException,
                                     decaf::lang::exceptions::IllegalMonitorStateException ) {

            mutex.notify();
        }

        virtual void notifyAll() throw( decaf::lang::exceptions::RuntimeException,
                                        decaf::lang::exceptions::IllegalMonitorStateException ) {

            mutex.notifyAll();
        }

    };

}}

#endif /*_DECAF_IO_OUTPUTSTREAM_H*/
