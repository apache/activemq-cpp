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
#include <activemq/wireformat/stomp/StompHelper.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/io/IOException.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace wireformat {
namespace stomp {

    using decaf::lang::Pointer;
    using activemq::commands::Command;

    class StompWireformatProperties;

    class AMQCPP_API StompWireFormat : public WireFormat {
    private:

        /**
         * Performs conversions for stomp types and canonical types.
         */
        StompHelper helper;

        // Stored after we connect to use when validating that a durable subscribe
        // and unsubscribe are set to use the client Id.
        std::string clientId;

        // Indicates when we are in the doUnmarshal call
        decaf::util::concurrent::atomic::AtomicBoolean receiving;

        // Internal structure for holding class internal data that can change without
        // affecting binary compatibility.
        StompWireformatProperties* properties;

    public:

        StompWireFormat();

        virtual ~StompWireFormat();

    public:

        /**
         * Stream based marshaling of a Command, this method blocks until the entire
         * Command has been written out to the output stream.
         *
         * @param command
         *      The Command to Marshal to the output stream.
         * @param transport
         *      The Transport that initiated this marshal call.
         * @param out
         *      The output stream to write the command to.
         *
         * @throws IOException
         */
        virtual void marshal( const Pointer<commands::Command>& command,
                              const activemq::transport::Transport* transport,
                              decaf::io::DataOutputStream* out );

        /**
         * Stream based un-marshaling, blocks on reads on the input stream until a complete
         * command has been read and unmarshaled into the correct form.  Returns a Pointer
         * to the newly unmarshaled Command.
         *
         * @param transport - Pointer to the transport that is making this request.
         * @param in - the input stream to read the command from.
         * @returns the newly marshaled Command, caller owns the pointer
         * @throws IOException
         */
        virtual Pointer<commands::Command> unmarshal( const activemq::transport::Transport* transport,
                                                      decaf::io::DataInputStream* in );

        /**
         * Set the Version
         * @param the version of the wire format
         */
        virtual void setVersion( int version AMQCPP_UNUSED ) {}

        /**
         * Get the Version
         * @return the version of the wire format
         */
        virtual int getVersion() const {
            return 1;
        }

        /**
         * Is there a Message being unmarshaled?
         *
         * @return true while in the doUnmarshal method.
         */
        virtual bool inReceive() const {
            return this->receiving.get();
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
            const Pointer<transport::Transport>& transport );

    private:

        Pointer<Command> unmarshalMessage( const Pointer<StompFrame>& frame );
        Pointer<Command> unmarshalReceipt( const Pointer<StompFrame>& frame );
        Pointer<Command> unmarshalConnected( const Pointer<StompFrame>& frame );
        Pointer<Command> unmarshalError( const Pointer<StompFrame>& frame );

        Pointer<StompFrame> marshalMessage( const Pointer<Command>& command );
        Pointer<StompFrame> marshalAck( const Pointer<Command>& command );
        Pointer<StompFrame> marshalConnectionInfo( const Pointer<Command>& command );
        Pointer<StompFrame> marshalTransactionInfo( const Pointer<Command>& command );
        Pointer<StompFrame> marshalShutdownInfo( const Pointer<Command>& command );
        Pointer<StompFrame> marshalRemoveInfo( const Pointer<Command>& command );
        Pointer<StompFrame> marshalConsumerInfo( const Pointer<Command>& command );
        Pointer<StompFrame> marshalRemoveSubscriptionInfo( const Pointer<Command>& command );

    };

}}}

#endif /* _ACTIVEMQ_WIREFORMAT_STOMP_STOMPWIREFORMAT_H_ */
