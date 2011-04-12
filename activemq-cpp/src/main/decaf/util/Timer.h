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

#ifndef _DECAF_UTIL_TIMER_H_
#define _DECAF_UTIL_TIMER_H_

#include <memory>

#include <decaf/util/Config.h>
#include <decaf/util/Date.h>

#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace decaf {
namespace util {

    class TimerTask;
    class TimerImpl;

    /**
     * A facility for threads to schedule tasks for future execution in a background thread.
     * Tasks may be scheduled for one-time execution, or for repeated execution at regular
     * intervals.
     *
     * Corresponding to each Timer object is a single background thread that is used to execute
     * all of the timer's tasks, sequentially. Timer tasks should complete quickly. If a timer
     * task takes excessive time to complete, it "hogs" the timer's task execution thread. This
     * can, in turn, delay the execution of subsequent tasks, which may "bunch up" and execute
     * in rapid succession when (and if) the offending task finally completes.
     *
     * This class is thread-safe: multiple threads can share a single Timer object without the
     * need for external synchronization.
     *
     * This class does not offer real-time guarantees: it schedules tasks using the wait(long)
     * method.
     *
     * @since 1.0
     */
    class DECAF_API Timer {
    private:

        TimerImpl* internal;

    private:

        Timer( const Timer& );
        Timer operator= ( const Timer& );

    public:

        Timer();

        /**
         * Create a new Timer whose associated thread is assigned the name given.
         *
         * @param name
         *      The name to assign to this Timer's Thread.
         */
        Timer(const std::string& name);

        virtual ~Timer();

        /**
         * Terminates this timer, discarding any currently scheduled tasks. Does not interfere with a
         * currently executing task (if it exists). Once a timer has been terminated, its execution thread
         * terminates gracefully, and no more tasks may be scheduled on it.
         *
         * Note that calling this method from within the run method of a timer task that was invoked by this
         * timer absolutely guarantees that the ongoing task execution is the last task execution that will ever
         * be performed by this timer.
         *
         * This method may be called repeatedly; the second and subsequent calls have no effect.
         */
        void cancel();

        /**
         * Removes all canceled tasks from this timer's task queue. Calling this method has no effect on the
         * behavior of the timer, but eliminates the canceled tasks from the queue causing the Timer to destroy
         * the TimerTask pointer it was originally given, the caller should ensure that they no longer have
         * any references to TimerTasks that were previously scheduled.
         *
         * Most programs will have no need to call this method. It is designed for use by the rare application
         * that cancels a large number of tasks. Calling this method trades time for space: the runtime of the
         * method may be proportional to n + c log n, where n is the number of tasks in the queue and c is the
         * number of canceled tasks.
         *
         * This method can be called on a Timer object that has no scheduled tasks without error.
         *
         * @returns the number of tasks removed from the queue.
         */
        int purge();

        /**
         * Schedules the specified task for execution after the specified delay.
         *
         * The TimerTask pointer is considered to be owned by the Timer class once it has been scheduled, the
         * Timer will destroy its TimerTask's once they have been canceled or the Timer itself is canceled.
         * A TimerTask is considered scheduled only when this method return without throwing an exception, until
         * that time ownership is not considered to have been transferred to the Timer and the caller should
         * ensure that the TimerTask gets deleted if an exception is thrown and no further attempts to schedule
         * that TimerTask instance are planned.
         *
         * @param task - task to be scheduled.
         * @param delay - delay in milliseconds before task is to be executed.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if delay is negative, or delay + System.currentTimeMillis() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, or timer was canceled.
         */
        void schedule( TimerTask* task, long long delay );

        /**
         * Schedules the specified task for execution after the specified delay.
         *
         * @param task - task to be scheduled.
         * @param delay - delay in milliseconds before task is to be executed.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if delay is negative, or delay + System.currentTimeMillis() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, or timer was canceled.
         */
        void schedule( const decaf::lang::Pointer<TimerTask>& task, long long delay );

        /**
         * Schedules the specified task for execution at the specified time. If the time is in the past, the
         * task is scheduled for immediate execution.
         *
         * The TimerTask pointer is considered to be owned by the Timer class once it has been scheduled, the
         * Timer will destroy its TimerTask's once they have been canceled or the Timer itself is canceled.
         * A TimerTask is considered scheduled only when this method return without throwing an exception, until
         * that time ownership is not considered to have been transferred to the Timer and the caller should
         * ensure that the TimerTask gets deleted if an exception is thrown and no further attempts to schedule
         * that TimerTask instance are planned.
         *
         * @param task - task to be scheduled.
         * @param time - time at which task is to be executed.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if time.getTime() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, timer was canceled, or
         *                                timer thread terminated.
         */
        void schedule( TimerTask* task, const Date& time );

