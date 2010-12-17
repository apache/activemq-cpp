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

#ifndef _DECAF_UTIL_ABSTRACTLIST_H_
#define _DECAF_UTIL_ABSTRACTLIST_H_

#include <decaf/util/Config.h>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/ConcurrentModificationException.h>
#include <decaf/lang/Iterable.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/List.h>
#include <decaf/util/AbstractCollection.h>
#include <memory>

namespace decaf {
namespace util {

    /**
     * This class provides a skeletal implementation of the List  interface to minimize
     * the effort required to implement this interface backed by a "random access" data
     * store (such as an array). For sequential access data (such as a linked list),
     * AbstractSequentialList should be used in preference to this class.
     *
     * To implement an unmodifiable list, the programmer needs only to extend this class
     * and provide implementations for the get(int) and size() methods.
     *
     * To implement a modifiable list, the programmer must additionally override the
     * set(int, E) method (which otherwise throws an UnsupportedOperationException). If
     * the list is variable-size the programmer must additionally override the add(int, E)
     * and remove(int) methods.
     *
     * The programmer should generally provide a void (no argument) and collection
     * constructor, as per the recommendation in the Collection interface specification.
     *
     * Unlike the other abstract collection implementations, the programmer does not have
     * to provide an iterator implementation; the iterator and list iterator are implemented
     * by this class, on top of the "random access" methods: get(int), set(int, E),
     * add(int, E) and remove(int).
     *
     * The documentation for each non-abstract method in this class describes its
     * implementation in detail. Each of these methods may be overridden if the collection
     * being implemented admits a more efficient implementation.
     *
     * @since 1.0
     */
    template< typename E >
    class AbstractList : public decaf::util::List<E>,
                         public decaf::util::AbstractCollection<E> {
    protected:

        int modCount;

    private:

        class SimpleListIterator : public ListIterator<E> {
        protected:

            AbstractList<E>* parent;
            int numLeft;
            int expectedModCount;
            int lastPosition;

        private:

            SimpleListIterator( const SimpleListIterator& );
            SimpleListIterator operator= ( const SimpleListIterator& );

        public:

            SimpleListIterator( AbstractList<E>* parent, int start ) :
                ListIterator<E>(), parent(NULL), numLeft(0), expectedModCount(0), lastPosition(-1) {

                if( parent == NULL ) {
                    throw decaf::lang::exceptions::NullPointerException(
                        __FILE__, __LINE__, "List Iterator constructed with NULL parent" );
                }

                if( start < 0 || start > parent->size() ) {
                    throw decaf::lang::exceptions::IndexOutOfBoundsException(
                         __FILE__, __LINE__, "start index passed was negative or greater than size()" );
                }

                this->numLeft = parent->size() - start;
                this->parent = parent;
                this->expectedModCount = parent->modCount;
            }

            virtual ~SimpleListIterator() {}

            virtual bool hasNext() const {
                return this->numLeft > 0;
            }

            virtual E next() {

                if( this->expectedModCount != this->parent->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "Concurrent Modification of Parent List detected." );
                }

                try {

                    int index = this->parent->size() - this->numLeft;
                    E result = this->parent->get( index );
                    this->lastPosition = index;
                    this->numLeft--;

                    return result;
                } catch( decaf::lang::exceptions::IndexOutOfBoundsException& e ) {
                    throw decaf::util::NoSuchElementException(
                        __FILE__, __LINE__, "Next called without a next element to process." );
                }
            }

            virtual void remove() {

                if( this->lastPosition == -1 ) {
                    throw decaf::lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__, "Remove called before next() was called." );
                }

                if( this->expectedModCount != this->parent->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "Concurrent Modification of Parent List detected." );
                }

                try {

                    if( this->lastPosition == this->parent->size() - this->numLeft ) {
                        this->numLeft--; // we're removing after a call to previous()
                    }

                    this->parent->removeAt( lastPosition );

                } catch( decaf::lang::exceptions::IndexOutOfBoundsException& e ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "Concurrent Modification detected." );
                }

