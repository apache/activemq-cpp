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

#ifndef _DECAF_UTIL_CONCURRENT_SYNCHRONOUSQUEUE_H_
#define _DECAF_UTIL_CONCURRENT_SYNCHRONOUSQUEUE_H_

#include <decaf/util/Config.h>

#include <decaf/util/concurrent/BlockingQueue.h>

#include <vector>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * A {@link BlockingQueue blocking queue} in which each insert operation
     * must wait for a corresponding remove operation by another thread, and
     * vice versa.  A synchronous queue does not have any internal capacity,
     * not even a capacity of one.  You cannot <tt>peek</tt> at a synchronous
     * queue because an element is only present when you try to remove it; you
     * cannot insert an element (using any method) unless another thread is trying
     * to remove it; you cannot iterate as there is nothing to iterate.  The
     * <em>head</em> of the queue is the element that the first queued inserting
     * thread is trying to add to the queue; if there is no such queued thread
     * then no element is available for removal and <tt>poll()</tt> will return
     * <tt>null</tt>.  For purposes of other <tt>Collection</tt> methods (for
     * example <tt>contains</tt>), a <tt>SynchronousQueue</tt> acts as an
     * empty collection.  This queue does not permit <tt>null</tt> elements.
     *
     * <p>Synchronous queues are similar to rendezvous channels used in
     * CSP and Ada. They are well suited for handoff designs, in which an
     * object running in one thread must sync up with an object running
     * in another thread in order to hand it some information, event, or
     * task.
     *
     * <p> This class supports an optional fairness policy for ordering
     * waiting producer and consumer threads.  By default, this ordering
     * is not guaranteed. However, a queue constructed with fairness set
     * to <tt>true</tt> grants threads access in FIFO order.
     *
     * <p>This class and its iterator implement all of the
     * <em>optional</em> methods of the {@link Collection} and {@link
     * Iterator} interfaces.
     *
     * @since 1.0
     */
    template< typename E >
    class SynchronousQueue : public BlockingQueue<E> {
    private:

        class EmptyIterator : public Iterator<E> {
        public:

            virtual E next() {

                throw NoSuchElementException(
                    __FILE__, __LINE__,
                    "Cannot traverse a Synchronous Queue." );
            }

            virtual bool hasNext() const {
                return false;
            }

            virtual void remove() {

                throw lang::exceptions::IllegalStateException(
                    __FILE__, __LINE__,
                    "No Elements to remove from a Synchronous Queue." );
            }
        };

    private:

        SynchronousQueue( const SynchronousQueue& );
        SynchronousQueue& operator= ( const SynchronousQueue& );

    public:

        SynchronousQueue() {}

        virtual ~SynchronousQueue() {}

        /**
         * Adds the specified element to this queue, waiting if necessary for
         * another thread to receive it.
         *
         * @param value the element to add to the Queue.
         *
         * @throws InterruptedException {@inheritDoc}
         * @throws NullPointerException {@inheritDoc}
         * @throws IllegalArgumentException {@inheritDoc}
         */
        virtual void put( const E& value ) {

            //if (o == null) throw new NullPointerException();
            //if (transferer.transfer(o, false, 0) == null) {
            //    Thread.interrupted();
            //    throw new InterruptedException();
            //}
        }

        /**
         * Inserts the specified element into this queue, waiting if necessary
         * up to the specified wait time for another thread to receive it.
         *
         * @return <tt>true</tt> if successful, or <tt>false</tt> if the
         *         specified waiting time elapses before a consumer appears.
         *
         * @throws InterruptedException {@inheritDoc}
         * @throws NullPointerException {@inheritDoc}
         * @throws IllegalArgumentException {@inheritDoc}
         */
        virtual bool offer( const E& e, long long timeout, const TimeUnit& unit ) {

            //if (o == null) throw new NullPointerException();
            //if (transferer.transfer(o, true, unit.toNanos(timeout)) != null)
            //    return true;
            //if (!Thread.interrupted())
            //    return false;
            //throw new InterruptedException();

            throw false;
        }

        /**
         * Inserts the specified element into this queue, if another thread is
         * waiting to receive it.
         *
         * @param value the element to add to the Queue
         *
         * @return <tt>true</tt> if the element was added to this queue, else
         *         <tt>false</tt>
         *
         * @throws NullPointerException if the Queue implementation does not allow Null values to
         *         be inserted into the Queue.
         * @throws IllegalArgumentException if some property of the specified
         *         element prevents it from being added to this queue
         */
        virtual bool offer( const E& value ) {

            //if (e == null) throw new NullPointerException();
            //return transferer.transfer(e, true, 0) != null;

            return false;
        }

        /**
         * Retrieves and removes the head of this queue, waiting if necessary
         * for another thread to insert it.
         *
         * @return the head of this queue
         * @throws InterruptedException {@inheritDoc}
         */
        virtual E take() {
            //Object e = transferer.transfer(null, false, 0);
            //if (e != null)
            //    return (E)e;
            //Thread.interrupted();
            //throw new InterruptedException();

            return E();
        }

        /**
         * Retrieves and removes the head of this queue, waiting
         * if necessary up to the specified wait time, for another thread
         * to insert it.
         *
         * @param result a reference to the value where the head of the Queue
         *               should be copied to.
         * @param timeout the time that the method should block if there is no
         *                element available to return.
         * @param unit the Time Units that the timeout value represents.
         *
         * @return true if the head of the Queue was copied to the result param
         *         or false if no value could be returned.
         */
        virtual bool poll( E& result, long long timeout, const TimeUnit& unit ) {

            //Object e = transferer.transfer(null, true, unit.toNanos(timeout));
            //if (e != null || !Thread.interrupted())
            //    return (E)e;
            //throw new InterruptedException();

            return false;
        }

        /**
         * Retrieves and removes the head of this queue, if another thread
         * is currently making an element available.
         *
         * @param result a reference to the value where the head of the Queue
         *               should be copied to.
         *
         * @return true if the head of the Queue was copied to the result param
         *         or false if no value could be returned.
         */
        virtual bool poll( E& result ) {
            return false; // (E)transferer.transfer(null, true, 0);
        }

        virtual bool equals( const Collection<E>& value ) const {
            if( (void*)&value == this ) {
                return true;
            }

            return false;
        }

        virtual decaf::util::Iterator<E>* iterator() {
            return new EmptyIterator();
        }

        virtual decaf::util::Iterator<E>* iterator() const {
            return new EmptyIterator();
        }

        virtual bool isEmpty() const {
            return true;
        }

        virtual int size() const {
            return 0;
        }

        virtual int remainingCapacity() const {
            return 0;
        }

        virtual void clear() {}

        virtual bool contains( const E& value DECAF_UNUSED ) const {
            return false;
        }

        virtual bool containsAll( const Collection<E>& collection ) const {
            return collection.isEmpty();
        }

        virtual bool remove( const E& value DECAF_UNUSED ) {
            return false;
        }

        virtual bool removeAll( const Collection<E>& collection DECAF_UNUSED ) {
            return false;
        }

        virtual bool retainAll( const Collection<E>& collection DECAF_UNUSED ) {
            return false;
        }

        virtual bool peek( E& result DECAF_UNUSED ) const {
            return false;
        }

        virtual std::vector<E> toArray() const { return std::vector<E>(); }

        virtual int drainTo( Collection<E>& c ) {

            if( (void*)&c == this ) {
                throw decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__,
                    "Cannot drain a Collection to Itself." );
            }

            int count = 0;
            E element;

            while( ( poll( element ) ) != false ) {
                c.add( element );
                ++count;
            }

            return count;
        }

        virtual int drainTo( Collection<E>& c, int maxElements ) {

            if( (void*)&c == this ) {
                throw decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__,
                    "Cannot drain a Collection to Itself." );
            }

            int count = 0;
            E element;

            while( count < maxElements && ( poll( element ) != false ) ) {
                c.add( element );
                ++count;
            }

            return count;
        }

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_SYNCHRONOUSQUEUE_H_ */
