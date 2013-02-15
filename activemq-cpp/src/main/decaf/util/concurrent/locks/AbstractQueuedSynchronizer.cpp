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
#include <decaf/util/concurrent/atomic/AtomicReference.h>
#include <decaf/internal/util/concurrent/Atomics.h>
#include <decaf/internal/util/concurrent/PlatformThread.h>

#include <deque>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace decaf::util::concurrent::locks;
using namespace decaf::internal::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace {

    /**
     * Wait queue node class.
     *
     * The wait queue is a variant of a "CLH" (Craig, Landin, and Hagersten)
     * lock queue. CLH locks are normally used for spinlocks.  We instead use
     * them for blocking synchronizers, but use the same basic tactic of holding
     * some of the control information about a thread in the predecessor of its
     * node.  A "status" field in each node keeps track of whether a thread
     * should block.  A node is signalled when its predecessor releases.  Each
     * node of the queue otherwise serves as a specific-notification-style monitor
     * holding a single waiting thread. The status field does NOT control whether
     * threads are granted locks etc though.  A thread may try to acquire if it is
     * first in the queue. But being first does not guarantee success; it only gives
     * the right to contend.  So the currently released contender thread may need
     * to rewait.
     *
     * To enqueue into a CLH lock, you atomically splice it in as new tail.
     * To dequeue, you just set the head field.
     * <pre>
     *      +------+  prev +-----+       +-----+
     * head |      | <---- |     | <---- |     |  tail
     *      +------+       +-----+       +-----+
     * </pre>
     *
     * Insertion into a CLH queue requires only a single atomic operation on "tail", so
     * there is a simple atomic point of demarcation from unqueued to queued. Similarly,
     * dequeing involves only updating the "head". However, it takes a bit more work for
     * nodes to determine who their successors are, in part to deal with possible
     * cancellation due to timeouts and interrupts.
     *
     * The "prev" links (not used in original CLH locks), are mainly needed to handle
     * cancellation. If a node is cancelled, its successor is (normally) relinked to a
     * non-cancelled predecessor. For explanation of similar mechanics in the case of
     * spin locks, see the papers by Scott and Scherer at:
     *
     *     http://www.cs.rochester.edu/u/scott/synchronization/
     *
     * We also use "next" links to implement blocking mechanics. The thread id for
     * each node is kept in its own node, so a predecessor signals the next node to
     * wake up by traversing next link to determine which thread it is.  Determination
     * of successor must avoid races with newly queued nodes to set the "next" fields
     * of their predecessors.  This is solved when necessary by checking backwards
     * from the atomically updated "tail" when a node's successor appears to be null.
     * (Or, said differently, the next-links are an optimization so that we don't
     * usually need a backward scan.)
     *
     * Cancellation introduces some conservatism to the basic algorithms.  Since we
     * must poll for cancellation of other nodes, we can miss noticing whether a
     * cancelled node is ahead or behind us. This is dealt with by always unparking
     * successors upon cancellation, allowing them to stabilize on a new predecessor,
     * unless we can identify an uncancelled predecessor who will carry this
     * responsibility.
     *
     * CLH queues need a dummy header node to get started. But we don't create them on
     * construction, because it would be wasted effort if there is never contention.
     * Instead, the node is constructed and head and tail pointers are set upon first
     * contention.
     *
     * Threads waiting on Conditions use the same nodes, but use an additional link.
     * Conditions only need to link nodes in simple (non-concurrent) linked queues
     * because they are only accessed when exclusively held.  Upon await, a node is
     * inserted into a condition queue.  Upon signal, the node is transferred to the
     * main queue.  A special value of status field is used to mark which queue a node
     * is on.
     */
    class Node {
    public:

        /**
         * Status field, taking on only the values:
         *   SIGNAL:     The successor of this node is (or will soon be) blocked (via park),
         *               so the current node must unpark its successor when  it releases or
         *               cancels. To avoid races, acquire methods must first indicate they
         *               need a signal, then retry the atomic acquire, and then, on failure,
         *               block.
         *   CANCELLED:  This node is canceled due to timeout or interrupt.  Nodes never leave
         *               this state. In particular, a thread with canceled node never again
         *               blocks.
         *   CONDITION:  This node is currently on a condition queue. It will not be used as a
         *               sync queue node until transferred, at which time the status will be
         *               set to 0. (Use of this value here has nothing to do with the other
         *               uses of the field, but simplifies mechanics.)
         *   PROPAGATE:  A releaseShared should be propagated to other nodes. This is set
         *               (for head node only) in doReleaseShared to ensure propagation
         *               continues, even if other operations have since intervened.
         *   0:          None of the above
         *
         * The values are arranged numerically to simplify use. Non-negative values mean that
         * a node doesn't need to signal. So, most code doesn't need to check for particular
         * values, just for sign.
         *
         * The field is initialized to 0 for normal sync nodes, and CONDITION for condition nodes.
         * It is modified using CAS (or when possible, unconditional volatile writes).
         */
        enum WaitStatus {
            CANCELLED = 1,
            SIGNAL = -1,
            CONDITION = -2,
            PROPAGATE = -3
        };

        static Node SHARED;
        static Node* EXCLUSIVE;

    public:

        int waitStatus;

        /**
         * Link to predecessor node that current node/thread relies on
         * for checking waitStatus. Assigned during enqueing, and nulled
         * out only upon dequeuing.  Also, upon cancellation of a predecessor,
         * we short-circuit while finding a non-canceled one, which will always
         * exist because the head node is never canceled: A node becomes head
         * only as a result of successful acquire. A canceled thread never
         * succeeds in acquiring, and a thread only cancels itself, not any
         * other node.
         */
        Node* prev;

        /**
         * Link to the successor node that the current node/thread unparks upon
         * release. Assigned during enqueuing, adjusted when bypassing canceled
         * predecessors, and nulled out when dequeued.  The enq operation does not
         * assign next field of a predecessor until after attachment, so seeing a
         * NULL next field does not necessarily mean that node is at end of queue.
         * However, if a next field appears to be NULL, we can scan prev's from the
         * tail to double-check.
         */
        Node* next;

        /**
         * The thread that created this Node as is waiting to acquire the lock.
         */
        Thread* thread;

        /**
         * Link to next node waiting on condition, or the special value SHARED. Because
         * condition queues are accessed only when holding in exclusive mode, we just
         * need a simple linked queue to hold nodes while they are waiting on conditions.
         * They are then transferred to the queue to re-acquire. And because conditions
         * can only be exclusive, we save a field by using special value to indicate
         * shared mode.
         */
        Node* nextWaiter;

        /**
         * Linked nodes in the free pool using this special next trail to avoid
         * live nodes stepping into the pool and getting stuck.
         */
        Node* nextFree;

    private:

        Node(const Node&);
        Node& operator= (const Node&);

    public:

        Node() : waitStatus(0), prev(NULL), next(NULL), thread(NULL), nextWaiter(NULL), nextFree(NULL) {}
        Node(Thread* thread, Node* node) : waitStatus(0), prev(NULL), next(NULL), thread(thread), nextWaiter(node), nextFree(NULL) {}
        Node(Thread* thread, int waitStatus) : waitStatus(waitStatus), prev(NULL), next(NULL), thread(thread), nextWaiter(NULL), nextFree(NULL) {}
        Node(Thread* thread, int waitStatus, Node* node) : waitStatus(waitStatus), prev(NULL), next(NULL), thread(thread), nextWaiter(node), nextFree(NULL) {}

        ~Node() {}

        bool isShared() const {
            return this->nextWaiter == &SHARED;
        }

        Node* predecessor() {
            Node* p = prev;
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

    /**
     * When a thread no longer needs its Node in the AQS it is moved to the NodePool.
     *
     * The Pool will create Node's on demand or access them from its internal pool of
     * old Nodes.  The Pool can at times shrink the list of old nodes if it needs to.
     */
    class NodePool {
    private:

        Node head;
        Node* tail;
        int size;
        decaf_mutex_t lock;

    private:

        NodePool(const NodePool&);
        NodePool& operator= (const NodePool&);

    public:

        NodePool() : head(), tail(NULL), size(0), lock() {
            PlatformThread::createMutex(&lock);
        }

        ~NodePool() {

            PlatformThread::lockMutex(lock);

            while (head.nextFree != NULL) {
                Node* node = head.nextFree;
                head.nextFree = node->nextFree;
                delete node;
            }

            this->size = 0;

            PlatformThread::unlockMutex(lock);

            PlatformThread::destroyMutex(lock);
        }

        Node* takeNode() {
            return takeNode(NULL, 0, NULL);
        }

        Node* takeNode(Thread* thread, int waitStatus) {
            return takeNode(thread, waitStatus, NULL);
        }

        Node* takeNode(Thread* thread, Node* nextWaiter) {
            return takeNode(thread, 0, nextWaiter);
        }

        Node* takeNode(Thread* thread, int waitStatus, Node* nextWaiter) {
            return new Node(thread, waitStatus, nextWaiter);
        }

        void returnNode(Node* node) {

            if (node == NULL) {
                return;
            }

            PlatformThread::lockMutex(lock);

            Node* t = tail;
            size++;

            if (t != NULL) {
                t->nextFree = node;
                tail = node;
                tail->nextFree = NULL;
            } else {
                tail = node;
                head.nextFree = tail;
            }

            if (size == 1024) {
                Node* toDelete = head.nextFree;
                head.nextFree = toDelete->nextFree;
                delete toDelete;
                size--;
            }

            PlatformThread::unlockMutex(lock);
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    /**
     * The real implementation of AQS hidden here to allow code fixes without
     * altering ABI.
     */
    class SynchronizerState {
    public:

        /**
         * The object that owns this one, allows this object to call back into its parent
         * as well as checking for exclusive ownership of Conditions.
         */
        AbstractQueuedSynchronizer* parent;

        /**
         * The Sync state, subclasses can get / set this value to indicate when a Thread
         * can acquire the lock or must wait.
         */
        volatile int state;

        /**
         * Head of the wait queue, lazily initialized.  Except for initialization, it is
         * modified only via method setHead.  Note: If head exists, its waitStatus is
         * guaranteed not to be CANCELLED.
         */
        AtomicReference<Node> head;

        /**
         * Tail of the wait queue, lazily initialized.  Modified only via method
         * enqueue() to add new wait node.
         */
        AtomicReference<Node> tail;

        /**
         * Pool used to store discarded Nodes for reuse by another thread.
         */
        NodePool nodePool;

    private:

        SynchronizerState(const SynchronizerState&);
        SynchronizerState& operator=(const SynchronizerState&);

    public:

        SynchronizerState(AbstractQueuedSynchronizer* parent) :
            parent(parent), state(0), head(), tail(), nodePool() {
        }

        virtual ~SynchronizerState() {
            while (tail.get() != NULL) {
                nodePool.returnNode(tail.getAndSet(tail.get()->prev));
            }
        }

        bool isHeldExclusively() const {
            return this->parent->isHeldExclusively();
        }

        /**
         * Enqueue of a Node is Atomic with respect to the end of the list, if the list
         * is empty so no lock needs to occur.  If the head and tail were previously set
         * then we have to account for contention on the list by two or more enqueues.
         *
         * @param node
         *      The new node to add.
         *
         * @returns the predecessor of the newly inserted node.
         */
        Node* enqueue(Node* node) {

            for (;;) {
                Node* t = tail.get();
                if (t == NULL) { // Must initialize
                    if (compareAndSetHead(nodePool.takeNode())) {
                        tail.set(head.get());
                    }
                } else {
                    node->prev = t;
                    if (compareAndSetTail(t, node)) {
                        t->next = node;
                        return t;
                    }
                }
            }

            return NULL;
        }

        /**
         * Since we can append itself in one atomic step we don't lock here.  If we
         * can't get the fast append done we will enter into the longer looping
         * enqueue method.
         *
         * @param mode
         *      Node::EXCLUSIVE for exclusive, Node::SHARED for shared
         *
         * @return the newly added Node
         */
        Node* addWaiter(Node* mode) {
            Node* node = nodePool.takeNode(Thread::currentThread(), mode);

            // Try the fast add method first, then fall-back to the slower one.
            Node* pred = tail.get();
            if (pred != NULL) {
                node->prev = pred;
                if (compareAndSetTail(pred, node)) {
                    pred->next = node;
                    return node;
                }
            }

            enqueue(node);

            return node;
        }

        /**
         * The only place head is altered, we NULL out prev since that Node will be
         * Destroyed or pooled after this, but leave next alone since it should still
         * be valid.  The method calling this needs to be the lock holder which ensures
         * that no other thread can alter head.
         *
         * @param node
         *      The Node that is to become the new Head of the queue.
         */
        Node* setHead(Node* node) {
            Node* oldHead = head.get();

            head.set(node);
            node->thread = NULL;
            node->prev = NULL;

            return oldHead;
        }

        /**
         * Wakes up node's successor, if one exists.
         *
         * @param node
         *      the node whose successor will be unparked.
         */
        void unparkSuccessor(Node* node) {

            // If status is negative (i.e., possibly needing signal) try to clear
            // in anticipation of signaling.  It is OK if this fails or if status
            // is changed by waiting thread.
            int ws = node->waitStatus;
            if (ws < 0) {
                compareAndSetWaitStatus(node, ws, 0);
            }

            // Thread to un-park is held in successor, which is normally just the
            // next node.  But if canceled or apparently NULL, traverse backwards
            // from tail to find the actual non-canceled successor.
            Node* successor = node->next;
            if (successor == NULL || successor->waitStatus > 0) {
                successor = NULL;
                for (Node* t = tail.get(); t != NULL && t != node; t = t->prev) {
                    if (t->waitStatus <= 0) {
                        successor = t;
                    }
                }
            }

            if (successor != NULL) {
                LockSupport::unpark((Thread*)successor->thread);
            }
        }

        /**
         * Release action for shared mode -- signal successor and ensure
         * propagation. (Note: For exclusive mode, release just amounts
         * to calling unparkSuccessor of head if it needs signal.)
         */
        void doReleaseShared() {

            // Ensure that a release propagates, even if there are other in-progress
            // acquires/releases.  This proceeds in the usual way of trying to
            // unparkSuccessor of head if it needs signal. But if it does not,
            // status is set to PROPAGATE to ensure that upon release, propagation
            // continues. Additionally, we must loop in case a new node is added
            // while we are doing this. Also, unlike other uses of unparkSuccessor,
            // we need to know if CAS to reset status fails, if so rechecking.
            for (;;) {
                Node* h = head.get();
                if (h != NULL && h != tail.get()) {
                    int ws = h->waitStatus;
                    if (ws == Node::SIGNAL) {
                        if (!compareAndSetWaitStatus(h, Node::SIGNAL, 0)) {
                            continue;            // loop to recheck cases
                        }

                        unparkSuccessor(h);
                    } else if (ws == 0 && !compareAndSetWaitStatus(h, 0, Node::PROPAGATE)) {
                        continue;                // loop on failed CAS
                    }
                }
                if (h == head.get()) {           // loop if head changed
                    break;
                }
            }
        }

        /**
         * Sets head of queue, and checks if successor may be waiting in shared mode,
         * if so propagating if either propagate > 0 or PROPAGATE status was set.
         *
         * @param node
         *      The node that will become head
         * @param propagate
         *      The return value from a tryAcquireShared
         *
         * @return the Node that was the head.
         */
        Node* setHeadAndPropagate(Node* node, int propagate) {

            Node* head = setHead(node); // Record old head for check below

            // Try to signal next queued node if:
            //   Propagation was indicated by caller, or was recorded (as head->waitStatus)
            //   by a previous operation (note: this uses sign-check of waitStatus because
            //   PROPAGATE status may transition to SIGNAL.) and the next node is waiting
            //   in shared mode, or we don't know, because it appears NULL.
            //
            // The conservatism in both of these checks may cause unnecessary wake-ups,
            // but only when there are multiple racing acquires/releases, so most need
            // signals now or soon anyway.
            if (propagate > 0 || head == NULL || head->waitStatus < 0) {
                Node* successor = node->next;
                if (successor == NULL || successor->isShared()) {
                    doReleaseShared();
                }
            }

            return head;
        }

        /**
         * Cancels an ongoing attempt to acquire.  A canceled node can be returned to
         * the pool once its status has been updated and its links are updated.
         *
         * @param node
         *      The node that was attempting to acquire, will be returned to the pool.
         */
        void cancelAcquire(Node* node) {

            if (node == NULL) {
                return;
            }

            node->thread = NULL;

            // Skip canceled predecessors
            Node* pred = node->prev;
            while (pred->waitStatus > 0) {
                node->prev = pred = pred->prev;
            }

            // predNext is the apparent node to unsplice. CASes below will
            // fail if not, in which case, we lost race vs another cancel
            // or signal, so no further action is necessary.
            Node* predNext = pred->next;

            // Can use unconditional write instead of CAS here.
            // After this atomic step, other Nodes can skip past us.
            // Before, we are free of interference from other threads.
            node->waitStatus = Node::CANCELLED;

            // If we are the tail, remove ourselves.
            if (node == tail.get() && compareAndSetTail(node, pred)) {
                compareAndSetNext(pred, predNext, NULL);
            } else {
                // If successor needs signal, try to set pred's next-link
                // so it will get one. Otherwise wake it up to propagate.
                int ws;
                if (pred != head.get() &&
                    ((ws = pred->waitStatus) == Node::SIGNAL ||
                     (ws <= 0 && compareAndSetWaitStatus(pred, ws, Node::SIGNAL))) &&
                    pred->thread != NULL) {

                    Node* next = node->next;
                    if (next != NULL && next->waitStatus <= 0) {
                        compareAndSetNext(pred, predNext, next);
                    }
                } else {
                    unparkSuccessor(node);
                }
            }

            node->next = NULL;   // Help any lingering threads that land on this node
            nodePool.returnNode(node);
        }

        /**
         * Checks and updates status for a node that failed to acquire. Returns true
         * if thread should block. This is the main signal control in all acquire loops.
         * Requires that pred == node->prev
         *
         * @param pred
         *      The ode's predecessor holding status
         * @param node
         *      The node whose acquire attempt failed.
         *
         * @return true if thread should block.
         */
        bool shouldParkAfterFailedAcquire(Node* pred, Node* node) {

            int ws = pred->waitStatus;

            if (ws == Node::SIGNAL) {
                // This node has already set status asking a release to signal
                // it, so it can safely park.
                return true;
            }

            if (ws > 0) {
                 // Predecessor was canceled. Skip over predecessors and indicate retry.
                do {
                    node->prev = pred = pred->prev;
                } while (pred->waitStatus > 0);
                pred->next = node;
            } else {
                // waitStatus must be 0 or PROPAGATE.  Indicate that we need a
                // signal, but don't park yet.  Caller will need to retry to
                // make sure it cannot acquire before parking.
                compareAndSetWaitStatus(node->prev, ws, Node::SIGNAL);
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
         * @return true if interrupted.
         */
        bool parkAndCheckInterrupt() const {
            LockSupport::park();
            return Thread::interrupted();
        }

        /**
         * Acquires in exclusive uninterruptible mode for thread already in queue
         * Used by condition wait methods as well as acquire.  We can access the
         * Node's data here as it can't be altered on us because it is guaranteed
         * to exist until this method returns.
         *
         * @param node
         *      The node.
         * @param arg
         *      The value passed to acquire.
         *
         * @return true if interrupted while waiting
         */
        bool acquireQueued(Node* node, int arg) {
            try {
                bool interrupted = false;
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get() && parent->tryAcquire(arg)) {
                        setHead(node);
                        p->waitStatus = Node::CANCELLED;
                        p->next = NULL;
                        nodePool.returnNode(p);
                        return interrupted;
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && parkAndCheckInterrupt()) {
                        interrupted = true;
                    }
                }

            } catch (Exception& ex) {
                cancelAcquire(node);
                throw;
            }
        }

        /**
         * Acquires in exclusive interruptible mode.
         * @param arg the acquire argument
         */
        void doAcquireInterruptibly(int arg) {
            Node* node = addWaiter(Node::EXCLUSIVE);
            try {
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get() && parent->tryAcquire(arg)) {
                        setHead(node);
                        p->waitStatus = Node::CANCELLED;
                        p->next = NULL;
                        nodePool.returnNode(p);
                        return;
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && parkAndCheckInterrupt()) {
                        throw InterruptedException(__FILE__, __LINE__, "Interrupted while waiting for lock.");
                    }
                }

            } catch (Exception& ex) {
                cancelAcquire(node);
                throw InterruptedException(ex);
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
            try {
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get() && parent->tryAcquire(arg)) {
                        setHead(node);
                        p->waitStatus = Node::CANCELLED;
                        p->next = NULL;
                        nodePool.returnNode(p);
                        return true;
                    }

                    if (nanosTimeout <= 0) {
                        break;
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && nanosTimeout > spinForTimeoutLimit) {
                        LockSupport::parkNanos(nanosTimeout);
                    }

                    long long now = System::nanoTime();
                    nanosTimeout -= now - lastTime;
                    lastTime = now;

                    if (Thread::interrupted()) {
                        throw InterruptedException(__FILE__, __LINE__, "Interrupted while waiting for lock.");
                    }
                }

            } catch (Exception& ex) {
                cancelAcquire(node);
                throw;
            }

            cancelAcquire(node);
            return false;
        }

        /**
         * Acquires in shared uninterruptible mode.
         * @param arg the acquire argument
         */
        void doAcquireShared(int arg) {
            Node* node = addWaiter(&Node::SHARED);
            try {
                bool interrupted = false;
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get()) {
                        int r = parent->tryAcquireShared(arg);
                        if (r >= 0) {
                            setHeadAndPropagate(node, r);
                            p->waitStatus = Node::CANCELLED;
                            p->next = NULL;
                            nodePool.returnNode(p);

                            if (interrupted) {
                                selfInterrupt();
                            }
                            return;
                        }
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && parkAndCheckInterrupt()) {
                        interrupted = true;
                    }
                }
            } catch (Exception& ex) {
                cancelAcquire(node);
                throw;
            }

            cancelAcquire(node);
        }

        /**
         * Acquires in shared interruptible mode.
         * @param arg the acquire argument
         */
        void doAcquireSharedInterruptibly(int arg) {
            Node* node = addWaiter(&Node::SHARED);
            try {
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get()) {
                        int r = parent->tryAcquireShared(arg);
                        if (r >= 0) {
                            setHeadAndPropagate(node, r);
                            p->waitStatus = Node::CANCELLED;
                            p->next = NULL;
                            nodePool.returnNode(p);
                            return;
                        }
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && parkAndCheckInterrupt()) {
                        throw InterruptedException(__FILE__, __LINE__, "Interrupted while waiting for lock.");
                    }
                }
            } catch (Exception& ex) {
                cancelAcquire(node);
                throw;
            }
        }

        /**
         * Acquires in shared timed mode.
         *
         * @param arg
         *      The acquire argument (implementation specific).
         * @param nanosTimeout
         *      Max wait time for the Aquire in nanos
         *
         * @return true if acquired
         */
        bool doAcquireSharedNanos(int arg, long long nanosTimeout) {

            long long lastTime = System::nanoTime();
            Node* node = addWaiter(&Node::SHARED);
            try {
                for (;;) {
                    Node* p = node->predecessor();
                    if (p == head.get()) {
                        int r = parent->tryAcquireShared(arg);
                        if (r >= 0) {
                            setHeadAndPropagate(node, r);
                            p->waitStatus = Node::CANCELLED;
                            p->next = NULL;
                            nodePool.returnNode(p);
                            return true;
                        }
                    }
                    if (nanosTimeout <= 0) {
                        break;
                    }

                    if (shouldParkAfterFailedAcquire(p, node) && nanosTimeout > spinForTimeoutLimit) {
                        LockSupport::parkNanos(nanosTimeout);
                    }

                    long long now = System::nanoTime();
                    nanosTimeout -= now - lastTime;
                    lastTime = now;

                    if (Thread::interrupted()) {
                        throw InterruptedException(__FILE__, __LINE__, "Interrupted while waiting for lock.");
                    }
                }
            } catch (Exception& ex) {
                cancelAcquire(node);
                throw;
            }

            cancelAcquire(node);
            return false;
        }

        Thread* fullGetFirstQueuedThread() {

            // Head's next field might not have been set yet, or may have been
            // unset after setHead. So we must check to see if tail is actually
            // first node. If not, we continue on, safely traversing from tail
            // back to head to find first, guaranteeing termination.

            Node* t = tail.get();
            Thread* firstThread = NULL;
            while (t != NULL && t != head.get()) {
                Thread* tt = (Thread*)t->thread;
                if (tt != NULL) {
                    firstThread = tt;
                }
                t = t->prev;
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
            if (node->waitStatus == Node::CONDITION || node->prev == NULL) {
                return false;
            }

            if (node->next != NULL) { // If has successor, it must be on queue
                return true;
            }

            // node->prev can be non-NULL, but not yet on queue because the CAS
            // to place it on queue can fail. So we have to traverse from tail to
            // make sure it actually made it.  It will always be near the tail in
            // calls to this method, and unless the CAS failed (which is unlikely),
            // it will be there, so we hardly ever traverse much.
            return findNodeFromTail(node);
        }

        /**
         * Returns true if node is on sync queue by searching backwards from tail.
         * Called only when needed by isOnSyncQueue.
         *
         * @return true if present
         */
        bool findNodeFromTail(Node* node) {
            bool found = false;

            Node* t = tail.get();
            for (;;) {

                if (t == node) {
                    found = true;
                    break;
                }

                if (t == NULL) {
                    break;
                }

                t = t->prev;
            }

            return found;
        }

        /**
         * Transfers a node from a condition queue onto sync queue.
         * Returns true if successful.
         *
         * @param node
         *      The node to transfer to the wait Queue
         *
         * @return true if successfully transferred (else the node was
         *         canceled before signal and deleted).
         */
        bool transferForSignal(Node* node) {

            // If we cannot change waitStatus, the node has been canceled.
            if (!compareAndSetWaitStatus(node, Node::CONDITION, 0)) {
                return false;
            }

            // Get the Node onto the list, once there we need to update its
            // predecessor to indicate it should signal this Node once it is
            // woken up either by cancel or by acquiring the lock.
            enqueue(node);

            // Splice onto queue and try to set waitStatus of predecessor to
            // indicate that thread is (probably) waiting. If canceled or
            // attempt to set waitStatus fails, wake up to resync (in which
            // case the waitStatus can be transiently and harmlessly wrong).
            Node* p = node->prev;
            int ws = p->waitStatus;
            if (ws > 0 || !compareAndSetWaitStatus(p, ws, Node::SIGNAL)) {
                LockSupport::unpark((Thread*)node->thread);
            }

            return true;
        }

        /**
         * Transfers node, if necessary, to sync queue after a canceled wait. Returns
         * true if thread was canceled before being signaled.  If the Node is a Condition
         * and has been signaled already then its not added as the signal will have
         * already done so, we must wait though until it appears on the sync queue
         * otherwise the attempt to re-acquire the lock could throw a NullPointerException.
         *
         * @param node
         *      The node that is to be transferred onto the sync queue.
         *
         * @return true if canceled before the node was signaled
         */
        bool transferAfterCancelledWait(Node* node) {
            if (compareAndSetWaitStatus(node, Node::CONDITION, 0)) {
                enqueue(node);
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
         * Invokes release with current state value; returns saved state.  Cancels node
         * and throws exception on failure.  When a monitor state exception is thrown
         * the Node is added to the sync queue so that it can be deleted safely later.
         *
         * @param node
         *      The condition node for this wait.
         *
         * @return previous sync state
         */
        int fullyRelease(Node* node) {
            try {
                int savedState = parent->getState();
                if (parent->release(savedState)) {
                    return savedState;
                } else {
                    throw IllegalMonitorStateException();
                }
            } catch(IllegalMonitorStateException& ex) {
                node->waitStatus = Node::CANCELLED;
                throw;
            }
        }

        bool compareAndSetHead(Node* update) {
            return this->head.compareAndSet(NULL, update);
        }
        bool compareAndSetTail(Node* expect, Node* update) {
            return this->tail.compareAndSet(expect, update);
        }
        static bool compareAndSetWaitStatus(Node* node, int expect, int update) {
            return Atomics::compareAndSet32(&node->waitStatus, expect, update);
        }
        static bool compareAndSetNext(Node* node, Node* expect, Node* update) {
            return Atomics::compareAndSet((volatile void **)(&node->next), (void*)expect, (void*)update);
        }
    };

    /**
     * Provides a default implementation that most AbstractQueuedSynchronizer derived classes
     * can use without needing to write one from scratch.  Since the methods of this Object
     * must always be called from a locked synchronizer we can assume that only one thread
     * at a time will read or modify the list of waiters so we don't need to lock around
     * modifications to the list.
     *
     * This object creates Node instance but leaves the deletion of those objects up to the
     * sync queue.  In all cases the Node needs to be transfered to the sync queue with its
     * state value set to zero or CANCELLED.
     */
    class DefaultConditionObject : public AbstractQueuedSynchronizer::ConditionObject {
    private:

        SynchronizerState* impl;
        Node* head;
        Node* tail;

        enum INTERRUPTION_MODE {
            REINTERRUPT = 1,   // Re-interrupt thread on exit from a wait call.
            THROW_IE = -1      // Throw a new InterruptedException on wait call exit.
        };

    private:

        DefaultConditionObject(const DefaultConditionObject&);
        DefaultConditionObject& operator= (const DefaultConditionObject&);

    public:

        DefaultConditionObject(SynchronizerState* impl) :
            ConditionObject(), impl(impl), head(NULL), tail(NULL) {}
        virtual ~DefaultConditionObject() {
            try {
                unlinkCancelledWaiters();
            }
            DECAF_CATCHALL_NOTHROW()
        }

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
            if (node->nextWaiter != NULL) {
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

        virtual long long awaitNanos(long long nanosTimeout) {
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

                long long now = System::nanoTime();
                nanosTimeout -= now - lastTime;
                lastTime = now;
            }

            if (impl->acquireQueued(node, savedState) && interruptMode != THROW_IE) {
                interruptMode = REINTERRUPT;
            }
            if (node->nextWaiter != NULL) {
                unlinkCancelledWaiters();
            }
            if (interruptMode != 0) {
                reportInterruptAfterWait(interruptMode);
            }

            return nanosTimeout - (System::nanoTime() - lastTime);
        }

        virtual bool await(long long time, const TimeUnit& unit) {
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
            if (node->nextWaiter != NULL) {
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
            if (node->nextWaiter != NULL) {
                unlinkCancelledWaiters();
            }
            if (interruptMode != 0) {
                reportInterruptAfterWait(interruptMode);
            }

            return !timedout;
        }

        /**
         * The Node that's been waiting the longest is moved from this Conditions
         * wait queue to that of the parent Synchronizer.
         */
        virtual void signal() {
            if (!impl->isHeldExclusively()) {
                throw IllegalMonitorStateException();
            }
            Node* first = head;
            if (first != NULL) {
                doSignal(first);
            }
        }

        virtual void signalAll() {
            if (!impl->isHeldExclusively()) {
                throw IllegalMonitorStateException();
            }
            Node* first = head;
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

            for (Node* w = head; w != NULL; w = w->nextWaiter) {
                if (w->waitStatus == Node::CONDITION) {
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

            for (Node* w = head; w != NULL; w = w->nextWaiter) {
                if (w->waitStatus == Node::CONDITION) {
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

            for (Node* w = head; w != NULL; w = w->nextWaiter) {
                if (w->waitStatus == Node::CONDITION) {
                    Thread* t = (Thread*)w->thread;
                    if (t != NULL) {
                        list->add(t);
                    }
                }
            }

            return list;
        }

    private:

        /**
         * Adds a new Node to this Condition object to be used by one of the wait methods.
         * During this addition we scan for canceled Nodes and remove them as they are
         * now contained on the sync queue and will be removed from there.  Its safe to do
         * this here as this method is called while the sync queue is locked so no other
         * changes can occur to the list of Nodes.
         *
         * @returns the newly added Node instance.
         */
        Node* addConditionWaiter() {
            Node* t = tail;
            // If last Waiter is canceled, clean out.
            if (t != NULL && t->waitStatus != Node::CONDITION) {
                unlinkCancelledWaiters();
                t = tail;
            }
            Node* node = impl->nodePool.takeNode(Thread::currentThread(), Node::CONDITION);
            if (t == NULL) {
                head = node;
            } else {
                t->nextWaiter = node;
            }
            tail = node;
            return node;
        }

        /**
         * Removes and transfers nodes until hit non-canceled one or a NULL one.
         * or stated another way traverses the list of waiters removing canceled
         * nodes until it finds a non-canceled one to signal.
         *
         * @param first (non-NULL)
         *      The first node on condition queue.
         */
        void doSignal(Node* first) {
            do {
                head = first->nextWaiter;
                first->nextWaiter = NULL;

                if (head == NULL) {
                    tail = NULL;
                }
            } while (!impl->transferForSignal(first) && (first = head) != NULL);
        }

        /**
         * Removes and transfers all nodes.  Removes all Nodes from this Condition object
         * starting from the given node pointer, canceled waiter Nodes are moved to so we don't
         * need to track them anymore.  This is safe to do here since this method must be
         * called while the sync queue lock is held.
         *
         * @param first (non-NULL)
         *      The first node on condition queue to start transferring from.
         */
        void doSignalAll(Node* first) {
            head = tail = NULL;
            do {
                Node* next = first->nextWaiter;
                first->nextWaiter = NULL;
                impl->transferForSignal(first);
                first = next;
            } while (first != NULL);
        }

        /**
         * Unlinks canceled waiter nodes from condition queue.  Called only while holding
         * lock. This is called when cancellation occurred during condition wait, and upon
         * insertion of a new waiter when tail is seen to have been canceled. This method
         * is needed to avoid garbage retention in the absence of signals. So even though
         * it may require a full traversal, it comes into play only when timeouts or
         * cancellations occur in the absence of signals. It traverses all nodes rather
         * than stopping at a particular target to unlink all pointers to garbage nodes
         * without requiring many re-traversals during cancellation storms.
         */
        void unlinkCancelledWaiters() {

            Node* t = head;
            Node* trail = NULL;
            while (t != NULL) {
                Node* next = t->nextWaiter;
                if (t->waitStatus != Node::CONDITION) {
                    t->nextWaiter = NULL;

                    if (trail == NULL) {
                        head = next;
                    } else {
                        trail->nextWaiter = next;
                    }

                    if (next == NULL) {
                        tail = trail;
                    }

                    impl->nodePool.returnNode(t);

                } else {
                    trail = t;
                }
                t = next;
            }
        }

        /**
         * Checks for interrupt, returning THROW_IE if interrupted before signaled,
         * REINTERRUPT if after signaled, or 0 if not interrupted.  The canceled node
         * is transfered to the sync queue so that its waiter can safely re-acquire the
         * lock.
         *
         * @param node
         *      The node to transfer to the sync queue if interrupted.
         *
         * @returns value indicating if an action is needed in response to an interrupt.
         */
        int checkInterruptWhileWaiting(Node* node) {
            return Thread::interrupted() ? (impl->transferAfterCancelledWait(node) ? THROW_IE : REINTERRUPT) : 0;
        }

        /**
         * Throws InterruptedException, re-interrupts current thread, or does nothing, depending
         * on mode passed, abstract the logic needed for all the interruptible wait methods out
         * into a single place.
         *
         * @param interruptMode
         *      indicates what action is needed for interruption handling (if any).
         */
        void reportInterruptAfterWait(int interruptMode) {
            if (interruptMode == THROW_IE) {
                throw InterruptedException(__FILE__, __LINE__, "Thread was Interrupted");
            } else if (interruptMode == REINTERRUPT) {
                impl->selfInterrupt();
            }
        }

    };

}}}}

////////////////////////////////////////////////////////////////////////////////
AbstractQueuedSynchronizer::AbstractQueuedSynchronizer() : AbstractOwnableSynchronizer(), impl(NULL) {
    this->impl = new SynchronizerState(this);
}

////////////////////////////////////////////////////////////////////////////////
AbstractQueuedSynchronizer::~AbstractQueuedSynchronizer() {
    try {
        delete this->impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
int AbstractQueuedSynchronizer::getState() const {
    return this->impl->state;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractQueuedSynchronizer::setState(int value) {
    this->impl->state = value;
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::compareAndSetState(int expect, int update) {
    return Atomics::compareAndSet32(&this->impl->state, expect, update);
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
        if (h != NULL && h->waitStatus != 0) {
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

    for (Node* p = this->impl->tail.get(); p != NULL; p = p->prev) {
        if (p->thread == thread) {
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
int AbstractQueuedSynchronizer::getQueueLength() const {
    int n = 0;

    for (Node* p = this->impl->tail.get(); p != NULL; p = p->prev) {
        if (p->thread != NULL) {
            ++n;
        }
    }

    return n;
}

////////////////////////////////////////////////////////////////////////////////
Collection<Thread*>* AbstractQueuedSynchronizer::getQueuedThreads() const {
    ArrayList<Thread*>* list = new ArrayList<Thread*>();

    for (Node* p = this->impl->tail.get(); p != NULL; p = p->prev) {
        Thread* t = (Thread*)p->thread;
        if (t != NULL) {
            list->add(t);
        }
    }

    return list;
}

////////////////////////////////////////////////////////////////////////////////
Collection<Thread*>* AbstractQueuedSynchronizer::getExclusiveQueuedThreads() const {
    ArrayList<Thread*>* list = new ArrayList<Thread*>();

    for (Node* p = this->impl->tail.get(); p != NULL; p = p->prev) {
        if (!p->isShared()) {
            Thread* t = (Thread*)p->thread;
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

    for (Node* p = this->impl->tail.get(); p != NULL; p = p->prev) {
        if (p->isShared()) {
            Thread* t = (Thread*)p->thread;
            if (t != NULL) {
                list->add(t);
            }
        }
    }

    return list;
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::owns(const ConditionObject* condition) const {
    if (condition == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Condition Pointer arg was NULL");
    }
    return condition->isOwnedBy(this);
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::hasWaiters(const ConditionObject* condition) const {
    if (!owns(condition)) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Not owner");
    }
    return condition->hasWaiters();
}

////////////////////////////////////////////////////////////////////////////////
int AbstractQueuedSynchronizer::getWaitQueueLength(const ConditionObject* condition) const {
    if (!owns(condition)) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Not owner");
    }
    return condition->getWaitQueueLength();
}

////////////////////////////////////////////////////////////////////////////////
Collection<Thread*>* AbstractQueuedSynchronizer::getWaitingThreads(const ConditionObject* condition) const {
    if (!owns(condition)) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Not owner");
    }
    return condition->getWaitingThreads();
}

////////////////////////////////////////////////////////////////////////////////
AbstractQueuedSynchronizer::ConditionObject* AbstractQueuedSynchronizer::createDefaultConditionObject() {
    return new DefaultConditionObject(this->impl);
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractQueuedSynchronizer::hasQueuedPredecessors() const {

    bool result = false;

    // The correctness of this depends on head being initialized
    // before tail and on head->next being accurate if the current
    // thread is first in queue.
    Node* t = this->impl->tail.get(); // Read fields in reverse initialization order
    Node* h = this->impl->head.get();
    Node* s = NULL;

    result = h != t && ((s = h->next) == NULL || s->thread != Thread::currentThread());

    return result;
}
