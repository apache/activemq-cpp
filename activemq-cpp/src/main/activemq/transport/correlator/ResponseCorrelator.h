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

#ifndef _ACTIVEMQ_TRANSPORT_CORRELATOR_RESPONSECORRELATOR_H_
#define _ACTIVEMQ_TRANSPORT_CORRELATOR_RESPONSECORRELATOR_H_

#include <activemq/util/Config.h>
#include <activemq/transport/TransportFilter.h>
#include <activemq/transport/correlator/FutureResponse.h>
#include <activemq/commands/Command.h>
#include <activemq/commands/Response.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <map>
#include <stdio.h>

namespace activemq{
namespace transport{
namespace correlator{

    using decaf::lang::Pointer;
    using activemq::commands::Command;
    using activemq::commands::Response;

    /**
     * This type of transport filter is responsible for correlating
     * asynchronous responses with requests.  Non-response messages
     * are simply sent directly to the CommandListener.  It owns
     * the transport that it
     */
    class AMQCPP_API ResponseCorrelator : public TransportFilter {
    private:

        /**
         * The next command id for sent commands.
         */
        decaf::util::concurrent::atomic::AtomicInteger nextCommandId;

        /**
         * Map of request ids to future response objects.
         */
        std::map<unsigned int, Pointer<FutureResponse> > requestMap;

        /**
         * Sync object for accessing the request map.
         */
        decaf::util::concurrent::Mutex mapMutex;

        /**
         * Flag to indicate the closed state.
         */
        bool closed;

    public:

        /**
         * Constructor.
         * @param next the next transport in the chain
         */
        ResponseCorrelator( const Pointer<Transport>& next );

        virtual ~ResponseCorrelator();

    public:  // Transport Methods

        virtual void oneway( const Pointer<Command>& command );

        virtual Pointer<Response> request( const Pointer<Command>& command );

        virtual Pointer<Response> request( const Pointer<Command>& command, unsigned int timeout );

        virtual void start();

        virtual void close();

        /**
         * This is called in the context of the nested transport's
         * reading thread.  In the case of a response object,
         * updates the request map and notifies those waiting on the
         * response.  Non-response messages are just delegated to
         * the command listener.
         *
         * @param command
         *      The received from the nested transport.
         */
        virtual void onCommand( const Pointer<Command>& command );

        /**
         * Event handler for an exception from a command transport.
         *
         * @param source
         *      The source of the exception.
         * @param ex
         *      The exception that was caught.
         */
        virtual void onTransportException( Transport* source, const decaf::lang::Exception& ex );

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_CORRELATOR_RESPONSECORRELATOR_H_*/
