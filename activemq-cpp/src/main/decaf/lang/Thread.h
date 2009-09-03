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

#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/InterruptedException.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/Config.h>
#include <memory>

namespace decaf{
namespace lang{

    class ThreadGroup;
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

        /** The minimum priority that a thread can have. */
        static const int MIN_PRIORITY = 1;

        /** The default priority that a thread is given at create time. */
        static const int NORM_PRIORITY = 5;

        /** The maximum priority that a thread can have. */
        static const int MAX_PRIORITY = 10;

    public:

        /**
         * Interface for handlers invoked when a Thread abruptly terminates due to an
         * uncaught exception.
         */
        class UncaughtExceptionHandler {
        public:

            /**
             * Method invoked when the given thread terminates due to the given uncaught exception.
             *
             * This method is defined to indicate that it will not throw an exception, throwing
             * and exception from this method will on most systems result in a segmentation fault.
             */
            virtual void uncaughtException( const Thread* thread, const Throwable& error ) throw() = 0;

        };

    public:

        /**
         * Constructs a new Thread. This constructor has the same effect as
         * Thread( NULL, NULL, GIVEN_NAME ), where GIVEN_NAME is a newly generated name.
         * When no name is given the name is automatically generated and are of the form
         * "Thread-"+n, where n is an integer.
         */
        Thread();

        /**
         * Constructs a new Thread with the given target Runnable task. This constructor
         * has the same effect as Thread( NULL, task, GIVEN_NAME ), where GIVEN_NAME is a
         * newly generated name.  When no name is given the name is automatically generated
         * and are of the form  "Thread-"+n, where n is an integer.
         *
         * @param task the Runnable that this thread manages, if the task is NULL the Thread's
         *        run method is used instead.
         */
        Thread( Runnable* task );

        /**
         * Constructs a new Thread with the given name. This constructor has the same effect
         * as Thread( NULL, NULL, GIVEN_NAME ), where GIVEN_NAME is a newly generated name.
         * When no name is given the name is automatically generated and are of the form
         * "Thread-"+n, where n is an integer.
         *
         * @param name the name to assign to this Thread.
         */
        Thread( const std::string& name );

        /**
         * Constructs a new Thread with the given target Runnable task and name. This constructor
         * has the same effect as Thread( NULL, task, GIVEN_NAME ), where GIVEN_NAME is a
         * newly generated name.  When no name is given the name is automatically generated
         * and are of the form  "Thread-"+n, where n is an integer.
         *
         * @param task the Runnable that this thread manages, if the task is NULL the Thread's
         *        run method is used instead.
         * @param name the name to assign to this Thread.
         */
        Thread( Runnable* task, const std::string& name );

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

        /**
         * Returns the Thread's assigned name.
         * @returns the Name of the Thread.
         */
        std::string getName() const;

        /**
         * Sets the name of the Thread to the new Name given by the argument <em>name</em>
         *
         * @paran name the new name of the Thread.
         */
        void setName( const std::string& name );

        /**
         * Gets the currently set priority for this Thread.
         *
         * @return an int value representing the Thread's current priority.
         */
        int getPriority() const;

        /**
         * Sets the current Thread's priority to the newly specified value.  The given value
         * must be within the rane Thread::MIN_PRIORITY and Thread::MAX_PRIORITY.
         *
         * @param value the new priority value to assign to this Thread.
         *
         * @throws IllegalArgumentException if the value is out of range.
         */
        void setPriority( int value ) throw( decaf::lang::exceptions::IllegalArgumentException );

        /**
         * Set the handler invoked when this thread abruptly terminates due to an uncaught exception.
         *
         * @returns a pointer to the set UncaughtExceptionHandler.
         */
        const UncaughtExceptionHandler* getUncaughtExceptionHandler() const;

        /**
         * Set the handler invoked when this thread abruptly terminates due to an uncaught exception.
         *
         * @param handler the UncaightExceptionHandler to invoke when the Thread terminates due
         *                to an uncaught exception.
         */
        void setUncaughtExceptionHandler( UncaughtExceptionHandler* handler );

        /**
         * Returns a string that describes the Thread.
         *
         * @return string describing the Thread.
         */
        std::string toString() const;

    public:

        /**
         * Causes the currently executing thread to halt execution for the specified number of
         * milliseconds, subject to the precision and accuracy of system timers and schedulers.
         *
         * Note that this method is a static method that applies to the
         * calling thread and not to the thread object.
         *
         * @param millisecs time in milliseconds to halt execution.
         *
         * @throws IllegalArgumentException if the milliseconds parameter is negative.
         * @throws InterruptedException if the Thread was interrupted while sleeping.
         */
        static void sleep( long long millisecs )
            throw( lang::exceptions::InterruptedException,
                   lang::exceptions::IllegalArgumentException );

        /**
         * Causes the currently executing thread to halt execution for the specified number of
         * milliseconds plus any additionally specified nanoseconds given, subject to the precision
         * and accuracy of system timers and schedulers.
         *
         * Note that this method is a static method that applies to the
         * calling thread and not to the thread object.
         *
         * @param millisecs time in milliseconds to halt execution.
         * @param nanos 0-999999 extra nanoseconds to sleep.
         *
         * @throws IllegalArgumentException if the nanoseconds parameter is out of range
         *         or the milliseconds paramter is negative.
         * @throws InterruptedException if the Thread was interrupted while sleeping.
         */
        static void sleep( long long millisecs, unsigned int nanos )
            throw( lang::exceptions::InterruptedException,
                   lang::exceptions::IllegalArgumentException );

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
        void initialize( Runnable* task, const std::string& name );

    };

}}

#endif /*_DECAF_LANG_THREAD_H_*/
