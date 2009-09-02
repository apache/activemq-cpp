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
#include <memory>

namespace decaf{
namespace lang{

    class ThreadProperties;

    /**
     * A {@code Thread} is a concurrent unit of execution. It has its own call stack for
     * methods being invoked, their arguments and local variables. Each process has at
     * least one main {@code Thread} running when it is started; typically, there are
     * several others for housekeeping. The application might decide to launch additional
     * {@code Thread}s for specific purposes.
     *
     * {@code Thread}s in the same process interact and synchronize by the use of shared
     * objects and monitors associated with these objects.
     *
     * There are basically two main ways of having a {@code Thread} execute application
     * code. One is providing a new class that extends {@code Thread} and overriding
     * its {@link #run()} method. The other is providing a new {@code Thread} instance
     * with a {@link Runnable} object during its creation. In both cases, the
     * {@link #start()} method must be called to actually execute the new {@code Thread}.
     *
     * Each {@code Thread} has an integer priority that basically determines the amount
     * of CPU time the {@code Thread} gets. It can be set using the {@link #setPriority(int)}
     * method. A {@code Thread} can also be made a daemon, which makes it run in the
     * background. The latter also affects VM termination behavior: the VM does not
     * terminate automatically as long as there are non-daemon threads running.
     *
     * @see decaf.lang.ThreadGroup
     *
     * @since 1.0
     */
    class DECAF_API Thread : public Runnable {
    private:

        /**
         * The internal data necessary to manage a Thread instance.
         */
        std::auto_ptr<ThreadProperties> properties;

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

        // Initialize the Threads internal state
        void initialize( Runnable* task );

    };

}}

#endif /*_DECAF_LANG_THREAD_H_*/
