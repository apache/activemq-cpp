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

#include "CompositeTaskRunner.h"

#include <memory>

#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace activemq;
using namespace activemq::threads;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
CompositeTaskRunner::CompositeTaskRunner() :
    tasks(), mutex(), thread(), threadTerminated(false), pending(false), shutDown(false) {

    this->thread.reset(new Thread(this, "ActiveMQ CompositeTaskRunner Thread"));
}

////////////////////////////////////////////////////////////////////////////////
CompositeTaskRunner::~CompositeTaskRunner() {
    try {
        this->shutdown();
        this->thread->join();
        this->thread.reset(NULL);
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::start() {

    synchronized(&mutex) {
        if (!shutDown && !this->thread->isAlive()) {
            this->thread->start();
            this->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool CompositeTaskRunner::isStarted() const {

    bool result = false;

    synchronized(&mutex) {
        if (this->thread != NULL) {
            result = true;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::shutdown(long long timeout) {

    synchronized(&mutex) {
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
void CompositeTaskRunner::shutdown() {

    synchronized(&mutex) {
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
void CompositeTaskRunner::wakeup() {

    synchronized(&mutex) {
        if (shutDown) {
            return;
        }
        pending = true;
        mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::run() {

    try {

        while (true) {

            synchronized(&mutex) {
                pending = false;
                if (shutDown) {
                    return;
                }
            }

            if (!this->iterate()) {

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

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::addTask(CompositeTask* task) {

    if (task != NULL) {
        synchronized(&tasks) {
            this->tasks.add(task);
            this->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::removeTask(CompositeTask* task) {

    if (task != NULL) {
        synchronized(&tasks) {
            this->tasks.remove(task);
            this->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool CompositeTaskRunner::iterate() {

    synchronized(&tasks) {

        for (int i = 0; i < tasks.size(); ++i) {
            CompositeTask* task = tasks.pop();

            if (task->isPending()) {
                task->iterate();
                tasks.addLast(task);

                // Always return true, so that we check again for any of
                // the other tasks that might now be pending.
                return true;
            } else {
                tasks.addLast(task);
            }
        }
    }

    return false;
}
