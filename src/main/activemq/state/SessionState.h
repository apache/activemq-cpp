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
#include <decaf/util/StlMap.h>

#include <string>
#include <memory>

namespace activemq {
namespace state {

    using namespace activemq::commands;
    using namespace decaf::lang;

    class AMQCPP_API SessionState {
    private:

        Pointer<SessionInfo> info;

        decaf::util::StlMap< Pointer<ProducerId>,
                             Pointer<ProducerState>,
                             ProducerId::COMPARATOR > producers;

        decaf::util::StlMap< Pointer<ConsumerId>,
                             Pointer<ConsumerState>,
                             ConsumerId::COMPARATOR > consumers;

        decaf::util::concurrent::atomic::AtomicBoolean disposed;

    public:

        SessionState( const Pointer<SessionInfo>& info );

        virtual ~SessionState();

        std::string toString() const;

        const Pointer<SessionInfo> getInfo() const {
            return this->info;
        }

//        void addProducer(commands::ProducerInfo info) {
//            checkShutdown();
//            producers.put(info.getProducerId(), new ProducerState(info));
//        }
//
//        ProducerState removeProducer(commands::ProducerId id) {
//            return producers.remove(id);
//        }
//
//        void addConsumer(commands::ConsumerInfo info) {
//            checkShutdown();
//            consumers.put(info.getConsumerId(), new ConsumerState(info));
//        }
//
//        ConsumerState removeConsumer(commands::ConsumerId id) {
//            return consumers.remove(id);
//        }
//
//        Set<commands::ConsumerId> getConsumerIds() {
//            return consumers.keySet();
//        }
//
//        Set<commands::ProducerId> getProducerIds() {
//            return producers.keySet();
//        }
//
//        Collection<ProducerState> getProducerStates() {
//            return producers.values();
//        }
//
//        ProducerState getProducerState(commands::ProducerId producerId) {
//            return producers.get(producerId);
//        }
//
//        Collection<ConsumerState> getConsumerStates() {
//            return consumers.values();
//        }
//
//        ConsumerState getConsumerState( const commands::ConsumerId& consumerId) {
//            return consumers.get(consumerId);
//        }

        void checkShutdown() const;

        void shutdown() {
            this->disposed.set( true );
        }

    };

}}

#endif /*_ACTIVEMQ_STATE_SESSIONSTATE_H_*/
