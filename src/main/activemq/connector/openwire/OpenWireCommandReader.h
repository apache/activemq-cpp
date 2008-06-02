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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECOMMANDREADER_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECOMMANDREADER_H_

#include <activemq/util/Config.h>
#include <activemq/transport/CommandReader.h>
#include <activemq/transport/CommandIOException.h>
#include <activemq/transport/Command.h>
#include <activemq/connector/openwire/OpenWireFormat.h>

#include <decaf/io/InputStream.h>
#include <decaf/io/DataInputStream.h>

namespace activemq{
namespace connector{
namespace openwire{

    class AMQCPP_API OpenWireCommandReader : public transport::CommandReader {
    private:

        /**
         * The target input stream.
         */
        decaf::io::InputStream* inputStream;

        /**
         * Pointer to the OpenWireFormat object for this Reader
         */
        OpenWireFormat* openWireFormat;

        /**
         * This is our DataInputStream that we will wrap around the target
         * input stream
         */
        decaf::io::DataInputStream* dataInputStream;

    public:

        /**
         * Constructor.
         */
        OpenWireCommandReader();

        /**
         * Constructor.
         * @param inputStream the target input stream.
         * @param owFormat the OpenWireFormat object that handles unmarshaling
         */
        OpenWireCommandReader( decaf::io::InputStream* inputStream,
                               OpenWireFormat* openWireFormat );

        virtual ~OpenWireCommandReader();

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
        virtual void setInputStream( decaf::io::InputStream* inputStream ){
            this->inputStream = inputStream;

            if( dataInputStream != NULL ) {
                delete dataInputStream;
            }

            dataInputStream = new decaf::io::DataInputStream( inputStream );
        }

        /**
         * Gets the target input stream.
         * @return Target Input Stream
         */
        virtual decaf::io::InputStream* getInputStream(void){
            return inputStream;
        }

        /**
         * Sets the target input stream.
         * @param Target Input Stream
         */
        virtual void setOpenWireFormat( OpenWireFormat* openWireFormat ){
            this->openWireFormat = openWireFormat;
        }

        /**
         * Gets the target input stream.
         * @return Target Input Stream
         */
        virtual OpenWireFormat* getOpenWireFormat(void){
            return this->openWireFormat;
        }

        /**
         * Attempts to read an array of bytes from the stream.
         * @param buffer The target byte buffer.
         * @param count The number of bytes to read.
         * @return The number of bytes read.
         * @throws IOException thrown if an error occurs.
         */
        virtual size_t read( unsigned char* buffer, size_t count )
            throw( decaf::io::IOException );

        /**
         * Attempts to read a byte from the input stream
         * @return The byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char readByte() throw( decaf::io::IOException );

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECOMMANDREADER_H_*/
