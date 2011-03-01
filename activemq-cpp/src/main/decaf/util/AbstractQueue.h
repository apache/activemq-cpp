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

#ifndef ABSTRACTQUEUE_H_
#define ABSTRACTQUEUE_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/lang/Iterable.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/Queue.h>
#include <memory>

namespace decaf {
namespace util {

    /**
     * This class provides skeletal implementations of some Queue  operations.
     * Methods add, remove, and element are based on offer, poll, and peek, respectively.
     *
     * A Queue implementation that extends this class must minimally define a method Queue.
     * offer(E) which does not permit insertion of null elements, along with methods Queue.
     * peek(), Queue.poll(), Collection.size(), and a Collection.iterator() supporting
     * Iterator.remove(). Typically, additional methods will be overridden as well. If these
     * requirements cannot be met, consider instead subclassing AbstractCollection.
     *
     * @since 1.0
     */
    template< typename E >
    class AbstractQueue : public decaf::util::Queue<E> {
    public:

        AbstractQueue() : Queue<E>() {}

        virtual ~AbstractQueue() {}

        /**
         * Inserts the specified element into this queue if it is possible to do so
         * immediately without violating capacity restrictions, returning true upon
         * success and throwing an IllegalStateException  if no space is currently available.
         *
         * This implementation returns true if offer succeeds, else throws an
         * IllegalStateException.
         *
         * @param value - the element to offer to the Queue.
         *
         * @return true if the add succeeds.
         *
         * @throw IllegalArgumentException if the element cannot be added.
         */
        virtual bool add( const E& value )
            throw ( decaf::lang::exceptions::UnsupportedOperationException,
                    decaf::lang::exceptions::IllegalArgumentException,
                    decaf::lang::exceptions::IllegalStateException ) {

            if( offer( value ) ) {
                return true;
            }

            throw decaf::lang::exceptions::IllegalArgumentException(
                __FILE__, __LINE__, "Unable to add specified element to the Queue." );
        }

        /**
         * Adds all the elements of a collection to the queue. If the collection is
         * the queue itself, then an IllegalArgumentException will be thrown out. If
         * during the process, some runtime exception is thrown out, then part of
         * the elements in the collection that have successfully added will remain
         * in the queue.
         *
         * The result of the method is undefined if the collection is modified
         * during the process of the method.
         *
         * @param collection - the collection to be added to the queue.
         * @return true if the operation succeeds.
         *
         * @throws IllegalArgumentException If the collection to be added to the
         *         queue is the queue itself.
         */
        virtual bool addAll( const Collection<E>& collection )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException,
                    lang::exceptions::IllegalStateException ) {

            if( this == &collection ) {
                throw decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__, "A Queue cannot be added to itself." );
            }

            return Queue<E>::addAll( collection );
        }

        /**
         * Retrieves and removes the head of this queue. This method differs from poll
         * only in that it throws an exception if this queue is empty.
         *
         * This implementation returns the result of poll unless the queue is empty.
         *
         * @return a copy of the element in the head of the queue.
         *
         * @throws NoSuchElementException if the queue is empty.
         */
        virtual E remove() throw ( decaf::lang::exceptions::NoSuchElementException ) {

            E result;
            if( this->poll( result ) == true ) {
                return result;
            }

            throw decaf::lang::exceptions::NoSuchElementException(
                __FILE__, __LINE__, "Unable to remove specified element from the Queue." );
        }

        /**
         * Retrieves, but does not remove, the head of this queue. This method differs
         * from peek only in that it throws an exception if this queue is empty.
         *
         * This implementation returns the result of peek  unless the queue is empty.
         *
         * @return the element in the head of the queue.
         * @throws NoSuchElementException if the queue is empty.
         */
        virtual E element() const
            throw( decaf::lang::exceptions::NoSuchElementException ) {

            E result;
            if( this->peek( result ) == true ) {
                return result;
            }

            throw decaf::lang::exceptions::NoSuchElementException(
                __FILE__, __LINE__, "Unable to remove specified element from the Queue." );
        }

        /**
         * Removes all elements of the queue.
         *
         * This implementation repeatedly invokes poll until it returns the empty marker.
         */
        virtual void clear() throw ( lang::exceptions::UnsupportedOperationException ) {

            if( this->isEmpty() ) {
                return;
            }

            E result;
            bool successful = true;

            do {
                successful = this->poll( result );
            } while( successful );
        }

    };

}}

#endif /* ABSTRACTQUEUE_H_ */
