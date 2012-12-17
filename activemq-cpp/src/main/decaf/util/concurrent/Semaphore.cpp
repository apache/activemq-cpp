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

#include "Semaphore.h"

#include <decaf/lang/Integer.h>

#include <decaf/util/concurrent/locks/AbstractQueuedSynchronizer.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::locks;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {
namespace concurrent {

    class SemSync : public AbstractQueuedSynchronizer{
    public:

        SemSync(int permits) : AbstractQueuedSynchronizer() {
            this->setState(permits);
        }
        virtual ~SemSync() {}

        virtual bool isFair() const = 0;

        int getPermits() {
            return getState();
        }

        int nonfairTryAcquireShared(int acquires) {
            for (;;) {
                int available = getState();
                int remaining = available - acquires;
                if (remaining < 0 || compareAndSetState(available, remaining)) {
                    return remaining;
                }
            }
        }

        void reducePermits(int reductions) {
            for (;;) {
                int current = getState();
                int next = current - reductions;
                if (compareAndSetState(current, next)) {
                    return;
                }
            }
        }

        int drainPermits() {
            for (;;) {
                int current = getState();
                if (current == 0 || compareAndSetState(current, 0)) {
                    return current;
                }
            }
        }

    protected:

        virtual bool tryReleaseShared(int releases) {
            for (;;) {
                int p = getState();
                if (compareAndSetState(p, p + releases)) {
                    return true;
                }
            }
        }

    };

    class NonFairSemSync : public SemSync {
    public:

        NonFairSemSync(int permits) : SemSync(permits) {}
        virtual ~NonFairSemSync() {}

        virtual bool isFair() const {
            return false;
        }

    protected:

        virtual int tryAcquireShared(int acquires) {
            return nonfairTryAcquireShared(acquires);
        }
    };

    class FairSemSync : public SemSync {
    public:

        FairSemSync(int permits) : SemSync(permits) {}
        virtual ~FairSemSync() {}

        virtual bool isFair() const {
            return true;
        }

    protected:

        virtual int tryAcquireShared(int acquires) {
            Thread* current = Thread::currentThread();
            for(;;) {
                Thread* first = this->getFirstQueuedThread();

                if (first != NULL && first != current) {
                    return -1;
                }

                int available = getState();
                int remaining = available - acquires;
                if (remaining < 0 || compareAndSetState(available, remaining)) {
                    return remaining;
                }
            }
        }
    };

}}}

////////////////////////////////////////////////////////////////////////////////
Semaphore::Semaphore(int permits) : sync(new NonFairSemSync(permits)) {
}

////////////////////////////////////////////////////////////////////////////////
Semaphore::Semaphore(int permits, bool fair) : sync(NULL) {
    fair == true ? sync = new FairSemSync(permits) : sync = new NonFairSemSync(permits);
}

////////////////////////////////////////////////////////////////////////////////
Semaphore::~Semaphore() {
    try{
        delete sync;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquire() {
    this->sync->acquireSharedInterruptibly(1);
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquireUninterruptibly() {
    this->sync->acquireShared(1);
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire() {
    return this->sync->nonfairTryAcquireShared(1) >= 0;
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire(long long timeout, const TimeUnit& unit) {
    return this->sync->tryAcquireSharedNanos(1, unit.toNanos(timeout));
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::release() {
    this->sync->releaseShared(1);
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquire(int permits) {

    if (permits < 0) {
        throw new IllegalArgumentException(__FILE__, __LINE__,
            "Value of acquired permits must be greater than zero.");
    }

    this->sync->acquireSharedInterruptibly(permits);
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquireUninterruptibly( int permits DECAF_UNUSED ) {

    if (permits < 0) {
        throw new IllegalArgumentException(__FILE__, __LINE__,
            "Value of acquired permits must be greater than zero.");
    }

    this->sync->acquireShared(permits);
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire(int permits) {

    if (permits < 0) {
        throw new IllegalArgumentException(__FILE__, __LINE__,
            "Value of acquired permits must be greater than zero.");
    }

    return this->sync->nonfairTryAcquireShared(permits) >= 0;
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire(int permits, long long timeout, const TimeUnit& unit) {

    if (permits < 0) {
        throw new IllegalArgumentException(__FILE__, __LINE__,
            "Value of acquired permits must be greater than zero.");
    }

    return this->sync->tryAcquireSharedNanos(permits, unit.toNanos(timeout));
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::release(int permits) {

    if (permits < 0) {
        throw new IllegalArgumentException(__FILE__, __LINE__,
            "Value of acquired permits must be greater than zero.");
    }

    this->sync->releaseShared(permits);
}

////////////////////////////////////////////////////////////////////////////////
int Semaphore::availablePermits() const {
    return this->sync->getPermits();
}

////////////////////////////////////////////////////////////////////////////////
int Semaphore::drainPermits() {
    return this->sync->drainPermits();
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::reducePermits(int reduceBy) {
    return this->sync->reducePermits(reduceBy);
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::isFair() const {
    return this->sync->isFair();
}

////////////////////////////////////////////////////////////////////////////////
std::string Semaphore::toString() const {
    return std::string("Semaphore[Permits = ") + Integer::toString(this->sync->getPermits()) + "]";
}

////////////////////////////////////////////////////////////////////////////////
int Semaphore::getQueueLength() const {
    return this->sync->getQueueLength();
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::hasQueuedThreads() const {
    return this->sync->hasQueuedThreads();
}

////////////////////////////////////////////////////////////////////////////////
Collection<decaf::lang::Thread*>* Semaphore::getQueuedThreads() const {
    return this->sync->getQueuedThreads();
}
