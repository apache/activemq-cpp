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

#include "DedicatedTaskRunner.h"

#include <activemq/exceptions/ActiveMQException.h>

using namespace activemq;
using namespace activemq::threads;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
DedicatedTaskRunner::DedicatedTaskRunner(Task* task) :
    mutex(), thread(), threadTerminated(false), pending(false), shutDown(false), task(task) {

    if (this->task == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Task passed was null");
    }

    this->thread.reset(new Thread(this, "ActiveMQ Dedicated Task Runner"));
}

////////////////////////////////////////////////////////////////////////////////
DedicatedTaskRunner::~DedicatedTaskRunner() {
    try {
        this->shutdown();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void DedicatedTaskRunner::start() {

    synchronized(&mutex) {
        if (!shutDown && !this->thread->isAlive()) {
            this->thread->start();
            this->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool DedicatedTaskRunner::isStarted() const {

    bool result = false;

    synchronized(&mutex) {
        if (this->thread != NULL) {
            result = true;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void DedicatedTaskRunner::shutdown(long long timeout) {

    synchronized(&mutex) {

        if (this->thread == NULL) {
            return;
        }

        shutDown = true;
        pending = true;
        mutex.notifyAll();
    }

    // Wait till the thread stops ( no need to wait if shutdown
    // is called from thread that is shutting down)
    if (Thread::currentThread() != this->thread.get() && !threadTerminated) {
        this->thread->join(timeout);
    }
}

////////////////////////////////////////////////////////////////////////////////
void DedicatedTaskRunner::shutdown() {

    synchronized(&mutex) {

        if (this->thread == NULL) {
            return;
        }

        shutDown = true;
        pending = true;
        mutex.notifyAll();
    }

    // Wait till the thread stops ( no need to wait if shutdown
    // is called from thread that is shutting down)
    if (Thread::currentThread() != this->thread.get() && !threadTerminated) {
        this->thread->join();
    }
}

////////////////////////////////////////////////////////////////////////////////
void DedicatedTaskRunner::wakeup() {

    synchronized(&mutex) {
        if (shutDown) {
            return;
        }
        pending = true;
        mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void DedicatedTaskRunner::run() {

    try {

        while (true) {

            synchronized(&mutex) {
                pending = false;
                if (shutDown) {
                    return;
                }
            }

            if (!this->task->iterate()) {

                // wait to be notified.
                synchronized(&mutex) {
                    if (shutDown) {
                        return;
                    }
                    while (!pending && !shutDown) {
                        mutex.wait();
                    }
                }
            }

        }
    }
    AMQ_CATCHALL_NOTHROW()

    // Make sure we notify any waiting threads that thread
    // has terminated.
    synchronized(&mutex) {
        threadTerminated = true;
        mutex.notifyAll();
    }
}
