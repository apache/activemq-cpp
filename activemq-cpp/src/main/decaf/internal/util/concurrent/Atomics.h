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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_ATOMICS_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_ATOMICS_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace internal {
namespace util {
namespace concurrent {

    class Threading;

    class DECAF_API Atomics {
    private:

        Atomics();
        Atomics(const Atomics&);
        Atomics& operator= (const Atomics&);

    public:

        template<typename T>
        static bool compareAndSwap(T*& target, T* expect, T* update) {

            return Atomics::compareAndSet((volatile void**)&target, (void*)expect, (void*)update);
        }

    public:

        static bool compareAndSet32(volatile int* target, int expect, int update);
        static bool compareAndSet(volatile void** target, void* expect, void* update);

        static void* getAndSet(volatile void** target, void* value);
        static int getAndSet(volatile int* target, int value);

        static int getAndIncrement(volatile int* target);
        static int getAndDecrement(volatile int* target);

        static int getAndAdd(volatile int* target, int delta);
        static int addAndGet(volatile int* target, int delta);

        static int incrementAndGet(volatile int* target);
        static int decrementAndGet(volatile int* target);

    private:

        static void initialize();
        static void shutdown();

        friend class Threading;
    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_ATOMICS_H_ */
