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
#ifndef _ACTIVEMQ_CONNECTOR_STOMP_STOMPCOMMANDWRITER_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_STOMPCOMMANDWRITER_H_

#include <activemq/util/Config.h>
#include <activemq/transport/CommandWriter.h>
#include <activemq/transport/CommandIOException.h>
#include <activemq/connector/stomp/StompConnectorException.h>
#include <activemq/connector/stomp/marshal/Marshaler.h>
#include <activemq/transport/Command.h>
#include <decaf/io/OutputStream.h>
#include <decaf/io/InputStream.h>

namespace activemq{
namespace connector{
namespace stomp{

    class AMQCPP_API StompCommandWriter : public transport::CommandWriter {
    private:

        /**
         * Target output stream.
         */
        decaf::io::OutputStream* outputStream;

        /**
         * Marshaler of Stomp Commands
         */
        marshal::Marshaler marshaler;

    public:

        /**
         * Default Constructor
         */
        StompCommandWriter();

        /**
         * Constructor.
         * @param os the target output stream.
         */
        StompCommandWriter( decaf::io::OutputStream* os );

        virtual ~StompCommandWriter() {}

        /**
         * Sets the target output stream.
         */
        virtual void setOutputStream( decaf::io::OutputStream* os ){
            outputStream = os;
        }

        /**
         * Gets the target output stream.
         */
        virtual decaf::io::OutputStream* getOutputStream(){
            return outputStream;
        }

        /**
         * Writes a command to the given output stream.
         * @param command the command to write.
         * @param os the target stream for the write.
         * @throws CommandIOException if a problem occurs during the write.
         */
        virtual void writeCommand( transport::Command* command )
            throw ( transport::CommandIOException );

        /**
         * Writes a byte array to the output stream.
         * @param buffer a byte array
         * @param count the number of bytes in the array to write.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const unsigned char* buffer, std::size_t count )
            throw( decaf::io::IOException );

        /**
         * Writes a byte to the output stream.
         * @param v The value to be written.
         * @throws IOException thrown if an error occurs.
         */
        virtual void writeByte( unsigned char v ) throw( decaf::io::IOException );

    private:

        /**
         * Writes a char array to the output stream.
         * @param buffer a char array
         * @param count the number of bytes in the array to write.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const char* buffer, std::size_t count )
            throw( decaf::io::IOException );

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_STOMPCOMMANDWRITER_H_*/
