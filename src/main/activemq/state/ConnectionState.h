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

#ifndef _ACTIVEMQ_STATE_CONNECTIONSTATE_H_
#define _ACTIVEMQ_STATE_CONNECTIONSTATE_H_

#include <activemq/util/Config.h>
#include <activemq/commands/ConnectionInfo.h>
#include <activemq/commands/DestinationInfo.h>
#include <activemq/commands/SessionInfo.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/ProducerId.h>
#include <activemq/commands/TransactionId.h>
#include <activemq/state/ConsumerState.h>
#include <activemq/state/ProducerState.h>
#include <activemq/state/SessionState.h>
#include <activemq/state/TransactionState.h>

#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/Map.h>
#include <decaf/util/List.h>

#include <string>
#include <memory>

namespace activemq {
namespace state {

    class ConnectionState {
    private:

        std::auto_ptr<commands::ConnectionInfo> info;
        decaf::util::Map< commands::TransactionId*, TransactionState* > transactions;
        decaf::util::Map< commands::SessionId*, SessionState* > sessions;
        decaf::util::List< commands::DestinationInfo* > tempDestinations;
        decaf::util::concurrent::atomic::AtomicBoolean disposed;

    public:

        ConnectionState( commands::ConnectionInfo* info );

        virtual ~ConnectionState();

        std::string toString() const;

        const commands::ConnectionInfo* getInfo() const {
            return this->info.get();
        }

        void checkShutdown() const;

        void shutdown();

//        void reset(ConnectionInfo info) {
//            this.info = info;
//            transactions.clear();
//            sessions.clear();
//            tempDestinations.clear();
//            shutdown.set(false);
//        }
//
//        void addTempDestination(DestinationInfo info) {
//            checkShutdown();
//            tempDestinations.add(info);
//        }
//
//        void removeTempDestination(ActiveMQDestination destination) {
//            for (Iterator<DestinationInfo> iter = tempDestinations.iterator(); iter.hasNext();) {
//                DestinationInfo di = iter.next();
//                if (di.getDestination().equals(destination)) {
//                    iter.remove();
//                }
//            }
//        }
//
//        void addTransactionState(TransactionId id) {
//            checkShutdown();
//            transactions.put(id, new TransactionState(id));
//        }
//
//        TransactionState getTransactionState(TransactionId id) {
//            return transactions.get(id);
//        }
//
//        Collection<TransactionState> getTransactionStates() {
//            return transactions.values();
//        }
//
//        TransactionState removeTransactionState(TransactionId id) {
//            return transactions.remove(id);
//        }
//
//        void addSession(SessionInfo info) {
//            checkShutdown();
//            sessions.put(info.getSessionId(), new SessionState(info));
//        }
//
//        SessionState removeSession(SessionId id) {
//            return sessions.remove(id);
//        }
//
//        SessionState getSessionState(SessionId id) {
//            return sessions.get(id);
//        }
//
//        ConnectionInfo getInfo() {
//            return info;
//        }
//
//        Set<SessionId> getSessionIds() {
//            return sessions.keySet();
//        }
//
//        List<DestinationInfo> getTempDesinations() {
//            return tempDestinations;
//        }
//
//        Collection<SessionState> getSessionStates() {
//            return sessions.values();
//        }

    };

}}

#endif /*_ACTIVEMQ_STATE_CONNECTIONSTATE_H_*/
