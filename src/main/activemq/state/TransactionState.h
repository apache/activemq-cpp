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

#ifndef _ACTIVEMQ_STATE_TRANSACTIONSTATE_H_
#define _ACTIVEMQ_STATE_TRANSACTIONSTATE_H_

#include <activemq/util/Config.h>
#include <activemq/commands/Command.h>
#include <activemq/commands/TransactionId.h>

#include <decaf/lang/Pointer.h>
#include <decaf/util/StlList.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>

#include <string>
#include <memory>

namespace activemq {
namespace state {

    using decaf::lang::Pointer;
    using decaf::util::StlList;
    using decaf::util::concurrent::atomic::AtomicBoolean;
    using namespace activemq::commands;

    class AMQCPP_API TransactionState {
    private:

        StlList< Pointer<Command> > commands;
        Pointer<TransactionId> id;
        AtomicBoolean disposed;
        bool prepared;
        int preparedResult;

    public:

        TransactionState( const Pointer<TransactionId>& id );

        virtual ~TransactionState();

        std::string toString() const;

        void addCommand( const Pointer<Command>& operation );

        void checkShutdown() const;

        void shutdown() {
            this->disposed.set( true );
        }

        const StlList< Pointer<Command> >& getCommands() const {
            return commands;
        }

        const Pointer<TransactionId>& getId() const {
            return id;
        }

        void setPrepared( bool prepared ) {
            this->prepared = prepared;
        }

        bool isPrepared() const {
            return this->prepared;
        }

        void setPreparedResult( int preparedResult ) {
            this->preparedResult = preparedResult;
        }

        int getPreparedResult() const {
            return this->preparedResult;
        }

    };

}}

#endif /*_ACTIVEMQ_STATE_TRANSACTIONSTATE_H_*/
