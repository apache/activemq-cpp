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

#ifndef _DECAF_INTERNAL_IO_STANDARDOUTPUTSTREAM_H_
#define _DECAF_INTERNAL_IO_STANDARDOUTPUTSTREAM_H_

#include <decaf/util/Config.h>
#include <decaf/io/OutputStream.h>
#include <decaf/util/concurrent/Mutex.h>

namespace decaf {
namespace internal {
namespace io {

    class DECAF_API StandardOutputStream : public decaf::io::OutputStream {
    private:

        /**
         * Synchronization object.
         */
        util::concurrent::Mutex mutex;

    public:

        StandardOutputStream();
        virtual ~StandardOutputStream();

        /**
         * Writes a single byte to the output stream.
         * @param c the byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( unsigned char c )
            throw ( decaf::io::IOException );

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The bytes to write.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const std::vector<unsigned char>& buffer )
            throw ( decaf::io::IOException );

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The array of bytes to write.
         * @param offset, the position to start writing in buffer.
         * @param len The number of bytes from the buffer to be written.
         * @throws IOException thrown if an error occurs.
         * @throws NullPointerException if buffer is null.
         */
        virtual void write( const unsigned char* buffer,
                            std::size_t offset,
                            std::size_t len )
            throw ( decaf::io::IOException, lang::exceptions::NullPointerException );

        /**
         * Invokes flush on the target output stream.
         * throws IOException if an error occurs
         */
        virtual void flush() throw ( decaf::io::IOException );

        /**
         * Invokes close on the target output stream.
         * throws IOException if an error occurs
         */
        virtual void close() throw( decaf::io::IOException ){
            this->flush();
        }

    public:

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

}}}

#endif /* _DECAF_INTERNAL_IO_STANDARDOUTPUTSTREAM_H_ */
