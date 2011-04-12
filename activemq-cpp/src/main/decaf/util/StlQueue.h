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
#ifndef _DECAF_UTIL_STLQUEUE_H_
#define _DECAF_UTIL_STLQUEUE_H_

#include <list>
#include <vector>
#include <decaf/util/Iterator.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/Exception.h>

namespace decaf{
namespace util{

    /**
     * The Queue class accepts messages with an psuh(m) command
     * where m is the message to be queued.  It destructively
     * returns the message with pop().  pop() returns messages in
     * the order they were enqueued.
     *
     * Queue is implemented with an instance of the STL queue object.
     * The interface is essentially the same as that of the STL queue
     * except that the pop method actually reaturns a reference to the
     * element popped.  This frees the app from having to call the
     * <code>front</code> method before calling pop.
     *
     *  Queue<string> sq;     // make a queue to hold string messages
     *  sq.push(s);           // enqueues a message m
     *  string s = sq.pop();  // dequeues a message
     *
     * = DESIGN CONSIDERATIONS
     *
     * The Queue class inherits from the Synchronizable interface and
     * provides methods for locking and unlocking this queue as well as
     * waiting on this queue.  In a multi-threaded app this can allow
     * for multiple threads to be reading from and writing to the same
     * Queue.
     *
     * Clients should consider that in a multiple threaded app it is
     * possible that items could be placed on the queue faster than
     * you are taking them off, so protection should be placed in your
     * polling loop to ensure that you don't get stuck there.
     */

    template <typename T>
    class StlQueue : public concurrent::Synchronizable {
    private:

        // The real queue
        std::list<T> queue;

        // Object used for sync
        mutable util::concurrent::Mutex mutex;

    private:

        class QueueIterator : public Iterator<T> {
        private:

            typename std::list<T>::iterator current;
            typename std::list<T>::iterator previous;
            typename std::list<T>* queue;

        public:

            QueueIterator( typename std::list<T>* queue ) :
                current( queue->begin() ), previous( queue->end() ), queue( queue ) {
            }

            virtual ~QueueIterator() {}

            virtual T next() {
                if( this->current == queue->end() ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__,
                        "Queue::Iterator::next - No more elements to return" );
                }

                this->previous = this->current;
                return *( this->current++ );
            }

            virtual bool hasNext() const {
                return ( this->current != queue->end() );
            }

            virtual void remove() {
                if( this->previous == queue->end() ) {
                    throw lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "Queue::Iterator::remove - Invalid State to call remove" );
                }

                this->queue->erase( this->previous );
                this->previous = this->queue->end();
            }
        };

    public:

        StlQueue() : queue(), mutex() {}

        virtual ~StlQueue() {}

        /**
         * Gets an Iterator over this Queue.
         * @return new iterator pointer that is owned by the caller.
         */
        Iterator<T>* iterator() {
            return new QueueIterator( &queue );
        }

        /**
         * Empties this queue.
         */
        void clear() {
            queue.clear();
        }

        /**
         * Returns a Reference to the element at the head of the queue
         * @return reference to a queue type object or (safe)
         */
        T& front() {
            if( queue.empty() ) {
                return getSafeValue();
            }

            return queue.front();
        }

        /**
         * Returns a Reference to the element at the head of the queue
         * @return reference to a queue type object or (safe)
         */
        const T& front() const {
            if( queue.empty() ) {
                return getSafeValue();
            }

            return queue.front();
        }

        /**
         * Returns a Reference to the element at the tail of the queue
         * @return reference to a queue type object or (safe)
         */
        T& back() {
            if( queue.empty() ) {
                return getSafeValue();
            }

            return queue.back();
        }

        /**
         * Returns a Reference to the element at the tail of the queue
         * @return reference to a queue type object or (safe)
         */
        const T& back() const {
            if( queue.empty() ) {
                return getSafeValue();
            }

            return queue.back();
        }

        /**
         * Places a new Object at the Tail of the queue
         * @param t - Queue Object Type reference.
         */
        void push( const T &t ) {
            queue.push_back( t );
        }

        /**
         * Places a new Object at the front of the queue
         * @param t - Queue Object Type reference.
         */
        void enqueueFront( const T &t ) {
            queue.push_front( t );
        }

        /**
         * Removes and returns the element that is at the Head of the queue
         * @return reference to a queue type object or (safe)
         */
        T pop() {
            if( queue.empty() ) {
                return getSafeValue();
            }

            // Pop the element into a temp, since we need to remain locked.
            // this means getting front and then popping.
            T temp = queue.front();
            queue.pop_front();

            return temp;
        }

        /**
         * Gets the Number of elements currently in the Queue
         * @return Queue Size
         */
        size_t size() const{
            return queue.size();
        }

        /**
         * Checks if this Queue is currently empty
         * @return boolean indicating queue emptiness
         */
        bool empty() const {
            return queue.empty();
        }

        /**
         * @return the all values in this queue as a std::vector.
         */
        virtual std::vector<T> toArray() const {
            std::vector<T> valueArray( queue.begin(), queue.end() );
            return valueArray;
        }

        /**
         * Reverses the order of the contents of this queue and stores them
         * in the target queue.
         * @param target - The target queue that will receive the contents of
         * this queue in reverse order.
         */
        void reverse( StlQueue<T>& target ) const {
            target.queue.insert( target.queue.end(), queue.rbegin(), queue.rend() );
        }

    public:  // Synchronizable

        virtual void lock() {
            mutex.lock();
        }

        virtual bool tryLock() {
            return mutex.tryLock();
        }

        virtual void unlock() {
            mutex.unlock();
        }

        virtual void wait() {
            mutex.wait();
        }

        virtual void wait( long long millisecs ) {
            mutex.wait( millisecs );
        }

        virtual void wait( long long millisecs, int nanos ) {
            mutex.wait( millisecs, nanos );
        }

        virtual void notify() {
            mutex.notify();
        }

        virtual void notifyAll() {
            mutex.notifyAll();
        }

    public:   // Statics

        /**
         * Fetch a reference to the safe value this object will return
         * when there is nothing to fetch from the queue.
         * @return Reference to this Queues safe object
         */
        T& getSafeValue() {
            static T safe;
            return safe;
        }

    };

}}

#endif /*_DECAF_UTIL_STLQUEUE_H_*/