        /**
         * Schedules the specified task for execution at the specified time. If the time is in the past, the
         * task is scheduled for immediate execution.
         *
         * @param task - task to be scheduled.
         * @param time - time at which task is to be executed.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if time.getTime() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, timer was canceled, or
         *                                timer thread terminated.
         */
        void schedule( const decaf::lang::Pointer<TimerTask>& task, const Date& time );

        /**
         * Schedules the specified task for repeated fixed-delay execution, beginning after the specified delay.
         * Subsequent executions take place at approximately regular intervals separated by the specified period.
         *
         * The TimerTask pointer is considered to be owned by the Timer class once it has been scheduled, the
         * Timer will destroy its TimerTask's once they have been canceled or the Timer itself is canceled.
         * A TimerTask is considered scheduled only when this method return without throwing an exception, until
         * that time ownership is not considered to have been transferred to the Timer and the caller should
         * ensure that the TimerTask gets deleted if an exception is thrown and no further attempts to schedule
         * that TimerTask instance are planned.
         *
         * In fixed-delay execution, each execution is scheduled relative to the actual execution time of the
         * previous execution. If an execution is delayed for any reason (such as other background activity),
         * subsequent executions will be delayed as well. In the long run, the frequency of execution will
         * generally be slightly lower than the reciprocal of the specified period (assuming the system clock
         * underlying Object.wait(long long) is accurate).
         *
         * Fixed-delay execution is appropriate for recurring activities that require "smoothness." In other
         * words, it is appropriate for activities where it is more important to keep the frequency accurate
         * in the short run than in the long run. This includes most animation tasks, such as blinking a cursor
         * at regular intervals. It also includes tasks wherein regular activity is performed in response to
         * human input, such as automatically repeating a character as long as a key is held down.
         *
         * @param task - task to be scheduled.
         * @param delay - delay in milliseconds before task is to be executed.
         * @param period - time in milliseconds between successive task executions.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if delay is negative, or delay + System.currentTimeMillis() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, timer was canceled, or
         *                                timer thread terminated.
         */
        void schedule( TimerTask* task, long long delay, long long period );

        /**
         * Schedules the specified task for repeated fixed-delay execution, beginning after the specified delay.
         * Subsequent executions take place at approximately regular intervals separated by the specified period.
         *
         * In fixed-delay execution, each execution is scheduled relative to the actual execution time of the
         * previous execution. If an execution is delayed for any reason (such as other background activity),
         * subsequent executions will be delayed as well. In the long run, the frequency of execution will
         * generally be slightly lower than the reciprocal of the specified period (assuming the system clock
         * underlying Object.wait(long long) is accurate).
         *
         * Fixed-delay execution is appropriate for recurring activities that require "smoothness." In other
         * words, it is appropriate for activities where it is more important to keep the frequency accurate
         * in the short run than in the long run. This includes most animation tasks, such as blinking a cursor
         * at regular intervals. It also includes tasks wherein regular activity is performed in response to
         * human input, such as automatically repeating a character as long as a key is held down.
         *
         * @param task - task to be scheduled.
         * @param delay - delay in milliseconds before task is to be executed.
         * @param period - time in milliseconds between successive task executions.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if delay is negative, or delay + System.currentTimeMillis() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, timer was canceled, or
         *                                timer thread terminated.
         */
        void schedule( const decaf::lang::Pointer<TimerTask>& task, long long delay, long long period );

