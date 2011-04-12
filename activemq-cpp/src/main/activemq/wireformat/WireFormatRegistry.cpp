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
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
WireFormatRegistry::WireFormatRegistry() : registry() {
}

////////////////////////////////////////////////////////////////////////////////
WireFormatRegistry::~WireFormatRegistry() {

    try {
        this->unregisterAllFactories();
    } catch(...) {}
}

////////////////////////////////////////////////////////////////////////////////
WireFormatFactory* WireFormatRegistry::findFactory( const std::string& name ) const {

    if( !this->registry.containsKey( name ) ) {
        throw NoSuchElementException( __FILE__, __LINE__,
            "No Matching Factory Registered for format := %s", name.c_str() );
    }

    return this->registry.get( name );
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatRegistry::registerFactory( const std::string& name, WireFormatFactory* factory ) {

    if( name == "" ) {
        throw IllegalArgumentException( __FILE__, __LINE__,
            "WireFormatFactory name cannot be the empty string" );
    }

    if( factory == NULL ) {
        throw NullPointerException( __FILE__, __LINE__,
            "Supplied WireFormatFactory pointer was NULL" );
    }

    this->registry.put( name, factory );
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatRegistry::unregisterFactory( const std::string& name ) {
    if( this->registry.containsKey( name ) ) {
        delete this->registry.get( name );
        this->registry.remove( name );
    }
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatRegistry::unregisterAllFactories() {

    std::vector<WireFormatFactory*> factories = this->registry.values();
    std::vector<WireFormatFactory*>::iterator iter = factories.begin();

    for( ; iter != factories.end(); ++iter ) {
        delete *iter;
    }

    this->registry.clear();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> WireFormatRegistry::getWireFormatNames() const {
    return this->registry.keySet();
}

////////////////////////////////////////////////////////////////////////////////
WireFormatRegistry& WireFormatRegistry::getInstance() {
    static WireFormatRegistry registry;
    return registry;
}
