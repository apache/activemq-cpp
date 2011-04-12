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

#ifndef _DECAF_UTIL_CONCURRENT_BLOCKINGQUEUE_H_
#define _DECAF_UTIL_CONCURRENT_BLOCKINGQUEUE_H_

#include <decaf/util/Config.h>
#include <decaf/util/AbstractQueue.h>

#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/lang/exceptions/InterruptedException.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * A decaf::util::Queue that additionally supports operations that wait for
     * the queue to become non-empty when retrieving an element, and wait for space
     * to become available in the queue when storing an element.
     *
     * <tt>BlockingQueue</tt> methods come in four forms, with different ways of
     * handling operations that cannot be satisfied immediately, but may be satisfied
     * at some point in the future:
     * one throws an exception, the second returns a special value (either
     * <tt>true</tt> or <tt>false</tt>, depending on the operation), the third
     * blocks the current thread indefinitely until the operation can succeed,
     * and the fourth blocks for only a given maximum time limit before giving
     * up.  These methods are summarized in the following table:
     *
     * <table BORDER CELLPADDING=3 CELLSPACING=1>
     *  <tr>
     *    <td></td>
     *    <td ALIGN=CENTER><em>Throws exception</em></td>
     *    <td ALIGN=CENTER><em>Boolean Flag</em></td>
     *    <td ALIGN=CENTER><em>Blocks</em></td>
     *    <td ALIGN=CENTER><em>Times out</em></td>
     *  </tr>
     *  <tr>
     *    <td><b>Insert</b></td>
     *    <td>{@link #add add(e)}</td>
     *    <td>{@link #offer offer(e)}</td>
     *    <td>{@link #put put(e)}</td>
     *    <td>{@link #offer(Object, long, TimeUnit) offer(e, time, unit)}</td>
     *  </tr>
     *  <tr>
     *    <td><b>Remove</b></td>
     *    <td>{@link #remove remove()}</td>
     *    <td>{@link #poll poll()}</td>
     *    <td>{@link #take take()}</td>
     *    <td>{@link #poll(long, TimeUnit) poll(time, unit)}</td>
     *  </tr>
     *  <tr>
     *    <td><b>Examine</b></td>
     *    <td>{@link #element element()}</td>
     *    <td>{@link #peek peek()}</td>
     *    <td><em>not applicable</em></td>
     *    <td><em>not applicable</em></td>
     *  </tr>
     * </table>
     *
     * A <tt>BlockingQueue</tt> may be capacity bounded. At any given time it may have
     * a <tt>remainingCapacity</tt> beyond which no additional elements can be <tt>put</tt>
     * without blocking.  A <tt>BlockingQueue</tt> without any intrinsic capacity
     * constraints always reports a remaining capacity of <tt>Integer::MAX_VALUE</tt>.
     *
     * <tt>BlockingQueue</tt> implementations are designed to be used primarily for
     * producer-consumer queues, but additionally support {@link decaf::util::Collection}
     * interface.  So, for example, it is possible to remove an arbitrary element from a
     * queue using <tt>remove(x)</tt>. However, such operations are in general <em>not</em>
     * performed very efficiently, and are intended for only occasional use, such as
     * when a queued message is cancelled.
     *
     * <tt>BlockingQueue</tt> implementations are thread-safe.  All queuing methods
     * achieve their effects atomically using internal locks or other forms of concurrency
     * control. However, the <em>bulk</em> Collection operations <tt>addAll</tt>,
     * <tt>containsAll</tt>, <tt>retainAll</tt> and <tt>removeAll</tt> are <em>not</em>
     * necessarily performed atomically unless specified otherwise in an implementation.
     * So it is possible, for example, for <tt>addAll(c)</tt> to fail (throwing an exception)
     * after adding only some of the elements in <tt>c</tt>.
     *
     * A <tt>BlockingQueue</tt> does <em>not</em> intrinsically support any kind of
     * &quot;close&quot; or &quot;shutdown&quot; operation to indicate that no more
     * items will be added.  The needs and usage of such features tend to be
     * implementation-dependent. For example, a common tactic is for producers to
     * insert special <em>end-of-stream</em> or <em>poison</em> objects, that are
     * interpreted accordingly when taken by consumers.
     *
     * <p>
     * Usage example, based on a typical producer-consumer scenario.  Note that a
     * <tt>BlockingQueue</tt> can safely be used with multiple producers and multiple
     * consumers.
     * <pre>
     * class Producer : public Runnable {
     * private:
     *
     *     BlockingQueue* queue;
     *
     * public:
     *
     *     Producer( BlockingQueue* q ) : queue( q ) {}
     *
     *     virtual void run() {
     *         try {
     *             while( true ) { queue->put( produce() ); }
     *         } catch( InterruptedException& ex ) { ... handle ...}
     *     }
     *
     *     Object produce() { ... }
     * }
     *
     * class Consumer : public Runnable {
     * private:
     *
     *     BlockingQueue* queue;
     *
     * public:
     *
     *     Consumer( BlockingQueue* q ) : queue( q ) {}
     *
     *     virtual void run() {
     *         try {
     *             while( true ) { consume( queue->take() ); }
     *         } catch( InterruptedException& ex ) { ... handle ...}
     *     }
     *
     *     void consume( Object& x ) { ... }
     * }
     *
     * int main( int argc, char** argv ) {
     *
     *     BlockingQueue q = new SomeQueueImplementation();
     *     Producer p( &q );
     *     Consumer c1( &q );
     *     Consumer c2( &q );
     *     Thread t1( &p ).start();
     *     Thread t2( &c1 ).start();
     *     Thread t3( &c2 ).start();
     * }
     * </pre>
     *
     * <p>Memory consistency effects: As with other concurrent collections, actions in a
     * thread prior to placing an object into a BlockingQueue <em>happen-before</em>
     * actions subsequent to the access or removal of that element from the
     * BlockingQueue in another thread.
     *
     * @since 1.0
     */
    template< typename E >
    class BlockingQueue : public AbstractQueue<E> {
    public:

        virtual ~BlockingQueue() {
        }

        using Queue<E>::offer;
        using Queue<E>::poll;

        /**
         * Inserts the specified element into this queue, waiting if necessary for space
         * to become available.
         *
         * @param value the element to add
         * @throws InterruptedException if interrupted while waiting
         * @throws NullPointerException if the specified element is null
         * @throws IllegalArgumentException if some property of the specified
         *         element prevents it from being added to this queue
         */
        virtual void put( const E& value ) = 0;

        /**
         * Inserts the specified element into this queue, waiting up to the specified wait
         * time if necessary for space to become available.
         *
         * @param e the element to add
         * @param timeout how long to wait before giving up, in units of
         *        <tt>unit</tt>
         * @param unit a <tt>TimeUnit</tt> determining how to interpret the
         *        <tt>timeout</tt> parameter
         *
         * @return <tt>true</tt> if successful, or <tt>false</tt> if
         *         the specified waiting time elapses before space is available
         *
         * @throws InterruptedException if interrupted while waiting
         * @throws NullPointerException if the specified element is null
         * @throws IllegalArgumentException if some property of the specified
         *         element prevents it from being added to this queue
         */
        virtual bool offer( const E& e, long long timeout, const TimeUnit& unit ) = 0;

        /**
         * Retrieves and removes the head of this queue, waiting if necessary until an
         * element becomes available.
         *
         * @return the head of this queue
         * @throws InterruptedException if interrupted while waiting
         */
        virtual E take() = 0;

        /**
         * Retrieves and removes the head of this queue, waiting up to the specified
         * wait time if necessary for an element to become available.
         *
         * @param result the referenced value that will be assigned the value
         *        retrieved from the Queue.  Undefined if this methods returned false.
         * @param timeout how long to wait before giving up, in units of <tt>unit</tt>
         * @param unit a <tt>TimeUnit</tt> determining how to interpret the
         *        <tt>timeout</tt> parameter.
         * @return <tt>true</tt> if successful or <tt>false</tt> if the specified
         *         waiting time elapses before an element is available.
         * @throws InterruptedException if interrupted while waiting
         */
        virtual bool poll( E& result, long long timeout, const TimeUnit& unit ) = 0;

        /**
         * Returns the number of additional elements that this queue can ideally
         * (in the absence of memory or resource constraints) accept without
         * blocking, or <tt>Integer::MAX_VALUE</tt> if there is no intrinsic
         * limit.
         *
         * <p>Note that you <em>cannot</em> always tell if an attempt to insert
         * an element will succeed by inspecting <tt>remainingCapacity</tt>
         * because it may be the case that another thread is about to
         * insert or remove an element.
         *
         * @return the remaining capacity
         */
        virtual int remainingCapacity() const = 0;

        /**
         * Removes all available elements from this queue and adds them to the given
         * collection.  This operation may be more efficient than repeatedly polling
         * this queue.  A failure encountered while attempting to add elements to
         * collection <tt>c</tt> may result in elements being in neither, either or
         * both collections when the associated exception is thrown.  Attempts to
         * drain a queue to itself result in <tt>IllegalArgumentException</tt>.
         * Further, the behavior of this operation is undefined if the specified
         * collection is modified while the operation is in progress.
         *
         * @param c the collection to transfer elements into
         * @return the number of elements transferred
         * @throws UnsupportedOperationException if addition of elements
         *         is not supported by the specified collection
         * @throws IllegalArgumentException if the specified collection is this
         *         queue, or some property of an element of this queue prevents
         *         it from being added to the specified collection
         */
        virtual int drainTo( Collection<E>& c ) = 0;

        /**
         * Removes at most the given number of available elements from
         * this queue and adds them to the given collection.  A failure
         * encountered while attempting to add elements to
         * collection <tt>c</tt> may result in elements being in neither,
         * either or both collections when the associated exception is
         * thrown.  Attempts to drain a queue to itself result in
         * <tt>IllegalArgumentException</tt>. Further, the behavior of
         * this operation is undefined if the specified collection is
         * modified while the operation is in progress.
         *
         * @param c the collection to transfer elements into
         * @param maxElements the maximum number of elements to transfer
         * @return the number of elements transferred
         * @throws UnsupportedOperationException if addition of elements
         *         is not supported by the specified collection
         * @throws IllegalArgumentException if the specified collection is this
         *         queue, or some property of an element of this queue prevents
         *         it from being added to the specified collection
         */
        virtual int drainTo( Collection<E>& c, int maxElements ) = 0;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_BLOCKINGQUEUE_H_ */
