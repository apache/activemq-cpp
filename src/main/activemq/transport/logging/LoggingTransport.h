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

#ifndef _ACTIVEMQ_TRANSPORT_LOGGING_LOGGINGTRANSPORT_H_
#define _ACTIVEMQ_TRANSPORT_LOGGING_LOGGINGTRANSPORT_H_

#include <activemq/util/Config.h>
#include <activemq/transport/TransportFilter.h>
#include <decaf/util/logging/LoggerDefines.h>
#include <decaf/lang/Pointer.h>

namespace activemq{
namespace transport{
namespace logging{

    using decaf::lang::Pointer;

    /**
     * A transport filter that logs commands as they are sent/received.
     */
    class AMQCPP_API LoggingTransport : public TransportFilter {
    private:

        LOGDECAF_DECLARE( logger )

    public:

        /**
         * Constructor.
         * @param next - the next Transport in the chain
         */
        LoggingTransport( const Pointer<Transport>& next );

        virtual ~LoggingTransport() {}

        /**
         * Event handler for the receipt of a command.
         * @param command - the received command object.
         */
        virtual void onCommand( const Pointer<Command>& command );

        /**
         * Sends a one-way command.  Does not wait for any response from the
         * broker.
         * @param command the command to be sent.
         * @throws IOException if an exception occurs during writing of
         * the command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual void oneway( const Pointer<Command>& command )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Not supported by this class - throws an exception.
         * @param command the command that is sent as a request
         * @throws UnsupportedOperationException.
         */
        virtual Pointer<Response> request( const Pointer<Command>& command )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Not supported by this class - throws an exception.
         * @param command the command that is sent as a request
         * @param timeout the time to wait for a response.
         * @throws UnsupportedOperationException.
         */
        virtual Pointer<Response> request( const Pointer<Command>& command, unsigned int timeout )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::UnsupportedOperationException );

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_LOGGING_LOGGINGTRANSPORT_H_*/
