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

#ifndef _ACTIVEMQ_STATE_SESSIONSTATE_H_
#define _ACTIVEMQ_STATE_SESSIONSTATE_H_

#include <activemq/util/Config.h>
#include <activemq/commands/SessionInfo.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/ProducerId.h>
#include <activemq/state/ConsumerState.h>
#include <activemq/state/ProducerState.h>

#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/ConcurrentStlMap.h>

#include <string>

namespace activemq {
namespace state {

    using decaf::lang::Pointer;
    using decaf::util::concurrent::ConcurrentStlMap;
    using decaf::util::concurrent::atomic::AtomicBoolean;
    using namespace activemq::commands;

    class AMQCPP_API SessionState {
    private:

        Pointer<SessionInfo> info;

        ConcurrentStlMap<Pointer<ProducerId>,
                         Pointer<ProducerState>,
                         ProducerId::COMPARATOR> producers;

        ConcurrentStlMap<Pointer<ConsumerId>,
                         Pointer<ConsumerState>,
                         ConsumerId::COMPARATOR> consumers;

        AtomicBoolean disposed;

    private:

        SessionState(const SessionState&);
        SessionState& operator=(const SessionState&);

    public:

        SessionState(Pointer<SessionInfo> info);

        virtual ~SessionState();

        std::string toString() const;

        const Pointer<SessionInfo> getInfo() const {
            return this->info;
        }

        void addProducer(Pointer<ProducerInfo> info);

        Pointer<ProducerState> removeProducer(Pointer<ProducerId> id);

        void addConsumer(Pointer<ConsumerInfo> info);

        Pointer<ConsumerState> removeConsumer(Pointer<ConsumerId> id);

        const decaf::util::Collection<Pointer<ProducerState> >& getProducerStates() const {
            return producers.values();
        }

        Pointer<ProducerState> getProducerState(Pointer<ProducerId> id) {
            return producers.get(id);
        }

        const decaf::util::Collection<Pointer<ConsumerState> >& getConsumerStates() const {
            return consumers.values();
        }

        Pointer<ConsumerState> getConsumerState(Pointer<ConsumerId> id) {
            return consumers.get(id);
        }

        void checkShutdown() const;

        void shutdown();

    };

}}

#endif /*_ACTIVEMQ_STATE_SESSIONSTATE_H_*/
