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

#include "Timer.h"

#include <decaf/lang/Thread.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/System.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/internal/util/TimerTaskHeap.h>
#include <decaf/internal/util/concurrent/SynchronizableImpl.h>
#include <decaf/lang/exceptions/InterruptedException.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {

    class TimerImpl: public decaf::lang::Thread, public SynchronizableImpl {
    public:

        TimerTaskHeap heap;
        bool cancelled;

    public:

        TimerImpl() : Thread(), heap(), cancelled(false) {}

        TimerImpl(const std::string& name) : Thread(name), heap(), cancelled(false) {}

        virtual ~TimerImpl() {
            try {
                this->cancel();
                this->join();
            }
            DECAF_CATCHALL_NOTHROW()
        }

        /**
         * This method will be launched on separate thread for each Timer
         * object.
         */
        virtual void run() {

            while (true) {

                Pointer<TimerTask> task;
                synchronized(this) {

                    if (cancelled) {
                        return;
                    }

                    if (heap.isEmpty()) {
                        // no tasks scheduled -- sleep until any task appear
                        try {
                            this->wait();
                        } catch (InterruptedException& e) {}
                        continue;
                    }

                    long long currentTime = System::currentTimeMillis();

                    task = heap.peek();
                    long long timeToSleep = 0LL;

                    synchronized(&(task->lock)) {
                        if (task->cancelled) {
                            heap.remove(0);
                            continue;
                        }

                        // check the time to sleep for the first task scheduled
                        timeToSleep = task->when - currentTime;
                    }

                    if (timeToSleep > 0) {
                        task.reset(NULL);
                        try {
                            this->wait(timeToSleep);
                        } catch (InterruptedException& e) {
                        }
                        continue;
                    }

                    // Time to run the task, but first we need to check to see if any other
                    // tasks where scheduled and caused this one to be moved someplace new on
                    // the heap and find it new location.. We also need to check that the task
                    // wasn't canceled while we were sleeping.
                    synchronized(&(task->lock)) {
                        std::size_t pos = 0;

                        if (heap.peek()->when != task->when) {
                            pos = heap.find(task);
                        }

                        if (task->cancelled) {
                            heap.remove(heap.find(task));
                            continue;
                        }

                        // set time to schedule
                        task->setScheduledTime(task->when);

                        // remove task from queue
                        heap.remove(pos);

                        // set when the next task should be launched
                        if (task->period >= 0) {

                            // this is a repeating task,
                            if (task->fixedRate) {
                                // task is scheduled at fixed rate
                                task->when = task->when + task->period;
                            } else {
                                // task is scheduled at fixed delay
                                task->when = System::currentTimeMillis() + task->period;
                            }

                            // insert this task into queue, it will be ordered by the heap for
                            // its next run time.
                            insertTask(task);
                        } else {
                            // Task was a one-shot, setting when to zero indicates it
                            // won't run anymore.
                            task->when = 0;
                        }
                    }
                }

                // run the task, suppress all exceptions, we can't deal with them.
                if (task != NULL && !task->cancelled) {
                    try {
                        task->run();
                    } catch(...) {
                    }
                }
            }
        }

        void insertTask(const Pointer<TimerTask>& task) {
            // callers are synchronized
            heap.insert(task);
            this->notify();
        }

        void cancel() {
            synchronized(this) {
                cancelled = true;
                heap.reset();
                this->notify();
            }
        }

        int purge() {
            std::size_t result = 0;
            synchronized(this) {
                if (heap.isEmpty()) {
                    return 0;
                }

                result = heap.deleteIfCancelled();
            }

            return (int)result;
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
Timer::Timer() : internal(new TimerImpl()) {
    try {
        this->internal->start();
    } catch(...) {
        delete this->internal;
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
Timer::Timer(const std::string& name) : internal(new TimerImpl(name)) {
    try {
        this->internal->start();
    } catch(...) {
        delete this->internal;
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
Timer::~Timer() {
    try {
        delete this->internal;
    }
    DECAF_CATCH_NOTHROW(Exception)
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void Timer::cancel() {
    this->internal->cancel();
}

////////////////////////////////////////////////////////////////////////////////
bool Timer::awaitTermination(long long timeout, const TimeUnit& unit) {

    if (!this->internal->isAlive()) {
        return true;
    }

    this->internal->join(unit.toMillis(timeout));

    return this->internal->isAlive();
}

////////////////////////////////////////////////////////////////////////////////
int Timer::purge() {
    return this->internal->purge();
}

////////////////////////////////////////////////////////////////////////////////
void Timer::schedule(TimerTask* task, long long delay) {

    if (delay < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled to start in the Future but delay was Negative");
    }

    Pointer<TimerTask> wrapper(task);

    try {
        scheduleTask(wrapper, delay, -1, false);
    } catch (Exception& ex) {
        wrapper.release();
        ex.setMark(__FILE__, __LINE__);
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Timer::schedule(const Pointer<TimerTask>& task, long long delay) {

    if (delay < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled to start in the Future but delay was Negative");
    }

    scheduleTask(task, delay, -1, false);
}

////////////////////////////////////////////////////////////////////////////////
void Timer::schedule(TimerTask* task, const Date& when) {

    if (when.getTime() < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Task must be scheduled to start in the Future.");
    }

    Pointer<TimerTask> wrapper(task);
    long long delay = when.getTime() - System::currentTimeMillis();

    try {
        scheduleTask(wrapper, delay < 0 ? 0 : delay, -1, false);
    } catch (Exception& ex) {
        wrapper.release();
        ex.setMark(__FILE__, __LINE__);
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Timer::schedule(const Pointer<TimerTask>& task, const Date& when) {

    if (when.getTime() < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Task must be scheduled to start in the Future.");
    }

    long long delay = when.getTime() - System::currentTimeMillis();
    scheduleTask(task, delay < 0 ? 0 : delay, -1, false);
}

////////////////////////////////////////////////////////////////////////////////
void Timer::schedule(TimerTask* task, long long delay, long long period) {

    if (delay < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled to start in the Future but delay was Negative");
    }

    if (period <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled non-negative or non-zero period.");
    }

    Pointer<TimerTask> wrapper(task);

    try {
        scheduleTask(wrapper, delay, period, false);
    } catch (Exception& ex) {
        wrapper.release();
        ex.setMark(__FILE__, __LINE__);
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Timer::schedule(const Pointer<TimerTask>& task, long long delay, long long period) {

    if (delay < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled to start in the Future but delay was Negative");
    }

    if (period <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled non-negative or non-zero period.");
    }

    scheduleTask(task, delay, period, false);
}

////////////////////////////////////////////////////////////////////////////////
void Timer::schedule(TimerTask* task, const Date& when, long long period) {

    if (when.getTime() < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled to start in the Future.");
    }

    if (period <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled non-negative or non-zero period.");
    }

    Pointer<TimerTask> wrapper(task);
    long long delay = when.getTime() - System::currentTimeMillis();

    try {
        scheduleTask(wrapper, delay < 0 ? 0 : delay, period, false);
    } catch (Exception& ex) {
        wrapper.release();
        ex.setMark(__FILE__, __LINE__);
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Timer::schedule(const Pointer<TimerTask>& task, const Date& when, long long period) {

    if (when.getTime() < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled to start in the Future.");
    }

    if (period <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled non-negative or non-zero period.");
    }

    long long delay = when.getTime() - System::currentTimeMillis();
    scheduleTask(task, delay < 0 ? 0 : delay, period, false);
}

////////////////////////////////////////////////////////////////////////////////
void Timer::scheduleAtFixedRate(TimerTask* task, long long delay, long long period) {

    if (delay < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled to start in the Future but delay was Negative");
    }

    if (period <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled non-negative or non-zero period.");
    }

    Pointer<TimerTask> wrapper(task);

    try {
        scheduleTask(wrapper, delay, period, true);
    } catch (Exception& ex) {
        wrapper.release();
        ex.setMark(__FILE__, __LINE__);
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Timer::scheduleAtFixedRate(const Pointer<TimerTask>& task, long long delay, long long period) {

    if (delay < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled to start in the Future but delay was Negative");
    }

    if (period <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled non-negative or non-zero period.");
    }

    scheduleTask(task, delay, period, true);
}

////////////////////////////////////////////////////////////////////////////////
void Timer::scheduleAtFixedRate(TimerTask* task, const Date& when, long long period) {

    if (when.getTime() < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled to start in the Future.");
    }

    if (period <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled non-negative or non-zero period.");
    }

    Pointer<TimerTask> wrapper(task);
    long long delay = when.getTime() - System::currentTimeMillis();

    try {
        scheduleTask(wrapper, delay < 0 ? 0 : delay, period, true);
    } catch (Exception& ex) {
        wrapper.release();
        ex.setMark(__FILE__, __LINE__);
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Timer::scheduleAtFixedRate(const Pointer<TimerTask>& task, const Date& when, long long period) {

    if (when.getTime() < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled to start in the Future.");
    }

    if (period <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Task must be scheduled non-negative or non-zero period.");
    }

    long long delay = when.getTime() - System::currentTimeMillis();
    scheduleTask(task, delay < 0 ? 0 : delay, period, true);
}

////////////////////////////////////////////////////////////////////////////////
void Timer::scheduleTask(const Pointer<TimerTask>& task, long long delay, long long period, bool fixed) {

    if (task == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Task pointer passed in was Null");
    }

    synchronized(this->internal) {

        if (this->internal->cancelled) {
            throw IllegalStateException(__FILE__, __LINE__, "Timer was cancelled.");
        }

        long long when = delay + System::currentTimeMillis();

        if (when < 0) {
            throw IllegalArgumentException(__FILE__, __LINE__,
                "Task must be scheduled to start in the Future but delay was Negative");
        }

        synchronized(&(task->lock)) {

            if (task->isScheduled()) {
                throw IllegalStateException(__FILE__, __LINE__,
                    "Task is already scheduled in a Timer, cannot add again.");
            }

            if (task->cancelled) {
                throw IllegalStateException(__FILE__, __LINE__,
                    "Task is already has been cancelled cannot be restarted.");
            }

            task->when = when;
            task->period = period;
            task->fixedRate = fixed;
        }

        // insert the new Task into priority queue
        this->internal->insertTask(task);
    }
}
