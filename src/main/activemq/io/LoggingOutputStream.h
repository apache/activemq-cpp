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

    protected:

        virtual void doWriteByte( unsigned char c );

        virtual void doWriteArrayBounded( const unsigned char* buffer, int size, int offset, int length );

    private:

        /**
         * Logs the data in the buffer.
         */
        void log( const unsigned char* buffer, int len );

    };

}}

#endif /*ACTIVEMQ_IO_LOGGINGOUTPUTSTREAM_H_*/
