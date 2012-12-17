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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_THREADLOCALIMPL_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_THREADLOCALIMPL_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace internal {
namespace util {
namespace concurrent {

    class DECAF_API ThreadLocalImpl {
    private:

        int tlsKey;

    private:

        ThreadLocalImpl(const ThreadLocalImpl&);
        ThreadLocalImpl& operator= (const ThreadLocalImpl&);

    public:

        ThreadLocalImpl();

        virtual ~ThreadLocalImpl();

        /**
         * Returns the current threads assigned value, but retains ownership
         * to this value unless the remove method is subsequently called.
         *
         * @returns the currently held value for this thread.
         */
        void* getRawValue() const;

        /**
         * Sets the raw void* value for the current thread.  If the value
         * is NULL and the old value is non-NULL then the library will call
         * the doDelete method to destroy the previous value.
         *
         * @param value
         *      Pointer to the value to be stored for the current thread or NULL.
         */
        void setRawValue(void* value);

        /**
         * Removes from all threads any allocated data stored for this ThreadLocal
         * instance.  Subclasses should call this method in their destructor to ensure
         * that all the values that are stored in each thread can be deallocated using
         * their custom doDelete method.
         */
        void removeAll();

    public:

        /**
         * Called to destroy the value held by the current thread or by the
         * library on shutdown if there are still ThreadLocalImpl instances that
         * have assigned TLS slots.  Its up to the implementor if this interface
         * to ensure that the value held in the void* is cleaned up correctly.
         *
         * @param value
         *      The value to be destroyed for the current thread.
         */
        virtual void doDelete(void* value) = 0;

    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_THREADLOCALIMPL_H_ */
