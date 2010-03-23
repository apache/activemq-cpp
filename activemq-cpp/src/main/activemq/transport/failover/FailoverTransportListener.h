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

#ifndef FAILOVERTRANSPORTLISTENER_H_
#define FAILOVERTRANSPORTLISTENER_H_

#include <activemq/util/Config.h>
#include <activemq/transport/TransportListener.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace transport {
namespace failover {

    class FailoverTransport;

    /**
     * Utility class used by the Transport to perform the work of responding to events
     * from the active Transport.
     *
     * @since 3.0
     */
    class AMQCPP_API FailoverTransportListener : public TransportListener {
    private:

        // The Transport that created this listener
        FailoverTransport* parent;

    private:

        FailoverTransportListener( const FailoverTransportListener& );
        FailoverTransportListener& operator= ( const FailoverTransportListener& );

    public:

        FailoverTransportListener( FailoverTransport* parent );

        virtual ~FailoverTransportListener();

        /**
         * Event handler for the receipt of a command.  The transport passes
         * off all received commands to its listeners, the listener then owns
         * the Object.  If there is no registered listener the Transport deletes
         * the command upon receipt.
         *
         * @param command the received command object.
         */
        virtual void onCommand( const Pointer<Command>& command );

        /**
         * Event handler for an exception from a command transport.
         *
         * @param ex The exception.
         */
        virtual void onException( const decaf::lang::Exception& ex );

        /**
         * The transport has suffered an interruption from which it hopes to recover
         */
        virtual void transportInterrupted();

        /**
         * The transport has resumed after an interruption
         */
        virtual void transportResumed();

    };

}}}

#endif /* FAILOVERTRANSPORTLISTENER_H_ */
