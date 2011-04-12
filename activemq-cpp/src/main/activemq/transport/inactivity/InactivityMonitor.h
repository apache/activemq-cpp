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
#include <activemq/wireformat/WireFormat.h>

#include <decaf/lang/Pointer.h>
#include <decaf/util/Timer.h>
#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>

namespace activemq {
namespace transport {
namespace inactivity {

    using decaf::lang::Pointer;

    class ReadChecker;
    class WriteChecker;
    class AsyncSignalReadErrorkTask;
    class AsyncWriteTask;
    class InactivityMonitorData;

    class AMQCPP_API InactivityMonitor : public TransportFilter {
    private:

        // Internal Class used to house the data structures for this object
        InactivityMonitorData* members;

        friend class ReadChecker;
        friend class AsyncSignalReadErrorkTask;
        friend class WriteChecker;
        friend class AsyncWriteTask;

    private:

        InactivityMonitor( const InactivityMonitor& );
        InactivityMonitor operator= ( const InactivityMonitor& );

    public:

        InactivityMonitor( const Pointer<Transport>& next,
                           const Pointer<wireformat::WireFormat>& wireFormat );

        InactivityMonitor( const Pointer<Transport>& next,
                           const decaf::util::Properties& properties,
                           const Pointer<wireformat::WireFormat>& wireFormat );

        virtual ~InactivityMonitor();

    public:  // TransportFilter Methods

        virtual void close();

        virtual void onException( const decaf::lang::Exception& ex );

        virtual void onCommand( const Pointer<Command>& command );

        virtual void oneway( const Pointer<Command>& command );

    public:

        bool isKeepAliveResponseRequired() const;

        void setKeepAliveResponseRequired( bool value );

        long long getReadCheckTime() const;

        void setReadCheckTime( long long value );

        long long getWriteCheckTime() const;

        void setWriteCheckTime( long long value );

        long long getInitialDelayTime() const;

        void setInitialDelayTime( long long value ) const;

    private:

        // Throttles read checking
        bool allowReadCheck( long long elapsed );

        // Performs a Read Check on the current connection, called from a separate Thread.
        void readCheck();

        // Perform a Write Check on the current connection, called from a separate Thread.
        void writeCheck();

        // Stops all the monitoring Threads, cannot restart once called.
        void stopMonitorThreads();

        // Starts the monitoring Threads,
        void startMonitorThreads();

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_INACTIVITY_INACTIVITYMONITOR_H_ */
