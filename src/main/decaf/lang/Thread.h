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
#ifndef _DECAF_LANG_THREAD_H_
#define _DECAF_LANG_THREAD_H_

#include <decaf/lang/Exception.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/Config.h>
#include <stdexcept>
#include <assert.h>

#include <apr_thread_proc.h>

namespace decaf{
namespace lang{

    /**
     * Basic thread class - mimics the Java Thread.  Derived classes may
     * implement the run method, or this class can be used as is with
     * a provided Runnable delegate.
     */
    class DECAF_API Thread : public Runnable
    {
    private:

        /**
         * The task to be run by this thread, defaults to
         * this thread object.
         */
        Runnable* task;

        /**
         * APR Thread Handle
         */
        apr_thread_t* threadHandle;

        /**
         * Started state of this thread.
         */
        bool started;

        /**
         * Indicates whether the thread has already been
         * joined.
         */
        bool joined;

    public:

        /**
         * default Constructor
         */
        Thread();

        /**
         * Constructor
         * @param task the Runnable that this thread manages
         */
        Thread( Runnable* task );

        virtual ~Thread();

        /**
         * Creates a system thread and starts it in a joinable mode.
         * Upon creation, the
         * run() method of either this object or the provided Runnable
         * object will be invoked in the context of this thread.
         * @exception runtime_error is thrown if the system could
         * not start the thread.
         */
        virtual void start() throw ( Exception );

        /**
         * Wait til the thread exits. This is when the run()
         * method has returned or has thrown an exception.
         */
        virtual void join() throw ( Exception );

        /**
         * Default implementation of the run method - does nothing.
         */
        virtual void run(){};

    public:

        /**
         * Halts execution of the calling thread for a specified no of millisec.
         *
         * Note that this method is a static method that applies to the
         * calling thread and not to the thread object.
         * @param millisecs time in milliseconds to sleep
         */
        static void sleep( int millisecs );

        /**
         * Causes the currently executing thread object to temporarily pause
         * and allow other threads to execute.
         */
        static void yield();

        /**
         * Obtains the Thread Id of the current thread
         * @return Thread Id
         */
        static unsigned long getId();

    private:

        // Internal thread handling
        static void* APR_THREAD_FUNC runCallback( apr_thread_t* self, void* param );

    };

}}

#endif /*_DECAF_LANG_THREAD_H_*/
