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

#include "AbstractQueuedSynchronizer.h"

#include <decaf/lang/System.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

#include <decaf/util/ArrayList.h>
#include <decaf/util/concurrent/locks/LockSupport.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/concurrent/atomic/AtomicReference.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace decaf::util::concurrent::locks;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class Node {
    public:

        enum WaitStatus {
            CANCELLED = 1,
            SIGNAL = -1,
            CONDITION = -2,
            PROPAGATE = -3
        };

        static Node SHARED;
        static Node* EXCLUSIVE;

    public:

        AtomicInteger waitStatus;
        AtomicReference<Node> prev;
        AtomicReference<Node> next;
        Thread* thread;
        AtomicReference<Node> nextWaiter;

    public:

        Node() : waitStatus(0), prev(NULL), next(NULL), thread(NULL), nextWaiter(NULL) {
        }
        Node(Thread* thread, Node* node) : waitStatus(0), prev(NULL), next(NULL), thread(thread), nextWaiter(node) {
        }
        Node(Thread* thread, int waitStatus) : waitStatus(waitStatus), prev(NULL), next(NULL), thread(thread), nextWaiter(NULL) {
        }

        ~Node() {
            std::cout << "Deleted Node: " << std::hex << this << std::endl;
        }

        bool isShared() const {
            return this->nextWaiter.get() == &SHARED;
        }

        Node* predecessor() {
            Node* p = prev.get();
            if (p == NULL) {
                throw NullPointerException();
            } else {
                return p;
            }
        }
    };

    Node Node::SHARED;
    Node* Node::EXCLUSIVE = NULL;

    // For very short timeouts its usually more efficient to just spin instead of
    // parking a thread.
    const long long spinForTimeoutLimit = 1000LL;
}

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class SynchronizerState {
    public:

        AbstractQueuedSynchronizer* parent;
        AtomicInteger state;
        AtomicReference<Node> head;
        AtomicReference<Node> tail;

    public:

        SynchronizerState(AbstractQueuedSynchronizer* parent) : parent(parent), state(0), head(), tail() {}
        virtual ~SynchronizerState() {
            Node* oldHead = head.getAndSet(NULL);
            if (oldHead != tail.get()) {
                delete tail.get();
            }
            delete oldHead;
        }

        bool isHeldExclusively() const {
            return this->parent->isHeldExclusively();
        }

        Node* enq(Node* node) {
            for (;;) {
                Node* t = tail.get();
                if (t == NULL) { // Must initialize
                    Node* newHead = new Node();
                    std::cout << "Enq first call, allocated head = " << std::hex << newHead << std::endl;
                    if (head.compareAndSet(NULL, newHead)) {
                        tail.set(head.get());
                    }
                } else {
                    node->prev.set(t);
                    if (tail.compareAndSet(t, node)) {
                        t->next.set(node);
                        return t;
                    }
                }
            }
        }

        Node* addWaiter(Node* mode) {
            Node* node = new Node(Thread::currentThread(), mode);
            std::cout << "Add Waiter Allocated Node: " << std::hex << node << std::endl;
            // Try the fast path of enq; backup to full enq on failure
            Node* pred = tail.get();
            if (pred != NULL) {
                node->prev.set(pred);
                if (tail.compareAndSet(pred, node)) {
                    pred->next.set(node);
                    return node;
                }
            }
            enq(node);
            return node;
        }

        void setHead(Node* node) {
            head.set(node);
            node->thread = NULL;
            node->prev.set(NULL);
        }

        void unparkSuccessor(Node* node) {
            /*
             * If status is negative (i.e., possibly needing signal) try
             * to clear in anticipation of signalling.  It is OK if this
             * fails or if status is changed by waiting thread.
             */
            int ws = node->waitStatus.get();
            if (ws < 0) {
                node->waitStatus.compareAndSet(ws, 0);
            }

            /*
             * Thread to unpark is held in successor, which is normally
             * just the next node.  But if cancelled or apparently NULL,
             * traverse backwards from tail to find the actual
             * non-cancelled successor.
             */
            Node* s = node->next.get();
            if (s == NULL || s->waitStatus.get() > 0) {
                s = NULL;
                for (Node* t = tail.get(); t != NULL && t != node; t = t->prev.get())
                    if (t->waitStatus.get() <= 0) {
                        s = t;
                    }
            }

            if (s != NULL) {
                LockSupport::unpark(s->thread);
            }
        }

        /**
         * Release action for shared mode -- signal successor and ensure
         * propagation. (Note: For exclusive mode, release just amounts
         * to calling unparkSuccessor of head if it needs signal.)
         */
        void doReleaseShared() {
            /*
             * Ensure that a release propagates, even if there are other
             * in-progress acquires/releases.  This proceeds in the usual
             * way of trying to unparkSuccessor of head if it needs
             * signal. But if it does not, status is set to PROPAGATE to
             * ensure that upon release, propagation continues.
             * Additionally, we must loop in case a new node is added
             * while we are doing this. Also, unlike other uses of
             * unparkSuccessor, we need to know if CAS to reset status
             * fails, if so rechecking.
             */
            for (;;) {
                Node* h = head.get();
                if (h != NULL && h != tail.get()) {
                    int ws = h->waitStatus.get();
                    if (ws == Node::SIGNAL) {
                        if (!h->waitStatus.compareAndSet(Node::SIGNAL, 0)) {
                            continue;            // loop to recheck cases
                        }
                        unparkSuccessor(h);
                    } else if (ws == 0 && !h->waitStatus.compareAndSet(0, Node::PROPAGATE)) {
                        continue;                // loop on failed CAS
                    }
                }
                if (h == head.get()) {                  // loop if head changed
                    break;
                }
            }
        }

        /**
         * Sets head of queue, and checks if successor may be waiting
         * in shared mode, if so propagating if either propagate > 0 or
         * PROPAGATE status was set.
         *
         * @param node the node
         * @param propagate the return value from a tryAcquireShared
         */
        void setHeadAndPropagate(Node* node, int propagate) {
            Node* h = head.get(); // Record old head for check below
            setHead(node);
            /*
             * Try to signal next queued node if:
             *   Propagation was indicated by caller,
             *     or was recorded (as h.waitStatus) by a previous operation
             *     (note: this uses sign-check of waitStatus because
             *      PROPAGATE status may transition to SIGNAL.)
             * and
             *   The next node is waiting in shared mode,
             *     or we don't know, because it appears NULL
             *
             * The conservatism in both of these checks may cause
             * unnecessary wake-ups, but only when there are multiple
             * racing acquires/releases, so most need signals now or soon
             * anyway.
             */
            if (propagate > 0 || h == NULL || h->waitStatus.get() < 0) {
                Node* s = node->next.get();
                if (s == NULL || s->isShared()) {
                    doReleaseShared();
                }
            }
        }

        /**
         * Cancels an ongoing attempt to acquire.
         *
         * @param node the node
         */
        void cancelAcquire(Node* node) {
            // Ignore if node doesn't exist
            if (node == NULL) {
                return;
            }

            node->thread = NULL;

            // Skip cancelled predecessors
            Node* pred = node->prev.get();
            while (pred->waitStatus.get() > 0) {
                pred = pred->prev.get();
                node->prev.set(pred);
            }

            // predNext is the apparent node to unsplice. CASes below will
            // fail if not, in which case, we lost race vs another cancel
            // or signal, so no further action is necessary.
            Node* predNext = pred->next.get();

            // Can use unconditional write instead of CAS here.
            // After this atomic step, other Nodes can skip past us.
            // Before, we are free of interference from other threads.
            node->waitStatus.set(Node::CANCELLED);

            // If we are the tail, remove ourselves.
            if (node == tail.get() && tail.compareAndSet(node, pred)) {
                pred->next.compareAndSet(predNext, NULL);
                delete node;
            } else {
                // If successor needs signal, try to set pred's next-link
                // so it will get one. Otherwise wake it up to propagate.
                int ws;
                if (pred != head.get() &&
                    ((ws = pred->waitStatus.get()) == Node::SIGNAL ||
                     (ws <= 0 && pred->waitStatus.compareAndSet(ws, Node::SIGNAL))) && pred->thread != NULL) {
                    Node* next = node->next.get();
                    if (next != NULL && next->waitStatus.get() <= 0) {
                        pred->next.compareAndSet(predNext, next);
                    }
                } else {
                    unparkSuccessor(node);
                }
            }

            delete node;
        }

        /**
         * Checks and updates status for a node that failed to acquire.
         * Returns true if thread should block. This is the main signal
         * control in all acquire loops.  Requires that pred == node.prev
         *
         * @param pred node's predecessor holding status
         * @param node the node
         * @return {@code true} if thread should block
         */
        static bool shouldParkAfterFailedAcquire(Node* pred, Node* node) {
            int ws = pred->waitStatus.get();

            if (ws == Node::SIGNAL)
                /*
                 * This node has already set status asking a release
                 * to signal it, so it can safely park.
                 */
                return true;
            if (ws > 0) {
                /*
                 * Predecessor was cancelled. Skip over predecessors and
                 * indicate retry.
                 */
                do {
                    pred = pred->prev.get();
                    node->prev.set(pred);
                } while (pred->waitStatus.get() > 0);
                pred->next.set(node);
            } else {
                /*
                 * waitStatus must be 0 or PROPAGATE.  Indicate that we
                 * need a signal, but don't park yet.  Caller will need to
                 * retry to make sure it cannot acquire before parking.
                 */
                pred->waitStatus.compareAndSet(ws, Node::SIGNAL);
            }

            return false;
        }

        /**
         * Convenience method to interrupt current thread.
         */
        static void selfInterrupt() {
            Thread::currentThread()->interrupt();
        }

        /**
         * Convenience method to park and then check if interrupted
         *
         * @return {@code true} if interrupted
         */
        bool parkAndCheckInterrupt() const {
            LockSupport::park();
            return Thread::interrupted();
        }

        /**
         * Acquires in exclusive uninterruptible mode for thread already in
         * queue. Used by condition wait methods as well as acquire.
         *
         * @param node
         *      The node.
         * @param arg
         *      The value passed to acquire.
         *
         * @return {@code true} if interrupted while waiting
         */
        bool acquireQueued(Node* node, int arg) {
            bool failed = true;
            try {

                bool interrupted = false;
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get() && parent->tryAcquire(arg)) {
                        setHead(node);
                        delete p;
                        failed = false;
                        return interrupted;
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && parkAndCheckInterrupt()) {
                        interrupted = true;
                    }
                }

            } catch(Exception& ex) {
                if (failed) {
                    cancelAcquire(node);
                }

                throw ex;
            }
        }

        /**
         * Acquires in exclusive interruptible mode.
         * @param arg the acquire argument
         */
        void doAcquireInterruptibly(int arg) {
            Node* node = addWaiter(Node::EXCLUSIVE);
            bool failed = true;
            try {
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get() && parent->tryAcquire(arg)) {
                        setHead(node);
                        std::cout << "doAcquireInterruptibly waiting thread acquired the sync" << std::endl;
                        delete p;
                        failed = false;
                        return;
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && parkAndCheckInterrupt()) {
                        std::cout << "doAcquireInterruptibly waiting thread interrupted" << std::endl;
                        throw InterruptedException();
                    }
                }

            } catch(InterruptedException& ex) {
                if (failed) {
                    cancelAcquire(node);
                }

                throw ex;
            }
        }

        /**
         * Acquires in exclusive timed mode.
         *
         * @param arg the acquire argument
         * @param nanosTimeout max wait time
         * @return {@code true} if acquired
         */
        bool doAcquireNanos(int arg, long long nanosTimeout) {
            long long lastTime = System::nanoTime();
            Node* node = addWaiter(Node::EXCLUSIVE);
            bool failed = true;
            try {
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get() && parent->tryAcquire(arg)) {
                        setHead(node);
                        delete p;
                        failed = false;
                        return true;
                    }

                    if (nanosTimeout <= 0) {
                        return false;
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && nanosTimeout > spinForTimeoutLimit) {
                        LockSupport::parkNanos(nanosTimeout);
                    }

                    long long now = System::nanoTime();
                    nanosTimeout -= now - lastTime;
                    lastTime = now;

                    if (Thread::interrupted()) {
                        throw InterruptedException();
                    }
                }

            } catch(InterruptedException& ex) {
                if (failed) {
                    cancelAcquire(node);
                }

                throw ex;
            }
        }

        /**
         * Acquires in shared uninterruptible mode.
         * @param arg the acquire argument
         */
        void doAcquireShared(int arg) {
            Node* node = addWaiter(&Node::SHARED);
            bool failed = true;
            try {
                bool interrupted = false;
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get()) {
                        int r = parent->tryAcquireShared(arg);
                        if (r >= 0) {
                            setHeadAndPropagate(node, r);
                            delete p;
                            if (interrupted) {
                                selfInterrupt();
                            }
                            failed = false;
                            return;
                        }
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && parkAndCheckInterrupt()) {
                        interrupted = true;
                    }
                }
            } catch(InterruptedException& ex) {
                if (failed) {
                    cancelAcquire(node);
                }

                throw ex;
            }
        }

        /**
         * Acquires in shared interruptible mode.
         * @param arg the acquire argument
         */
        void doAcquireSharedInterruptibly(int arg) {
            Node* node = addWaiter(&Node::SHARED);
            bool failed = true;
            try {
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get()) {
                        int r = parent->tryAcquireShared(arg);
                        if (r >= 0) {
                            setHeadAndPropagate(node, r);
                            delete p;
                            failed = false;
                            return;
                        }
                    }
                    if (shouldParkAfterFailedAcquire(p, node) && parkAndCheckInterrupt()) {
                        throw InterruptedException();
                    }
                }
            } catch(InterruptedException& ex) {
                if (failed) {
                    cancelAcquire(node);
                }

                throw ex;
            }
        }

        /**
         * Acquires in shared timed mode.
         *
         * @param arg the acquire argument
         * @param nanosTimeout max wait time
         * @return {@code true} if acquired
         */
        bool doAcquireSharedNanos(int arg, long long nanosTimeout) {

            long long lastTime = System::nanoTime();
            Node* node = addWaiter(&Node::SHARED);
            bool failed = true;
            try {
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get()) {
                        int r = parent->tryAcquireShared(arg);
                        if (r >= 0) {
                            setHeadAndPropagate(node, r);
                            delete p;
                            failed = false;
                            return true;
                        }
                    }
                    if (nanosTimeout <= 0) {
                        return false;
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && nanosTimeout > spinForTimeoutLimit) {
                        LockSupport::parkNanos(nanosTimeout);
                    }

                    long now = System::nanoTime();
                    nanosTimeout -= now - lastTime;
                    lastTime = now;

                    if (Thread::interrupted()) {
                        throw InterruptedException();
                    }
                }
            } catch(InterruptedException& ex) {
                if (failed) {
                    cancelAcquire(node);
                }

                throw ex;
            }
        }

        Thread* fullGetFirstQueuedThread() {
            /*
             * The first node is normally head->next. Try to get its
             * thread field, ensuring consistent reads: If thread
             * field is nulled out or s->prev is no longer head, then
             * some other thread(s) concurrently performed setHead in
             * between some of our reads. We try this twice before
             * resorting to traversal.
             */
            Node* h = NULL;
            Node* s = NULL;
            Thread* st = NULL;

            if (((h = head.get()) != NULL && (s = h->next.get()) != NULL &&
                 s->prev.get() == head.get() && (st = s->thread) != NULL) ||
                ((h = head.get()) != NULL && (s = h->next.get()) != NULL &&
                 s->prev.get() == head.get() && (st = s->thread) != NULL)) {

                return st;
            }

            /*
             * Head's next field might not have been set yet, or may have
             * been unset after setHead. So we must check to see if tail
             * is actually first node. If not, we continue on, safely
             * traversing from tail back to head to find first,
             * guaranteeing termination.
             */

            Node* t = tail.get();
            Thread* firstThread = NULL;
            while (t != NULL && t != head.get()) {
                Thread* tt = t->thread;
                if (tt != NULL)
                    firstThread = tt;
                t = t->prev.get();
            }
            return firstThread;
        }

        /**
         * Returns true if a node, always one that was initially placed on
         * a condition queue, is now waiting to reacquire on sync queue.
         *
         * @param node the node
         *
         * @return true if is reacquiring
         */
        bool isOnSyncQueue(Node* node) {
            if (node->waitStatus.get() == Node::CONDITION || node->prev.get() == NULL) {
                return false;
            }

            if (node->next.get() != NULL) { // If has successor, it must be on queue
                return true;
            }

            /*
             * node->prev can be non-NULL, but not yet on queue because
             * the CAS to place it on queue can fail. So we have to
             * traverse from tail to make sure it actually made it.  It
             * will always be near the tail in calls to this method, and
             * unless the CAS failed (which is unlikely), it will be
             * there, so we hardly ever traverse much.
             */
            return findNodeFromTail(node);
        }

        /**
         * Returns true if node is on sync queue by searching backwards from tail.
         * Called only when needed by isOnSyncQueue.
         * @return true if present
         */
        bool findNodeFromTail(Node* node) {
            Node* t = tail.get();
            for (;;) {

                if (t == node) {
                    return true;
                }

                if (t == NULL) {
                    return false;
                }

                t = t->prev.get();
            }
        }

        /**
         * Transfers a node from a condition queue onto sync queue.
         * Returns true if successful.
         * @param node the node
         * @return true if successfully transferred (else the node was
         * cancelled before signal).
         */
        bool transferForSignal(Node* node) {
            /*
             * If cannot change waitStatus, the node has been cancelled.
             */
            if (!node->waitStatus.compareAndSet(Node::CONDITION, 0)) {
                return false;
            }

            /*
             * Splice onto queue and try to set waitStatus of predecessor to
             * indicate that thread is (probably) waiting. If cancelled or
             * attempt to set waitStatus fails, wake up to resync (in which
             * case the waitStatus can be transiently and harmlessly wrong).
             */
            Node* p = enq(node);
            int ws = p->waitStatus.get();
            if (ws > 0 || !p->waitStatus.compareAndSet(ws, Node::SIGNAL)) {
                LockSupport::unpark(node->thread);
            }

            return true;
        }

        /**
         * Transfers node, if necessary, to sync queue after a cancelled
         * wait. Returns true if thread was cancelled before being
         * signalled.
         * @param current the waiting thread
         * @param node its node
         * @return true if cancelled before the node was signalled
         */
        bool transferAfterCancelledWait(Node* node) {
            if (node->waitStatus.compareAndSet(Node::CONDITION, 0)) {
                enq(node);
                return true;
            }

            /*
             * If we lost out to a signal(), then we can't proceed
             * until it finishes its enq().  Cancelling during an
             * incomplete transfer is both rare and transient, so just
             * spin.
             */
            while (!isOnSyncQueue(node)) {
                Thread::yield();
            }

            return false;
        }

        /**
         * Invokes release with current state value; returns saved state.
         * Cancels node and throws exception on failure.
         * @param node the condition node for this wait
         * @return previous sync state
         */
        int fullyRelease(Node* node) {
            bool failed = true;
            try {
                int savedState = parent->getState();
                if (parent->release(savedState)) {
                    failed = false;
                    return savedState;
                } else {
                    throw IllegalMonitorStateException();
                }
            } catch(IllegalMonitorStateException& ex) {
                if (failed) {
                    node->waitStatus.set(Node::CANCELLED);
                }

                throw ex;
            }
        }

    };

    /**
     * Provides a default implementation that most AbstractQueuedSynchronizer derived classes
     * can use without needing to write one from scratch.
     */
    class DefaultConditionObject : public AbstractQueuedSynchronizer::ConditionObject {
    private:

        SynchronizerState* impl;
        Node* firstWaiter;
        Node* lastWaiter;

        /** Mode meaning to reinterrupt on exit from wait */
        static const int REINTERRUPT;
        /** Mode meaning to throw InterruptedException on exit from wait */
        static const int THROW_IE;

    private:

        DefaultConditionObject(const DefaultConditionObject&);
        DefaultConditionObject& operator= (const DefaultConditionObject&);

    public:

        DefaultConditionObject(SynchronizerState* impl) :
            ConditionObject(), impl(impl), firstWaiter(NULL), lastWaiter(NULL) {}
        virtual ~DefaultConditionObject() {};

        virtual void await() {
            if (Thread::interrupted()) {
                throw InterruptedException(__FILE__, __LINE__, "Thread was interrupted");
            }
            Node* node = addConditionWaiter();
            int savedState = impl->fullyRelease(node);
            int interruptMode = 0;
            while (!impl->isOnSyncQueue(node)) {
                LockSupport::park();
                if ((interruptMode = checkInterruptWhileWaiting(node)) != 0) {
                    break;
                }
            }

            if (impl->acquireQueued(node, savedState) && interruptMode != THROW_IE) {
                interruptMode = REINTERRUPT;
            }
            if (node->nextWaiter.get() != NULL) { // clean up if cancelled
                unlinkCancelledWaiters();
            }
            if (interruptMode != 0) {
                reportInterruptAfterWait(interruptMode);
            }
        }

        virtual void awaitUninterruptibly() {
            Node* node = addConditionWaiter();
            int savedState = impl->fullyRelease(node);
            bool interrupted = false;
            while (!impl->isOnSyncQueue(node)) {
                LockSupport::park();
                if (Thread::interrupted()) {
                    interrupted = true;
                }
            }

            if (impl->acquireQueued(node, savedState) || interrupted) {
                impl->selfInterrupt();
            }
        }

        virtual long long awaitNanos( long long nanosTimeout ) {
            if (Thread::interrupted()) {
                throw InterruptedException(__FILE__, __LINE__, "Thread was interrupted");
            }
            Node* node = addConditionWaiter();
            int savedState = impl->fullyRelease(node);
            long long lastTime = System::nanoTime();
            int interruptMode = 0;
            while (!impl->isOnSyncQueue(node)) {
                if (nanosTimeout <= 0L) {
                    impl->transferAfterCancelledWait(node);
                    break;
                }
                LockSupport::parkNanos(nanosTimeout);
                if ((interruptMode = checkInterruptWhileWaiting(node)) != 0) {
                    break;
                }

                long now = System::nanoTime();
                nanosTimeout -= now - lastTime;
                lastTime = now;
            }

            if (impl->acquireQueued(node, savedState) && interruptMode != THROW_IE) {
                interruptMode = REINTERRUPT;
            }
            if (node->nextWaiter.get() != NULL) {
                unlinkCancelledWaiters();
            }
            if (interruptMode != 0) {
                reportInterruptAfterWait(interruptMode);
            }
            return nanosTimeout - (System::nanoTime() - lastTime);
        }

        virtual bool await( long long time, const TimeUnit& unit ) {
            long long nanosTimeout = unit.toNanos(time);
            if (Thread::interrupted()) {
                throw InterruptedException(__FILE__, __LINE__, "Thread was interrupted");
            }
            Node* node = addConditionWaiter();
            int savedState = impl->fullyRelease(node);
            long long lastTime = System::nanoTime();
            bool timedout = false;
            int interruptMode = 0;
            while (!impl->isOnSyncQueue(node)) {
                if (nanosTimeout <= 0L) {
                    timedout = impl->transferAfterCancelledWait(node);
                    break;
                }
                if (nanosTimeout >= spinForTimeoutLimit) {
                    LockSupport::parkNanos(nanosTimeout);
                }
                if ((interruptMode = checkInterruptWhileWaiting(node)) != 0) {
                    break;
                }
                long long now = System::nanoTime();
                nanosTimeout -= now - lastTime;
                lastTime = now;
            }

            if (impl->acquireQueued(node, savedState) && interruptMode != THROW_IE) {
                interruptMode = REINTERRUPT;
            }
            if (node->nextWaiter.get() != NULL) {
                unlinkCancelledWaiters();
            }
            if (interruptMode != 0) {
                reportInterruptAfterWait(interruptMode);
            }

            return !timedout;
        }

        virtual bool awaitUntil(const Date& deadline) {
            long long abstime = deadline.getTime();
            if (Thread::interrupted()) {
                throw InterruptedException(__FILE__, __LINE__, "Thread was interrupted");
            }
            Node* node = addConditionWaiter();
            int savedState = impl->fullyRelease(node);
            bool timedout = false;
            int interruptMode = 0;
            while (!impl->isOnSyncQueue(node)) {
                if (System::currentTimeMillis() > abstime) {
                    timedout = impl->transferAfterCancelledWait(node);
                    break;
                }
                LockSupport::parkUntil(abstime);
                if ((interruptMode = checkInterruptWhileWaiting(node)) != 0) {
                    break;
                }
            }

            if (impl->acquireQueued(node, savedState) && interruptMode != THROW_IE) {
                interruptMode = REINTERRUPT;
            }
            if (node->nextWaiter.get() != NULL) {
                unlinkCancelledWaiters();
            }
            if (interruptMode != 0) {
                reportInterruptAfterWait(interruptMode);
            }

            return !timedout;
        }

        virtual void signal() {
            if (!impl->isHeldExclusively()) {
                throw IllegalMonitorStateException();
            }
            Node* first = firstWaiter;
            if (first != NULL) {
                doSignal(first);
            }
        }

        virtual void signalAll() {
            if (!impl->isHeldExclusively()) {
                throw IllegalMonitorStateException();
            }
            Node* first = firstWaiter;
            if (first != NULL) {
                doSignalAll(first);
            }
        }

        virtual bool isOwnedBy(const AbstractQueuedSynchronizer* sync) const {
            return sync == this->impl->parent;
        }

        virtual bool hasWaiters() const {
            if (!impl->isHeldExclusively()) {
                throw IllegalMonitorStateException();
            }
            for (Node* w = firstWaiter; w != NULL; w = w->nextWaiter.get()) {
                if (w->waitStatus.get() == Node::CONDITION) {
                    return true;
                }
            }
            return false;
        }

        virtual int getWaitQueueLength() const {
            if (!impl->isHeldExclusively()) {
                throw IllegalMonitorStateException();
            }
            int n = 0;
            for (Node* w = firstWaiter; w != NULL; w = w->nextWaiter.get()) {
                if (w->waitStatus.get() == Node::CONDITION) {
                    ++n;
                }
            }
            return n;
        }

        virtual Collection<decaf::lang::Thread*>* getWaitingThreads() const {
            if (!impl->isHeldExclusively()) {
                throw IllegalMonitorStateException();
            }
            ArrayList<Thread*>* list = new ArrayList<Thread*>();
            for (Node* w = firstWaiter; w != NULL; w = w->nextWaiter.get()) {
                if (w->waitStatus.get() == Node::CONDITION) {
                    Thread* t = w->thread;
                    if (t != NULL) {
                        list->add(t);
                    }
                }
            }
            return list;
        }

    private:

        Node* addConditionWaiter() {
            Node* t = lastWaiter;
            // If lastWaiter is cancelled, clean out.
            if (t != NULL && t->waitStatus.get() != Node::CONDITION) {
                unlinkCancelledWaiters();
                t = lastWaiter;
            }
            Node* node = new Node(Thread::currentThread(), Node::CONDITION);
            if (t == NULL) {
                firstWaiter = node;
            } else {
                t->nextWaiter.set(node);
            }
            lastWaiter = node;
            return node;
        }

        /**
         * Removes and transfers nodes until hit non-cancelled one or
         * NULL. Split out from signal in part to encourage compilers
         * to inline the case of no waiters.
         * @param first (non-NULL) the first node on condition queue
         */
        void doSignal(Node* first) {
            do {
                if ((firstWaiter = first->nextWaiter.get()) == NULL) {
                    lastWaiter = NULL;
                }
                first->nextWaiter.set(NULL);
            } while (!impl->transferForSignal(first) && (first = firstWaiter) != NULL);
        }

        /**
         * Removes and transfers all nodes.
         * @param first (non-NULL) the first node on condition queue
         */
        void doSignalAll(Node* first) {
            lastWaiter = firstWaiter = NULL;
            do {
                Node* next = first->nextWaiter.get();
                first->nextWaiter.set(NULL);
                impl->transferForSignal(first);
                first = next;
            } while (first != NULL);
        }

        /**
         * Unlinks cancelled waiter nodes from condition queue.
         * Called only while holding lock. This is called when
         * cancellation occurred during condition wait, and upon
         * insertion of a new waiter when lastWaiter is seen to have
         * been cancelled. This method is needed to avoid garbage
         * retention in the absence of signals. So even though it may
         * require a full traversal, it comes into play only when
         * timeouts or cancellations occur in the absence of
         * signals. It traverses all nodes rather than stopping at a
         * particular target to unlink all pointers to garbage nodes
         * without requiring many re-traversals during cancellation
         * storms.
         */
        void unlinkCancelledWaiters() {
            Node* t = firstWaiter;
            Node* trail = NULL;
            while (t != NULL) {
                Node* next = t->nextWaiter.get();
                if (t->waitStatus.get() != Node::CONDITION) {
                    t->nextWaiter.set(NULL);

                    if (trail == NULL) {
                        firstWaiter = next;
                    } else {
                        trail->nextWaiter.set(next);
                    }

                    if (next == NULL) {
                        lastWaiter = trail;
                    }

                } else {
                    trail = t;
                }
                t = next;
            }
        }

        /**
         * Checks for interrupt, returning THROW_IE if interrupted
         * before signalled, REINTERRUPT if after signalled, or
         * 0 if not interrupted.
         */
        int checkInterruptWhileWaiting(Node* node) {
            return Thread::interrupted() ? (impl->transferAfterCancelledWait(node) ? THROW_IE : REINTERRUPT) : 0;
        }

        /**
         * Throws InterruptedException, reinterrupts current thread, or
         * does nothing, depending on mode.
         */
        void reportInterruptAfterWait(int interruptMode) {
            if (interruptMode == THROW_IE) {
                throw InterruptedException(__FILE__, __LINE__, "Thread was Interrupted");
            } else if (interruptMode == REINTERRUPT) {
                impl->selfInterrupt();
            }
        }

    };

    const int DefaultConditionObject::REINTERRUPT = 1;
    const int DefaultConditionObject::THROW_IE = -1;

}}}}

