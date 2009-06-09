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

#ifndef _DECAF_UTIL_ABSTRACTMAP_H_
#define _DECAF_UTIL_ABSTRACTMAP_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/Map.h>
#include <decaf/util/Set.h>
#include <memory>

namespace decaf {
namespace util {

    /**
     * This class provides a skeletal implementation of the Map  interface, to minimize the
     * effort required to implement this interface.
     *
     * To implement an unmodifiable map, the programmer needs only to extend this class and
     * provide an implementation for the entrySet method, which returns a set-view of the
     * map's mappings. Typically, the returned set will, in turn, be implemented atop
     * AbstractSet. This set should not support the add or remove methods, and its iterator
     * should not support the remove method.
     *
     * To implement a modifiable map, the programmer must additionally override this class's
     * put method (which otherwise throws an UnsupportedOperationException), and the iterator
     * returned by entrySet().iterator() must additionally implement its remove method.
     *
     * The programmer should generally provide a void (no argument) and map constructor, as
     * per the recommendation in the Map interface specification.
     *
     * The documentation for each non-abstract method in this class describes its
     * implementation in detail. Each of these methods may be overridden if the map being
     * implemented admits a more efficient implementation.
     *
     * @since 1.0
     */
    template< typename K, typename V, typename COMPARATOR>
    class AbstractMap : public decaf::util::Map<K, V, COMPARATOR> {
    public:

        virtual ~AbstractMap() {}

    };

}}

#endif /* _DECAF_UTIL_ABSTRACTMAP_H_ */
