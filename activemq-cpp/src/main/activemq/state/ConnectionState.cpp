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

#include "ConnectionState.h"

#include <decaf/lang/exceptions/IllegalStateException.h>

using namespace activemq;
using namespace activemq::state;

////////////////////////////////////////////////////////////////////////////////
ConnectionState::ConnectionState( const Pointer<ConnectionInfo>& info ) :
    info(info),
    transactions(),
    sessions(),
    tempDestinations(),
    disposed(false),
    connectionInterruptProcessingComplete(true),
    recoveringPullConsumers() {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionState::~ConnectionState() {
}

////////////////////////////////////////////////////////////////////////////////
std::string ConnectionState::toString() const {

    if( this->info.get() != NULL ) {
        return this->info->toString();
    }

    return "NULL";
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionState::reset( const Pointer<ConnectionInfo>& info ) {

    this->info = info;
    transactions.clear();
    sessions.clear();
    tempDestinations.clear();
    disposed.set( false );
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionState::shutdown() {

    if( this->disposed.compareAndSet( false, true ) ) {

        std::vector< Pointer<SessionState> > values = this->sessions.values();
        std::vector< Pointer<SessionState> >::iterator iter = values.begin();

        for( ; iter != values.end(); ++iter ) {
            (*iter)->shutdown();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionState::checkShutdown() const {

    if( this->disposed.get() ) {
        throw decaf::lang::exceptions::IllegalStateException(
            __FILE__, __LINE__, "Connection already Disposed" );
    }
}
