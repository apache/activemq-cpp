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

#include <decaf/lang/exceptions/IllegalThreadStateException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/InterruptedException.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace util{
namespace concurrent{
namespace locks{
    class LockSupport;
}}}
namespace lang{

    class Runtime;
    class ThreadGroup;
    class ThreadProperties;

    /**
     * A Thread is a concurrent unit of execution. It has its own call stack for
     * methods being invoked, their arguments and local variables. Each process has at
     * least one main Thread running when it is started; typically, there are
     * several others for housekeeping. The application might decide to launch additional
     * Threads for specific purposes.
     *
     * Threads in the same process interact and synchronize by the use of shared
     * objects and monitors associated with these objects.
     *
     * There are basically two main ways of having a Thread execute application
     * code. One is providing a new class that extends Thread and overriding
     * its {@link #run()} method. The other is providing a new Thread instance
     * with a {@link Runnable} object during its creation. In both cases, the
     * {@link #start()} method must be called to actually execute the new Thread.
     *
     * Each Thread has an integer priority that basically determines the amount
     * of CPU time the Thread gets. It can be set using the {@link #setPriority(int)}
     * method. A Thread can also be made a daemon, which makes it run in the
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
        ThreadProperties* properties;

    public:

        /** The minimum priority that a thread can have. */
        static const int MIN_PRIORITY = 1;

        /** The default priority that a thread is given at create time. */
        static const int NORM_PRIORITY = 5;

        /** The maximum priority that a thread can have. */
        static const int MAX_PRIORITY = 10;

        /** Represents the various states that the Thread can be in during its lifetime. */
        enum State{

            /** Before a Thread is started it exists in this State. */
            NEW = 0,

            /** While a Thread is running and is not blocked it is in this State. */
            RUNNABLE = 1,

            /** A Thread that is waiting to acquire a lock is in this state. */
            BLOCKED = 2,

            /** A Thread that is waiting for another Thread to perform an action is in this state */
            WAITING = 3,

            /**
             * A Thread that is waiting for another Thread to perform an action up to a specified
             * time interval is in this state.
             */
            TIMED_WAITING = 4,

            /** A Thread that is blocked in a Sleep call is in this state. */
            SLEEPING = 5,

            /** A Thread whose run method has exited is in this state. */
            TERMINATED = 6

        };

    public:

        /**
         * Interface for handlers invoked when a Thread abruptly terminates due to an
         * uncaught exception.
         */
        class UncaughtExceptionHandler {
        public:

            virtual ~UncaughtExceptionHandler() {}

            /**
             * Method invoked when the given thread terminates due to the given uncaught exception.
             *
             * This method is defined to indicate that it will not throw an exception, throwing
             * and exception from this method will on most systems result in a segmentation fault.
             */
            virtual void uncaughtException( const Thread* thread, const Throwable& error ) throw() = 0;

        };

    private:

        Thread( const Thread& );
        Thread& operator= ( const Thread& );

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
         * Creates a system thread and starts it in a joinable mode.  Upon creation, the
         * run() method of either this object or the provided Runnable object will be
         * invoked in the context of this thread.
         *
         * @throws IllegalThreadStateException if the thread has already been started.
         * @throws RuntimeException if the Thread cannot be created for some reason.
         */
        virtual void start();

        /**
         * Forces the Current Thread to wait until the thread exits.
         *
         * @throws InterruptedException if any thread has interrupted the current thread.
         *         The interrupted status of the current thread is cleared when this
         *         exception is thrown.
         */
        virtual void join();

        /**
         * Forces the Current Thread to wait until the thread exits.
         *
         * @param millisecs the time in Milliseconds before the thread resumes
         *
         * @throws IllegalArgumentException if the milliseconds parameter is negative.
         * @throws InterruptedException if any thread has interrupted the current thread.
         *         The interrupted status of the current thread is cleared when this
         *         exception is thrown.
         */
        virtual void join( long long millisecs );

        /**
         * Forces the Current Thread to wait until the thread exits.
         *
         * @param millisecs the time in Milliseconds before the thread resumes
         * @param nanos 0-999999 extra nanoseconds to sleep.
         *
         * @throws IllegalArgumentException if the nanoseconds parameter is out of range
         *         or the milliseconds paramter is negative.
         * @throws InterruptedException if any thread has interrupted the current thread.
         *         The interrupted status of the current thread is cleared when this
         *         exception is thrown.
         */
        virtual void join( long long millisecs, unsigned int nanos );

        /**
         * Default implementation of the run method - does nothing.
         */
        virtual void run();

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
        void setPriority( int value );

        /**
         * Sets if the given Thread is a Daemon Thread or not.  Daemon threads cannot be
         * joined and its resource are automatically reclaimed when it terminates.
         *
         * @param value
         *      Boolean indicating if this thread should be a daemon thread or not.
         *
         * @throws IllegalThreadStateException if the thread is already active.
         */
        void setDaemon(bool value);

        /**
         * Returns whether this thread is a daemon thread or not, if true this thread cannot
         * be joined.
         *
         * @return true if the thread is a daemon thread.
         */
        bool isDaemon() const;

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

        /**
         * Returns true if the Thread is alive, meaning it has been started and has not yet
         * died.
         *
         * @return true if the thread is alive.
         */
        bool isAlive() const;

        /**
         * Returns the currently set State of this Thread.
         *
         * @return the Thread's current state.
         */
        Thread::State getState() const;

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
        static void sleep( long long millisecs );

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
        static void sleep( long long millisecs, unsigned int nanos );

        /**
         * Causes the currently executing thread object to temporarily pause
         * and allow other threads to execute.
         */
        static void yield();

        /**
         * Obtains the Thread Id of the current thread
         * @return Thread Id
         */
        static long long getId();

        /**
         * Returns a pointer to the currently executing thread object.
         *
         * @return Pointer to the Thread object representing the currently running Thread.
         */
        static Thread* currentThread();

    private:

        // Initialize the Threads internal state
        void initialize( Runnable* task, const std::string& name );

        // Create a Thread Facade for threads not created by the Decaf Library.
        static Thread* createForeignThreadInstance( const std::string& name );

        // Called by the Decaf Runtime at startup to allow the Platform Threading
        // code to initialize any necessary Threading constructs needed to support
        // the features of this class.
        static void initThreading();

        // Called by the Decf Runtime at Shutdown to allow the Platform Threading
        // code to return any resources that were allocated at startup for the
        // Threading library.
        static void shutdownThreading();

        // Called from LockSupport to Park a Thread (suspend it from execution)
        static void park( Thread* thread );

        // Called from LockSupport to Park a Thread (suspend it from execution)
        static void park( Thread* thread, long long mills, long long nanos );

        // Called from LockSupport to UnPark a Thread (resume its execution status).
        static void unpark( Thread* thread );

        // Allow some Decaf Classes greater access to the Threading model.
        friend class decaf::util::concurrent::locks::LockSupport;
        friend class decaf::lang::Runtime;

    };

}}

#endif /*_DECAF_LANG_THREAD_H_*/
