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

#ifndef _DECAF_INTERNAL_LANG_THREADIMPL_H_
#define _DECAF_INTERNAL_LANG_THREADIMPL_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace lang{
    class Thread;
    class ThreadHandle;
}
namespace internal {
namespace lang {

    class DECAF_API ThreadImpl {
    private:

        ThreadImpl() {}

    public:

        /**
         * Create a new OS Thread.  When the new thread is created the given start routine is
         * called and the supplied argument is passed along with ThreadHandle associated with
         * this thread.  If successful this method returns the active ThreadHandle to the caller,
         * however if the Thread cannot be created then a RuntimeException is thrown.
         *
         * The method must be passed a pointer to the parent Thread instance that is creating it
         * so that the underlying OS thread functions can be used to associate the running Thread.
         *
         * @param parent pinter to the Thread object instance that creates this thread.
         * @param threadEntry the method to call when the thread starts.
         * @param userArg the value to pass into the threadEntry method when the thread starts.
         *
         * @return a pointer to the newly created ThreadHandle for the caller to store and use as
         *         a key to pass to the other thread utility methods.
         *
         * @throw NullPointerException if the parent or threadEntry pointers are null.
         * @throw RuntimeException if the thread cannot be created for some reason.
         */
        static decaf::lang::ThreadHandle* create(
           decaf::lang::Thread* parent,
           void (*threadEntry)( decaf::lang::ThreadHandle* self, void* data ),
           void* userArg );

        /**
         * Causes the current thread to sleep for the given time interval.
         *
         * @param mills time in milliseconds to sleep
         * @param nanos additional time in nanoseconds [0-999999] to sleep
         */
        static void sleep( long long mills, long long nanos );

        /**
         * Causes the given thread to block until the specified thread terminates.
         *
         * @param handle the handle to the Thread that should be joined.
         */
        static void join( decaf::lang::ThreadHandle* handle );

        /**
         * Causes the given thread to wait for the given time interval for the specified thread
         * to exit.  If the thread does not terminate in time this method returns normally.
         *
         * @param handle the handle to the Thread that should be joined.
         * @param mills time in milliseconds to sleep
         * @param nanos additional time in nanoseconds [0-999999] to sleep
         */
        static void join( decaf::lang::ThreadHandle* handle, long long mills, long long nanos );

        /**
         * Sets the Scheduling priority of the specified thread to the new value.
         *
         * @param handle the thread handle whose priority is to be altered
         * @param priority the new priority value for the thread [0-20]
         */
        static void setPriority( decaf::lang::ThreadHandle* handle, unsigned int priority );

        /**
         * Causes the current thread to give up its slot in the scheduler to allow another
         * thread time to run.
         */
        static void yeild();

        /**
         * Gets and returns the Current Thread Id as an 64bit long, the actual type is OS
         * dependent.
         *
         * @returns a 64bit long value that represents the OS specific Thread Id.
         */
        static long long getThreadId();

    };

}}}

#endif /* _DECAF_INTERNAL_LANG_THREADIMPL_H_ */
