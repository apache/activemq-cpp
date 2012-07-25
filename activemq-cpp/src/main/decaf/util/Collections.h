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
         * Creates and returns a new STL vector instance that contains a copy of each
         * element in the supplied collection instance.  If the Collection is empty the
         * returned vector will also be empty.
         *
         * @param collection
         *      The collection whose elements are to be copied to a std::vector.
         *
         * @returns a new std::vector containing a copy of each element in the Collection.
         */
        template<typename E>
        static std::vector<E> toStlVector(const Collection<E>& collection) {

            std::vector<E> result(collection.size());

            typename std::auto_ptr< Iterator<E> > iterator(collection.iterator());
            while (iterator->hasNext()) {
                result.push_back(iterator->next());
            }

            return result;
        }
    };

}}

#endif /* _DECAF_UTIL_COLLECTIONS_H_ */
