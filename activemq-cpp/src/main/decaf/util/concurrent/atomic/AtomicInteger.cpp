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
#include <decaf/internal/util/concurrent/Atomics.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace decaf::internal::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
AtomicInteger::AtomicInteger() :
    value(0) {
}

////////////////////////////////////////////////////////////////////////////////
AtomicInteger::AtomicInteger(int initialValue) :
    value(initialValue) {
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::getAndSet(int newValue) {
    return Atomics::getAndSet(&this->value, newValue);
}

////////////////////////////////////////////////////////////////////////////////
bool AtomicInteger::compareAndSet(int expect, int update) {
    return Atomics::compareAndSet32(&this->value, expect, update);
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::getAndIncrement() {
    return Atomics::getAndIncrement(&this->value);
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::getAndDecrement() {
    return Atomics::getAndDecrement(&this->value);
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::getAndAdd(int delta) {
    return Atomics::getAndAdd(&this->value, delta);
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::incrementAndGet() {
    return Atomics::incrementAndGet(&this->value);
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::decrementAndGet() {
    return Atomics::decrementAndGet(&this->value);
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::addAndGet(int delta) {
    return Atomics::addAndGet(&this->value, delta);
}

////////////////////////////////////////////////////////////////////////////////
std::string AtomicInteger::toString() const {
    return Integer::toString(this->value);
}

////////////////////////////////////////////////////////////////////////////////
int AtomicInteger::intValue() const {
    return this->value;
}

////////////////////////////////////////////////////////////////////////////////
long long AtomicInteger::longValue() const {
    return Integer(value).longValue();
}

////////////////////////////////////////////////////////////////////////////////
float AtomicInteger::floatValue() const {
    return Integer(value).floatValue();
}

////////////////////////////////////////////////////////////////////////////////
double AtomicInteger::doubleValue() const {
    return Integer(value).doubleValue();
}
