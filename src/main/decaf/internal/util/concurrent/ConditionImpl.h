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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_CONDITIONIMPL_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_CONDITIONIMPL_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace util{
namespace concurrent{
    class MutexHandle;
    class ConditionHandle;
}}
namespace internal {
namespace util {
namespace concurrent {

    class DECAF_API ConditionImpl {
    private:

        ConditionImpl() {}

    public:

        /**
         * Creates the Condition object and attaches it to the given MutexHandle.
         *
         * @param mutex the Mutex handle that this Condition is attached to.
         *
         * @return a newly constructed Condition handle that is attached to the given handle.
         */
        static decaf::util::concurrent::ConditionHandle* create( decaf::util::concurrent::MutexHandle* mutex );

        /**
         * Destroy a previously create Condition instance.
         *
         * @param handle The Condition handle to be destroyed.
         */
        static void destroy( decaf::util::concurrent::ConditionHandle* handle );

        /**
         * Waits for the condition to be signaled.
         *
         * @param condition the handle to the condition to wait on.
         */
        static void wait( decaf::util::concurrent::ConditionHandle* condition );

        /**
         * Waits for the condition to be signaled or for the time specified to ellapse.
         *
         * @param condition the handle to the condition to wait on.
         * @param mills the time in milliseconds to wait for the condition to be signaled.
         * @param nanos additional time in nanoseconds to wait for the thread to be signaled.
         */
        static void wait( decaf::util::concurrent::ConditionHandle* condition, long long mills, long long nanos );

        /**
         * Signals one Thread that is waiting on this condition to wake up.
         *
         * @param condition the handle to the condition to wait on.
         */
        static void notify( decaf::util::concurrent::ConditionHandle* condition );

        /**
         * Signals all Threads that is waiting on this condition to wake up.
         *
         * @param condition the handle to the condition to wait on.
         */
        static void notifyAll( decaf::util::concurrent::ConditionHandle* condition );

    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_CONDITIONIMPL_H_ */
