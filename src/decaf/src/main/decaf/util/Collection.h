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

#ifndef _DECAF_UTIL_COLLECTION_H_
#define _DECAF_UTIL_COLLECTION_H_

#include <decaf/util/Config.h>

namespace decaf{
namespace util{

    /**
     * The root interface in the collection hierarchy. A collection represents
     * a group of objects, known as its elements. Some collections allow
     * duplicate elements and others do not. Some are ordered and others
     * unordered. This interface is typically used to pass collections
     * around and manipulate them where maximum generality is desired.
     *
     * All general-purpose Collection implementation classes (which typically
     * implement Collection indirectly through one of its subinterfaces) should
     * provide two "standard" constructors: a void (no arguments) constructor,
     * which creates an empty collection, and a constructor with a single argument
     * of type Collection, which creates a new collection with the same elements
     * as its argument. In effect, the latter constructor allows the user to copy
     * any collection, producing an equivalent collection of the desired
     * implementation type. There is no way to enforce this convention
     * (as interfaces cannot contain constructors) but all of the general-purpose
     * Collection implementations in the Decaf platform libraries comply.
     *
     * The "destructive" methods contained in this interface, that is, the methods
     * that modify the collection on which they operate, are specified to throw
     * UnsupportedOperationException if this collection does not support the
     * operation. If this is the case, these methods may, but are not required
     * to, throw an UnsupportedOperationException if the invocation would have
     * no effect on the collection. For example, invoking the addAll(Collection)
     * method on an unmodifiable collection may, but is not required to, throw
     * the exception if the collection to be added is empty.
     *
     * Many methods in Collections Framework interfaces are defined in terms of
     * the equals method. For example, the specification for the
     * contains(Object o) method says: "returns true if and only if this
     * collection contains at least one element e such that
     * (o==null ? e==null : o.equals(e))."
     *
     * @since 1.0
     */
    template< typename T >
    class DECAF_API Collection
    {
    public:

        virtual ~Collection() {}

    };

}}

#endif /*_DECAF_UTIL_COLLECTION_H_*/
