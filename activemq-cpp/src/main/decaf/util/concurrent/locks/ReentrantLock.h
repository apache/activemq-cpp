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

#ifndef _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTLOCK_H_
#define _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTLOCK_H_

#include <decaf/util/Config.h>

#include <decaf/util/concurrent/locks/Lock.h>

#include <memory>

namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class LockHandle;

    /**
     * A reentrant mutual exclusion Lock with extended capabilities.
     *
     * A ReentrantLock is owned by the thread last successfully locking, but not yet unlocking it.
     * A thread invoking lock will return, successfully acquiring the lock, when the lock is not
     * owned by another thread. The method will return immediately if the current thread already
     * owns the lock. This can be checked using methods isHeldByCurrentThread(), and getHoldCount().
     *
     * The constructor for this class accepts an optional fairness parameter. When set true, under
     * contention, locks favor granting access to the longest-waiting thread. Otherwise this lock
     * does not guarantee any particular access order. Programs using fair locks accessed by many
     * threads may display lower overall throughput (i.e., are slower; often much slower) than
     * those using the default setting, but have smaller variances in times to obtain locks and
     * guarantee lack of starvation. Note however, that fairness of locks does not guarantee
     * fairness of thread scheduling. Thus, one of many threads using a fair lock may obtain it
     * multiple times in succession while other active threads are not progressing and not currently
     * holding the lock. Also note that the untimed tryLock method does not honor the fairness
     * setting. It will succeed if the lock is available even if other threads are waiting.
     *
     * It is recommended practice to always immediately follow a call to lock with a try block,
     * most typically in a before/after construction such as:
     *
     *    class X {
     *    private:
     *
     *        ReentrantLock lock;
     *        // ...
     *
     *    public:
     *
     *        void m() {
     *            lock.lock();  // block until condition holds
     *
     *            try {
     *                // ... method body
     *            } finally {
     *                lock.unlock()
     *            }
     *        }
     *    }
     *
     * In addition to implementing the Lock interface, this class defines methods isLocked and
     * getLockQueueLength, as well as some associated protected access methods that may be useful
     * for instrumentation and monitoring.
     *
     * This lock supports a maximum of 2147483647 recursive locks by the same thread. Attempts
     * to exceed this limit result in Error throws from locking methods.
     *
     * @since 1.0
     */
    class DECAF_API ReentrantLock : public Lock {
    private:

        std::auto_ptr<LockHandle> handle;

    public:

        ReentrantLock();

        virtual ~ReentrantLock();

    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_LOCKS_REENTRANTLOCK_H_ */
