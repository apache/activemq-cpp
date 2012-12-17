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

#ifndef _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERQUEUE_H_
#define _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERQUEUE_H_

#include <decaf/internal/util/concurrent/Transferer.h>

#include <decaf/util/concurrent/locks/LockSupport.h>
#include <decaf/util/concurrent/atomic/AtomicReference.h>
#include <decaf/util/concurrent/TimeoutException.h>
#include <decaf/lang/exceptions/InterruptedException.h>

#include <decaf/lang/Thread.h>

namespace decaf {
namespace internal {
namespace util {
namespace concurrent {

    using decaf::util::concurrent::atomic::AtomicReference;

    /**
     * This extends Scherer-Scott dual queue algorithm, differing,
     * among other ways, by using modes within nodes rather than
     * marked pointers. The algorithm is a little simpler than
     * that for stacks because fulfillers do not need explicit
     * nodes, and matching is done by CAS'ing QNode.item field
     * from non-null to null (for put) or vice versa (for take).
     */
    template< typename E >
    class TransferQueue : public Transferer<E> {
    private:

//        /** Node class for TransferQueue. */
//        class QNode {
//        private:
//
//            AtomicReference<QNode> next;           // next node in queue
//            volatile decaf::lang::Thread* waiter;  // to control park/unpark
//            AtomicReference<E*> item;              // CAS'ed to or from null
//
//            bool data;
//            bool cancelled;
//
//        public:
//
//            QNode() : cancelled( false ), data( false ) {
//            }
//
//            QNode( E* item ) : cancelled( false ), data( true ) {
//                this->item.set( item );
//            }
//
//            bool casNext( QNode* cmp, QNode* val ) {
//                return ( this->next == cmp && this->next.compareAndSet( cmp, val ) );
//            }
//
//            bool casItem( E* cmp, E* val ) {
//                return ( this->item == cmp && this->item.compareAndSet( cmp, val ) );
//            }
//
//            /**
//             * Tries to cancel by CAS'ing ref to NULL if that succeeds then we mark as cancelled.
//             */
//            void tryCancel( E* cmp ) {
//                if( item.compareAndSet( cmp, NULL ) ) {
//                    this->cancelled = true;
//                }
//            }
//
//            bool isCancelled() {
//                return this->item == this;
//            }
//
//            /**
//             * Returns true if this node is known to be off the queue
//             * because its next pointer has been forgotten due to
//             * an advanceHead operation.
//             */
//            bool isOffList() {
//                return this->next == NULL;
//            }
//        };

    private:

//        decaf::util::concurrent::atomic::AtomicReference<QNode> head;
//        decaf::util::concurrent::atomic::AtomicReference<QNode> tail;
//
//        decaf::util::concurrent::atomic::AtomicReference<QNode> cleanMe;

    public:

        TransferQueue() {
//            // Initialize with a dummy Node.
//            this->head.set( new QNode() );
//            this->tail.set( this->head.get() );
        }

        virtual ~TransferQueue() {}

        virtual void transfer( E* e, bool timed, long long nanos ) {

        }

        virtual E* transfer( bool timed, long long nanos ) {
            return NULL;
        }

        /**
         * Puts or takes an item.
         */
//        E* transfer( E* e, bool timed, long long nanos ) {

            /* Basic algorithm is to loop trying to take either of
             * two actions:
             *
             * 1. If queue apparently empty or holding same-mode nodes,
             *    try to add node to queue of waiters, wait to be
             *    fulfilled (or cancelled) and return matching item.
             *
             * 2. If queue apparently contains waiting items, and this
             *    call is of complementary mode, try to fulfill by CAS'ing
             *    item field of waiting node and dequeuing it, and then
             *    returning matching item.
             *
             * In each case, along the way, check for and try to help
             * advance head and tail on behalf of other stalled/slow
             * threads.
             *
             * The loop starts off with a null check guarding against
             * seeing uninitialized head or tail values. This never
             * happens in current SynchronousQueue, but could if
             * callers held non-volatile/final ref to the
             * transferer. The check is here anyway because it places
             * null checks at top of loop, which is usually faster
             * than having them implicitly interspersed.
             */

//            QNode s = null; // constructed/reused as needed
//            boolean isData = (e != null);
//
//            for (;;) {
//                QNode t = tail;
//                QNode h = head;
//                if (t == null || h == null)         // saw uninitialized value
//                    continue;                       // spin
//
//                if (h == t || t.isData == isData) { // empty or same-mode
//                    QNode tn = t.next;
//                    if (t != tail)                  // inconsistent read
//                        continue;
//                    if (tn != null) {               // lagging tail
//                        advanceTail(t, tn);
//                        continue;
//                    }
//                    if (timed && nanos <= 0)        // can't wait
//                        return null;
//                    if (s == null)
//                        s = new QNode(e, isData);
//                    if (!t.casNext(null, s))        // failed to link in
//                        continue;
//
//                    advanceTail(t, s);              // swing tail and wait
//                    Object x = awaitFulfill(s, e, timed, nanos);
//                    if (x == s) {                   // wait was cancelled
//                        clean(t, s);
//                        return null;
//                    }
//
//                    if (!s.isOffList()) {           // not already unlinked
//                        advanceHead(t, s);          // unlink if head
//                        if (x != null)              // and forget fields
//                            s.item = s;
//                        s.waiter = null;
//                    }
//                    return (x != null)? x : e;
//
//                } else {                            // complementary-mode
//                    QNode m = h.next;               // node to fulfill
//                    if (t != tail || m == null || h != head)
//                        continue;                   // inconsistent read
//
//                    Object x = m.item;
//                    if (isData == (x != null) ||    // m already fulfilled
//                        x == m ||                   // m cancelled
//                        !m.casItem(x, e)) {         // lost CAS
//                        advanceHead(h, m);          // dequeue and retry
//                        continue;
//                    }
//
//                    advanceHead(h, m);              // successfully fulfilled
//                    LockSupport.unpark(m.waiter);
//                    return (x != null)? x : e;
//                }
//            }
//        }

