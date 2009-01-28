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

#include <activemq/util/Config.h>
#include <activemq/commands/Command.h>
#include <activemq/transport/Transport.h>

#include <decaf/lang/exceptions/UnsupportedOperationException.h>

namespace activemq{
namespace wireformat{

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
         * Stream based marshaling
         * @param command - The Command to Marshal
         * @param out - the output stream to write the command to.
         * @throws IOException
         */
        virtual void marshal( commands::Command* command, decaf::io::DataOutputStream* out )
            throw ( decaf::io::IOException ) = 0;

        /**
         * Packet based un-marshaling
         * @param in - the input stream to read the command from.
         * @returns the newly marshaled Command, caller owns the pointer
         * @throws IOException
         */
        virtual commands::Command* unmarshal( decaf::io::DataInputStream* in )
            throw ( decaf::io::IOException ) = 0;

        /**
         * Set the Version
         * @param the version of the wire format
         */
        virtual void setVersion( int version ) = 0;

        /**
         * Get the Version
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
         * If the Transport Provides a Negotiator this method will create and return
         * a news instance of the Negotiator.
         * @returns new instance of a WireFormatNegotiator.
         * @throws UnsupportedOperationException if the WireFormat doesn't have a Negotiator.
         */
        virtual WireFormatNegotiator* createNegotiator( transport::Transport* transport )
            throw( decaf::lang::exceptions::UnsupportedOperationException ) = 0;

    };

}}

#endif /*_ACTIVEMQ_WIREFORMAT_WIREFORMAT_H_*/
