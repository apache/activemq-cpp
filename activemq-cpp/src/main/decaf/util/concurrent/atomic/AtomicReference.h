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

#ifndef _DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICREFERENCE_H_
#define _DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICREFERENCE_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Long.h>
#include <decaf/internal/util/concurrent/Atomics.h>

namespace decaf {
namespace util {
namespace concurrent {
namespace atomic {

    /**
     * An Pointer reference that may be updated atomically.
     */
    template< typename T >
    class AtomicReference {
    private:

        volatile void* value;

    private:

        AtomicReference(const AtomicReference&);
        AtomicReference& operator= (const AtomicReference&);

    public:

        AtomicReference() : value( NULL ) {}
        AtomicReference(T* value) : value((void*)value) {}

        virtual ~AtomicReference() {}

        /**
         * Gets the Current Value.
         * @return the current value of this Reference.
         */
        T* get() const {
            return (T*)value;
        }

        /**
         * Sets the Current value of this Reference.
         *
         * @param newValue
         *        The new Value of this Reference.
         */
        void set( T* newValue ) {
            internal::util::concurrent::Atomics::getAndSet(&this->value, (void*)newValue);
        }

        /**
         * Atomically sets the value to the given updated value if the current value ==
         * the expected value.
         *
         * @param expect - the expected value
         * @param update - the new value
         *
         * @return true if successful. False return indicates that the actual value was
         *         not equal to the expected value.
         */
        bool compareAndSet( T* expect, T* update ) {
            return internal::util::concurrent::Atomics::compareAndSet(&this->value, (void*)expect, (void*)update);
        }

        /**
         * Atomically sets to the given value and returns the old value.
         *
         * @param newValue- the new value
         *
         * @return the previous value.
         */
        T* getAndSet( T* newValue ) {
            return (T*)internal::util::concurrent::Atomics::getAndSet(&this->value, (void*)newValue);
        }

        /**
         * Returns the String representation of the current value.
         * @returns string representation of the current value.
         */
        std::string toString() const {
            return decaf::lang::Long::toString( (long long)this->value );
        }

    };

}}}}

#endif /*_DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICREFERENCE_H_*/