        /**
         * Schedules the specified task for repeated fixed-delay execution, beginning at the specified time.
         * Subsequent executions take place at approximately regular intervals separated by the specified period.
         *
         * The TimerTask pointer is considered to be owned by the Timer class once it has been scheduled, the
         * Timer will destroy its TimerTask's once they have been canceled or the Timer itself is canceled.
         * A TimerTask is considered scheduled only when this method return without throwing an exception, until
         * that time ownership is not considered to have been transferred to the Timer and the caller should
         * ensure that the TimerTask gets deleted if an exception is thrown and no further attempts to schedule
         * that TimerTask instance are planned.
         *
         * In fixed-delay execution, each execution is scheduled relative to the actual execution time of the
         * previous execution. If an execution is delayed for any reason (such as other background activity),
         * subsequent executions will be delayed as well. In the long run, the frequency of execution will
         * generally be slightly lower than the reciprocal of the specified period (assuming the system clock
         * underlying Object.wait(long long) is accurate).
         *
         * Fixed-delay execution is appropriate for recurring activities that require "smoothness." In other
         * words, it is appropriate for activities where it is more important to keep the frequency accurate
         * in the short run than in the long run. This includes most animation tasks, such as blinking a cursor
         * at regular intervals. It also includes tasks wherein regular activity is performed in response to
         * human input, such as automatically repeating a character as long as a key is held down.
         *
         * @param task - task to be scheduled.
         * @param firstTime - First time at which task is to be executed.
         * @param period - time in milliseconds between successive task executions.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if time.getTime() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, timer was canceled, or
         *                                timer thread terminated.
         */
        void schedule( TimerTask* task, const Date& firstTime, long long period );

        /**
         * Schedules the specified task for repeated fixed-delay execution, beginning at the specified time.
         * Subsequent executions take place at approximately regular intervals separated by the specified period.
         *
         * In fixed-delay execution, each execution is scheduled relative to the actual execution time of the
         * previous execution. If an execution is delayed for any reason (such as other background activity),
         * subsequent executions will be delayed as well. In the long run, the frequency of execution will
         * generally be slightly lower than the reciprocal of the specified period (assuming the system clock
         * underlying Object.wait(long long) is accurate).
         *
         * Fixed-delay execution is appropriate for recurring activities that require "smoothness." In other
         * words, it is appropriate for activities where it is more important to keep the frequency accurate
         * in the short run than in the long run. This includes most animation tasks, such as blinking a cursor
         * at regular intervals. It also includes tasks wherein regular activity is performed in response to
         * human input, such as automatically repeating a character as long as a key is held down.
         *
         * @param task - task to be scheduled.
         * @param firstTime - First time at which task is to be executed.
         * @param period - time in milliseconds between successive task executions.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if time.getTime() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, timer was canceled, or
         *                                timer thread terminated.
         */
        void schedule( const decaf::lang::Pointer<TimerTask>& task, const Date& firstTime, long long period );

        /**
         * Schedules the specified task for repeated fixed-rate execution, beginning after the specified delay.
         * Subsequent executions take place at approximately regular intervals, separated by the specified period.
         *
         * The TimerTask pointer is considered to be owned by the Timer class once it has been scheduled, the
         * Timer will destroy its TimerTask's once they have been canceled or the Timer itself is canceled.
         * A TimerTask is considered scheduled only when this method return without throwing an exception, until
         * that time ownership is not considered to have been transferred to the Timer and the caller should
         * ensure that the TimerTask gets deleted if an exception is thrown and no further attempts to schedule
         * that TimerTask instance are planned.
         *
         * In fixed-rate execution, each execution is scheduled relative to the scheduled execution time of the
         * initial execution. If an execution is delayed for any reason (such as garbage collection or other
         * background activity), two or more executions will occur in rapid succession to "catch up." In the
         * long run, the frequency of execution will be exactly the reciprocal of the specified period (assuming
         * the system clock underlying Object.wait(long) is accurate).
         *
         * Fixed-rate execution is appropriate for recurring activities that are sensitive to absolute time, such
         * as ringing a chime every hour on the hour, or running scheduled maintenance every day at a particular
         * time. It is also appropriate for recurring activities where the total time to perform a fixed number
         * of executions is important, such as a count down timer that ticks once every second for ten seconds.
         * Finally, fixed-rate execution is appropriate for scheduling multiple repeating timer tasks that must
         * remain synchronized with respect to one another.
         *
         * @param task - task to be scheduled.
         * @param delay - delay in milliseconds before task is to be executed.
         * @param period - time in milliseconds between successive task executions.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if delay is negative, or delay + System.currentTimeMillis() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, timer was canceled, or
         *                                timer thread terminated.
         */
        void scheduleAtFixedRate( TimerTask* task, long long delay, long long period );

