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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_THREADING_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_THREADING_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Thread.h>

namespace decaf {
namespace internal {
namespace util {
namespace concurrent {

    using decaf::lang::Thread;

    class ThreadLocalImpl;
    struct ThreadHandle;
    struct MonitorHandle;

    class DECAF_API Threading {
    private:

        Threading();
        Threading(const Threading&);
        Threading& operator= (const Threading&);

    public:

        /**
         * Called by the Decaf Runtime at startup to allow the Platform Threading
         * code to initialize any necessary Threading constructs needed to support
         * the features of this class.
         */
        static void initialize();

        /**
         * Called by the Decaf Runtime at Shutdown to allow the Platform Threading
         * code to return any resources that were allocated at startup for the
         * Threading library.
         */
        static void shutdown();

        /**
         * Locks the Threading library allowing an object to perform some operations safely in
         * a multi-threaded environment.
         */
        static void lockThreadsLib();

        /**
         * Unlocks the Threading library when locked.
         */
        static void unlockThreadsLib();

        /**
         * Diagnostic method dumps all threads info to console.
         */
        static void dumpRunningThreads();

    public:  // Monitors

        /**
         * Gets a monitor for use as a locking mechanism.  The monitor returned will be
         * initialized and ready for use.  Each monitor that is taken must be returned before
         * the Threading library is shutdown.
         *
         * @returns handle to a Monitor instance that has been initialized.
         */
        static MonitorHandle* takeMonitor(bool alreadyLocked = false);

        /**
         * Returns a given monitor to the Monitor pool after the Monitor is no longer needed.
         *
         * @param monitor
         *      The handle of the Monitor to return to the Monitor pool.
         *
         * @throws IllegalMonitorStateException if the monitor is in use when returned.
         */
        static void returnMonitor(MonitorHandle* monitor, bool alreadyLocked = false);

        /**
         * Monitor locking method.  The calling thread blocks until it acquires the
         * monitor.  A thread can enter the same monitor more than once, but must then
         * exit the monitor the same number of times.
         *
         * @param monitor
         *      The handle to the monitor that the current thread is attempting to lock.
         */
        static void enterMonitor(MonitorHandle* monitor);

        /**
         * Monitor locking method.  If the calling thread cannot immediately acquire the lock
         * on the monitor then this method returns false, otherwise the thread gains the lock
         * on the monitor and the method returns true.  A thread can enter a monitor multiple
         * times, but must ensure that it exits the monitor the same number of times that it
         * entered it.
         *
         * @param monitor
         *      The handle to the monitor that the current thread is attempting to lock.
         *
         * @returns true if the caller obtains the lock on the Monitor, false otherwise.
         */
        static bool tryEnterMonitor(MonitorHandle* monitor);

        /**
         * Exit the acquired monitor giving up the lock that is held and allowing other threads
         * to acquire the monitor.  If the calling thread has entered the monitor more than once
         * then it must exit that monitor the same number of times.
         *
         * @param monitor
         *      Handle to the monitor instance that is to be excited.
         *
         * @throws IllegalMonitorStateException if the caller is not the owner of the monitor.
         */
        static void exitMonitor(MonitorHandle* monitor);

        /**
         * Waits on a monitor to be signaled by another thread.  The caller can wait for a
         * given timeout or pass zero for both mills and nanos to indicate it wants to wait
         * forever.  If the caller specifies a timeout and that timeout expires before the
         * monitor is signaled this method returns true.  The calling thread must own the
         * monitor in order to call this method, otherwise an IllegalMonitorStateException
         * is thrown.
         *
         * @param monitor
         *      Handle to the monitor that the calling thread is to wait on for a signal.
         * @param mills
         *      The time in milliseconds to wait for the monitor to be signaled.
         * @param nanos
         *      The time in nanoseconds to wait for the monitor to be signaled.
         *
         * @returns true if the timeout given expires before the caller was signaled.
         *
         * @throws IllegalMonitorStateException if the caller does not own the monitor.
         */
        static bool waitOnMonitor(MonitorHandle* monitor, long long mills, int nanos);

        /**
         * Notify a single waiter on the given Monitor instance, if there is no thread currently
         * waiting on the specified monitor then no action is taken.  The calling thread must own
         * the given monitor otherwise an IllegalMonitorStateException is thrown.
         *
         * @param monitor
         *      The monitor handle that is to have a single waiting thread signaled.
         *
         * @throws IllegalMonitorStateException if the caller does not own the monitor.
         */
        static void notifyWaiter(MonitorHandle* monitor);

        /**
         * Notifies all waiting threads for the given Monitor.  If there are no threads currently
         * waiting on the given monitor instance then no action is taken.  The calling thread must
         * own the given monitor otherwise an IllegalMonitorStateException is thrown.
         *
         * @param monitor
         *      The monitor handle that is to have all of its waiting thread signaled.
         *
         * @throws IllegalMonitorStateException if the caller does not own the monitor.
         */
        static void notifyAllWaiters(MonitorHandle* monitor);

        /**
         * Query the monitor object to determine if it is currently locked.  This method is a mainly
         * a diagnostic tool and its return value is not guaranteed to reflect the locked state after
         * its been called as the state can change quickly.
         */
        static bool isMonitorLocked(MonitorHandle* monitor);

