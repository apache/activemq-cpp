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

#include <decaf/internal/util/concurrent/Atomics.h>

using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
void Atomics::initialize() {
}

////////////////////////////////////////////////////////////////////////////////
void Atomics::shutdown() {
}

////////////////////////////////////////////////////////////////////////////////
bool Atomics::compareAndSet32(volatile int* target, int expect, int update ) {
    return ::InterlockedCompareExchange((volatile LONG*)target, update, expect) == (unsigned int)expect;
}

////////////////////////////////////////////////////////////////////////////////
bool Atomics::compareAndSet(volatile void** target, void* expect, void* update) {
    return ::InterlockedCompareExchangePointer((volatile PVOID*)target, (void*)update, (void*)expect ) == (void*)expect;
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::getAndSet(volatile int* target, int newValue) {
    return ::InterlockedExchange((volatile LONG*)target, newValue);
}

////////////////////////////////////////////////////////////////////////////////
void* Atomics::getAndSet(volatile void** target, void* newValue) {
    return InterlockedExchangePointer((volatile PVOID*)target, newValue);
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::getAndIncrement(volatile int* target) {
    return ::InterlockedIncrement((volatile LONG*)target) - 1;
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::getAndDecrement(volatile int* target) {
    return ::InterlockedExchangeAdd((volatile LONG*)target, 0xFFFFFFFF);
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::getAndAdd(volatile int* target, int delta) {
    return ::InterlockedExchangeAdd((volatile LONG*)target, delta);
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::addAndGet(volatile int* target, int delta) {
    return ::InterlockedExchangeAdd((volatile LONG*)target, delta) + delta;
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::incrementAndGet(volatile int* target) {
    return ::InterlockedIncrement((volatile LONG*)target);
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::decrementAndGet(volatile int* target) {
    return ::InterlockedExchangeAdd((volatile LONG*)target, 0xFFFFFFFF) - 1;
}

