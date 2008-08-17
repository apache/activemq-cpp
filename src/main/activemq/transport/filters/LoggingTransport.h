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

#ifndef ACTIVEMQ_TRANSPORT_FILTERS_LOGGINGTRANSPORT_H_
#define ACTIVEMQ_TRANSPORT_FILTERS_LOGGINGTRANSPORT_H_

#include <activemq/util/Config.h>
#include <activemq/transport/TransportFilter.h>
#include <decaf/util/logging/LoggerDefines.h>

namespace activemq{
namespace transport{
namespace filters{

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
         * @param own - true if this filter owns the next and should delete it
         */
        LoggingTransport( Transport* next, bool own = true );

        virtual ~LoggingTransport() {}

        /**
         * Event handler for the receipt of a command.
         * @param command - the received command object.
         */
        virtual void onCommand( Command* command );

        /**
         * Sends a one-way command.  Does not wait for any response from the
         * broker.
         * @param command the command to be sent.
         * @throws CommandIOException if an exception occurs during writing of
         * the command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual void oneway( Command* command )
            throw( CommandIOException,
                   decaf::lang::exceptions::UnsupportedOperationException);

        /**
         * Not supported by this class - throws an exception.
         * @param command the command that is sent as a request
         * @throws UnsupportedOperationException.
         */
        virtual Response* request( Command* command )
            throw( CommandIOException,
                   decaf::lang::exceptions::UnsupportedOperationException);

        /**
         * Not supported by this class - throws an exception.
         * @param command the command that is sent as a request
         * @param timeout the time to wait for a response.
         * @throws UnsupportedOperationException.
         */
        virtual Response* request( Command* command, unsigned int timeout )
            throw( CommandIOException,
                   decaf::lang::exceptions::UnsupportedOperationException);

    };

}}}

#endif /*ACTIVEMQ_TRANSPORT_FILTERS_LOGGINGTRANSPORT_H_*/
