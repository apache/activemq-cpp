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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERSTACK_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERSTACK_H_

#include <decaf/internal/util/concurrent/Transferer.h>

#include <decaf/util/concurrent/TimeoutException.h>
#include <decaf/lang/exceptions/InterruptedException.h>

namespace decaf {
namespace internal {
namespace util {
namespace concurrent {

    template<typename E>
    class TransferStack : public Transferer<E> {
    public:

        TransferStack() {}
        virtual ~TransferStack() {}

        virtual void transfer( E* e, bool timed, long long nanos ) {
        }

        virtual E* transfer( bool timed, long long nanos ) {
            return NULL;
        }

    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERSTACK_H_ */
