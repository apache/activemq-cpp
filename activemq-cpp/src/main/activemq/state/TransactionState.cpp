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

#include "TransactionState.h"

#include <activemq/state/ProducerState.h>

#include <decaf/lang/exceptions/IllegalStateException.h>

using namespace activemq;
using namespace activemq::commands;
using namespace activemq::state;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
TransactionState::TransactionState( const Pointer<TransactionId>& id ) :
    commands(), id(id), disposed(false), prepared(false), preparedResult(0), producers() {
}

////////////////////////////////////////////////////////////////////////////////
TransactionState::~TransactionState() {
    this->commands.clear();
}

////////////////////////////////////////////////////////////////////////////////
std::string TransactionState::toString() const {

    if( this->id != NULL ) {
        return this->id->toString();
    }

    return "NULL";
}

////////////////////////////////////////////////////////////////////////////////
void TransactionState::addCommand( const Pointer<Command>& operation ) {

    checkShutdown();
    commands.add( operation );
}

////////////////////////////////////////////////////////////////////////////////
void TransactionState::checkShutdown() const {

    if( this->disposed.get() ) {
        throw decaf::lang::exceptions::IllegalStateException(
            __FILE__, __LINE__, "Transaction already Disposed" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TransactionState::addProducerState( const Pointer<ProducerState>& producerState ) {

    if( producerState != NULL ) {
        producers.put( producerState->getInfo()->getProducerId(), producerState );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::vector< Pointer<ProducerState> > TransactionState::getProducerStates() {
    return this->producers.values();
}
