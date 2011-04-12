/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_WIREFORMAT_WIREFORMAT_H_
#define _ACTIVEMQ_WIREFORMAT_WIREFORMAT_H_

#include <activemq/wireformat/WireFormatNegotiator.h>

#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/io/IOException.h>
#include <decaf/lang/Pointer.h>

#include <activemq/util/Config.h>
#include <activemq/commands/Command.h>
#include <activemq/transport/Transport.h>

#include <decaf/lang/exceptions/UnsupportedOperationException.h>

namespace activemq{
namespace wireformat{

    using decaf::lang::Pointer;

    /**
     * Provides a mechanism to marshal commands into and out of packets
     * or into and out of streams, Channels and Datagrams.
     *
     * @version $Revision: 1.1 $
     */
    class AMQCPP_API WireFormat {
    public:

        virtual ~WireFormat() {}

        /**
         * Stream based marshaling of a Command, this method blocks until the entire
         * Command has been written out to the output stream.
         *
         * @param command
         *      The Command to Marshal
         * @param transport
         *      The Transport that called this method.
         * @param out
         *      The output stream to write the command to.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void marshal( const Pointer<commands::Command>& command,
                              const activemq::transport::Transport* transport,
                              decaf::io::DataOutputStream* out ) = 0;

        /**
         * Stream based unmarshaling, blocks on reads on the input stream until a complete
         * command has been read and unmarshaled into the correct form.  Returns a Pointer
         * to the newly unmarshaled Command.
         *
         * @param transport
         *      Pointer to the transport that is making this request.
         * @param in
         *      The input stream to read the command from.
         *
         * @returns the newly marshaled Command, caller owns the pointer
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual Pointer<commands::Command> unmarshal( const activemq::transport::Transport* transport,
                                                      decaf::io::DataInputStream* in ) = 0;

        /**
         * Set the Version
         *
         * @param version
         *      the version of the wire format
         */
        virtual void setVersion( int version ) = 0;

        /**
         * Get the Version
         *
         * @return the version of the wire format
         */
        virtual int getVersion() const = 0;

        /**
         * Returns true if this WireFormat has a Negotiator that needs to wrap the
         * Transport that uses it.
         * @returns true if the WireFormat provides a Negotiator.
         */
        virtual bool hasNegotiator() const = 0;

        /**
         * Indicates if the WireFromat object is in the process of receiving a message.  This
         * is useful for monitoring inactivity and the WireFormat is processing a large message
         * which takes longer than some configured timeout to unmarshal, the inactivity monitor
         * can query the WireFormat instance to determine if its busy or not and not mark the
         * connection as inactive if so.
         *
         * @returns true if the WireFormat object is unmarshaling a message.
         */
        virtual bool inReceive() const = 0;

        /**
         * If the Transport Provides a Negotiator this method will create and return
         * a new instance of the Negotiator.
         *
         * @param transport
         *      The Transport to Wrap the Negotiator around.
         *
         * @returns new instance of a WireFormatNegotiator as a Pointer<Transport>.
         *
         * @throws UnsupportedOperationException if the WireFormat doesn't have a Negotiator.
         */
        virtual Pointer<transport::Transport> createNegotiator(
            const Pointer<transport::Transport>& transport ) = 0;

    };

}}

#endif /*_ACTIVEMQ_WIREFORMAT_WIREFORMAT_H_*/
