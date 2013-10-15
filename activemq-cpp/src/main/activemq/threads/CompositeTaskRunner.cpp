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
namespace activemq {
namespace threads {

    class CompositeTaskRunnerImpl {
    private:

        CompositeTaskRunnerImpl(const CompositeTaskRunnerImpl&);
        CompositeTaskRunnerImpl& operator= (const CompositeTaskRunnerImpl&);

    public:

        decaf::util::LinkedList<CompositeTask*> tasks;
        mutable decaf::util::concurrent::Mutex mutex;

        decaf::lang::Pointer<decaf::lang::Thread> thread;

        bool threadTerminated;
        bool pending;
        bool shutdown;

    public:

        CompositeTaskRunnerImpl() : tasks(),
                                    mutex(),
                                    thread(),
                                    threadTerminated(false),
                                    pending(false),
                                    shutdown(false) {
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
CompositeTaskRunner::CompositeTaskRunner() : impl(new CompositeTaskRunnerImpl) {
    this->impl->thread.reset(new Thread(this, "ActiveMQ CompositeTaskRunner Thread"));
}

////////////////////////////////////////////////////////////////////////////////
CompositeTaskRunner::~CompositeTaskRunner() {
    try {
        shutdown();
        impl->thread->join();
        impl->thread.reset(NULL);
    }
    AMQ_CATCHALL_NOTHROW()

    try {
        delete this->impl;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::start() {

    synchronized(&impl->mutex) {
        if (!impl->shutdown && !this->impl->thread->isAlive()) {
            this->impl->thread->start();
            this->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool CompositeTaskRunner::isStarted() const {

    bool result = false;

    synchronized(&impl->mutex) {
        if (this->impl->thread != NULL && this->impl->thread->isAlive()) {
            result = true;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::shutdown(long long timeout) {

    synchronized(&impl->mutex) {
        impl->shutdown = true;
        impl->pending = true;
        impl->mutex.notifyAll();
    }

    // Wait till the thread stops ( no need to wait if shutdown
    // is called from thread that is shutting down)
    if (Thread::currentThread() != this->impl->thread.get() && !impl->threadTerminated) {
        this->impl->thread->join(timeout);
    }
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::shutdown() {

    synchronized(&impl->mutex) {
        impl->shutdown = true;
        impl->pending = true;
        impl->mutex.notifyAll();
    }

    // Wait till the thread stops ( no need to wait if shutdown
    // is called from thread that is shutting down)
    if (Thread::currentThread() != this->impl->thread.get() && !impl->threadTerminated) {
        impl->thread->join();
    }
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::wakeup() {

    synchronized(&impl->mutex) {
        if (impl->shutdown) {
            return;
        }
        impl->pending = true;
        impl->mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::run() {

    try {

        while (true) {
            synchronized(&impl->mutex) {
                impl->pending = false;
                if (impl->shutdown) {
                    return;
                }
            }

            if (!this->iterate()) {
                // wait to be notified.
                synchronized(&impl->mutex) {
                    if (impl->shutdown) {
                        return;
                    }
                    while (!impl->pending && !impl->shutdown) {
                        impl->mutex.wait();
                    }
                }
            }
        }
    }
    AMQ_CATCHALL_NOTHROW()

    // Make sure we notify any waiting threads that thread
    // has terminated.
    synchronized(&impl->mutex) {
        impl->threadTerminated = true;
        impl->mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::addTask(CompositeTask* task) {

    if (task != NULL) {
        synchronized(&impl->tasks) {
            impl->tasks.add(task);
            wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void CompositeTaskRunner::removeTask(CompositeTask* task) {

    if (task != NULL) {
        synchronized(&impl->tasks) {
            impl->tasks.remove(task);
            wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool CompositeTaskRunner::iterate() {

    synchronized(&impl->tasks) {

        for (int i = 0; i < impl->tasks.size(); ++i) {
            CompositeTask* task = impl->tasks.pop();

            if (task->isPending()) {
                task->iterate();
                impl->tasks.addLast(task);

                // Always return true, so that we check again for any of
                // the other tasks that might now be pending.
                return true;
            } else {
                impl->tasks.addLast(task);
            }
        }
    }

    return false;
}
