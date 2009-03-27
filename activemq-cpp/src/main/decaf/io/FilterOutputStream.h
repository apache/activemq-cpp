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
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/exceptions/NullPointerException.h>

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

        // Synchronization object.
        util::concurrent::Mutex mutex;

        // Indicates if we own the wrapped stream
        bool own;

        // Indicates that this stream was closed
        volatile bool closed;

    public:

        /**
         * Constructor, creates a wrapped output stream
         * @param outputStream the OutputStream to wrap
         * @param own If true, this object will control the lifetime of the
         * output stream that it encapsulates.
         */
        FilterOutputStream( OutputStream* outputStream, bool own = false ){
            this->outputStream = outputStream;
            this->own = own;
            this->closed = false;
        }

        virtual ~FilterOutputStream() {
            try {
                this->close();

                if( own ) {
                    delete outputStream;
                }
                outputStream = NULL;
            }
            DECAF_CATCH_NOTHROW( IOException )
            DECAF_CATCHALL_NOTHROW( )
        }

        /**
         * Writes a single byte to the output stream.  The write method of
         * FilterOutputStream calls the write method of its underlying output
         * stream, that is, it performs out.write(b).
         * @param c the byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( unsigned char c ) throw ( IOException ) {
            try {

                if( isClosed() ) {
                    throw IOException(
                        __FILE__, __LINE__,
                        "FilterOutputStream::write - Stream is closed" );
                }

                outputStream->write( c );
            }
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCHALL_THROW( IOException )
        }

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The bytes to write.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const std::vector<unsigned char>& buffer )
            throw ( IOException ) {

            try {

                if( isClosed() ) {
                    throw IOException(
                        __FILE__, __LINE__,
                        "FilterOutputStream::write - Stream is closed" );
                }

                for( std::size_t ix = 0; ix < buffer.size(); ++ix ) {
                    outputStream->write( buffer[ix] );
                }
            }
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCHALL_THROW( IOException )
        }

        /**
         * Writes an array of bytes to the output stream.  The write method of
         * FilterOutputStream calls the write method of one argument on each
         * byte to output.
         * @param buffer The array of bytes to write.
         * @param offset, the position to start writing in buffer.
         * @param len The number of bytes from the buffer to be written.
         * @throws IOException thrown if an error occurs.
         * @throws NullPointerException thrown if buffer is Null.
         */
        virtual void write( const unsigned char* buffer,
                            std::size_t offset DECAF_UNUSED,
                            std::size_t len )
            throw ( IOException,
                    lang::exceptions::NullPointerException ) {
            try {

                if( isClosed() ) {
                    throw IOException(
                        __FILE__, __LINE__,
                        "FilterOutputStream::write - Stream is closed" );
                }

                if( buffer == NULL ) {
                    throw lang::exceptions::NullPointerException(
                        __FILE__, __LINE__,
                        "FilterOutputStream::write - Buffer passed is Null.");
                }

                for( std::size_t ix = 0; ix < len; ++ix ) {
                    outputStream->write( buffer[ix] );
                }
            }
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCHALL_THROW( IOException )
        }

        /**
         * Flushes any pending writes in this output stream.
         * The flush method of FilterOutputStream calls the flush method
         * of its underlying output stream
         * @throws IOException
         */
        virtual void flush() throw ( IOException ) {
            try {

                if( isClosed() ) {
                    throw IOException(
                        __FILE__, __LINE__,
                        "FilterOutputStream::flush - Stream is closed" );
                }

                outputStream->flush();
            }
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCHALL_THROW( IOException )
        }

        /**
         * Close the Stream.  The close method of FilterOutputStream calls its
         * flush method, and then calls the close method of its underlying output
         * stream, it then destroys the output stream if it is the owner.
         * @throws Exception
         */
        virtual void close() throw ( lang::Exception ) {
            try {
                if( !closed && outputStream != NULL ) {
                    outputStream->flush();
                    outputStream->close();
                }
                this->closed = true;
            }
            DECAF_CATCH_RETHROW( IOException )
            DECAF_CATCHALL_THROW( IOException )
        }

    public:  // Synchronizable

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void lock() throw( lang::Exception ){
            mutex.lock();
        }

        /**
         * Unlocks the object.
         * @throws Exception
         */
        virtual void unlock() throw( lang::Exception ){
            mutex.unlock();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void wait() throw( lang::Exception ){
            mutex.wait();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param millisecs the time in millisecsonds to wait, or WAIT_INIFINITE
         * @throws Exception
         */
        virtual void wait( unsigned long millisecs ) throw( lang::Exception ){
            mutex.wait( millisecs );
        }

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notify() throw( lang::Exception ){
            mutex.notify();
        }

        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notifyAll() throw( lang::Exception ){
            mutex.notifyAll();
        }

    protected:

        /**
         * @returns true if this stream has been closed.
         */
        virtual bool isClosed() const {
            return this->closed || this->outputStream == NULL;
        }

    };

}}

#endif /*_DECAF_IO_FILTEROUTPUTSTREAM_H_*/
