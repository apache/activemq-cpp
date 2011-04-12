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


#ifndef _DECAF_UTIL_CONCURRENT_MUTEXHANDLE_H_
#define _DECAF_UTIL_CONCURRENT_MUTEXHANDLE_H_

#include <decaf/util/Config.h>

#if HAVE_PTHREAD_H
#include <pthread.h>
#endif

namespace decaf {
namespace util {
namespace concurrent {

    class DECAF_API MutexHandle {
    public:

        MutexHandle() : mutex(), lock_owner(0), lock_count(0) {
        }

        ~MutexHandle() {
        }

        // The mutex object.
        pthread_mutex_t mutex;

        // List of waiting threads
        // std::list<pthread_cond_t*> eventQ;

        // Lock Status Members
        volatile long long lock_owner;
        volatile long long lock_count;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_MUTEXHANDLE_H_ */
