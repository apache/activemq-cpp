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

#include "AtomicInteger.h"

#include <decaf/lang/Integer.h>
#include <apr_atomic.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
AtomicInteger::AtomicInteger() : value( 0 ) {
}

////////////////////////////////////////////////////////////////////////////////
AtomicInteger::AtomicInteger( int initialValue ) : value( initialValue ) {
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::getAndSet( int newValue ) {
    return apr_atomic_xchg32( &this->value, newValue );
}

////////////////////////////////////////////////////////////////////////////////
bool AtomicInteger::compareAndSet( int expect, int update ) {
    return apr_atomic_cas32( &this->value, update, expect ) == (unsigned int)expect;
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::getAndIncrement() {
    return apr_atomic_inc32( &this->value );
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::getAndDecrement() {
    return apr_atomic_add32( &this->value, 0xFFFFFFFF );
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::getAndAdd( int delta ) {
    return apr_atomic_add32( &this->value, delta );
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::incrementAndGet() {
    return apr_atomic_inc32( &this->value ) + 1;
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::decrementAndGet() {
    return apr_atomic_add32( &this->value, 0xFFFFFFFF ) - 1;
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::addAndGet( int delta ) {
    return apr_atomic_add32( &this->value, delta ) + delta;
}

////////////////////////////////////////////////////////////////////////////////
std::string AtomicInteger::toString() const {
    return Integer::toString( this->value );
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::intValue() const {
    return this->value;
}

////////////////////////////////////////////////////////////////////////////////
long long AtomicInteger::longValue() const {
    return Integer( value ).longValue();
}

////////////////////////////////////////////////////////////////////////////////
float AtomicInteger::floatValue() const {
    return Integer( value ).floatValue();
}

////////////////////////////////////////////////////////////////////////////////
double AtomicInteger::doubleValue() const {
    return Integer( value ).doubleValue();
}
