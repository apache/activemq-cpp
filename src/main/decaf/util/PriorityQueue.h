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

#ifndef _DECAF_UTIL_PRIORITYQUEUE_H_
#define _DECAF_UTIL_PRIORITYQUEUE_H_

#include <decaf/util/Config.h>
#include <decaf/util/AbstractQueue.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/Comparator.h>
#include <decaf/util/comparators/Less.h>

#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

namespace decaf {
namespace util {

    /**
     * An unbounded priority queue based on a binary heap algorithm. The elements of the priority queue
     * are ordered according to their natural ordering, or by a Comparator provided to one of the constructors
     * that accepts Comparators.  A priority queue relying on natural ordering also does not permit insertion
     * of non-comparable objects (doing so may result in a compiler error).
     *
     * The head of this queue is the least element with respect to the specified ordering. If multiple
     * elements are tied for least value, the head is one of those elements -- ties are broken arbitrarily.
     * The queue retrieval operations poll, remove, peek, and element access the element at the head of
     * the queue.
     *
     * A priority queue is unbounded, but has an internal capacity governing the size of an array used to store
     * the elements on the queue. It is always at least as large as the queue size. As elements are added to
     * a priority queue, its capacity grows automatically. The details of the growth policy are not specified.
     *
     * This class and its iterator implement all of the optional methods of the Collection and Iterator
     * interfaces.  The Iterator provided in method iterator() is not guaranteed to traverse the elements of
     * the priority queue in any particular order. If you need ordered traversal, consider using
     * <code>Arrays::sort( pq.toArray() )</code>.
     *
     * Note that this implementation is not synchronized. Multiple threads should not access a PriorityQueue
     * instance concurrently if any of the threads modifies the queue. Instead, use the thread-safe
     * PriorityBlockingQueue class.
     *
     * Implementation note: this implementation provides O(log(n)) time for the enqueing and dequeing methods
     * (offer, poll, remove() and add); linear time for the remove(Object) and contains(Object) methods;
     * and constant time for the retrieval methods (peek, element, and size).
     *
     * @since 1.0
     */
    template< typename E >
    class PriorityQueue : public AbstractQueue<E> {
    private:

        static const int DEFAULT_CAPACITY = 256;

        std::size_t _size;
        E* elements;
        decaf::lang::Pointer< Comparator<E> > comparator;

    private:

        class PriorityQueueIterator : public Iterator<E> {
        private:

            std::size_t position;
            PriorityQueue* queue;

        public:

            PriorityQueueIterator( PriorityQueue* queue ) : position( 0 ), queue( queue ) {}

            virtual E next() throw( lang::exceptions::NoSuchElementException ) {
                return E();
            }

            virtual bool hasNext() const {
                return false;
            }

            virtual void remove() throw ( lang::exceptions::IllegalStateException,
                                          lang::exceptions::UnsupportedOperationException ) {

            }
        };

        class ConstPriorityQueueIterator : public PriorityQueueIterator {
        public:

            ConstPriorityQueueIterator( const PriorityQueue* queue ) :
                PriorityQueueIterator( const_cast<PriorityQueue*>( queue ) ) {}

            virtual void remove() throw ( lang::exceptions::IllegalStateException,
                                          lang::exceptions::UnsupportedOperationException ) {

                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "PriorityQueue::Iterator::remove - Not Valid on a Const Iterator" );
            }
        };

    public:

        /**
         * Creates a Priority Queue with the default initial capacity.
         */
        PriorityQueue() : _size( 0 ){
            this->initQueue( DEFAULT_CAPACITY, NULL );
        }

        /**
         * Creates a Priority Queue with the capacity value supplied.
         *
         * @param initialCapacity
         *      The initial number of elements allocated to this PriorityQueue.
         */
        PriorityQueue( std::size_t initialCapacity ) : _size( 0 ) {
            this->initQueue( initialCapacity, new comparators::Less<E>() );
        }

        /**
         * Creates a Priority Queue with the default initial capacity.  This new PriorityQueue takes
         * ownership of the passed Comparator instance and uses that to determine the ordering of the
         * elements in the Queue.
         *
         * @param initialCapacity
         *      The initial number of elements allocated to this PriorityQueue.
         * @param comparator
         *      The Comparator instance to use in sorting the elements in the Queue.
         *
         * @throws NullPointerException if the passed Comparator is NULL.
         */
        PriorityQueue( std::size_t initialCapacity, Comparator<E>* comparator ) : _size( 0 ){

            if( comparator == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(
                    __FILE__, __LINE__, "Passed Comparator Cannot be NULL." );
            }

            this->initQueue( initialCapacity, comparator );
        }

        /**
         * Creates a PriorityQueue containing the elements in the specified Collection.
         *
         * @param source
         *      the Collection whose elements are to be placed into this priority queue
         */
        PriorityQueue( const Collection<E>& source ) : _size( 0 ) {
            // TODO
        }

        /**
         * Creates a PriorityQueue containing the elements in the specified priority queue. This priority
         * queue will be ordered according to the same ordering as the given priority queue.
         *
         * @param source
         *      the priority queue whose elements are to be placed into this priority queue
         */
        PriorityQueue( const PriorityQueue<E>& source ) : _size( 0 ) {
            // TODO
        }

        virtual ~PriorityQueue() {}

        virtual decaf::util::Iterator<E>* iterator() {
            return new PriorityQueueIterator( this );
        }

        virtual decaf::util::Iterator<E>* iterator() const {
            return new ConstPriorityQueueIterator( this );
        }

        virtual std::size_t size() const {
            return this->_size;
        }

        const E& getEmptyMarker() const {
            static const E marker = E();
            return marker;
        }

        bool offer( const E& value ) {
            return false;
        }

        E poll() {
            return this->getEmptyMarker();
        }

        const E& peek() const {
            return this->getEmptyMarker();
        }

    private:

        void initQueue( std::size_t initialSize, Comparator<E>* comparator ) {
            this->elements = new E[initialSize];
            this->comparator.reset( comparator );
        }

    };

}}

#endif /* _DECAF_UTIL_PRIORITYQUEUE_H_ */
