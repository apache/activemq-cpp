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
#include <activemq/commands/LocalTransactionId.h>
#include <activemq/state/ConsumerState.h>
#include <activemq/state/ProducerState.h>
#include <activemq/state/SessionState.h>
#include <activemq/state/TransactionState.h>

#include <decaf/util/StlMap.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/ConcurrentStlMap.h>
#include <decaf/util/LinkedList.h>
#include <decaf/lang/Pointer.h>

#include <string>
#include <memory>

namespace activemq {
namespace state {

    using decaf::lang::Pointer;
    using namespace decaf::util;
    using namespace activemq::commands;

    class AMQCPP_API ConnectionState {
    private:

        Pointer< ConnectionInfo > info;
        ConcurrentStlMap< Pointer<LocalTransactionId>,
                          Pointer<TransactionState>,
                          LocalTransactionId::COMPARATOR > transactions;
        ConcurrentStlMap< Pointer<SessionId>,
                          Pointer<SessionState>,
                          SessionId::COMPARATOR > sessions;
        LinkedList< Pointer<DestinationInfo> > tempDestinations;
        decaf::util::concurrent::atomic::AtomicBoolean disposed;

        bool connectionInterruptProcessingComplete;
        StlMap< Pointer<ConsumerId>,
                Pointer<ConsumerInfo>,
                ConsumerId::COMPARATOR > recoveringPullConsumers;

    public:

        ConnectionState( const Pointer<ConnectionInfo>& info );

        virtual ~ConnectionState();

        std::string toString() const;

        const Pointer<commands::ConnectionInfo>& getInfo() const {
            return this->info;
        }

        void checkShutdown() const;

        void shutdown();

        void reset( const Pointer<ConnectionInfo>& info );

        void addTempDestination( const Pointer<DestinationInfo>& info ) {
            checkShutdown();
            tempDestinations.add( info );
        }

        void removeTempDestination( const Pointer<ActiveMQDestination>& destination ) {

            std::auto_ptr< decaf::util::Iterator< Pointer<DestinationInfo> > > iter(
                tempDestinations.iterator() );

            while( iter->hasNext() ) {
                Pointer<DestinationInfo> di = iter->next();
                if( di->getDestination()->equals( destination.get() ) ) {
                    iter->remove();
                }
            }
        }

        void addTransactionState( const Pointer<TransactionId>& id ) {
            checkShutdown();
            transactions.put( id.dynamicCast<LocalTransactionId>(),
                              Pointer<TransactionState>( new TransactionState( id ) ) );
        }

        const Pointer<TransactionState>& getTransactionState( const Pointer<TransactionId>& id ) const {
            return transactions.get( id.dynamicCast<LocalTransactionId>() );
        }

        std::vector< Pointer<TransactionState> > getTransactionStates() const {
            return transactions.values();
        }

        Pointer<TransactionState> removeTransactionState( const Pointer<TransactionId>& id ) {
            return transactions.remove( id.dynamicCast<LocalTransactionId>() );
        }

        void addSession( const Pointer<SessionInfo>& info ) {
            checkShutdown();
            sessions.put(
                info->getSessionId(), Pointer<SessionState>( new SessionState( info ) ) );
        }

        Pointer<SessionState> removeSession( const Pointer<SessionId>& id ) {
            return sessions.remove( id );
        }

        const Pointer<SessionState>& getSessionState( const Pointer<SessionId>& id ) const {
            return sessions.get( id );
        }

        const LinkedList< Pointer<DestinationInfo> >& getTempDesinations() const {
            return tempDestinations;
        }

        std::vector< Pointer<SessionState> > getSessionStates() const {
            return sessions.values();
        }

        StlMap< Pointer<ConsumerId>, Pointer<ConsumerInfo>, ConsumerId::COMPARATOR > getRecoveringPullConsumers() {
            return recoveringPullConsumers;
        }

        void setConnectionInterruptProcessingComplete( bool connectionInterruptProcessingComplete ) {
            this->connectionInterruptProcessingComplete = connectionInterruptProcessingComplete;
        }

        bool isConnectionInterruptProcessingComplete() {
            return this->connectionInterruptProcessingComplete;
        }

    };

}}

#endif /*_ACTIVEMQ_STATE_CONNECTIONSTATE_H_*/
