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

#include "WireFormatRegistry.h"

using namespace std;
using namespace activemq;
using namespace activemq::wireformat;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
WireFormatRegistry::WireFormatRegistry() {
}

////////////////////////////////////////////////////////////////////////////////
WireFormatRegistry::~WireFormatRegistry() {
}

////////////////////////////////////////////////////////////////////////////////
const WireFormatFactory* WireFormatRegistry::findFactory( const std::string& name ) const
    throw( decaf::lang::exceptions::NoSuchElementException ) {

    if( !this->registry.containsKey( name ) ) {
        throw NoSuchElementException( __FILE__, __LINE__,
            "No Matching Factory Registered for format := %s", name.c_str() );
    }

    return this->registry.getValue( name );
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatRegistry::registerFactory( const std::string& name, WireFormatFactory* factory )
    throw( decaf::lang::exceptions::IllegalArgumentException,
           decaf::lang::exceptions::NullPointerException ) {

    if( name == "" ) {
        throw IllegalArgumentException( __FILE__, __LINE__,
            "WireFormatFactory name cannot be the empty string" );
    }

    if( factory == NULL ) {
        throw NullPointerException( __FILE__, __LINE__,
            "Supplied WireFormatFactory pointer was NULL" );
    }

    this->registry.setValue( name, factory );
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> WireFormatRegistry::getWireFormatNames() const {
    return this->registry.getKeys();
}

////////////////////////////////////////////////////////////////////////////////
WireFormatRegistry& WireFormatRegistry::getInstance() {
    static WireFormatRegistry registry;
    return registry;
}
