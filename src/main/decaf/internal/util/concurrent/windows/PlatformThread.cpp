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

#include <decaf/lang/Thread.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#include <memory>

#ifdef HAVE_PROCESS_H
#include <process.h>
#endif

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::createMutex(decaf_mutex_t* mutex) {
	*mutex = new CRITICAL_SECTION;
    ::InitializeCriticalSection(*mutex);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::lockMutex(decaf_mutex_t mutex) {
    ::EnterCriticalSection(mutex);
}

////////////////////////////////////////////////////////////////////////////////
bool PlatformThread::tryLockMutex(decaf_mutex_t mutex) {
    return ::TryEnterCriticalSection(mutex) > 0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::unlockMutex(decaf_mutex_t mutex) {
    ::LeaveCriticalSection(mutex);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::destroyMutex(decaf_mutex_t mutex) {
    ::DeleteCriticalSection(mutex);
	delete mutex;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::createCondition(decaf_condition_t* condition) {

    *condition = ::CreateEvent(NULL, TRUE, FALSE, NULL);

    if (condition == NULL) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to initialize OS Condition object.");
    }
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::notify(decaf_condition_t condition) {
    ::SetEvent(condition);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::notifyAll(decaf_condition_t condition) {
    ::SetEvent(condition);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::waitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex) {

    ::ResetEvent(condition);
    PlatformThread::unlockMutex(mutex);
    ::WaitForSingleObject(condition, INFINITE);
    PlatformThread::lockMutex(mutex);
}

////////////////////////////////////////////////////////////////////////////////
bool PlatformThread::waitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex,
                                     long long mills, int nanos) {

    ::ResetEvent(condition);
    PlatformThread::unlockMutex(mutex);
    int result = ::WaitForSingleObject(condition, (DWORD)mills);
    PlatformThread::lockMutex(mutex);

    if (result == WAIT_TIMEOUT) {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::interruptibleWaitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex,
                                                  CompletionCondition& complete) {

    do {

        ::ResetEvent(condition);
        PlatformThread::unlockMutex(mutex);
        ::WaitForSingleObject(condition, INFINITE);
        PlatformThread::lockMutex(mutex);

        if (complete()) {
            break;
        }

    } while(true);
}

////////////////////////////////////////////////////////////////////////////////
bool PlatformThread::interruptibleWaitOnCondition(decaf_condition_t condition, decaf_mutex_t mutex,
                                                  long long mills, int nanos, CompletionCondition& complete) {

    DWORD startTime = ::GetTickCount();
    DWORD timeOut = (DWORD)mills;
    DWORD initialTimeOut = (DWORD)mills;

    bool result = false;

    do {

        ::ResetEvent(condition);
        PlatformThread::unlockMutex(mutex);
        int timedOut = ::WaitForSingleObject(condition, timeOut);
        PlatformThread::lockMutex(mutex);

        if (timedOut == WAIT_TIMEOUT) {

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

        timeOut = initialTimeOut - (::GetTickCount() - startTime);
        if( timeOut < 0) {
            timeOut = 0;
        }

    } while(true);

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::destroyCondition(decaf_condition_t condition) {
    ::CloseHandle(condition);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::initPriorityMapping(int maxPriority, std::vector<int>& mapping) {

    mapping.clear();
    mapping.resize(maxPriority + 1);

    for (int i = 0; i <= maxPriority; ++i) {
        mapping[i] = i;
    }
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::createNewThread(decaf_thread_t* handle, threadMainMethod threadMain, void* threadArg,
                                     int priority, long long stackSize, long long* threadId) {

    unsigned int osThreadId = 0;

    *handle = (HANDLE)_beginthreadex(
         NULL, (DWORD)0, threadMain, threadArg, 0, &osThreadId );

    if (*handle == 0) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to create new Thread." );
    }

    *threadId = (long long)osThreadId;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::detachThread(decaf_thread_t handle) {
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::detachOSThread(decaf_thread_t handle) {
    ::CloseHandle(handle);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::joinThread(decaf_thread_t handle) {
    ::WaitForSingleObject(handle, INFINITE);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::exitThread() {
    _endthreadex(0);
}

////////////////////////////////////////////////////////////////////////////////
decaf_thread_t PlatformThread::getCurrentThread() {
    return ::GetCurrentThread();
}

////////////////////////////////////////////////////////////////////////////////
decaf_thread_t PlatformThread::getSafeOSThreadHandle() {

    decaf_thread_t value;

    ::DuplicateHandle(::GetCurrentProcess(), ::GetCurrentThread(),
                      ::GetCurrentProcess(), &value, 0, TRUE, DUPLICATE_SAME_ACCESS);

    return value;
}

////////////////////////////////////////////////////////////////////////////////
long long PlatformThread::getCurrentThreadId() {
    return ::GetCurrentThreadId();
}

////////////////////////////////////////////////////////////////////////////////
int PlatformThread::getPriority(decaf_thread_t thread) {
    return Thread::NORM_PRIORITY;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::setPriority(decaf_thread_t thread, int priority) {
}

////////////////////////////////////////////////////////////////////////////////
long long PlatformThread::getStackSize(decaf_thread_t thread DECAF_UNUSED) {
    return PLATFORM_DEFAULT_STACK_SIZE;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::setStackSize(decaf_thread_t thread DECAF_UNUSED, long long stackSize DECAF_UNUSED) {
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::yeild() {
    SwitchToThread();
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::createTlsKey(decaf_tls_key* tlsKey) {
    if (tlsKey == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "TLS Key pointer must not be NULL.");
    }

    *tlsKey = ::TlsAlloc();
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::destroyTlsKey(decaf_tls_key tlsKey) {
    ::TlsFree(tlsKey);
}

////////////////////////////////////////////////////////////////////////////////
void* PlatformThread::getTlsValue(decaf_tls_key tlsKey) {
    return ::TlsGetValue(tlsKey);
}

////////////////////////////////////////////////////////////////////////////////
void PlatformThread::setTlsValue(decaf_tls_key tlsKey, void* value) {
    ::TlsSetValue(tlsKey, value);
}
