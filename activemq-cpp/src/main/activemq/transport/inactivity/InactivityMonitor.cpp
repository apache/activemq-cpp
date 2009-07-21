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

#include "InactivityMonitor.h"

using namespace std;
using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::inactivity;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
InactivityMonitor::InactivityMonitor( const Pointer<Transport>& next )
:  TransportFilter( next ) {

}

////////////////////////////////////////////////////////////////////////////////
InactivityMonitor::~InactivityMonitor() {
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::onException( const decaf::lang::Exception& ex ) {
    TransportFilter::onException( ex );
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::onCommand( const Pointer<Command>& command ) {
    TransportFilter::onCommand( command );
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::oneway( const Pointer<Command>& command )
    throw( decaf::io::IOException, decaf::lang::exceptions::UnsupportedOperationException ) {

    TransportFilter::oneway( command );
}
