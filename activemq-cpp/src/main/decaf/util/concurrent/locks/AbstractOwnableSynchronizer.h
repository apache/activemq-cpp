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

#ifndef _DECAF_UTIL_CONCURRENT_ABSTRACTOWNABLESYNCHRONIZER_H_
#define _DECAF_UTIL_CONCURRENT_ABSTRACTOWNABLESYNCHRONIZER_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace lang {
    class Thread;
}
namespace util {
namespace concurrent {
namespace locks {

    /**
     * Base class for locks that provide the notion of Ownership, the types of locks
     * that are implemented using this base class would be owned by one specific Thread
     * at any given time.
     *
     * @since 1.0
     */
    class DECAF_API AbstractOwnableSynchronizer {
    private:

        decaf::lang::Thread* ownerThread;

    public:

        virtual ~AbstractOwnableSynchronizer();

    protected:

        AbstractOwnableSynchronizer();

        /**
         * Gets the Thread that was last set using the setExclusiveOwnerThread method, or NULL
         * if no Thread has been made the exclusive owner.
         *
         * @return pointer to the owner Thread or NULL if not set.
         */
        decaf::lang::Thread* getExclusiveOwnerThread() const;

        /**
         * Sets the Thread that has exclusive ownership of this Synchronizer, can be NULL
         * to indicate that no Thread now owns this Synchronizer.
         *
         * @param thread
         *      The Thread that now has ownership, or NULL if ownership is released.
         */
        void setExclusiveOwnerThread(decaf::lang::Thread* thread);

    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_ABSTRACTOWNABLESYNCHRONIZER_H_ */
