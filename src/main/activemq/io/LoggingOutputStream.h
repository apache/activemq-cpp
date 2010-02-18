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

#ifndef ACTIVEMQ_IO_LOGGINGOUTPUTSTREAM_H_
#define ACTIVEMQ_IO_LOGGINGOUTPUTSTREAM_H_

#include <activemq/util/Config.h>
#include <decaf/io/FilterOutputStream.h>
#include <decaf/util/logging/LoggerDefines.h>

namespace activemq{
namespace io{

    /**
     * OutputStream filter that just logs the data being
     * written.
     */
    class AMQCPP_API LoggingOutputStream : public decaf::io::FilterOutputStream {
    private:

        LOGDECAF_DECLARE(logger)

    public:

        /**
         * Constructor.
         *
         * @param next
         *      The OutputStream to wrap an write logs to.
         * @param own
         *      If true, this object will control the lifetime of the
         *      output stream that it encapsulates.
         */
        LoggingOutputStream( OutputStream* next, bool own = false );

        virtual ~LoggingOutputStream();

        /**
         * Writes a single byte to the output stream.
         * @param c the byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( unsigned char c ) throw ( decaf::io::IOException );

        /**
         * Writes an array of bytes to the output stream in order starting at buffer[offset]
         * and proceeding until the number of bytes specified by the length argument are
         * written or an error occurs.
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
         * @throws IndexOutOfBoundsException if the offset + length > size.
         */
        virtual void write( const unsigned char* buffer, std::size_t size,
                            std::size_t offset, std::size_t length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IndexOutOfBoundsException );

    private:

        /**
         * Logs the data in the buffer.
         */
        void log( const unsigned char* buffer, std::size_t len );

    };

}}

#endif /*ACTIVEMQ_IO_LOGGINGOUTPUTSTREAM_H_*/
