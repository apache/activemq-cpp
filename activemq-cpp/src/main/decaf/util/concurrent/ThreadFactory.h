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

#ifndef _DECAF_UTIL_CONCURRENT_THREADFACTORY_H_
#define _DECAF_UTIL_CONCURRENT_THREADFACTORY_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * public interface ThreadFactory
     *
     * An object that creates new threads on demand. Using thread factories removes
     * hardwiring of calls to new Thread, enabling applications to use special thread
     * subclasses, priorities, etc.
     *
     * The simplest implementation of this interface is just:
     *
     *    class SimpleThreadFactory : public ThreadFactory {
     *    public:
     *       Thread* newThread( Runnable* r ) {
     *          return new Thread(r);
     *       }
     *    }
     *
     * The Executors.defaultThreadFactory() method provides a more useful simple
     * implementation, that sets the created thread context to known values before
     * returning it.
     *
     * @since 1.0
     */
    class DECAF_API ThreadFactory {
    public:

        virtual ~ThreadFactory() {}

        /**
         * Constructs a new Thread. Implementations may also initialize priority, name,
         * daemon status, ThreadGroup, etc.  The pointer passed is still owned by the caller
         * and is not deleted by the Thread object.  The caller owns the returned Thread
         * object and must delete it when finished.
         *
         * @param r
         *      A pointer to a Runnable instance to be executed by new Thread instance returned.
         *
         * @returns constructed thread, or NULL if the request to create a thread is rejected
         *          the caller owns the returned pointer.
         */
        virtual decaf::lang::Thread* newThread( decaf::lang::Runnable* r ) = 0;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_THREADFACTORY_H_ */
