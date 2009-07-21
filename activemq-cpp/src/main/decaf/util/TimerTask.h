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

#ifndef _DECAF_UTIL_TIMERTASK_H_
#define _DECAF_UTIL_TIMERTASK_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/Mutex.h>

namespace decaf {
namespace internal{
    namespace util{
        class TimerTaskHeap;
    }
}
namespace util {

    class Timer;
    class TimerImpl;

    /**
     * A Base class for a task object that can be scheduled for one-time or
     * repeated execution by a Timer.
     *
     * @since 1.0
     */
    class DECAF_API TimerTask : public lang::Runnable {
    private:

        // Timer will use this lock to sync with mutating methods in this class.
        mutable decaf::util::concurrent::Mutex lock;

        bool fixedRate;
        bool cancelled;
        long long scheduledTime;
        long long when;
        long long period;

        friend class Timer;
        friend class TimerImpl;
        friend class decaf::internal::util::TimerTaskHeap;

    public:

        TimerTask();
        virtual ~TimerTask() {}

        /**
         * Cancels this timer task. If the task has been scheduled for one-time execution and has
         * not yet run, or has not yet been scheduled, it will never run. If the task has been
         * scheduled for repeated execution, it will never run again. (If the task is running when
         * this call occurs, the task will run to completion, but will never run again.)
         *
         * Note that calling this method from within the run method of a repeating timer task
         * absolutely guarantees that the timer task will not run again.
         *
         * This method may be called repeatedly; the second and subsequent calls have no effect.
         *
         * @returns true if this task is scheduled for one-time execution and has not yet run, or this
         * task is scheduled for repeated execution. Returns false if the task was scheduled for one-time
         * execution and has already run, or if the task was never scheduled, or if the task was already
         * canceled. (Loosely speaking, this method returns true if it prevents one or more scheduled
         * executions from taking place.)
         */
        bool cancel();

        /**
         * Returns the scheduled execution time of the most recent actual execution of this task. (If
         * this method is invoked while task execution is in progress, the return value is the scheduled
         * execution time of the ongoing task execution.)
         *
         * This method is typically invoked from within a task's run method, to determine whether the current
         * execution of the task is sufficiently timely to warrant performing the scheduled activity:
         *
         *      void run() {
         *          if( System::currentTimeMillis() - scheduledExecutionTime() >=
         *              MAX_TARDINESS)
         *                  return;  // Too late; skip this execution.
         *          // Perform the task
         *      }
         *
         * This method is typically not used in conjunction with fixed-delay execution repeating tasks, as
         * their scheduled execution times are allowed to drift over time, and so are not terribly significant.
         *
         * @returns the time at which the most recent execution of this task was scheduled to occur, in the
         * format returned by Date.getTime(). The return value is undefined if the task has yet to commence its
         * first execution.
         */
        long long scheduledExecutionTime() const;

    protected:

        bool isScheduled() const;
        void setScheduledTime( long long time );
        long long getWhen() const;

    };

}}

#endif /* _DECAF_UTIL_TIMERTASK_H_ */
