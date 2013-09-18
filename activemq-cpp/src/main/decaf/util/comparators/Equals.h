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
#ifndef _DECAF_UTIL_COMPARATORS_EQUALS_H_
#define _DECAF_UTIL_COMPARATORS_EQUALS_H_

#include <decaf/util/Comparator.h>

#include <decaf/lang/Pointer.h>

namespace decaf {
namespace util {
namespace comparators {

    /**
     * Equality comparator that utilizes the default equality comparison expression == to
     * determine if two values are considered equal.  Can be used in Collection classes as
     * a means of comparing objects.
     */
    template< typename E >
    class Equals : public decaf::util::Comparator<E> {
    public:

        Equals() {}
        virtual ~Equals() {}

        virtual bool operator()(const E& left, const E& right) const {
            return left == right;
        }

        virtual int compare(const E& o1, const E& o2) const {

            if (o1 == o2) {
                return 0;
            }

            if (o1 < o2) {
                return -1;
            }

            return 1;
        }

    };

    template< typename E >
    class Equals< decaf::lang::Pointer<E> > : public decaf::util::Comparator<decaf::lang::Pointer<E> > {
    public:

        Equals() {}
        virtual ~Equals() {}

        virtual bool operator()(const decaf::lang::Pointer<E>& left, const decaf::lang::Pointer<E>& right) const {
            if (left != NULL && right != NULL) {
                return left->equals(*right);
            } else if (left == NULL && right == NULL) {
                return true;
            }

            return false;
        }

        virtual int compare(const decaf::lang::Pointer<E>& o1, const decaf::lang::Pointer<E>& o2) const {
            if (o1 != NULL && o2 != NULL) {
                return o1->compareTo(*o2);
            }

            if (o1.get() > o2.get()) {
                return 1;
            } else if (o1.get() < o2.get()) {
                return -1;
            }

            return 0;
        }

    };

}}}

#endif /* _DECAF_UTIL_COMPARATORS_EQUALS_H_ */
