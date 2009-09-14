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

#ifndef _DECAF_NET_SOCKETOUTPUTSTREAM_H_
#define _DECAF_NET_SOCKETOUTPUTSTREAM_H_

#include <decaf/io/OutputStream.h>
#include <decaf/net/Socket.h>
#include <decaf/util/concurrent/Mutex.h>

namespace decaf{
namespace net{

    /**
     * Output stream for performing write operations
     * on a socket.
     */
    class DECAF_API SocketOutputStream : public io::OutputStream {
    private:

        // The socket.
        Socket::SocketHandle socket;
        util::concurrent::Mutex mutex;
        bool closed;

    public:

        /**
         * Constructor.
         * @param socket the socket handle.
         */
        SocketOutputStream( Socket::SocketHandle socket );

        virtual ~SocketOutputStream();

        /**
         * Writes a single byte to the output stream.
         * @param c the byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( unsigned char c ) throw ( io::IOException );

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The bytes to write.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const std::vector<unsigned char>& buffer )
            throw ( io::IOException );

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The array of bytes to write.
         * @param offset the position to start writing in buffer.
         * @param len The number of bytes from the buffer to be written.
         * @throws IOException thrown if an error occurs.
         * @throws NullPointerException thrown if buffer is Null.
         */
        virtual void write( const unsigned char* buffer,
                            std::size_t offset,
                            std::size_t len )
            throw ( io::IOException, lang::exceptions::NullPointerException );

        /**
         * Flush - does nothing.
         * @throws IOException
         */
        virtual void flush() throw ( io::IOException ){};

        /**
         * Close - does nothing.  It is the responsibility of the owner
         * of the socket object to close it.
         * @throws IOException
         */
        virtual void close() throw( decaf::io::IOException );

    public:

        virtual void lock() throw( lang::Exception ){
            mutex.lock();
        }

        virtual bool tryLock() throw( lang::Exception ) {
            return mutex.tryLock();
        }

        virtual void unlock() throw( lang::Exception ){
            mutex.unlock();
        }

        virtual void wait() throw( lang::Exception ){
            mutex.wait();
        }

        virtual void wait( long long millisecs ) throw( lang::Exception ) {
            mutex.wait( millisecs );
        }

        virtual void wait( long long millisecs, int nanos ) throw( lang::Exception ) {
            mutex.wait( millisecs, nanos );
        }

        virtual void notify() throw( lang::Exception ){
            mutex.notify();
        }

        virtual void notifyAll() throw( lang::Exception ){
            mutex.notifyAll();
         }

    };

}}

#endif /*_DECAF_NET_SOCKETOUTPUTSTREAM_H_*/
