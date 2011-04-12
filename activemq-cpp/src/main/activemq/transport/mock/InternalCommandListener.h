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

#ifndef _ACTIVEMQ_TRANSPORT_MOCK_INTERNALCOMMANDLISTENER_H_
#define _ACTIVEMQ_TRANSPORT_MOCK_INTERNALCOMMANDLISTENER_H_

#include <activemq/util/Config.h>
#include <activemq/transport/mock/ResponseBuilder.h>
#include <activemq/transport/DefaultTransportListener.h>

#include <decaf/lang/Thread.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/LinkedList.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/concurrent/CountDownLatch.h>

namespace activemq {
namespace transport {
namespace mock {

    class MockTransport;

    /**
     * Listens for Commands sent from the MockTransport.  This class
     * processes all outbound commands and sends responses that are
     * constructed by calling the Protocol provided ResponseBuilder
     * and getting a set of Commands to send back into the MockTransport
     * as incoming Commands and Responses.
     */
    class AMQCPP_API InternalCommandListener : public DefaultTransportListener,
                                               public decaf::lang::Thread {
    private:

        MockTransport* transport;
        Pointer<ResponseBuilder> responseBuilder;
        bool done;
        decaf::util::concurrent::CountDownLatch startedLatch;
        decaf::util::LinkedList< Pointer<Command> > inboundQueue;

    private:

        InternalCommandListener( const InternalCommandListener& );
        InternalCommandListener operator= ( const InternalCommandListener& );

    public:

        InternalCommandListener();

        virtual ~InternalCommandListener();

        void setTransport( MockTransport* transport ) {
            this->transport = transport;
        }

        void setResponseBuilder( const Pointer<ResponseBuilder>& responseBuilder ) {
            this->responseBuilder = responseBuilder;
        }

        virtual void onCommand( const Pointer<Command>& command );

        void run();

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_MOCK_INTERNALCOMMANDLISTENER_H_ */
