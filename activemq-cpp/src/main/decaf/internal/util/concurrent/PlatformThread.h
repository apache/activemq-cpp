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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_PLATFORMTHREAD_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_PLATFORMTHREAD_H_

#include <decaf/util/Config.h>

#include <decaf/internal/util/concurrent/ThreadingTypes.h>

#include <vector>

namespace decaf {
namespace internal {
namespace util {
namespace concurrent {

    struct ThreadHandle;

    class DECAF_API PlatformThread {
    private:

        PlatformThread();
        PlatformThread(const PlatformThread&);
        PlatformThread& operator= (const PlatformThread&);

    public:  // Mutex processing methods

        /**
         * Creates a new Mutex instance at the location given by the mutex pointer
         * argument.  The mutex must be destroyed by calling the destoryMutex
         * method when it is no longer needed.
         *
         * @param mutex
         *      Pointer to a memory location where the new Mutex is to be stored.
         */
        static void createMutex(decaf_mutex_t* mutex);

        static void lockMutex(decaf_mutex_t mutex);

        static bool tryLockMutex(decaf_mutex_t mutex);

        static void unlockMutex(decaf_mutex_t mutex);

        static void destroyMutex(decaf_mutex_t mutex);

    public: // Reader / Writer Mutex processing methods.

        static void createRWMutex(decaf_rwmutex_t* mutex);

        static void readerLockMutex(decaf_rwmutex_t mutex);
        static void writerLockMutex(decaf_rwmutex_t mutex);

        static bool tryReaderLockMutex(decaf_rwmutex_t mutex);
        static bool tryWriterLockMutex(decaf_rwmutex_t mutex);

        static void unlockRWMutex(decaf_rwmutex_t mutex);

        static void destroyRWMutex(decaf_rwmutex_t mutex);

    public:  // Condition processing methods

        static void createCondition(decaf_condition_t* condition);

        static void notify(decaf_condition_t condition);

        static void notifyAll(decaf_condition_t condition);

        static void waitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex);

        /**
         * @return true if the condition wait met the timeout parameters.
         */
        static bool waitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex,
                                    long long mills, int nanos);

        static void interruptibleWaitOnCondition(decaf_condition_t condition,
                                                 decaf_mutex_t mutex,
                                                 CompletionCondition& complete);

        /**
         * @return true if the condition wait met the timeout parameters without being signaled.
         */
        static bool interruptibleWaitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex,
                                                 long long mills, int nanos, CompletionCondition& complete);

        static void destroyCondition(decaf_condition_t condition);

    public:  // Thread Methods.

        /**
         * Given the Threading libraries max thread priority value, create a mapping to OS level thread
         * priorities and place them in the provided vector.   This mapping will be used by the Threading
         * library to map its values to the OS level values on calls to other methods like createNewThread
         * and setPriority, etc.
         *
         * @param maxPriority
         *      The maximum value that the Threading library uses for its priority range.
         * @param mapping
         *      A vector of int values that will be sized to maxPriority and maps the OS
         *      priority values to the Threading libs range of priority values.
         */
        static void initPriorityMapping(int maxPriority, std::vector<int>& mapping);

        static void createNewThread(decaf_thread_t* handle, threadMainMethod, void* threadArg,
                                    int priority, long long stackSize, long long* threadId);

        static void detachThread(decaf_thread_t handle);

        static void detachOSThread(decaf_thread_t handle);

        static void joinThread(decaf_thread_t handle);

        static void exitThread();

        static decaf_thread_t getCurrentThread();

        static decaf_thread_t getSafeOSThreadHandle();

        static long long getCurrentThreadId();

        static int getPriority(decaf_thread_t thread);

        static void setPriority(decaf_thread_t thread, int priority);

        static long long getStackSize(decaf_thread_t thread);

        static void setStackSize(decaf_thread_t thread, long long stackSize);

        /**
         * Pause the current thread allowing another thread to be scheduled for
         * execution, no guarantee that this will happen.
         */
        static void yeild();

    public:  // Thread Local Methods

        static void createTlsKey(decaf_tls_key* key);

        static void destroyTlsKey(decaf_tls_key key);

        static void* getTlsValue(decaf_tls_key tlsKey);

        static void setTlsValue(decaf_tls_key tlsKey, void* value);

    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_PLATFORMTHREAD_H_ */
