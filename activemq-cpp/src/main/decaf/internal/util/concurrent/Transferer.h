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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERER_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERER_H_

#include <decaf/lang/exceptions/InterruptedException.h>
#include <decaf/util/concurrent/TimeoutException.h>

namespace decaf {
namespace internal {
namespace util {
namespace concurrent {

    /**
    * Shared internal API for dual stacks and queues.
    */
    template< typename E >
    class Transferer {

        /**
         * Performs a put
         *
         * @param e the item to be handed to a consumer;
         * @param timed if this operation should timeout
         * @param nanos the timeout, in nanoseconds
         *
         * @throws TimeoutException if the operation timed out waiting for
         *         the consumer to accept the item offered.
         * @throws InterruptedException if the thread was interrupted while
         *         waiting for the consumer to accept the item offered.
         */
        virtual void transfer( E* e, bool timed, long long nanos ) = 0;

        /**
         * Performs a take.
         *
         * @param timed if this operation should timeout
         * @param nanos the timeout, in nanoseconds
         *
         * @return the item provided or received;
         *
         * @throws TimeoutException if the operation timed out waiting for
         *         the producer to offer an item.
         * @throws InterruptedException if the thread was interrupted while
         *         waiting for the producer to offer an item.
         */
        virtual E* transfer( bool timed, long long nanos ) = 0;

    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERER_H_ */
