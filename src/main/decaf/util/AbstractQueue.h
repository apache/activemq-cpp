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

#ifndef ABSTRACTQUEUE_H_
#define ABSTRACTQUEUE_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/Iterable.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/Queue.h>
#include <memory>

namespace decaf {
namespace util {

    /**
     * This class provides skeletal implementations of some Queue  operations.
     * Methods add, remove, and element are based on offer, poll, and peek, respectively
     * but throw exceptions instead of indicating failure via false or null returns.
     *
     * A Queue implementation that extends this class must minimally define a method Queue.
     * offer(E) which does not permit insertion of null elements, along with methods Queue.
     * peek(), Queue.poll(), Collection.size(), and a Collection.iterator() supporting
     * Iterator.remove(). Typically, additional methods will be overridden as well. If these
     * requirements cannot be met, consider instead subclassing AbstractCollection.
     *
     * @since 1.0
     */
    template< typename E >
    class DECAF_API AbstractQueue : public decaf::util::Queue<E> {
    public:

        virtual ~AbstractQueue() {}

    };

}}

#endif /* ABSTRACTQUEUE_H_ */
