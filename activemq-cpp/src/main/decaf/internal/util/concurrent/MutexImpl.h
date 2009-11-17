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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_MUTEXIMPL_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_MUTEXIMPL_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace util {
namespace concurrent {
    class Mutex;
    class MutexHandle;
}}
namespace internal {
namespace util {
namespace concurrent {

    class DECAF_API MutexImpl {
    private:

        MutexImpl();

    public:

        /**
         * Creates a Reentrant Mutex and returns the handle, throws a Runtime Exception
         * if the Mutex cannot be created for some reason.
         *
         * @returns handle to a newly created Mutex.
         */
        static decaf::util::concurrent::MutexHandle* create();

        /**
         * Destroy a previously create Mutex instance.
         *
         * @param mutex The Mutex instance to be destroyed.
         */
        static void destroy( decaf::util::concurrent::MutexHandle* handle );

        /**
         * Locks the Mutex.  If the Mutex is already locked by another thread this method
         * blocks until the Mutex becomes unlocked and this thread acquires the lock.
         *
         * @param handle the handle to the Mutex to Lock.
         */
        static void lock( decaf::util::concurrent::MutexHandle* handle );

        /**
         * Tries to lock the Mutex.  If the Mutex is unlocked this Thread acquires the lock on
         * the Mutex and this method returns true, if the Mutex is already locked then the lock
         * is not acquired and this method returns false.
         *
         * @param handle the handle to the Mutex to attempt to Lock.
         *
         * @return true if the lock was acquired false otherwise.
         */
        static bool trylock( decaf::util::concurrent::MutexHandle* handle );

        /**
         * Unlocks the Mutex allowing other Thread to then acquire the Lock on it.
         *
         * @param handle the handle to the Mutex to attempt to Lock.
         */
        static void unlock( decaf::util::concurrent::MutexHandle* handle );

    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_MUTEXIMPL_H_ */
