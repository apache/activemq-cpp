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

#include "AtomicBoolean.h"

#include <decaf/lang/Boolean.h>
#include <apr_atomic.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
AtomicBoolean::AtomicBoolean() : value(0) {
}

////////////////////////////////////////////////////////////////////////////////
AtomicBoolean::AtomicBoolean( bool initialValue ) {
    this->value = initialValue ? 1 : 0;
}

////////////////////////////////////////////////////////////////////////////////
bool AtomicBoolean::compareAndSet( bool expect, bool update ) {
    int upd = update ? 1 : 0;
    int exp = expect ? 1 : 0;
    return apr_atomic_cas32( &this->value, upd, exp ) == exp;
}

////////////////////////////////////////////////////////////////////////////////
bool AtomicBoolean::getAndSet( bool newValue ) {
    return apr_atomic_xchg32( &this->value, newValue ) > 0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
std::string AtomicBoolean::toString() const {
    return Boolean::toString( this->value ? true : false );
}
