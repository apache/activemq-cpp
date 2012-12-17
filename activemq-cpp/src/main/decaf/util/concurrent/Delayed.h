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

#ifndef _DECAF_UTIL_CONCURRENT_DELAYED_H_
#define _DECAF_UTIL_CONCURRENT_DELAYED_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Comparable.h>
#include <decaf/util/concurrent/TimeUnit.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * A mix-in style interface for marking objects that should be acted upon after a
     * given delay.
     * <p>
     * An implementation of this interface must define a Comparable methods that provides an
     * ordering consistent with its getDelay method.
     */
    class DECAF_API Delayed : public decaf::lang::Comparable<Delayed> {
    public:

        virtual ~Delayed() {}

        /**
         * Returns the remaining delay associated with this object, in the given time unit.
         *
         * @param unit
         *        The time unit
         *
         * @returns the remaining delay; zero or negative values indicate that the delay
         *          has already elapsed
         */
        virtual long long getDelay( const TimeUnit& unit ) = 0;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_DELAYED_H_ */
