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

#ifndef _DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICREFCOUNTER_H_
#define _DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICREFCOUNTER_H_

#include <decaf/util/concurrent/atomic/AtomicInteger.h>

namespace decaf{
namespace util{
namespace concurrent{
namespace atomic{

    class AtomicRefCounter {
    private:

        decaf::util::concurrent::atomic::AtomicInteger* counter;

    private:

        AtomicRefCounter& operator= ( const AtomicRefCounter& );

    public:

        AtomicRefCounter() :
            counter( new decaf::util::concurrent::atomic::AtomicInteger( 1 ) ) {}
        AtomicRefCounter( const AtomicRefCounter& other ) : counter( other.counter ) {
            this->counter->incrementAndGet();
        }

        virtual ~AtomicRefCounter() {}

    protected:

        /**
         * Swaps this instance's reference counter with the one given, this allows
         * for copy-and-swap semantics of this object.
         *
         * @param other
         *      The value to swap with this one's.
         */
        void swap( AtomicRefCounter& other ) {
            std::swap( this->counter, other.counter );
        }

        /**
         * Removes a reference to the counter Atomically and returns if the counter
         * has reached zero, once the counter hits zero, the internal counter is
         * destroyed and this instance is now considered to be unreferenced.
         *
         * @return true if the count is now zero.
         */
        bool release() {
            if( this->counter->decrementAndGet() == 0 ) {
                delete this->counter;
                return true;
            }
            return false;
        }
    };

}}}}

#endif /* _DECAF_UTIL_CONCURRENT_ATOMIC_ATOMICREFCOUNTER_H_ */
