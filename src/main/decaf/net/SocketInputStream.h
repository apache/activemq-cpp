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

#ifndef _DECAF_NET_SOCKETINPUTSTREAM_H_
#define _DECAF_NET_SOCKETINPUTSTREAM_H_

#include <decaf/io/InputStream.h>
#include <decaf/net/Socket.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/NullPointerException.h>

namespace decaf{
namespace net{

    /**
     * Input stream for performing reads on a socket.  This
     * class will only work properly for blocking sockets.
     */
    class DECAF_API SocketInputStream : public io::InputStream {
    private:

        // The socket handle.
        Socket::SocketHandle socket;
        util::concurrent::Mutex mutex;
        bool closed;

    public:

        /**
         * Constructor.
         * @param socket the socket handle.
         */
        SocketInputStream( Socket::SocketHandle socket );

        /**
         * Destructor.
         */
        virtual ~SocketInputStream();

        /**
         * Returns the number of bytes available on the socket to
         * be read right now.
         * @return The number of bytes currently available to
         * be read on the socket.
         */
        virtual std::size_t available() const throw ( io::IOException );

        /**
         * Reads a single byte from the buffer.  If no data
         * is available, blocks until their is.
         * @return The next byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char read() throw ( io::IOException );

        /**
         * Reads an array of bytes from the buffer.  If the desired amount
         * of data is not currently available, this operation
         * will block until the appropriate amount of data is available.
         * @param buffer (out) the target buffer
         * @param offset the position in the buffer to start from.
         * @param bufferSize the size of the output buffer.
         * @return the number of bytes read. or -1 if EOF
         * @throws IOException f an error occurs.
         */
        virtual int read( unsigned char* buffer,
                          std::size_t offset,
                          std::size_t bufferSize )
            throw ( io::IOException, lang::exceptions::NullPointerException );

        /**
         * Close - does nothing.  It is the responsibility of the owner
         * of the socket object to close it.
         * @throws CMSException
         */
        virtual void close() throw( lang::Exception );

        /**
         * Not supported.
         * @throws an UnsupportedOperationException.
         */
        virtual std::size_t skip( std::size_t num )
            throw ( io::IOException,
                    lang::exceptions::UnsupportedOperationException );

        /**
         * Marks the current position in the stream A subsequent call to the
         * reset method repositions this stream at the last marked position so
         * that subsequent reads re-read the same bytes.
         *
         * If a stream instance reports that marks are supported then the stream
         * will ensure that the same bytes can be read again after the reset method
         * is called so long the readLimit is not reached.
         * @param readLimit - max bytes read before marked position is invalid.
         */
        virtual void mark( int readLimit DECAF_UNUSED ) {}

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
         * If the method markSupported returns false, then:
         *   * The call to reset may throw an IOException.
         *   * If an IOException is not thrown, then the stream is reset to a fixed
         *     state that depends on the particular type of the input stream and how
         *     it was created. The bytes that will be supplied to subsequent callers
         *     of the read method depend on the particular type of the input stream.
         * @throws IOException
         */
        virtual void reset() throw ( io::IOException ) {
            throw io::IOException(
                __FILE__, __LINE__,
                "SocketInputStream::reset - Mark is not supported" );
        }

        /**
         * Determines if this input stream supports the mark and reset methods.
         * Whether or not mark and reset are supported is an invariant property of
         * a particular input stream instance.
         * @returns true if this stream instance supports marks
         */
        virtual bool markSupported() const {
            return false;
        }

    public:

        /**
         * Locks the object.
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
         * @param millisecs time in millisecsonds to wait, or WAIT_INIFINITE
         * @throws Exception
         */
        virtual void wait( unsigned long millisecs )
            throw( lang::Exception ) {

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

    };

}}

#endif /*_DECAF_NET_SOCKETINPUTSTREAM_H_*/
