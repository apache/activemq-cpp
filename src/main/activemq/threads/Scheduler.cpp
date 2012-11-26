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

#include "Scheduler.h"

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/threads/SchedulerTimerTask.h>
#include <activemq/util/ServiceStopper.h>

#include <decaf/lang/Pointer.h>
#include <decaf/util/Timer.h>
#include <decaf/lang/Runnable.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

using namespace activemq;
using namespace activemq::threads;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
Scheduler::Scheduler(const std::string& name) : mutex(), name(name), timer(NULL), tasks() {

    if (name.empty()) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Scheduler name must not be empty.");
    }
}

////////////////////////////////////////////////////////////////////////////////
Scheduler::~Scheduler() {
    try {

        if (this->timer != NULL) {
            this->timer->cancel();
        }

        this->tasks.clear();

        delete this->timer;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void Scheduler::executePeriodically(Runnable* task, long long period, bool ownsTask) {

    if (!isStarted()) {
        throw IllegalStateException(__FILE__, __LINE__, "Scheduler is not started.");
    }

    synchronized(&mutex) {
        TimerTask* timerTask = new SchedulerTimerTask(task, ownsTask);
        this->timer->scheduleAtFixedRate(timerTask, period, period);
        this->tasks.put(task, timerTask);
    }
}

////////////////////////////////////////////////////////////////////////////////
void Scheduler::schedualPeriodically(Runnable* task, long long period, bool ownsTask) {

    if (!isStarted()) {
        throw IllegalStateException(__FILE__, __LINE__, "Scheduler is not started.");
    }

    synchronized(&mutex) {
        TimerTask* timerTask = new SchedulerTimerTask(task, ownsTask);
        this->timer->schedule(timerTask, period, period);
        this->tasks.put(task, timerTask);
    }
}

////////////////////////////////////////////////////////////////////////////////
void Scheduler::cancel(Runnable* task) {

    if (!isStarted()) {
        throw IllegalStateException(__FILE__, __LINE__, "Scheduler is not started.");
    }

    synchronized(&mutex) {
        TimerTask* ticket = this->tasks.remove(task);
        if (ticket != NULL) {
            ticket->cancel();
            this->timer->purge();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void Scheduler::executeAfterDelay(Runnable* task, long long delay, bool ownsTask) {

    if (!isStarted()) {
        throw IllegalStateException(__FILE__, __LINE__, "Scheduler is not started.");
    }

    synchronized(&mutex) {
        TimerTask* timerTask = new SchedulerTimerTask(task, ownsTask);
        this->timer->schedule(timerTask, delay);
    }
}

////////////////////////////////////////////////////////////////////////////////
void Scheduler::shutdown() {
    if (this->timer != NULL) {
        this->timer->cancel();
    }
}

////////////////////////////////////////////////////////////////////////////////
void Scheduler::doStart() {
    synchronized(&mutex) {
        this->timer = new Timer(name);
    }
}

////////////////////////////////////////////////////////////////////////////////
void Scheduler::doStop(ServiceStopper* stopper AMQCPP_UNUSED) {
    synchronized(&mutex) {
        if (this->timer != NULL) {
            this->timer->cancel();
        }
    }
}