    private:

        /**
         * Spins/blocks until node s is fulfilled.
         *
         * @param s the waiting node
         * @param e the comparison value for checking match
         * @param timed true if timed wait
         * @param nanos timeout value
         * @return matched item, or s if cancelled
         */
//        Object awaitFulfill(QNode s, Object e, boolean timed, long nanos) {
//            /* Same idea as TransferStack.awaitFulfill */
//            long lastTime = (timed)? System.nanoTime() : 0;
//            Thread w = Thread.currentThread();
//            int spins = ((head.next == s) ?
//                         (timed? maxTimedSpins : maxUntimedSpins) : 0);
//            for (;;) {
//                if (w.isInterrupted())
//                    s.tryCancel(e);
//                Object x = s.item;
//                if (x != e)
//                    return x;
//                if (timed) {
//                    long now = System.nanoTime();
//                    nanos -= now - lastTime;
//                    lastTime = now;
//                    if (nanos <= 0) {
//                        s.tryCancel(e);
//                        continue;
//                    }
//                }
//                if (spins > 0)
//                    --spins;
//                else if (s.waiter == null)
//                    s.waiter = w;
//                else if (!timed)
//                    LockSupport.park();
//                else if (nanos > spinForTimeoutThreshold)
//                    LockSupport.parkNanos(nanos);
//            }
//        }

        /**
         * Gets rid of cancelled node s with original predecessor pred.
         */
//        void clean(QNode pred, QNode s) {
//            s.waiter = null; // forget thread
//            /*
//             * At any given time, exactly one node on list cannot be
//             * deleted -- the last inserted node. To accommodate this,
//             * if we cannot delete s, we save its predecessor as
//             * "cleanMe", deleting the previously saved version
//             * first. At least one of node s or the node previously
//             * saved can always be deleted, so this always terminates.
//             */
//            while (pred.next == s) { // Return early if already unlinked
//                QNode h = head;
//                QNode hn = h.next;   // Absorb cancelled first node as head
//                if (hn != null && hn.isCancelled()) {
//                    advanceHead(h, hn);
//                    continue;
//                }
//                QNode t = tail;      // Ensure consistent read for tail
//                if (t == h)
//                    return;
//                QNode tn = t.next;
//                if (t != tail)
//                    continue;
//                if (tn != null) {
//                    advanceTail(t, tn);
//                    continue;
//                }
//                if (s != t) {        // If not tail, try to unsplice
//                    QNode sn = s.next;
//                    if (sn == s || pred.casNext(s, sn))
//                        return;
//                }
//                QNode dp = cleanMe;
//                if (dp != null) {    // Try unlinking previous cancelled node
//                    QNode d = dp.next;
//                    QNode dn;
//                    if (d == null ||               // d is gone or
//                        d == dp ||                 // d is off list or
//                        !d.isCancelled() ||        // d not cancelled or
//                        (d != t &&                 // d not tail and
//                         (dn = d.next) != null &&  //   has successor
//                         dn != d &&                //   that is on list
//                         dp.casNext(d, dn)))       // d unspliced
//                        casCleanMe(dp, null);
//                    if (dp == pred)
//                        return;      // s is already saved node
//                } else if (casCleanMe(null, pred))
//                    return;          // Postpone cleaning s
//            }
//        }

//        /**
//         * Tries to cas nh as new head; if successful, unlink
//         * old head's next node to avoid garbage retention.
//         */
//        void advanceHead( QNode* h, QNode* nh ) {
//            if( h == head.get() && this->head.compareAndSet( h, nh ) ) {
//                h->next = h; // forget old next
//            }
//        }
//
//        /**
//         * Tries to cas nt as new tail.
//         */
//        void advanceTail( QNode* t, QNode* nt ) {
//            if( this->tail.get() == t ) {
//                this->tail.compareAndSet( t, nt );
//            }
//        }
//
//        /**
//         * Tries to CAS cleanMe slot.
//         */
//        bool casCleanMe( QNode* cmp, QNode* val ) {
//            return ( this->cleanMe.get() == cmp &&
//                     this->cleanMe.compareAndSet( cmp, val ) );
//        }

    };

}}}}

#endif /* _DECAF_INTERNAL_UTIL_CONCURRENT_TRANSFERQUEUE_H_ */
