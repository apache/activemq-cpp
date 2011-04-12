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
#include <decaf/util/Collection.h>
#include <decaf/util/AbstractQueue.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/Comparator.h>
#include <decaf/util/comparators/Less.h>

#include <decaf/lang/Math.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

#include <memory>

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

        static const int DEFAULT_CAPACITY = 11;
        static const int DEFAULT_CAPACITY_RATIO = 2;

        int _size;
        int capacity;
        E* elements;
        decaf::lang::Pointer< Comparator<E> > _comparator;

    private:

        class PriorityQueueIterator : public Iterator<E> {
        private:

            PriorityQueueIterator( const PriorityQueueIterator& );
            PriorityQueueIterator& operator= ( const PriorityQueueIterator& );

        private:

            int position;
            bool allowRemove;
            PriorityQueue* queue;

        public:

            PriorityQueueIterator( PriorityQueue* queue ) : position( 0 ), allowRemove( false ), queue( queue ) {}

            virtual E next() {

                if( !hasNext() ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__,
                        "No more elements to Iterate over." );
                }

                allowRemove = true;
                return queue->elements[position++];
            }

            virtual bool hasNext() const {
                return position < queue->_size;
            }

            virtual void remove() {

                if( !allowRemove ) {
                    throw lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "No more elements to Iterate over." );
                }

                allowRemove = false;
                queue->removeAt( position-- );
            }
        };

        class ConstPriorityQueueIterator : public PriorityQueueIterator {
        private:

            ConstPriorityQueueIterator( const ConstPriorityQueueIterator& );
            ConstPriorityQueueIterator& operator= ( const ConstPriorityQueueIterator& );

        public:

            ConstPriorityQueueIterator( const PriorityQueue* queue ) :
                PriorityQueueIterator( const_cast<PriorityQueue*>( queue ) ) {}

            virtual void remove() {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "PriorityQueue::Iterator::remove - Not Valid on a Const Iterator" );
            }
        };

        friend class PriorityQueueIterator;

    public:

        /**
         * Creates a Priority Queue with the default initial capacity.
         */
        PriorityQueue() : AbstractQueue<E>(), _size( 0 ), capacity( 0 ), elements( NULL ), _comparator() {
            this->initQueue( DEFAULT_CAPACITY, new comparators::Less<E>() );
        }

        /**
         * Creates a Priority Queue with the capacity value supplied.
         *
         * @param initialCapacity
         *      The initial number of elements allocated to this PriorityQueue.
         */
        PriorityQueue( int initialCapacity ) :
            AbstractQueue<E>(), _size( 0 ), capacity( 0 ), elements( NULL ), _comparator() {

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
        PriorityQueue( int initialCapacity, Comparator<E>* comparator ) :
            AbstractQueue<E>(), _size( 0 ), capacity( 0 ), elements( NULL ), _comparator() {

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
        PriorityQueue( const Collection<E>& source ) :
            AbstractQueue<E>(), _size( 0 ), capacity( 0 ), elements( NULL ), _comparator() {

            this->getFromCollection( source );
        }

        /**
         * Creates a PriorityQueue containing the elements in the specified priority queue. This priority
         * queue will be ordered according to the same ordering as the given priority queue.
         *
         * @param source
         *      the priority queue whose elements are to be placed into this priority queue
         */
        PriorityQueue( const PriorityQueue<E>& source ) :
            AbstractQueue<E>(), _size( 0 ), capacity( 0 ), elements( NULL ), _comparator() {

            this->getFromPriorityQueue( source );
        }

        virtual ~PriorityQueue() {
            delete [] elements;
        }

        /**
         * Assignment operator, assign another Collection to this one.
         *
         * @param source
         *        The Collection to copy to this one.
         */
        PriorityQueue<E>& operator= ( const Collection<E>& source ) {
            this->getFromCollection( source );
        }

        /**
         * Assignment operator, assign another PriorityQueue to this one.
         *
         * @param source
         *        The PriorityQueue to copy to this one.
         */
        PriorityQueue<E>& operator= ( const PriorityQueue<E>& source ) {
            this->getFromPriorityQueue( source );
        }

    public:

        virtual decaf::util::Iterator<E>* iterator() {
            return new PriorityQueueIterator( this );
        }

        virtual decaf::util::Iterator<E>* iterator() const {
            return new ConstPriorityQueueIterator( this );
        }

        virtual int size() const {
            return this->_size;
        }

        virtual void clear() {

            // TODO - Provide a more efficient way to clear the array without reallocating it
            //        we should keep the size it grew to since if reused it could get that big
            //        again and reallocating all that memory could be to slow.

            delete [] this->elements;

            this->elements = new E[DEFAULT_CAPACITY];
            this->capacity = DEFAULT_CAPACITY;
            this->_size = 0;
        }

        virtual bool offer( const E& value ) {

            // TODO - Check for Null and throw exception

            increaseCapacity( _size + 1 );
            elements[_size++] = value;
            upHeap();
            return true;
        }

        virtual bool poll( E& result ) {

            if( this->isEmpty() ) {
                return false;
            }

            result = elements[0];
            removeAt( 0 );
            return true;
        }

        virtual bool peek( E& result ) const {

            if( this->isEmpty() ) {
                return false;
            }

            result = elements[0];
            return true;
        }

        virtual E remove() {

            if( !this->isEmpty() ) {
                E result = elements[0];
                removeAt( 0 );
                return result;
            }

            throw decaf::util::NoSuchElementException(
                __FILE__, __LINE__, "Unable to remove specified element from the Queue." );
        }

        virtual bool remove( const E& value ) {

            int targetIndex = 0;
            for( targetIndex = 0; targetIndex < _size; targetIndex++ ) {
                if( 0 == _comparator->compare( value, elements[targetIndex] ) ) {
                    break;
                }
            }

            if( _size == 0 || _size == targetIndex ) {
                return false;
            }

            removeAt( targetIndex );
            return true;
        }

        virtual bool add( const E& value ) {

            try {
                return offer( value );
            }
            DECAF_CATCH_RETHROW( lang::exceptions::UnsupportedOperationException )
            DECAF_CATCH_RETHROW( lang::exceptions::IllegalArgumentException )
            DECAF_CATCH_RETHROW( lang::exceptions::IllegalStateException )
            DECAF_CATCH_EXCEPTION_CONVERT( lang::exceptions::NullPointerException, lang::exceptions::UnsupportedOperationException )
            DECAF_CATCHALL_THROW( lang::exceptions::UnsupportedOperationException )
        }

        /**
         * obtains a Copy of the Pointer instance that this PriorityQueue is using to compare the
         * elements in the queue with.  The returned value is a copy, the caller cannot change the
         * value if the internal Pointer value.
         *
         * @return a copy of the Comparator Pointer being used by this Queue.
         */
        decaf::lang::Pointer< Comparator<E> > comparator() const {
            return this->_comparator;
        }

    private:

        void initQueue( int initialSize, Comparator<E>* comparator ) {
            this->elements = new E[initialSize];
            this->capacity = initialSize;
            this->_size = 0;
            this->_comparator.reset( comparator );
        }

        void upHeap() {

            int current = _size - 1;
            int parent = ( current - 1 ) / 2;

            while( current != 0 && _comparator->compare( elements[current], elements[parent] ) < 0 ) {

                // swap the two
                E tmp = elements[current];
                elements[current] = elements[parent];
                elements[parent] = tmp;

                // update parent and current positions.
                current = parent;
                parent = ( current - 1 ) / 2;
            }
        }

        void downHeap( int pos ) {

            int current = pos;
            int child = 2 * current + 1;

            while( child < _size && !this->isEmpty() ) {

                // compare the children if they exist
                if( child + 1 < _size && _comparator->compare( elements[child + 1], elements[child] ) < 0 ) {
                    child++;
                }

                // compare selected child with parent
                if( _comparator->compare( elements[current], elements[child] ) < 0 ) {
                    break;
                }

                // swap the two
                E tmp = elements[current];
                elements[current] = elements[child];
                elements[child] = tmp;

                // update child and current positions
                current = child;
                child = 2 * current + 1;
            }
        }

        void getFromPriorityQueue( const PriorityQueue<E>& c ) {
            initCapacity( c );
            _comparator = c.comparator();
            for( int ix = 0; ix < c.size(); ++ix ) {
                this->elements[ix] = c.elements[ix];
            }
            _size = c.size();
        }

        void getFromCollection( const Collection<E>& c ) {
            initCapacity( c );
            _comparator.reset( new comparators::Less<E>() );
            std::auto_ptr< Iterator<E> > iter( c.iterator() );
            while( iter->hasNext() ) {
                this->offer( iter->next() );
            }
        }

        void removeAt( int index ) {
            _size--;
            elements[index] = elements[_size];
            downHeap(index);
            elements[_size] = E();
        }

        void initCapacity( const Collection<E>& c ) {

            delete [] elements;
            _size = 0;

            if( c.isEmpty() ) {
                capacity = 1;
                elements = new E[capacity];
            } else {
                capacity = (int) lang::Math::ceil( (double)c.size() * 1.1 );
                elements = new E[capacity];
            }
        }

        void increaseCapacity( int size ) {

            if( size > capacity ) {
                E* newElements = new E[ size * DEFAULT_CAPACITY_RATIO ];

                for( int ix = 0; ix < capacity; ix++ ) {
                    newElements[ix] = elements[ix];
                }

                delete [] elements;

                elements = newElements;
                capacity = size * DEFAULT_CAPACITY_RATIO;
            }
        }

    };

}}

#endif /* _DECAF_UTIL_PRIORITYQUEUE_H_ */
