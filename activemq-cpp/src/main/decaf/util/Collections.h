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

#ifndef _DECAF_UTIL_COLLECTIONS_H_
#define _DECAF_UTIL_COLLECTIONS_H_

#include <decaf/util/Config.h>

#include <decaf/util/Collection.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/ListIterator.h>
#include <decaf/util/List.h>
#include <decaf/lang/Pointer.h>

#include <vector>
#include <memory>

namespace decaf {
namespace util {

    class DECAF_API Collections {
    private:

        Collections();
        Collections(const Collections&);
        Collections& operator= (const Collections&);

    public:

        /**
         * Modifies the specified List by reversing the order of the elements.
         *
         * @param list
         *      The list to reverse.
         * @throws UnsupportedOperationException
         *      when replacing an element in the List is not supported.
         */
        template<typename E>
        static void reverse(List<E>& list) {
            int size = list.size();
            decaf::lang::Pointer<ListIterator<E> > front(list.listIterator());
            decaf::lang::Pointer<ListIterator<E> > back(list.listIterator(size));

            for (int i = 0; i < size / 2; i++) {
                E frontNext = front->next();
                E backPrev = back->previous();
                front->set(backPrev);
                back->set(frontNext);
            }
        }

    };

}}

#endif /* _DECAF_UTIL_COLLECTIONS_H_ */
