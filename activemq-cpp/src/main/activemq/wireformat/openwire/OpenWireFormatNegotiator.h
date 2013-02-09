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

#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_OPENWIREFORMATNEGOTIATOR_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_OPENWIREFORMATNEGOTIATOR_H_

#include <activemq/util/Config.h>
#include <activemq/transport/TransportFilter.h>
#include <activemq/wireformat/openwire/OpenWireFormat.h>
#include <activemq/wireformat/WireFormatNegotiator.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/lang/Pointer.h>

namespace activemq{
namespace wireformat{
namespace openwire{

    using decaf::lang::Pointer;

    class AMQCPP_API OpenWireFormatNegotiator: public wireformat::WireFormatNegotiator {
    private:

        /**
         * Time to wait before we declare that the negotiation has timed out.
         */
        static const int negotiationTimeout;

        /**
         * Have we started already?
         */
        decaf::util::concurrent::atomic::AtomicBoolean firstTime;

        /**
         * Latch objects to count down till we receive the wireFormat info
         */
        decaf::util::concurrent::CountDownLatch wireInfoSentDownLatch;
        decaf::util::concurrent::CountDownLatch readyCountDownLatch;

        /**
         * The OpenWireFormat object that we use in negotiation.
         */
        OpenWireFormat* openWireFormat;

    private:

        OpenWireFormatNegotiator(const OpenWireFormatNegotiator&);
        OpenWireFormatNegotiator& operator=(const OpenWireFormatNegotiator&);

    public:

        /**
         * Constructor - Initializes this object around another Transport
         * @param wireFormat - The WireFormat object we use to negotiate
         * @param next - The next transport in the chain
         */
        OpenWireFormatNegotiator(OpenWireFormat* wireFormat, const Pointer<transport::Transport> next);

        virtual ~OpenWireFormatNegotiator();

        virtual void oneway(const Pointer<commands::Command> command);

        virtual Pointer<commands::Response> request(const Pointer<commands::Command> command);

        virtual Pointer<commands::Response> request(const Pointer<commands::Command> command, unsigned int timeout);

    public:

        virtual void onCommand(const Pointer<commands::Command> command);

        virtual void onException(const decaf::lang::Exception& ex);

    protected:

        virtual void afterNextIsStarted();

        virtual void afterNextIsStopped();

    };

}}}

#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_OPENWIREFORMATNEGOTIATOR_H_*/
