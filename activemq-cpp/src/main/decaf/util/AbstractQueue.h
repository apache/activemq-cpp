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
    class AbstractQueue : public decaf::util::Queue<E>,
                          public decaf::util::AbstractCollection<E> {
    public:

        AbstractQueue() : Queue<E>() {}

        virtual ~AbstractQueue() {}

        /**
         * {@inheritDoc}
         *
         * This implementation returns true if offer succeeds, else throws an
         * IllegalStateException.
         */
        virtual bool add( const E& value ) {

            if( offer( value ) ) {
                return true;
            }

            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__, "Unable to add specified element to the Queue." );
        }

        /**
         * {@inheritDoc}
         *
         * This implementation checks to see if the Queue is being added to itself and
         * throws an IllegalArgumentException if so, otherwise it delegates the add to
         * the AbstractCollection's addAll implementation.
         */
        virtual bool addAll( const Collection<E>& collection ) {

            if( this == &collection ) {
                throw decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__, "A Queue cannot be added to itself." );
            }

            return AbstractCollection<E>::addAll( collection );
        }

        using AbstractCollection<E>::remove;

        /**
         * {@inheritDoc}
         *
         * This implementation returns the result of poll unless the queue is empty.
         */
        virtual E remove() {

            E result;
            if( this->poll( result ) == true ) {
                return result;
            }

            throw decaf::util::NoSuchElementException(
                __FILE__, __LINE__, "Unable to remove specified element from the Queue." );
        }

        /**
         * {@inheritDoc}
         *
         * This implementation returns the result of peek unless the queue is empty otherwise
         * it throws a NoSuchElementException.
         */
        virtual E element() const {

            E result;
            if( this->peek( result ) == true ) {
                return result;
            }

            throw decaf::util::NoSuchElementException(
                __FILE__, __LINE__, "Unable to remove specified element from the Queue." );
        }

        /**
         * {@inheritDoc}
         *
         * This implementation repeatedly invokes poll until it returns false.
         */
        virtual void clear() {

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