////////////////////////////////////////////////////////////////////////////////
AbstractQueuedSynchronizer::AbstractQueuedSynchronizer() : AbstractOwnableSynchronizer(), impl(NULL) {
    this->impl = new SynchronizerState(this);
}

////////////////////////////////////////////////////////////////////////////////
AbstractQueuedSynchronizer::~AbstractQueuedSynchronizer() {
    try{
        delete this->impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
int AbstractQueuedSynchronizer::getState() const {
    return this->impl->state.get();
}

////////////////////////////////////////////////////////////////////////////////
void AbstractQueuedSynchronizer::setState(int value) {
    this->impl->state.set(value);
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::compareAndSetState(int expect, int update) {
    return this->impl->state.compareAndSet(expect, update);
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::tryAcquire(int arg DECAF_UNUSED) {
    throw UnsupportedOperationException();
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::tryRelease(int arg DECAF_UNUSED) {
    throw UnsupportedOperationException();
}

////////////////////////////////////////////////////////////////////////////////
int AbstractQueuedSynchronizer::tryAcquireShared(int arg DECAF_UNUSED) {
    throw UnsupportedOperationException();
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::tryReleaseShared(int arg DECAF_UNUSED) {
    throw UnsupportedOperationException();
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::isHeldExclusively() const {
    throw UnsupportedOperationException();
}

////////////////////////////////////////////////////////////////////////////////
std::string AbstractQueuedSynchronizer::toString() const {
    int s = getState();
    std::string q = hasQueuedThreads() ? "non" : "";
    std::string prefix = "AbstractQueuedSynchronizer";
    return prefix + "[State = " + Integer::toString(s) + ", " + q + "empty queue]";
}

////////////////////////////////////////////////////////////////////////////////
void AbstractQueuedSynchronizer::acquire(int arg) {
    if (!tryAcquire(arg) && this->impl->acquireQueued(this->impl->addWaiter(Node::EXCLUSIVE), arg)) {
        this->impl->selfInterrupt();
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractQueuedSynchronizer::acquireInterruptibly(int arg) {

    if (Thread::interrupted()) {
        throw InterruptedException(__FILE__, __LINE__, "Thread interrupted before acquisition");
    }

    if (!tryAcquire(arg)) {
        this->impl->doAcquireInterruptibly(arg);
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::tryAcquireNanos(int arg, long long nanosTimeout) {

    if (Thread::interrupted()) {
        throw InterruptedException(__FILE__, __LINE__, "Thread interrupted before acquisition");
    }

    return tryAcquire(arg) || this->impl->doAcquireNanos(arg, nanosTimeout);
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::release(int arg) {

    if (tryRelease(arg)) {

        Node* h = this->impl->head.get();
        if (h != NULL && h->waitStatus.get() != 0) {
            this->impl->unparkSuccessor(h);
        }

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractQueuedSynchronizer::acquireShared(int arg) {
    if (tryAcquireShared(arg) < 0) {
        this->impl->doAcquireShared(arg);
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractQueuedSynchronizer::acquireSharedInterruptibly(int arg) {
    if (Thread::interrupted()) {
        throw InterruptedException(__FILE__, __LINE__, "Thread interrupted before acquisition");
    }

    if (tryAcquireShared(arg) < 0) {
        this->impl->doAcquireSharedInterruptibly(arg);
    }
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::tryAcquireSharedNanos(int arg, long long nanosTimeout) {
    if (Thread::interrupted()) {
        throw InterruptedException(__FILE__, __LINE__, "Thread interrupted before acquisition");
    }

    return tryAcquireShared(arg) >= 0 || this->impl->doAcquireSharedNanos(arg, nanosTimeout);
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::releaseShared(int arg) {

    if (tryReleaseShared(arg)) {
        this->impl->doReleaseShared();
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::hasQueuedThreads() const {
    return this->impl->head.get() != this->impl->tail.get();
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::hasContended() const {
    return this->impl->head.get() != NULL;
}

////////////////////////////////////////////////////////////////////////////////
Thread* AbstractQueuedSynchronizer::getFirstQueuedThread() const {
    // handle only fast path, else relay
    return (this->impl->head.get() == this->impl->tail.get()) ? NULL : this->impl->fullGetFirstQueuedThread();
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::isQueued(Thread* thread) const {

    if (thread == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Passed in thread was NULL");
    }

    for (Node* p = this->impl->tail.get(); p != NULL; p = p->prev.get()) {
        if (p->thread == thread) {
            return true;
        }
    }

    return false;
}

//    bool apparentlyFirstQueuedIsExclusive() {
//        Node h, s;
//        return (h = head) != NULL &&
//            (s = h.next)  != NULL &&
//            !s.isShared()         &&
//            s.thread != NULL;
//    }
//
//    bool hasQueuedPredecessors() {
//        // The correctness of this depends on head being initialized
//        // before tail and on head.next being accurate if the current
//        // thread is first in queue.
//        Node t = tail; // Read fields in reverse initialization order
//        Node h = head;
//        Node s;
//        return h != t &&
//            ((s = h.next) == NULL || s.thread != Thread.currentThread());
//    }

////////////////////////////////////////////////////////////////////////////////
int AbstractQueuedSynchronizer::getQueueLength() const {
    int n = 0;
    for (Node* p = this->impl->tail.get(); p != NULL; p = p->prev.get()) {
        if (p->thread != NULL) {
            ++n;
        }
    }
    return n;
}

////////////////////////////////////////////////////////////////////////////////
Collection<Thread*>* AbstractQueuedSynchronizer::getQueuedThreads() const {
    ArrayList<Thread*>* list = new ArrayList<Thread*>();
    for (Node* p = this->impl->tail.get(); p != NULL; p = p->prev.get()) {
        Thread* t = p->thread;
        if (t != NULL) {
            list->add(t);
        }
    }
    return list;
}

////////////////////////////////////////////////////////////////////////////////
Collection<Thread*>* AbstractQueuedSynchronizer::getExclusiveQueuedThreads() const {
    ArrayList<Thread*>* list = new ArrayList<Thread*>();
    for (Node* p = this->impl->tail.get(); p != NULL; p = p->prev.get()) {
        if (!p->isShared()) {
            Thread* t = p->thread;
            if (t != NULL) {
                list->add(t);
            }
        }
    }
    return list;
}

////////////////////////////////////////////////////////////////////////////////
Collection<Thread*>* AbstractQueuedSynchronizer::getSharedQueuedThreads() const {
    ArrayList<Thread*>* list = new ArrayList<Thread*>();
    for (Node* p = this->impl->tail.get(); p != NULL; p = p->prev.get()) {
        if (p->isShared()) {
            Thread* t = p->thread;
            if (t != NULL) {
                list->add(t);
            }
        }
    }
    return list;
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::owns(ConditionObject* condition) const {
    if (condition == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Condition Pointer arg was NULL");
    }
    return condition->isOwnedBy(this);
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::hasWaiters(ConditionObject* condition) const {
    if (!owns(condition)) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Not owner");
    }
    return condition->hasWaiters();
}

////////////////////////////////////////////////////////////////////////////////
int AbstractQueuedSynchronizer::getWaitQueueLength(ConditionObject* condition) const {
    if (!owns(condition)) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Not owner");
    }
    return condition->getWaitQueueLength();
}

////////////////////////////////////////////////////////////////////////////////
Collection<Thread*>* AbstractQueuedSynchronizer::getWaitingThreads(ConditionObject* condition) const {
    if (!owns(condition)) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Not owner");
    }
    return condition->getWaitingThreads();
}

////////////////////////////////////////////////////////////////////////////////
AbstractQueuedSynchronizer::ConditionObject* AbstractQueuedSynchronizer::createDefaultConditionObject() {
    return new DefaultConditionObject(this->impl);
}
