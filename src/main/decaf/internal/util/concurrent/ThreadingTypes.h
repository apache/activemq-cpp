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


#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_UNIX_THREADTYPES_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_UNIX_THREADTYPES_H_

#include <decaf/util/Config.h>

#ifdef HAVE_PTHREAD_H
#include <decaf/internal/util/concurrent/unix/PlatformDefs.h>
#else
#include <decaf/internal/util/concurrent/windows/PlatformDefs.h>
#endif

namespace decaf{
namespace lang{
    class Thread;
}
namespace internal{
namespace util{
namespace concurrent{

    /**
     * Max number of TLS keys that a thread can use.
     */
    #define DECAF_MAX_TLS_SLOTS 256

    /**
     * This is the main method for thread instances, this value is valid
     * on any platform, the PlatformThread methods will handle calling this
     * method and providing it with its assigned arg.
     *
     * @param arg
     *      A void* that was given when the thread was started.
     */
    typedef PLATFORM_THREAD_CALLBACK_TYPE (PLATFORM_CALLING_CONV *threadMainMethod)(PLATFORM_THREAD_ENTRY_ARG);

    /**
     * The ThreadHandle contains one of these and it should be the method that
     * does the actual work for the thread.
     */
    typedef void (*threadingTask)(void *);

    // Forward Declare some of the nested types.
    struct MonitorHandle;

    struct ThreadHandle {
        decaf::lang::Thread* parent;
        decaf_thread_t handle;
        decaf_mutex_t mutex;
        decaf_condition_t condition;
        volatile int state;
        volatile int references;
        int priority;
        bool interrupted;
        bool interruptible;
        bool timerSet;
        bool canceled;
        bool unparked;
        bool parked;
        bool sleeping;
        bool waiting;
        bool notified;
        bool blocked;
        bool suspended;
        char* name;
        long long stackSize;
        void *tls[DECAF_MAX_TLS_SLOTS];
        threadingTask threadMain;
        void* threadArg;
        long long threadId;
        bool osThread;
        ThreadHandle* interruptingThread;
        int numAttached;
        ThreadHandle* next;
        ThreadHandle* joiners;
        MonitorHandle* monitor;
    };

    struct MonitorHandle {
        char* name;
        decaf_mutex_t mutex;
        decaf_mutex_t lock;
        unsigned int count;
        ThreadHandle* owner;
        ThreadHandle* waiting;
        ThreadHandle* blocking;
        bool initialized;
        MonitorHandle* next;
    };

    class CompletionCondition {
    public:

        virtual ~CompletionCondition() {}

        /**
         * Called from timed wait condition methods to indicate if the timeout
         * has occurred, allows this method to take other actions based on the
         * timeout having occurred or not.
         *
         * By default this method just defers to the simple operator() method.
         *
         * @param timedOut
         *      Indicates that the calling wait condition timed out or not.
         */
        virtual bool operator()(bool timedOut DECAF_UNUSED) {
            return this->operator()();
        }

        /**
         * Called from non-timed wait conditions to determine if the condition
         * necessary to complete the wait has occurred or not.
         */
        virtual bool operator()() {
            return false;
        }

    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_UNIX_THREADTYPES_H_ */
