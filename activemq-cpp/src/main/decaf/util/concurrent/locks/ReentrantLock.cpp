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

#include <decaf/util/concurrent/locks/ReentrantLock.h>

#include <decaf/lang/Thread.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/exceptions/IllegalMonitorStateException.h>
#include <decaf/util/concurrent/locks/AbstractQueuedSynchronizer.h>

#include <sstream>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::locks;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{
namespace concurrent{
namespace locks{

    /**
     * Fairness Policy base class that uses the AbstraactQueuedSynchronizer to do
     * most of the heavy lifting for the ReentrantLock object.
     */
    class Sync : public AbstractQueuedSynchronizer {
    public:

        Sync() {}
        virtual ~Sync() {}

        /**
         * Performs the actual lock.  The locking policy subclasses can
         * optimize the locking process based on their fairness.
         */
        virtual void lock() = 0;

        virtual bool isFair() const = 0;

        /**
         * Performs non-fair tryLock.  tryAcquire is implemented by each of the
         * policy subclasses but both can make use of a non-fair try.
         */
        bool nonfairTryAcquire(int acquires) {

            Thread* current = Thread::currentThread();
            int c = this->getState();

            if (c == 0) {

                if (this->compareAndSetState(0, acquires)) {
                    this->setExclusiveOwnerThread(current);
                    return true;
                }

            } else if (current == this->getExclusiveOwnerThread()) {

                int nextc = c + acquires;

                // Check for overflow of the state counter.
                if (nextc < 0) {
                    throw new RuntimeException(__FILE__, __LINE__, "Maximum lock count exceeded");
                }

                this->setState(nextc);
                return true;
            }

            return false;
        }

        Condition* getNewCondition() {
            return AbstractQueuedSynchronizer::createDefaultConditionObject();
        }

        Thread* getOwner() const {
            return getState() == 0 ? NULL : getExclusiveOwnerThread();
        }

        int getHoldCount() const {
            return isHeldExclusively() ? getState() : 0;
        }

        bool isLocked() const {
            return getState() != 0;
        }

        bool isHeldExclusively() const {
            return getExclusiveOwnerThread() == Thread::currentThread();
        }

    protected:

        bool tryRelease(int releases) {

            int c = getState() - releases;

            if (Thread::currentThread() != getExclusiveOwnerThread()) {
                throw IllegalMonitorStateException();
            }

            bool free = false;

            if (c == 0) {
                free = true;
                setExclusiveOwnerThread(NULL);
            }

            setState(c);

            return free;
        }

    };

    class NonFairSync : public Sync {
    public:

        NonFairSync() : Sync() {}
        virtual ~NonFairSync() {}

    public:

        virtual bool isFair() const {
            return false;
        }

        virtual void lock() {

            if (this->compareAndSetState(0, 1)) {
                this->setExclusiveOwnerThread(Thread::currentThread());
            } else {
                acquire(1);
            }
        }

    protected:

        virtual bool tryAcquire(int acquires) {
            return this->nonfairTryAcquire(acquires);
        }
    };

    /**
     * The fair policy only lets a Thread acquire if the lock is not held, the
     * call amounts to a recursive lock, or the Thread is the first one in line.
     */
    class FairSync : public Sync {
    public:

        FairSync() : Sync() {}
        virtual ~FairSync() {}

    public:

        virtual void lock() {
            this->acquire(1);
        }

        virtual bool isFair() const {
            return true;
        }

    protected:

        virtual bool tryAcquire(int acquires) {

            Thread* current = Thread::currentThread();

            int c = this->getState();

            if (c == 0) {

                if (!hasQueuedPredecessors() && compareAndSetState(0, acquires)) {
                    this->setExclusiveOwnerThread(current);
                    return true;
                }

            } else if (current == this->getExclusiveOwnerThread()) {

                int nextc = c + acquires;

                // Check for overflow of the lock sstate variable.
                if (nextc < 0) {
                    throw new RuntimeException(__FILE__, __LINE__, "Maximum lock count exceeded");
                }

                setState(nextc);
                return true;
            }

            return false;
        }
    };

}}}}

