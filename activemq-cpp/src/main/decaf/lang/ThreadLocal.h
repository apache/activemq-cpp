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

#ifndef _DECAF_LANG_THREADLOCAL_H_
#define _DECAF_LANG_THREADLOCAL_H_

#include <decaf/util/Config.h>

#include <decaf/internal/util/concurrent/ThreadLocalImpl.h>

namespace decaf {
namespace lang {

    /**
     * This class provides thread-local variables. These variables differ from their normal
     * counterparts in that each thread that accesses one (via its get or set method) has its
     * own, independently initialized copy of the variable. ThreadLocal instances are typically
     * private static fields in classes that wish to associate state with a thread (e.g., a
     * user ID or Transaction ID).  This class imposes the restriction on the type that it will
     * contains that it must by both assignable and copyable.
     *
     * Each thread holds an implicit reference to its copy of a thread-local variable as long as
     * the thread is alive and the ThreadLocal instance is accessible; after a thread goes away,
     * all of its copies of thread-local instances are destroyed.
     *
     * @since 1.0
     */
    template <typename E>
    class ThreadLocal : protected decaf::internal::util::concurrent::ThreadLocalImpl {
    private:

        ThreadLocal(const ThreadLocal&);
        ThreadLocal& operator= (const ThreadLocal&);

    public:

        /**
         * Creates a new instance of a ThreadLocal
         */
        ThreadLocal() : ThreadLocalImpl() {}

        virtual ~ThreadLocal() {
            try {
                removeAll();
            } catch(...) {}
        }

        /**
         * Returns the value in the current thread's copy of this thread-local variable. If the
         * variable has no value for the current thread, it is first initialized to the value
         * returned by an invocation of the initialValue() method.
         *
         * @returns the current thread's value for this thread local.
         */
        E& get() {
            void* bytes = getRawValue();
            if (bytes == NULL) {
                E* value = new E();
                *value = initialValue();
                setRawValue((void*)value);
                bytes = value;
            }
            return *((E*)bytes);
        }

        /**
         * Sets the current thread's copy of this thread-local variable to the specified value.
         * Most subclasses will have no need to override this method, relying solely on the
         * initialValue() method to set the values of thread-locals.
         *
         * @param value
         *      The new value to assign to this Thread local.
         */
        void set(const E& value) {
            setRawValue((void*)(new E(value)));
        }

        /**
         * Removes the current thread's value for this thread-local variable. If this thread-local
         * variable is subsequently read by the current thread, its value will be reinitialized by
         * invoking its initialValue() method, unless its value is set by the current thread in the
         * interim. This may result in multiple invocations of the initialValue method in the
         * current thread.
         */
        void remove() {
            this->setRawValue(NULL);
        }

    protected:

        /**
         * Returns the current thread's "initial value" for this thread-local variable. This
         * method will be invoked the first time a thread accesses the variable with the get()
         * method, unless the thread previously invoked the set() method, in which case the
         * initialValue method will not be invoked for the thread. Normally, this method is
         * invoked at most once per thread, but it may be invoked again in case of subsequent
         * invocations of remove() followed by get().
         *
         * This implementation simply returns E(); if the programmer desires thread-local
         * variables to have an initial value other than E(), ThreadLocal must be subclassed,
         * and this method overridden. Typically, an inner class will be used.
         *
         * @param value
         *      Pointer to the thread local value created by this thread when get() is first called.
         */
        virtual E initialValue() const {
            return E();
        }

    protected:

        virtual void doDelete(void* value) {
            if (value != 0) {
                delete static_cast<E*>(value);
            }
        }

    };

}}

#endif /* _DECAF_LANG_THREADLOCAL_H_ */
