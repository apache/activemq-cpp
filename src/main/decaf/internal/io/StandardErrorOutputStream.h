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
#ifndef _DECAF_INTERNAL_IO_STANDARDERROROUTPUTSTREAM_H_
#define _DECAF_INTERNAL_IO_STANDARDERROROUTPUTSTREAM_H_

#include <decaf/util/Config.h>
#include <decaf/io/OutputStream.h>
#include <decaf/util/concurrent/Mutex.h>

namespace decaf{
namespace internal{
namespace io{

    /**
     * Wrapper Around the Standard error Output facility on the current
     * platform.  This allows for the use of alternate output methods on
     * platforms or compilers that do not support <code>std::cerr</code>.
     */
    class DECAF_API StandardErrorOutputStream : public decaf::io::OutputStream {
    private:

        /**
         * Synchronization object.
         */
        util::concurrent::Mutex mutex;

    public:

        /**
         * Default Constructor
         */
        StandardErrorOutputStream();

        virtual ~StandardErrorOutputStream();

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
         * @throws decaf::io::IOException thrown if an error occurs.
         * @throws NullPointerException if buffer is null.
         */
        virtual void write( const unsigned char* buffer,
                            std::size_t offset,
                            std::size_t len )
            throw ( decaf::io::IOException, lang::exceptions::NullPointerException );

        /**
         * Invokes flush on the target output stream.
         * throws decaf::io::IOException if an error occurs
         */
        virtual void flush() throw ( decaf::io::IOException );

        /**
         * Invokes close on the target output stream.
         * throws CMSException if an error occurs
         */
        virtual void close() throw( decaf::lang::Exception ){
            this->flush();
        }

    public:

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void lock() throw( decaf::lang::Exception ){
            mutex.lock();
        }

        /**
         * Unlocks the object.
         * @throws Exception
         */
        virtual void unlock() throw( decaf::lang::Exception ){
            mutex.unlock();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void wait() throw( decaf::lang::Exception ){
            mutex.wait();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param time in milliseconds to wait, or WAIT_INIFINITE
         * @throws Exception
         */
        virtual void wait( unsigned long millisecs ) throw( decaf::lang::Exception ){
            mutex.wait( millisecs );
        }

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notify() throw( decaf::lang::Exception ){
            mutex.notify();
        }

        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notifyAll() throw( decaf::lang::Exception ){
            mutex.notifyAll();
        }

    };

}}}

#endif /*_DECAF_INTERNAL_IO_STANDARDERROROUTPUTSTREAM_H_*/
