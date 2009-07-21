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

#ifndef _DECAF_UTIL_COMPARATORS_LESS_H_
#define _DECAF_UTIL_COMPARATORS_LESS_H_

#include <decaf/util/Comparator.h>

namespace decaf {
namespace util {
namespace comparators {

    /**
     * Simple Less Comparator that compares to elements to determine if the first is
     * less than the second.  This can be used in Collection classes to sort elements
     * according to their natural ordering.  By design the Comparator's compare function
     * return more information about comparison than the STL binary function's boolean
     * compare operator.  In this case the compare method will return
     *
     * @since 1.0
     */
    template< typename E >
    class Less : public decaf::util::Comparator<E> {
    public:

        Less() {}
        virtual ~Less() {}

        virtual bool operator() ( const E& left, const E& right ) const {
            return left < right;
        }

        virtual int compare( const E& o1, const E& o2 ) const {

            if( o1 > o2 ) {
                return 1;
            } else if( o1 < o2 ) {
                return -1;
            }

            return 0;
        }

    };

}}}

#endif /* _DECAF_UTIL_COMPARATORS_LESS_H_ */
