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

#ifndef _ACTIVEMQ_WIREFORMAT_STOMP_MARSHALER_H_
#define _ACTIVEMQ_WIREFORMAT_STOMP_MARSHALER_H_

#include <activemq/util/Config.h>
#include <activemq/commands/Command.h>
#include <activemq/wireformat/stomp/StompFrame.h>
#include <decaf/io/IOException.h>
#include <decaf/lang/Pointer.h>

namespace activemq{
namespace wireformat{
namespace stomp{
namespace marshal{

    using decaf::lang::Pointer;
    using activemq::commands::Command;

    /**
     * Interface for all marshalers between Commands and Stomp frames.
     */
    class AMQCPP_API Marshaler {
    public:

        Marshaler() {}
        virtual ~Marshaler() {}

        /**
         * Marshal a Stomp Frame read in from the Transport to a Command object.
         * @param frame Frame to Marshal.
         * @return Newly Marshaled Command.
         * @throws IOException
         */
        virtual Pointer<Command> marshal( const Pointer<StompFrame>& frame )
            throw ( decaf::io::IOException );

        /**
         * Marshal a Command to a Stomp Frame to write out to the Transport.
         * @param command The Command to Marshal.
         * @return newly Marshaled Stomp Frame.
         * @throws IOException
         */
        virtual Pointer<StompFrame> marshal( const Pointer<Command>& command )
            throw ( decaf::io::IOException );

    private:

        Pointer<Command> unmarshalMessage( const Pointer<StompFrame>& frame );
        Pointer<Command> unmarshalReceipt( const Pointer<StompFrame>& frame );
        Pointer<Command> unmarshalConnected( const Pointer<StompFrame>& frame );
        Pointer<Command> unmarshalError( const Pointer<StompFrame>& frame );

        Pointer<StompFrame> marshalMessage( const Pointer<Command>& command );
        Pointer<StompFrame> marshalAck( const Pointer<Command>& command );
        Pointer<StompFrame> marshalConnect( const Pointer<Command>& command );
        Pointer<StompFrame> marshalTransactionInfo( const Pointer<Command>& command );
        Pointer<StompFrame> marshalShutdownInfo( const Pointer<Command>& command );
        Pointer<StompFrame> marshalRemoveInfo( const Pointer<Command>& command );

    };

}}}}

#endif /*_ACTIVEMQ_WIREFORMAT_STOMP_MARSHALER_H_*/
