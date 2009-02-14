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

#ifndef _DECAF_UTIL_ABSTRACTSET_H_
#define _DECAF_UTIL_ABSTRACTSET_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/Iterable.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/Set.h>
#include <memory>

namespace decaf {
namespace util {

    /**
     * This class provides a skeletal implementation of the Set  interface to minimize the
     * effort required to implement this interface.
     *
     * The process of implementing a set by extending this class is identical to that of
     * implementing a Collection by extending AbstractCollection, except that all of the
     * methods and constructors in subclasses of this class must obey the additional
     * constraints imposed by the Set interface (for instance, the add method must not
     * permit addition of multiple instances of an object to a set).
     *
     * Note that this class does not override any of the implementations from the
     * AbstractCollection class. It merely adds implementations for equals and removeAll.
     */
    template<typename E >
    class DECAF_API AbstractSet : public decaf::util::Set<E> {
    public:

        virtual ~AbstractSet() {}

    };

}}

#endif /* _DECAF_UTIL_ABSTRACTSET_H_ */