        /**
         * Schedules the specified task for repeated fixed-rate execution, beginning after the specified delay.
         * Subsequent executions take place at approximately regular intervals, separated by the specified period.
         *
         * In fixed-rate execution, each execution is scheduled relative to the scheduled execution time of the
         * initial execution. If an execution is delayed for any reason (such as garbage collection or other
         * background activity), two or more executions will occur in rapid succession to "catch up." In the
         * long run, the frequency of execution will be exactly the reciprocal of the specified period (assuming
         * the system clock underlying Object.wait(long) is accurate).
         *
         * Fixed-rate execution is appropriate for recurring activities that are sensitive to absolute time, such
         * as ringing a chime every hour on the hour, or running scheduled maintenance every day at a particular
         * time. It is also appropriate for recurring activities where the total time to perform a fixed number
         * of executions is important, such as a countdown timer that ticks once every second for ten seconds.
         * Finally, fixed-rate execution is appropriate for scheduling multiple repeating timer tasks that must
         * remain synchronized with respect to one another.
         *
         * @param task - task to be scheduled.
         * @param delay - delay in milliseconds before task is to be executed.
         * @param period - time in milliseconds between successive task executions.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if delay is negative, or delay + System.currentTimeMillis() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, timer was canceled, or
         *                                timer thread terminated.
         */
        void scheduleAtFixedRate( const decaf::lang::Pointer<TimerTask>& task, long long delay, long long period );

        /**
         * Schedules the specified task for repeated fixed-rate execution, beginning at the specified time.
         * Subsequent executions take place at approximately regular intervals, separated by the specified period.
         *
         * The TimerTask pointer is considered to be owned by the Timer class once it has been scheduled, the
         * Timer will destroy its TimerTask's once they have been canceled or the Timer itself is canceled.
         * A TimerTask is considered scheduled only when this method return without throwing an exception, until
         * that time ownership is not considered to have been transferred to the Timer and the caller should
         * ensure that the TimerTask gets deleted if an exception is thrown and no further attempts to schedule
         * that TimerTask instance are planned.
         *
         * In fixed-rate execution, each execution is scheduled relative to the scheduled execution time of the
         * initial execution. If an execution is delayed for any reason (such as garbage collection or other
         * background activity), two or more executions will occur in rapid succession to "catch up." In the
         * long run, the frequency of execution will be exactly the reciprocal of the specified period (assuming
         * the system clock underlying Object.wait(long) is accurate).
         *
         * Fixed-rate execution is appropriate for recurring activities that are sensitive to absolute time, such
         * as ringing a chime every hour on the hour, or running scheduled maintenance every day at a particular
         * time. It is also appropriate for recurring activities where the total time to perform a fixed number
         * of executions is important, such as a countdown timer that ticks once every second for ten seconds.
         * Finally, fixed-rate execution is appropriate for scheduling multiple repeating timer tasks that must
         * remain synchronized with respect to one another.
         *
         * @param task - task to be scheduled.
         * @param firstTime - First time at which task is to be executed.
         * @param period - time in milliseconds between successive task executions.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if time.getTime() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, timer was canceled, or
         *                                timer thread terminated.
         */
        void scheduleAtFixedRate( TimerTask* task, const Date& firstTime, long long period );

        /**
         * Schedules the specified task for repeated fixed-rate execution, beginning at the specified time.
         * Subsequent executions take place at approximately regular intervals, separated by the specified period.
         *
         * In fixed-rate execution, each execution is scheduled relative to the scheduled execution time of the
         * initial execution. If an execution is delayed for any reason (such as garbage collection or other
         * background activity), two or more executions will occur in rapid succession to "catch up." In the
         * long run, the frequency of execution will be exactly the reciprocal of the specified period (assuming
         * the system clock underlying Object.wait(long) is accurate).
         *
         * Fixed-rate execution is appropriate for recurring activities that are sensitive to absolute time, such
         * as ringing a chime every hour on the hour, or running scheduled maintenance every day at a particular
         * time. It is also appropriate for recurring activities where the total time to perform a fixed number
         * of executions is important, such as a countdown timer that ticks once every second for ten seconds.
         * Finally, fixed-rate execution is appropriate for scheduling multiple repeating timer tasks that must
         * remain synchronized with respect to one another.
         *
         * @param task - task to be scheduled.
         * @param firstTime - First time at which task is to be executed.
         * @param period - time in milliseconds between successive task executions.
         *
         * @throw NullPointerException - if the TimerTask value is Null.
         * @throw IllegalArgumentException - if time.getTime() is negative.
         * @throw IllegalStateException - if task was already scheduled or canceled, timer was canceled, or
         *                                timer thread terminated.
         */
        void scheduleAtFixedRate( const decaf::lang::Pointer<TimerTask>& task, const Date& firstTime, long long period );

    private:

        void scheduleTask( const decaf::lang::Pointer<TimerTask>& task, long long delay, long long period, bool fixed );

    };

}}

#endif /* _DECAF_UTIL_TIMER_H_ */
