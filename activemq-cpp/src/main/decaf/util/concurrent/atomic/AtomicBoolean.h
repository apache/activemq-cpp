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

#ifndef _DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICBOOLEAN_H_
#define _DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICBOOLEAN_H_

#include <string>
#include <decaf/util/Config.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace atomic {

    /**
     * A boolean value that may be updated atomically. An AtomicBoolean is used
     * in applications such as atomically updated flags, and cannot be used as a
     * replacement for a Boolean.
     */
    class DECAF_API AtomicBoolean {
    private:

        volatile unsigned int value;

    public:

        /**
         * Creates a new AtomicBoolean whose initial value is false.
         */
        AtomicBoolean();

        /**
         * Creates a new AtomicBoolean with the initial value.
         * @param initialValue - The initial value of this boolean.
         */
        AtomicBoolean( bool initialValue );

        virtual ~AtomicBoolean() {}

        /**
         * Gets the current value of this AtomicBoolean.
         * @returns the currently set value.
         */
        bool get() const {
            return value == 0 ? false : true;
        }

        /**
         * Unconditionally sets to the given value.
         * @param newValue - the new value
         */
        void set( bool newValue ) {
            this->value = newValue ? 1 : 0;
        }

        /**
         * Atomically sets the value to the given updated value if the current
         * value == the expected value.
         *
         * @param expect - the expected value
         * @param update - the new value
         * @returns true if successful. False return indicates that the actual value
         * was not equal to the expected value.
         */
        bool compareAndSet( bool expect, bool update );

        /**
         * Atomically sets to the given value and returns the previous value.
         *
         * @param newValue - the new value
         * @returns the previous value
         */
        bool getAndSet( bool newValue );

        /**
         * Returns the String representation of the current value.
         * @returns the String representation of the current value.
         */
        std::string toString() const;

    };

}}}}

#endif /*_DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICBOOLEAN_H_*/
