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

#ifndef _ACTIVEMQ_IO_LOGGINGINPUTSTREAM_H_
#define _ACTIVEMQ_IO_LOGGINGINPUTSTREAM_H_

#include <activemq/util/Config.h>
#include <decaf/io/FilterInputStream.h>
#include <decaf/util/logging/LoggerDefines.h>
#include <decaf/lang/exceptions/NullPointerException.h>

namespace activemq{
namespace io{

    class AMQCPP_API LoggingInputStream : public decaf::io::FilterInputStream {
    private:

        LOGDECAF_DECLARE(logger)

    public:

        /**
         * Creates a DataInputStream that uses the specified underlying
         * InputStream.
         * @param inputStream the InputStream instance to wrap.
         * @param own, indicates if this class owns the wrapped string
         * defaults to false.
         */
        LoggingInputStream( decaf::io::InputStream* inputStream, bool own = false );

        virtual ~LoggingInputStream();

        /**
         * Reads a single byte from the buffer.  Blocks until
         * data is available.
         * @return The next byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char read() throw ( decaf::io::IOException );

        /**
         * Reads an array of bytes from the buffer.  Blocks until
         * the requested number of bytes are available.
         * @param buffer (out) the target buffer.
         * @param offset the position in the buffer to start at
         * @param bufferSize the size of the output buffer.
         * @return The number of bytes read or -1 if EOF is detected
         * @throws IOException thrown if an error occurs.
         * @throws NullPointerException if buffer is null
         */
        virtual int read( unsigned char* buffer,
                          std::size_t offset,
                          std::size_t bufferSize )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException );

    private:

        /**
         * Logs the data in the buffer.
         */
        void log( const unsigned char* buffer, std::size_t len );

    };

}}
#endif /*_ACTIVEMQ_IO_LOGGINGINPUTSTREAM_H_*/
