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

#ifndef _DECAF_UTIL_CONCURRENT_CALLABLE_H_
#define _DECAF_UTIL_CONCURRENT_CALLABLE_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Exception.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * A task that returns a result and may throw an exception. Implementors define a single method with no
     * arguments called call.  This interface differs from the Runnable interface in that a Callable object
     * can return a result and is allowed to throw an exceptions from its call method.
     *
     * The Executors class contains utility methods to convert from other common forms to Callable classes.
     *
     * @since 1.0
     */
    template<typename V>
    class DECAF_API Callable {
    public:

        virtual ~Callable() {}

        /**
         * Computes a result, or throws an exception if unable to do so.
         * @returns
         *      Computed Result.
         * @throws Exception
         *      If unable to compute a result.
         */
        virtual V call() = 0;

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_CALLABLE_H_*/
