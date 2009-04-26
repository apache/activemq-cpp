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

#ifndef _ACTIVEMQ_WIREFORMAT_STOMP_STOMPWIREFORMAT_H_
#define _ACTIVEMQ_WIREFORMAT_STOMP_STOMPWIREFORMAT_H_

#include <activemq/util/Config.h>
#include <activemq/wireformat/WireFormat.h>
#include <activemq/wireformat/stomp/StompFrame.h>
#include <activemq/wireformat/stomp/marshal/Marshaler.h>
#include <decaf/io/IOException.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace wireformat {
namespace stomp {

    using decaf::lang::Pointer;
    using activemq::commands::Command;

    class AMQCPP_API StompWireFormat : public WireFormat {
    private:

        /**
         * Vector Object used to buffer data
         */
        std::vector<unsigned char> buffer;

        /**
         * Marshaler of Stomp Commands
         */
        marshal::Marshaler marshaler;

    public:

        StompWireFormat();
        virtual ~StompWireFormat();

        /**
         * Stream based marshaling
         * @param command - The Command to Marshal
         * @param out - the output stream to write the command to.
         * @throws IOException
         */
        virtual void marshal( const Pointer<Command>& command, decaf::io::DataOutputStream* out )
            throw ( decaf::io::IOException );

        /**
         * Packet based un-marshaling
         * @param in - the input stream to read the command from.
         * @returns the newly marshaled Command, caller owns the pointer
         * @throws IOException
         */
        virtual Pointer<Command> unmarshal( decaf::io::DataInputStream* in )
            throw ( decaf::io::IOException );

        /**
         * Set the Version
         * @param the version of the wire format
         */
        virtual void setVersion( int version ) {}

        /**
         * Get the Version
         * @return the version of the wire format
         */
        virtual int getVersion() const {
            return 1;
        }

        /**
         * Returns true if this WireFormat has a Negotiator that needs to wrap the
         * Transport that uses it.
         * @returns true if the WireFormat provides a Negotiator.
         */
        virtual bool hasNegotiator() const {
            return false;
        }

        /**
         * If the Transport Provides a Negotiator this method will create and return
         * a news instance of the Negotiator.
         * @returns new instance of a WireFormatNegotiator.
         * @throws UnsupportedOperationException if the WireFormat doesn't have a Negotiator.
         */
        virtual Pointer<transport::Transport> createNegotiator(
            const Pointer<transport::Transport>& transport )
                throw( decaf::lang::exceptions::UnsupportedOperationException );

    private:

        /**
         * Read the Stomp Command from the Frame
         * @param reference to a Stomp Frame
         * @throws StompConnectorException
         */
        void readStompCommandHeader( StompFrame& frame, decaf::io::DataInputStream* in )
            throw ( decaf::io::IOException );

        /**
         * Read all the Stomp Headers for the incoming Frame
         * @param Frame to place data into
         * @throws StompConnectorException
         */
        void readStompHeaders( StompFrame& frame, decaf::io::DataInputStream* in )
            throw ( decaf::io::IOException );

        /**
         * Reads a Stomp Header line and stores it in the buffer object
         * @return number of bytes read, zero if there was a problem.
         * @throws StompConnectorException
         */
        std::size_t readStompHeaderLine( decaf::io::DataInputStream* in ) throw ( decaf::io::IOException );

        /**
         * Reads the Stomp Body from the Wire and store it in the frame.
         * @param Stomp Frame to place data in
         */
        void readStompBody( StompFrame& frame, decaf::io::DataInputStream* in )
            throw ( decaf::io::IOException );

    };

}}}

#endif /* _ACTIVEMQ_WIREFORMAT_STOMP_STOMPWIREFORMAT_H_ */