////////////////////////////////////////////////////////////////////////////////
ReentrantLock::ReentrantLock() : Lock(), sync(new NonFairSync) {
}

////////////////////////////////////////////////////////////////////////////////
ReentrantLock::ReentrantLock(bool fair) : Lock(), sync(NULL) {
    fair == true ? sync = new FairSync() : sync = new NonFairSync();
}

////////////////////////////////////////////////////////////////////////////////
ReentrantLock::~ReentrantLock() {
    try{
        delete this->sync;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLock::lock() {
    this->sync->lock();
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLock::lockInterruptibly() {
    this->sync->acquireInterruptibly(1);
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantLock::tryLock() {
    return this->sync->nonfairTryAcquire(1);
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantLock::tryLock(long long time, const TimeUnit& unit) {
    return this->sync->tryAcquireNanos(1, unit.toNanos(time));
}

////////////////////////////////////////////////////////////////////////////////
void ReentrantLock::unlock() {
    this->sync->release(1);
}

////////////////////////////////////////////////////////////////////////////////
Condition* ReentrantLock::newCondition() {
    return this->sync->getNewCondition();
}

////////////////////////////////////////////////////////////////////////////////
int ReentrantLock::getHoldCount() const {
    return this->sync->getHoldCount();
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantLock::isHeldByCurrentThread() const {
    return this->sync->isHeldExclusively();
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantLock::isLocked() const {
    return this->sync->isLocked();
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantLock::isFair() const {
    return this->sync->isFair();
}

////////////////////////////////////////////////////////////////////////////////
std::string ReentrantLock::toString() const {
    std::ostringstream result;

    result << "ReentrantLock: ";

    Thread* current = this->sync->getOwner();

    if( current != NULL ) {
        result << "[Locked by Thread: " << current->getName() << "]";
    } else {
        result << "[Unlocked]";
    }

    return result.str();
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Thread* ReentrantLock::getOwner() const {
    return this->sync->getOwner();
}

////////////////////////////////////////////////////////////////////////////////
int ReentrantLock::getQueueLength() const {
    return this->sync->getQueueLength();
}

////////////////////////////////////////////////////////////////////////////////
Collection<decaf::lang::Thread*>* ReentrantLock::getQueuedThreads() const {
    return this->sync->getQueuedThreads();
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantLock::hasQueuedThreads() const {
    return this->sync->hasQueuedThreads();
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantLock::hasQueuedThread(decaf::lang::Thread* thread) const {

    if (thread == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The thread to check was NULL");
    }

    return this->sync->isQueued(thread);
}

////////////////////////////////////////////////////////////////////////////////
int ReentrantLock::getWaitQueueLength(Condition* condition) const {

    if (condition == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The Condition to check was NULL");
    }

    const AbstractQueuedSynchronizer::ConditionObject* cond =
        dynamic_cast<const AbstractQueuedSynchronizer::ConditionObject*>(condition);

    if (cond == NULL) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Condition is not associated with this Lock");
    }

    return this->sync->getWaitQueueLength(cond);
}

////////////////////////////////////////////////////////////////////////////////
bool ReentrantLock::hasWaiters(Condition* condition) const {

    if (condition == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The Condition to check was NULL");
    }

    const AbstractQueuedSynchronizer::ConditionObject* cond =
        dynamic_cast<const AbstractQueuedSynchronizer::ConditionObject*>(condition);

    if (cond == NULL) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Condition is not associated with this Lock");
    }

    return this->sync->hasWaiters(cond);
}

////////////////////////////////////////////////////////////////////////////////
Collection<decaf::lang::Thread*>* ReentrantLock::getWaitingThreads(Condition* condition) const {

    if (condition == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The Condition to check was NULL");
    }

    const AbstractQueuedSynchronizer::ConditionObject* cond =
        dynamic_cast<const AbstractQueuedSynchronizer::ConditionObject*>(condition);

    if (cond == NULL) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Condition is not associated with this Lock");
    }

    return this->sync->getWaitingThreads(cond);
}
