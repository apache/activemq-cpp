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
#ifndef _DECAF_IO_READER_H
#define _DECAF_IO_READER_H

#include <string>
#include <decaf/io/IOException.h>
#include <decaf/io/InputStream.h>
#include <decaf/lang/exceptions/NullPointerException.h>

namespace decaf{
namespace io{

    /*
     * Reader interface that wraps around an input stream and provides
     * an interface for extracting the data from the input stream.
     */
    class DECAF_API Reader {
    public:

        virtual ~Reader(){}

        /**
         * Sets the target input stream.
         */
        virtual void setInputStream( InputStream* is ) = 0;

        /**
         * Gets the target input stream.
         */
        virtual InputStream* getInputStream() = 0;

        /**
         * Attempts to read an array of bytes from the stream.
         * @param buffer The target byte buffer.
         * @param count The number of bytes to read.
         * @return The number of bytes read.
         * @throws IOException thrown if an error occurs.
         */
        virtual std::size_t read( unsigned char* buffer, std::size_t count )
            throw( IOException, lang::exceptions::NullPointerException ) = 0;

        /**
         * Attempts to read a byte from the input stream
         * @return The byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char readByte() throw( IOException ) = 0;

    };

}}

#endif /*_DECAF_IO_READER_H*/
