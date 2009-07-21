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

#ifndef _ACTIVEMQ_TRANSPORT_INACTIVITY_INACTIVITYMONITOR_H_
#define _ACTIVEMQ_TRANSPORT_INACTIVITY_INACTIVITYMONITOR_H_

#include <activemq/util/Config.h>

#include <activemq/transport/TransportFilter.h>
#include <activemq/commands/Command.h>
#include <activemq/commands/Response.h>
#include <activemq/commands/WireFormatInfo.h>

#include <decaf/lang/Pointer.h>
#include <decaf/util/Timer.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>

namespace activemq {
namespace transport {
namespace inactivity {

    using decaf::lang::Pointer;

    class ReadChecker;
    class WriteChecker;

    class AMQCPP_API InactivityMonitor : public TransportFilter {
    private:

        Pointer<commands::WireFormatInfo> localWireFormatInfo;
        Pointer<commands::WireFormatInfo> remoteWireFormatInfo;

        decaf::util::Timer readCheckTimer;
        decaf::util::Timer writeCheckTimer;

        decaf::util::concurrent::atomic::AtomicBoolean commandSent;
        decaf::util::concurrent::atomic::AtomicBoolean commandReceived;

        decaf::util::concurrent::atomic::AtomicBoolean failed;
        decaf::util::concurrent::atomic::AtomicBoolean inRead;
        decaf::util::concurrent::atomic::AtomicBoolean inWrite;

        long long readCheckTime;
        long long writeCheckTime;
        long long initialDelayTime;

        friend class ReadChecker;
        friend class WriteChecker;

    public:

        /**
         * Constructor
         *
         * @param next
         *      The Transport instance that this TransportFilter wraps.
         */
        InactivityMonitor( const Pointer<Transport>& next );

        virtual ~InactivityMonitor();

        virtual void onException( const decaf::lang::Exception& ex );

        virtual void onCommand( const Pointer<Command>& command );

        virtual void oneway( const Pointer<Command>& command )
            throw( decaf::io::IOException, decaf::lang::exceptions::UnsupportedOperationException );

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_INACTIVITY_INACTIVITYMONITOR_H_ */
