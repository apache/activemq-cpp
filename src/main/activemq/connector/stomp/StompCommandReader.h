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
#ifndef _ACTIVEMQ_CONNECTOR_STOMP_STOMPCOMMANDREADER_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_STOMPCOMMANDREADER_H_

#include <activemq/util/Config.h>
#include <activemq/transport/CommandReader.h>
#include <activemq/transport/CommandIOException.h>
#include <activemq/transport/Command.h>
#include <activemq/connector/stomp/StompFrame.h>
#include <activemq/connector/stomp/StompConnectorException.h>
#include <activemq/connector/stomp/marshal/Marshaler.h>
#include <decaf/io/InputStream.h>

namespace activemq{
namespace connector{
namespace stomp{

    class AMQCPP_API StompCommandReader : public transport::CommandReader {
    private:

        /**
         * The target input stream.
         */
        decaf::io::InputStream* inputStream;

        /**
         * Vector Object used to buffer data
         */
        std::vector<unsigned char> buffer;

        /**
         * Marshaler of Stomp Commands
         */
        marshal::Marshaler marshaler;

    public:

        /**
         * Deafult Constructor
         */
        StompCommandReader();

        /**
         * Constructor.
         * @param is the target input stream.
         */
        StompCommandReader( decaf::io::InputStream* is );

        virtual ~StompCommandReader() {}

        /**
         * Reads a command from the given input stream.
         * @return The next command available on the stream.
         * @throws CommandIOException if a problem occurs during the read.
         */
        virtual transport::Command* readCommand()
            throw ( transport::CommandIOException );

        /**
         * Sets the target input stream.
         * @param Target Input Stream
         */
        virtual void setInputStream( decaf::io::InputStream* is ){
            inputStream = is;
        }

        /**
         * Gets the target input stream.
         * @return Target Input Stream
         */
        virtual decaf::io::InputStream* getInputStream(){
            return inputStream;
        }

        /**
         * Attempts to read an array of bytes from the stream.
         * @param buffer The target byte buffer.
         * @param count The number of bytes to read.
         * @return The number of bytes read.
         * @throws IOException thrown if an error occurs.
         */
        virtual std::size_t read( unsigned char* buffer, std::size_t count )
            throw( decaf::io::IOException );

        /**
         * Attempts to read a byte from the input stream
         * @return The byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char readByte() throw( decaf::io::IOException );

    private:

        /**
         * Read the Stomp Command from the Frame
         * @param reference to a Stomp Frame
         * @throws StompConnectorException
         */
        void readStompCommandHeader( StompFrame& frame )
            throw ( transport::CommandIOException );

        /**
         * Read all the Stomp Headers for the incoming Frame
         * @param Frame to place data into
         * @throws StompConnectorException
         */
        void readStompHeaders( StompFrame& frame )
            throw ( transport::CommandIOException );

        /**
         * Reads a Stomp Header line and stores it in the buffer object
         * @return number of bytes read, zero if there was a problem.
         * @throws StompConnectorException
         */
        std::size_t readStompHeaderLine() throw ( transport::CommandIOException );

        /**
         * Reads the Stomp Body from the Wire and store it in the frame.
         * @param Stomp Frame to place data in
         */
        void readStompBody( StompFrame& frame )
            throw ( transport::CommandIOException );

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_STOMPCOMMANDREADER_H_*/
