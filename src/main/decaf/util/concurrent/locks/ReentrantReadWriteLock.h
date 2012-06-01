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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTREADWRITELOCK_H_
#define _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTREADWRITELOCK_H_

#include <decaf/util/Config.h>

#include <decaf/util/concurrent/locks/ReadWriteLock.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class ReentrantReadWriteLockImpl;

    /**
     *
     * @since 1.0
     */
    class DECAF_API ReentrantReadWriteLock : public ReadWriteLock {
    private:

        ReentrantReadWriteLockImpl* impl;

    public:

        /**
         * Creates a new ReentrantReadWriteLock with the default ordering property of Not-Fair.
         */
        ReentrantReadWriteLock();

        /**
         * Creates a new ReentrantReadWriteLock with the given fairness policy.
         *
         * @param fair
         *      Boolean value indicating whether this lock uses a fair or non-fair policy.
         */
        ReentrantReadWriteLock(bool fair);

        virtual ~ReentrantReadWriteLock();

    public:

        /**
         * {@inheritDoc}
         */
        virtual Lock& readLock();

        /**
         * {@inheritDoc}
         */
        virtual Lock& writeLock();

    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTREADWRITELOCK_H_ */
