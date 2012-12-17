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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_WINDOWS_PLATFORMDEFS_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_WINDOWS_PLATFORMDEFS_H_

#include <decaf/util/Config.h>

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
#ifdef HAVE_PROCESS_H
#include <process.h>
#endif

namespace decaf{
namespace internal{
namespace util{
namespace concurrent{

    struct RWLOCK {
        HANDLE writeMutex;
        HANDLE readEvent;
        volatile LONG readers;
    };

    typedef void* PLATFORM_THREAD_ENTRY_ARG;

    #define PLATFORM_THREAD_RETURN() return 0;
    #define PLATFORM_THREAD_CALLBACK_TYPE unsigned
    #define PLATFORM_DEFAULT_STACK_SIZE 0x8000
    #define PLATFORM_CALLING_CONV __stdcall

    typedef HANDLE decaf_thread_t;
    typedef DWORD decaf_tls_key;
    typedef HANDLE decaf_condition_t;
    typedef LPCRITICAL_SECTION decaf_mutex_t;
    typedef RWLOCK* decaf_rwmutex_t;

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_WINDOWS_PLATFORMDEFS_H_ */
