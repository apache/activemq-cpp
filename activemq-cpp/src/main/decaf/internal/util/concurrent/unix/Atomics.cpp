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

#ifndef HAVE_ATOMIC_BUILTINS
#if defined(SOLARIS2) && SOLARIS2 >= 10
#include <atomic.h>
#endif
#endif

using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
#ifndef HAVE_ATOMIC_BUILTINS

#include <decaf/internal/util/concurrent/PlatformThread.h>

namespace {
    decaf_mutex_t atomicMutex;
}

#endif

////////////////////////////////////////////////////////////////////////////////
void Atomics::initialize() {
#ifndef HAVE_ATOMIC_BUILTINS
    PlatformThread::createMutex(&atomicMutex);
#endif
}

////////////////////////////////////////////////////////////////////////////////
void Atomics::shutdown() {
#ifndef HAVE_ATOMIC_BUILTINS
    PlatformThread::destroyMutex(atomicMutex);
#endif
}

////////////////////////////////////////////////////////////////////////////////
bool Atomics::compareAndSet32(volatile int* target, int expect, int update ) {

#ifdef HAVE_ATOMIC_BUILTINS
    return __sync_val_compare_and_swap(target, expect, update)  == expect;
#elif defined(SOLARIS2) && SOLARIS2 >= 10
    return atomic_cas_32((volatile unsigned int*)target, expect, update) == expect;
#else
    bool result = false;
    PlatformThread::lockMutex(atomicMutex);

    if (*target == expect) {
        *target = update;
        result = true;
    }

    PlatformThread::unlockMutex(atomicMutex);

    return result;
#endif
}

////////////////////////////////////////////////////////////////////////////////
bool Atomics::compareAndSet(volatile void** target, void* expect, void* update) {
#ifdef HAVE_ATOMIC_BUILTINS
    return __sync_val_compare_and_swap(target, (void*)expect, (void*)update) == (void*)expect;
#elif defined(SOLARIS2) && SOLARIS2 >= 10
    return atomic_cas_ptr(target, expect, update) == expect;
#else
    bool result = false;
    PlatformThread::lockMutex(atomicMutex);

    if (*target == expect) {
        *target = update;
        result = true;
    }

    PlatformThread::unlockMutex(atomicMutex);

    return result;
#endif
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::getAndSet(volatile int* target, int newValue) {
#ifdef HAVE_ATOMIC_BUILTINS
    __sync_synchronize();
    return __sync_lock_test_and_set(target, newValue);
#elif defined(SOLARIS2) && SOLARIS2 >= 10
    return atomic_swap_32((volatile unsigned int*)target, newValue);
#else
    int oldValue;
    PlatformThread::lockMutex(atomicMutex);

    oldValue = *target;
    *target = newValue;

    PlatformThread::unlockMutex(atomicMutex);

    return oldValue;
#endif
}

////////////////////////////////////////////////////////////////////////////////
void* Atomics::getAndSet(volatile void** target, void* newValue) {
#ifdef HAVE_ATOMIC_BUILTINS
    __sync_synchronize();
    return (void*) __sync_lock_test_and_set(target, newValue);
#elif defined(SOLARIS2) && SOLARIS2 >= 10
    return atomic_swap_ptr(target, newValue);
#elif defined(__APPLE__)
    return atomic_swap_32(target, newValue);
#else
    void* oldValue;
    PlatformThread::lockMutex(atomicMutex);

    oldValue = *(void **)target;
    *target = newValue;

    PlatformThread::unlockMutex(atomicMutex);

    return oldValue;
#endif
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::getAndIncrement(volatile int* target) {
#ifdef HAVE_ATOMIC_BUILTINS
    return __sync_fetch_and_add(target, 1);
#elif defined(SOLARIS2) && SOLARIS2 >= 10
    return atomic_add_32_nv((volatile unsigned int*)target, 1) - 1;
#else
    int oldValue;
    PlatformThread::lockMutex(atomicMutex);

    oldValue = *target;
    (*target)++;

    PlatformThread::unlockMutex(atomicMutex);

    return oldValue;
#endif
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::getAndDecrement(volatile int* target) {
#ifdef HAVE_ATOMIC_BUILTINS
    return __sync_fetch_and_add(target, 0xFFFFFFFF);
#elif defined(SOLARIS2) && SOLARIS2 >= 10
    return atomic_add_32_nv((volatile unsigned int*)target, 0xFFFFFFFF) + 1;
#else
    int oldValue;
    PlatformThread::lockMutex(atomicMutex);

    oldValue = *target;
    (*target)--;

    PlatformThread::unlockMutex(atomicMutex);

    return oldValue;
#endif
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::getAndAdd(volatile int* target, int delta) {
#ifdef HAVE_ATOMIC_BUILTINS
    return __sync_fetch_and_add(target, delta);
#elif defined(SOLARIS2) && SOLARIS2 >= 10
    return atomic_add_32_nv((volatile unsigned int*)target, delta) - delta;
#else
    int oldValue;
    PlatformThread::lockMutex(atomicMutex);

    oldValue = *target;
    *target += delta;

    PlatformThread::unlockMutex(atomicMutex);

    return oldValue;
#endif
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::addAndGet(volatile int* target, int delta) {
#ifdef HAVE_ATOMIC_BUILTINS
    return __sync_fetch_and_add(target, delta) + delta;
#elif defined(SOLARIS2) && SOLARIS2 >= 10
    return atomic_add_32_nv((volatile unsigned int*)target, delta);
#else
    int newValue;
    PlatformThread::lockMutex(atomicMutex);

    *target += delta;
    newValue = *target;

    PlatformThread::unlockMutex(atomicMutex);

    return newValue;
#endif
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::incrementAndGet(volatile int* target) {
#ifdef HAVE_ATOMIC_BUILTINS
    return __sync_fetch_and_add(target, 1) + 1;
#elif defined(SOLARIS2) && SOLARIS2 >= 10
    return atomic_add_32_nv((volatile unsigned int*)target, 1);
#else
    int newValue;
    PlatformThread::lockMutex(atomicMutex);

    (*target)++;
    newValue = *target;

    PlatformThread::unlockMutex(atomicMutex);

    return newValue;
#endif
}

////////////////////////////////////////////////////////////////////////////////
int Atomics::decrementAndGet(volatile int* target) {
#ifdef HAVE_ATOMIC_BUILTINS
    return __sync_fetch_and_add(target, 0xFFFFFFFF) - 1;
#elif defined(SOLARIS2) && SOLARIS2 >= 10
    return atomic_add_32_nv((volatile unsigned int*)target, 0xFFFFFFFF);
#else
    int newValue;
    PlatformThread::lockMutex(atomicMutex);

    (*target)--;
    newValue = *target;

    PlatformThread::unlockMutex(atomicMutex);

    return newValue;
#endif
}