    public:  // Threads

        /**
         * Creates a new thread instance with the given Thread object as its
         * parent, assigning it the given name and stack size.  The Thread class
         * provides its own main Runnable for executing task.
         *
         * @param parent
         *      The parent Thread object that the new thread is owned by.
         * @param name
         *      Name given to the new Thread, used for debugging purposes.
         * @param stackSize
         *      The size to allocate for the new thread's stack.
         *
         * @returns a new ThreadHandle that identifies the thread and allows the parent
         *          to interact with it.
         */
        static ThreadHandle* createNewThread(Thread* parant, const char* name,
                                             long long stackSize);

        /**
         * Starts the given thread running, if the thread is already running then this
         * method has no effect.
         *
         * @param thread
         *      The thread instance to start.
         */
        static void start(ThreadHandle* thread);

        /**
         * Joins the given thread instance and waits for it to either terminate or for the
         * given timeout period to expire.  If the value of of the timeout is zero then this
         * method waits forever.
         *
         * @param thread
         *      The target thread to join.
         * @param mills
         *      The number of milliseconds to wait.
         * @param nanos
         *      The number of nanoseconds to wait [0-999999].
         *
         * @returns true if the timeout period expired, false otherwise.
         *
         * @throws InterruptedException if the Join was interrupted.
         * @throws IllegalArgumentException if the value of mills or nanos is invalid.
         */
        static bool join(ThreadHandle* thread, long long mills, int nanos);

        static void interrupt(ThreadHandle* thread);
        static bool interrupted();
        static bool isInterrupted(ThreadHandle* thread, bool reset);

        static void yeild();
        static bool sleep(long long mills, int nanos);

        static long long getThreadId(ThreadHandle* thread);

        static int getThreadPriority(ThreadHandle* thread);

        static void setThreadPriority(ThreadHandle* thread, int priority);

        static const char* getThreadName(ThreadHandle* thread);

        static void setThreadName(ThreadHandle* thread, const char* name);

        static Thread::State getThreadState(ThreadHandle* thread);

        static bool isThreadAlive(ThreadHandle* thread);

        static void destroyThread(ThreadHandle* thread);

        /**
         * Creates and returns a ThreadHandle that references the currently running thread.
         *
         * This method is called to obtain a ThreadHandle that references an thread that was
         * not created using the Decaf Thread class.  A parent Thread instance is passed to
         * associate with the target thread so that a call to getCurrentThread can return a
         * Decaf Thread as it would for any thread created using Thread.
         *
         * @param parent
         *      The Decaf thread instace to associate with this thread handle.
         * @param name
         *      The name to assign to the returned ThreadHandle.
         *
         * @returns a new ThreadHandle instance for the parent Decaf Thread.
         */
        static ThreadHandle* createThreadWrapper(decaf::lang::Thread* parent, const char* name);

        /**
         * @returns the Decaf Thread pointer instance for the currently running thread.
         */
        static Thread* getCurrentThread();

        /**
         * @returns the ThreadHandle instance for the currently running thread.
         */
        static ThreadHandle* getCurrentThreadHandle();

        /**
         * Removes the given thread from scheduling unless a call to unpark has already
         * reset the park token in which case this method simple consumes the unpark
         * token and returned.
         *
         * @param thread
         *      The target thread to park.
         */
        static void park(Thread* thread);

        /**
         * Removes the given thread from scheduling unless a call to unpark has already
         * reset the park token in which case this method simple consumes the unpark
         * token and returned.
         *
         * @param thread
         *      The target thread to park.
         * @param mills
         *      The time in milliseconds to park the target thread.
         * @param nanos
         *      The additional time in nanoseconds to park the target thread.
         */
        static bool park(Thread* thread, long long mills, int nanos);

        /**
         * If the target thread is not currently parked then this method sets the un-park
         * token for the thread and returns.  If the thread is parked than this method
         * places the thread back in a state where it can be scheduled once more.
         *
         * @param thread
         *      The thread to unpark.
         */
        static void unpark(Thread* thread);

    public:  // Thread local storage

        /**
         * Allocates a slot in the library for a new ThreadLocalImpl to store its values
         * for each thread.  The parent ThreadLocalImpl is stored so that the library can
         * call each ThreadLocalImpl to cleanup its resources if there are active objects
         * at the time the library is shutdown or the Thread terminates.
         *
         * @param threadLocal
         *      The ThreadLocalImpl to assign a storage slot.
         *
         * @returns a new storage slot Id for the given ThreadLocalImpl's value to be assigned.
         */
        static int createThreadLocalSlot(ThreadLocalImpl* threadLocal);

        static void* getThreadLocalValue(int slot);

        static void setThreadLocalValue(int slot, void* value);

        static void destoryThreadLocalSlot(int slot);

    private:

        static ThreadHandle* attachToCurrentThread();

        static void monitorEnterUsingThreadId(MonitorHandle* monitor, ThreadHandle* thread);
        static bool monitorTryEnterUsingThreadId(MonitorHandle* monitor, ThreadHandle* thread);
        static void monitorExitUsingThreadId(MonitorHandle* monitor, ThreadHandle* thread);
    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_THREADING_H_ */