                this->expectedModCount = this->parent->modCount;
                this->lastPosition = -1;
            }

            virtual void add( const E& value ) {

                if( this->expectedModCount != this->parent->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "Concurrent Modification of Parent List detected." );
                }

                try {
                    this->parent->add( this->parent->size() - this->numLeft, value );
                    this->expectedModCount = this->parent->modCount;
                    this->lastPosition = -1;
                } catch( decaf::lang::exceptions::IndexOutOfBoundsException& e ) {
                    throw decaf::util::NoSuchElementException(
                        __FILE__, __LINE__, "Add called without a next element to process." );
                }
            }

            virtual bool hasPrevious() const {
                return this->numLeft < this->parent->size();
            }

            virtual int nextIndex() const {
                return this->parent->size() - this->numLeft;
            }

            virtual E previous() {

                if( this->expectedModCount != this->parent->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "Concurrent Modification detected." );
                }

                try {

                    int index = this->parent->size() - this->numLeft - 1;
                    E result = this->parent->get( index );
                    this->numLeft++;
                    this->lastPosition = index;

                    return result;
                } catch( decaf::lang::exceptions::IndexOutOfBoundsException& e ) {
                    throw decaf::util::NoSuchElementException(
                        __FILE__, __LINE__, "No previous element exists." );
                }
            }

            virtual int previousIndex() const {
                return this->parent->size() - this->numLeft - 1;
            }

            virtual void set( const E& value ) {

                if( this->expectedModCount != this->parent->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "Concurrent Modification detected." );
                }

                try {
                    this->parent->set( this->lastPosition, value );
                } catch( decaf::lang::exceptions::IndexOutOfBoundsException& e ) {
                    throw decaf::lang::exceptions::IllegalStateException();
                }
            }
        };

        class ConstSimpleListIterator : public ListIterator<E> {
        protected:

            const AbstractList<E>* parent;
            int numLeft;
            int expectedModCount;
            int lastPosition;

        private:

            ConstSimpleListIterator( const ConstSimpleListIterator& );
            ConstSimpleListIterator operator= ( const ConstSimpleListIterator& );

        public:

            ConstSimpleListIterator( const AbstractList<E>* parent, int start ) :
                ListIterator<E>(), parent(parent), numLeft(0), expectedModCount(0), lastPosition(-1) {

                if( parent == NULL ) {
                    throw decaf::lang::exceptions::NullPointerException(
                        __FILE__, __LINE__, "List Iterator constructed with NULL parent" );
                }

                if( start < 0 || start > parent->size() ) {
                    throw decaf::lang::exceptions::IndexOutOfBoundsException(
                         __FILE__, __LINE__, "start index passed was negative or greater than size()" );
                }

                this->numLeft = parent->size() - start;
                this->parent = parent;
                this->expectedModCount = parent->modCount;
            }

            virtual ~ConstSimpleListIterator() {}

            virtual bool hasNext() const {
                return this->numLeft > 0;
            }

            virtual E next() {

                if( this->expectedModCount != this->parent->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "Concurrent Modification of Parent List detected." );
                }

                try {

                    int index = this->parent->size() - this->numLeft;
                    E result = this->parent->get( index );
                    this->lastPosition = index;
                    this->numLeft--;

                    return result;
                } catch( decaf::lang::exceptions::IndexOutOfBoundsException& e ) {
                    throw decaf::util::NoSuchElementException(
                        __FILE__, __LINE__, "Next called without a next element to process." );
                }
            }

            virtual void remove() {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "AbstractList::Iterator::remove - Const Iterator." );
            }

            virtual void add( const E& value DECAF_UNUSED ) {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "AbstractList::ListIterator::radd - Const Iterator." );
            }

            virtual bool hasPrevious() const {
                return this->numLeft < this->parent->size();
            }

            virtual int nextIndex() const {
                return this->parent->size() - this->numLeft;
            }

            virtual E previous() {

                if( this->expectedModCount != this->parent->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "Concurrent Modification detected." );
                }

                try {

                    int index = this->parent->size() - this->numLeft - 1;
                    E result = this->parent->get( index );
                    this->numLeft++;
                    this->lastPosition = index;

                    return result;
                } catch( decaf::lang::exceptions::IndexOutOfBoundsException& e ) {
                    throw decaf::util::NoSuchElementException(
                        __FILE__, __LINE__, "No previous element exists." );
                }
            }

            virtual int previousIndex() const {
                return this->parent->size() - this->numLeft - 1;
            }

            virtual void set( const E& value DECAF_UNUSED ) {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "AbstractList::ListIterator::set - Const Iterator." );
            }
        };

    public:

        AbstractList() : modCount( 0 ) {}

        virtual ~AbstractList() {}

        virtual Iterator<E>* iterator() {
            return new SimpleListIterator( this, 0 );
        }
        virtual Iterator<E>* iterator() const {
            return new ConstSimpleListIterator( this, 0 );
        }

        virtual ListIterator<E>* listIterator() {
            return new SimpleListIterator( this, 0 );
        }
        virtual ListIterator<E>* listIterator() const {
            return new ConstSimpleListIterator( this, 0 );
        }

        virtual ListIterator<E>* listIterator( int index ) {
            return new SimpleListIterator( this, index );
        }
        virtual ListIterator<E>* listIterator( int index ) const {
            return new ConstSimpleListIterator( this, index );
        }

        virtual void clear() {
            this->removeRange( 0, this->size() );
        }

        virtual bool add( const E& value ) {
            this->add( this->size(), value );
            return true;
        }

        virtual void add( int index DECAF_UNUSED, const E& element DECAF_UNUSED ) {
            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "Abstract list does not implement the add method." );
        }

        // Use this method since our own addAll will hide the base class version.
        using AbstractCollection<E>::addAll;

        virtual bool addAll( int index, const Collection<E>& source )  {
            std::auto_ptr< decaf::util::Iterator<E> > iter( source.iterator() );
            while( iter->hasNext() ) {
                this->add( index++, iter->next() );
            }

            return !source.isEmpty();
        }

        virtual E removeAt( int index DECAF_UNUSED )  {
            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "Abstract list does not implement the removeAt method." );
        }

        virtual E set( int index DECAF_UNUSED, const E& element DECAF_UNUSED ) {
            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "Abstract list does not implement the set method." );
        }

        virtual int indexOf( const E& value ) const {

            std::auto_ptr< decaf::util::ListIterator<E> > iter( this->listIterator() );

            while( iter->hasNext() ) {

                if( value == iter->next() ) {
                    return iter->previousIndex();
                }
            }

            return -1;
        }

        virtual int lastIndexOf( const E& value ) const {

            std::auto_ptr< decaf::util::ListIterator<E> > iter( this->listIterator( this->size() ) );

            while( iter->hasPrevious() ) {

                if( value == iter->previous() ) {
                    return iter->nextIndex();
                }
            }

            return -1;
        }

    protected:

        void removeRange( int start, int end ) {
            std::auto_ptr< decaf::util::Iterator<E> > iter( this->listIterator( start ) );
            for( int i = start; i < end; i++ ) {
                iter->next();
                iter->remove();
            }
        }

    };

}}

#endif /* _DECAF_UTIL_ABSTRACTLIST_H_ */
