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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_SYNCHRONIZABLEIMPL_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_SYNCHRONIZABLEIMPL_H_

#include <decaf/util/Config.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>

namespace decaf {
namespace internal {
namespace util {
namespace concurrent {

    /**
     * A convenience class used by some Decaf classes to implement the Synchronizable interface
     * when there is no issues related to multiple inheritance.
     *
     * @since 1.0
     */
    class DECAF_API SynchronizableImpl : public decaf::util::concurrent::Synchronizable {
    private:

        decaf::util::concurrent::Mutex mutex;

    public:

        SynchronizableImpl();
        virtual ~SynchronizableImpl();

        virtual void lock();

        virtual bool tryLock();

        virtual void unlock();

        virtual void wait();

        virtual void wait( long long millisecs );

        virtual void wait( long long millisecs, int nanos );

        virtual void notify();

        virtual void notifyAll();

    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_SYNCHRONIZABLEIMPL_H_ */
