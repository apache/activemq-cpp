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

#include <decaf/internal/util/concurrent/PlatformThread.h>

#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <memory>

#if HAVE_ERRNO_H
#include <errno.h>
#endif
#if HAVE_LIMITS_H
#include <limits.h>
#endif
#if HAVE_PTHREAD_H
#include <pthread.h>
#endif
#if HAVE_SIGNAL_H
#include <signal.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_SCHED_H
#include <sched.h>
#endif
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_TIME_H
#include <time.h>
#endif

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::createMutex(decaf_mutex_t* mutex) {

    *mutex = new pthread_mutex_t;

    if( pthread_mutex_init(*mutex, NULL) != 0 ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to create OS Mutex object." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::lockMutex(decaf_mutex_t mutex) {

    if (pthread_mutex_lock(mutex) != 0) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to Lock OS Mutex" );
    }
}

////////////////////////////////////////////////////////////////////////////////
bool PlatformThread::tryLockMutex(decaf_mutex_t mutex) {
    return pthread_mutex_trylock(mutex) == 0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::unlockMutex(decaf_mutex_t mutex) {

    if (pthread_mutex_unlock(mutex) != 0) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to unlock OS Mutex" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::destroyMutex(decaf_mutex_t mutex) {
    pthread_mutex_destroy(mutex);
    delete mutex;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::createRWMutex(decaf_rwmutex_t* mutex) {

    *mutex = new pthread_rwlock_t;

    if( pthread_rwlock_init(*mutex, NULL) != 0 ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to create OS Mutex object." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::readerLockMutex(decaf_rwmutex_t mutex) {

    if (pthread_rwlock_rdlock(mutex) != 0) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to Lock OS RW Mutex" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::writerLockMutex(decaf_rwmutex_t mutex) {

    if (pthread_rwlock_wrlock(mutex) != 0) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to Lock OS RW Mutex" );
    }
}

////////////////////////////////////////////////////////////////////////////////
bool PlatformThread::tryReaderLockMutex(decaf_rwmutex_t mutex) {
    return pthread_rwlock_tryrdlock(mutex) == 0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
bool PlatformThread::tryWriterLockMutex(decaf_rwmutex_t mutex) {
    return pthread_rwlock_trywrlock(mutex) == 0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::unlockRWMutex(decaf_rwmutex_t mutex) {

    if (pthread_rwlock_unlock(mutex) != 0) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to Unlock OS RW Mutex" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::destroyRWMutex(decaf_rwmutex_t mutex) {
    pthread_rwlock_destroy(mutex);
    delete mutex;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::createCondition(decaf_condition_t* condition) {

    *condition = new pthread_cond_t;

    if (pthread_cond_init(*condition, NULL) != 0) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to initialize OS Condition object.");
    }
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::notify(decaf_condition_t condition) {
    pthread_cond_signal(condition);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::notifyAll(decaf_condition_t condition) {
    pthread_cond_broadcast(condition);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::waitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex) {
    pthread_cond_wait(condition, mutex);
}

////////////////////////////////////////////////////////////////////////////////
bool PlatformThread::waitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex,
                                     long long mills, int nanos) {

    struct timeval tv;
    gettimeofday( &tv, NULL );
    long long timeNow = TimeUnit::SECONDS.toNanos( tv.tv_sec ) +
                        TimeUnit::MICROSECONDS.toNanos( tv.tv_usec );

    // Convert delay to nanoseconds and add it to now.
    long long delay = TimeUnit::MILLISECONDS.toNanos( mills ) + nanos + timeNow;

    struct timespec abstime;
    abstime.tv_sec = TimeUnit::NANOSECONDS.toSeconds( delay );
    abstime.tv_nsec = delay % 1000000000;

    if (pthread_cond_timedwait(condition, mutex, &abstime) == ETIMEDOUT) {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::interruptibleWaitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex,
                                                  CompletionCondition& complete) {

    do {

        pthread_cond_wait(condition, mutex);

        if (complete()) {
            break;
        }

    } while(true);
}

////////////////////////////////////////////////////////////////////////////////
bool PlatformThread::interruptibleWaitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex,
                                                  long long mills, int nanos, CompletionCondition& complete) {

    struct timeval tv;
    gettimeofday( &tv, NULL );
    long long timeNow = TimeUnit::SECONDS.toNanos( tv.tv_sec ) +
                        TimeUnit::MICROSECONDS.toNanos( tv.tv_usec );

    // Convert delay to nanoseconds and add it to now.
    long long delay = TimeUnit::MILLISECONDS.toNanos( mills ) + nanos + timeNow;

    struct timespec abstime;
    abstime.tv_sec = TimeUnit::NANOSECONDS.toSeconds( delay );
    abstime.tv_nsec = delay % 1000000000;

    bool result = false;

    do {

        if (pthread_cond_timedwait(condition, mutex, &abstime) == ETIMEDOUT) {

            // interruption events take precedence over timeout.
            if (complete(true)) {
               break;
            }

            result = true;
            break;
        }

        // check if spurious wake or intentional.
        if (complete(false)) {
           break;
        }

    } while(true);

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::destroyCondition(decaf_condition_t condition) {
    pthread_cond_destroy(condition);
    delete condition;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::initPriorityMapping(int maxPriority, std::vector<int>& mapping) {

    int max = sched_get_priority_max(SCHED_OTHER);
    int min = sched_get_priority_min(SCHED_OTHER);

    if (max == min) {
        // get this thread's priority and use that for all threads.
        struct sched_param schedParam;
        int currPolicy;
        pthread_getschedparam(pthread_self(), &currPolicy, &schedParam);
        max = schedParam.sched_priority;
        min = max;
    }

    mapping.clear();
    mapping.resize(maxPriority + 1);

    int tmpmax = max * 1024;
    int tmpmin = min * 1024;
    int mid = (tmpmin + tmpmax) / 2;
    int midrange = maxPriority / 2;

    mapping[0] = min;

    int delta = (mid - tmpmin) / midrange;
    for(int i = 1; i < midrange; i++) {
        mapping[midrange - i] = (mid - delta * i) / 1024;
    }

    int tailcount = maxPriority - midrange;
    delta = (tmpmax - mid) / tailcount;

    for(int i = 0; i < tailcount; i++) {
        mapping[midrange + i] = (mid + delta * i) / 1024;
    }

    mapping[maxPriority] = max;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::createNewThread(decaf_thread_t* handle, threadMainMethod threadMain, void* threadArg,
                                     int priority, long long stackSize, long long* threadId) {
    pthread_attr_t attributes;
    int schedResult;
    int schedPolicy;

    schedResult = pthread_attr_init(&attributes);
    if (schedResult != 0) {
        throw RuntimeException(__FILE__, __LINE__,
            "Failed to initialize thread attribute, error value is: %d", schedResult);
    }

    schedResult = pthread_attr_getschedpolicy(&attributes, &schedPolicy);
    if (schedResult != 0) {
        throw RuntimeException(__FILE__, __LINE__,
            "Failed to get thread scheduling policy, error value is: %d.", schedResult);
    }

    // only set the priority if it's a policy that allows setting of the priority.
    if (SCHED_FIFO == schedPolicy || SCHED_RR == schedPolicy) {
        struct sched_param schedData;

        schedData.sched_priority = priority;
        schedResult = pthread_attr_setschedparam(&attributes, &schedData);
        if (schedResult != 0) {
            throw RuntimeException(__FILE__, __LINE__,
                                   "Failed to set new Thread priority to "
                                   "value: %d, error value is: %d.",
                                   schedData.sched_priority, schedResult);
        }
    }

    if (stackSize != -1) {

    #ifdef LINUX
        if (stackSize < PTHREAD_STACK_MIN) {
            stackSize = PTHREAD_STACK_MIN;
        }
    #endif

        if (pthread_attr_setstacksize(&attributes, (size_t)stackSize) == EINVAL) {
            throw RuntimeException(
                __FILE__, __LINE__,
                "Failed to create new Thread due to invalid stack size setting: %d.",
                stackSize);
        }
    }

    int result = pthread_create(handle, &attributes, threadMain, threadArg);
    *threadId = (long long)(&handle);
    pthread_attr_destroy(&attributes);

    if( result != 0 ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to create new Thread." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::detachThread(decaf_thread_t handle) {
    pthread_detach(handle);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::detachOSThread(decaf_thread_t handle DECAF_UNUSED) {
    // Nothing to do on Linux.
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::joinThread(decaf_thread_t handle) {
    void* theReturn = 0;
    pthread_join(handle, &theReturn);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::exitThread() {
    int result = 0;
    pthread_exit(&result);
}

////////////////////////////////////////////////////////////////////////////////
decaf_thread_t PlatformThread::getCurrentThread() {
    return pthread_self();
}

////////////////////////////////////////////////////////////////////////////////
decaf_thread_t PlatformThread::getSafeOSThreadHandle() {
    return pthread_self();
}

////////////////////////////////////////////////////////////////////////////////
long long PlatformThread::getCurrentThreadId() {

    // pthread_t can be an int or struct or who knows so we use the address
    // of the value returned which seems to remain fixed.
    return (long long)&(pthread_self);
}

////////////////////////////////////////////////////////////////////////////////
int PlatformThread::getPriority(decaf_thread_t thread) {
    struct sched_param schedParam;
    int policy = SCHED_OTHER;
    pthread_getschedparam(thread, &policy, &schedParam);
    return schedParam.sched_priority;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::setPriority(decaf_thread_t thread, int priority) {
    struct sched_param schedParam;
    schedParam.sched_priority = priority;
    pthread_setschedparam(thread, SCHED_OTHER, &schedParam);
}

////////////////////////////////////////////////////////////////////////////////
long long PlatformThread::getStackSize(decaf_thread_t thread DECAF_UNUSED) {

    pthread_attr_t attributes;
    pthread_attr_init( &attributes );
    size_t stackSize;

    if (pthread_attr_getstacksize(&attributes, &stackSize) == EINVAL) {
        pthread_attr_destroy( &attributes );
        throw RuntimeException(
            __FILE__, __LINE__,
            "Failed to get stack size setting.");
    }

    pthread_attr_destroy( &attributes );

    return (long long)stackSize;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::setStackSize(decaf_thread_t thread DECAF_UNUSED, long long stackSize) {

    pthread_attr_t attributes;
    pthread_attr_init( &attributes );

#ifdef LINUX
    if (stackSize < PTHREAD_STACK_MIN) {
        stackSize = PTHREAD_STACK_MIN;
    }
#endif

    if (pthread_attr_setstacksize(&attributes, (size_t)stackSize) == EINVAL) {
        pthread_attr_destroy( &attributes );
        throw RuntimeException(
            __FILE__, __LINE__,
            "Failed to set stack size setting: %d.", stackSize);
    }

    pthread_attr_destroy( &attributes );
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::yeild() {

    #ifdef HAVE_PTHREAD_YIELD
        pthread_yield();
    #else
        #ifdef HAVE_SCHED_YIELD
            sched_yield();
        #endif
    #endif
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::createTlsKey(decaf_tls_key* tlsKey) {
    pthread_key_create(tlsKey, NULL);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::destroyTlsKey(decaf_tls_key tlsKey) {
    pthread_key_delete(tlsKey);
}

////////////////////////////////////////////////////////////////////////////////
void* PlatformThread::getTlsValue(decaf_tls_key tlsKey) {
    return pthread_getspecific(tlsKey);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::setTlsValue(decaf_tls_key tlsKey, void* value) {
    pthread_setspecific(tlsKey, value);
}
