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

#ifndef _DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICINTEGER_H_
#define _DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICINTEGER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Number.h>
#include <string>

namespace decaf {
namespace util {
namespace concurrent {
namespace atomic {

    /**
     * An int value that may be updated atomically. An AtomicInteger is used in
     * applications such as atomically incremented counters, and cannot be used
     * as a replacement for an Integer. However, this class does extend Number
     * to allow uniform access by tools and utilities that deal with
     * numerically-based classes.
     */
    class DECAF_API AtomicInteger : public decaf::lang::Number {
    private:

        volatile unsigned int value;

    public:

        /**
         * Create a new AtomicInteger with an initial value of 0.
         */
        AtomicInteger();

        /**
         * Create a new AtomicInteger with the given initial value.
         * @param initialValue - The initial value of this object.
         */
        AtomicInteger( int initialValue );

        virtual ~AtomicInteger() {}

        /**
         * Gets the current value.
         * @returns the current value.
         */
        int get() const {
            return this->value;
        }

        /**
         * Sets to the given value.
         * @param newValue - the new value
         */
        void set( int newValue ) {
            this->value = newValue;
        }

        /**
         * Atomically sets to the given value and returns the old value.
         * @param newValue - the new value.
         * @returns the previous value.
         */
        int getAndSet( int newValue );

        /**
         * Atomically sets the value to the given updated value if the current
         * value == the expected value.
         *
         * @param expect - the expected value
         * @param update - the new value
         * @returns true if successful. False return indicates that the actual
         * value was not equal to the expected value.
         */
        bool compareAndSet( int expect, int update );

        /**
         * Atomically increments by one the current value.
         * @returns the previous value.
         */
        int getAndIncrement();

        /**
         * Atomically decrements by one the current value.
         * @returns the previous value.
         */
        int getAndDecrement();

        /**
         * Atomically adds the given value to the current value.
         * @param delta - The value to add.
         * @returns the previous value.
         */
        int getAndAdd( int delta );

        /**
         * Atomically increments by one the current value.
         * @returns the updated value.
         */
        int incrementAndGet();

        /**
         * Atomically decrements by one the current value.
         * @returns the updated value.
         */
        int decrementAndGet();

        /**
         * Atomically adds the given value to the current value.
         * @param delta - the value to add.
         * @returns the updated value.
         */
        int addAndGet( int delta );

        /**
         * Returns the String representation of the current value.
         * @returns the String representation of the current value.
         */
        std::string toString() const;

        /**
         * Description copied from class: Number
         * Returns the value of the specified number as an int. This may involve
         * rounding or truncation.
         * @returns the numeric value represented by this object after conversion
         * to type int.
         */
        int intValue() const;

        /**
         * Description copied from class: Number
         * Returns the value of the specified number as a long. This may involve
         * rounding or truncation.
         * @returns the numeric value represented by this object after conversion
         * to type long long.
         */
        long long longValue() const;

        /**
         * Description copied from class: Number
         * Returns the value of the specified number as a float. This may involve
         * rounding.
         * @returns the numeric value represented by this object after conversion
         * to type float.
         */
        float floatValue() const;

        /**
         * Description copied from class: Number
         * Returns the value of the specified number as a double. This may
         * involve rounding.
         * @returns the numeric value represented by this object after conversion
         * to type double.
         */
        double doubleValue() const;

    };

}}}}

#endif /*_DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICINTEGER_H_ */
