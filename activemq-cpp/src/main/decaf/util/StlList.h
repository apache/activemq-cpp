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

#ifndef _DECAF_UTIL_STLLIST_H_
#define _DECAF_UTIL_STLLIST_H_

#include <list>
#include <algorithm>
#include <memory>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/util/Config.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/ListIterator.h>
#include <decaf/util/List.h>
#include <decaf/util/AbstractList.h>

namespace decaf{
namespace util{

    /**
     * List class that wraps the STL list object to provide a simpler interface and
     * additional methods not provided by the STL type.
     */
    template <typename E>
    class StlList : public decaf::util::AbstractList<E> {
    private:

        std::list<E> values;

    private:

        class StlListIterator : public ListIterator<E> {
        private:

            typename std::list<E>::iterator current;
            typename std::list<E>::iterator prev;
            typename std::list<E>* list;

        private:

            StlListIterator( const StlListIterator& );
            StlListIterator operator= ( const StlListIterator& );

        public:

            StlListIterator( typename std::list<E>* list, int index ) :
                current( list->begin() ), prev( list->end() ), list( list ) {

                if( index < (int)list->size() ) {
                    std::advance( this->current, index );
                } else {
                    this->current = list->end();
                }
            }

            virtual ~StlListIterator() {}

            virtual E next() {
                if( this->current == list->end() ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__,
                        "List::Iterator::next - No more elements to return" );
                }

                this->prev = this->current;
                return *( this->current++ );
            }

            virtual bool hasNext() const {
                return ( this->current != list->end() );
            }

            virtual void remove() {
                if( this->prev == list->end() ) {
                    throw lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "List::Iterator::remove - Invalid State to call remove" );
                }

                this->list->erase( this->prev );
                this->prev = this->list->end();
            }

            virtual void add( const E& e ) {
                this->list->insert( this->current, e );
            }

            virtual void set( const E& e ) {

                if( this->current == list->end() ) {
                    this->list->insert( this->current, e );
                } else {
                    *( this->current ) = e;
                }
            }

            virtual bool hasPrevious() const {
                return ( this->current != this->list->begin() );
            }

            virtual E previous() {
                if( this->current == this->list->begin() ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__,
                        "List::ListIterator::previous - No Previous element." );
                }

                typename std::list<E>::const_iterator iter = this->current;
                return *( iter-- );
            }

            virtual int nextIndex() const {
                if( this->current == this->list->end() ) {
                    return (int)this->list->size();
                }

                return (int)std::distance( this->list->begin(), this->current );
            }

            virtual int previousIndex() const {
                if( this->current == this->list->begin() ) {
                    return -1;
                }

                return (int)std::distance( this->list->begin(), this->current ) - 1;
            }

        };

        class ConstStlListIterator : public decaf::util::ListIterator<E> {
        private:

            typename std::list<E>::const_iterator current;
            typename std::list<E>::const_iterator prev;
            const typename std::list<E>* list;

        private:

            ConstStlListIterator( const ConstStlListIterator& );
            ConstStlListIterator operator= ( const ConstStlListIterator& );

        public:

            ConstStlListIterator( const typename std::list<E>* list, int index ) :
                ListIterator<E>(), current( list->begin() ), prev( list->end() ), list( list ) {

                if( index < (int)list->size() ) {
                    std::advance( this->current, index );
                } else {
                    this->current = list->end();
                }
            }

            virtual ~ConstStlListIterator() {}

            virtual E next() {
                if( this->current == list->end() ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__,
                        "List::Iterator::next - No more elements to return" );
                }

                this->prev = this->current;
                return *( this->current++ );
            }

            virtual bool hasNext() const {
                return ( this->current != list->end() );
            }

            virtual void remove() {

                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "List::ListIterator::remove - Const Iterator." );
            }

            virtual void add( const E& e DECAF_UNUSED ) {

                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "List::ListIterator::add - Const Iterator." );
            }

            virtual void set( const E& e DECAF_UNUSED ) {

                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "List::ListIterator::set - Const Iterator." );
            }

            virtual bool hasPrevious() const {
                return ( this->current != this->list->begin() );
            }

            virtual E previous() {
                if( this->current == this->list->begin() ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__,
                        "List::ListIterator::previous - No Previous element." );
                }

                typename std::list<E>::const_iterator iter = this->current;
                return *( iter-- );
            }

            virtual int nextIndex() const {
                if( this->current == this->list->end() ) {
                    return (int)this->list->size();
                }

                return (int)std::distance( this->list->begin(), this->current );
            }

            virtual int previousIndex() const {
                if( this->current == this->list->begin() ) {
                    return -1;
                }

                return (int)std::distance( this->list->begin(), this->current ) - 1;
            }
        };

    public:

        /**
         * Default constructor - does nothing.
         */
        StlList() : AbstractList<E>(), values() {}

        /**
         * Copy constructor - copies the content of the given set into this
         * one.
         * @param source The source set.
         */
        StlList( const StlList& source ) : AbstractList<E>(), values() {
            copy( source );
        }

        /**
         * Copy constructor - copies the content of the given set into this
         * one.
         * @param source The source set.
         */
        StlList( const Collection<E>& source ) : AbstractList<E>(), values() {
            AbstractList<E>::copy( source );
        }

        /**
         * {@inheritDoc}
         */
        virtual bool equals( const Collection<E>& collection ) const {

            const StlList<E>* listptr = dynamic_cast<const StlList<E>*>( &collection );
            if( listptr == NULL ) {
                return AbstractList<E>::equals( collection );
            }

            return this->values == listptr->values;
        }

        /**
         * {@inheritDoc}
         */
        virtual void copy( const Collection<E>& collection ) {

            const StlList<E>* listptr = dynamic_cast<const StlList<E>*>( &collection );
            if( listptr == NULL ) {
                AbstractList<E>::copy( collection );
                return;
            }

            this->values.clear();
            this->values = listptr->values;
        }

        /**
         * {@inheritDoc}
         */
        virtual Iterator<E>* iterator() {
            return new StlListIterator( &values, 0 );
        }
        virtual Iterator<E>* iterator() const {
            return new ConstStlListIterator( &values, 0 );
        }

        /**
         * {@inheritDoc}
         */
        virtual ListIterator<E>* listIterator() {
            return new StlListIterator( &values, 0 );
        }
        virtual ListIterator<E>* listIterator() const {
            return new ConstStlListIterator( &values, 0 );
        }

        /**
         * {@inheritDoc}
         */
        virtual ListIterator<E>* listIterator( int index ) {

            if( index < 0 || index > this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::listIterator - Index greater than size() or negative" );
            }

            return new StlListIterator( &values, index );
        }
        virtual ListIterator<E>* listIterator( int index ) const {

            if( index < 0 || index > this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::listIterator - Index greater than size() or negative" );
            }

            return new ConstStlListIterator( &values, index );
        }

        /**
         * {@inheritDoc}
         */
        virtual void clear() {
            values.clear();
        }

        /**
         * {@inheritDoc}
         */
        virtual bool isEmpty() const {
            return values.empty();
        }

        /**
         * {@inheritDoc}
         */
        virtual int size() const {
            return (int)values.size();
        }

        /**
         * {@inheritDoc}
         */
        virtual E get( int index ) const {

            if( index < 0 || index >= this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::get - Index greater than size() or negative" );
            }

            // Advance from begin and return the value at that location.
            typename std::list<E>::const_iterator iter = this->values.begin();
            std::advance( iter, index );
            return *( iter );
        }

        /**
         * {@inheritDoc}
         */
        virtual E set( int index, const E& element ) {

            if( index < 0 || index >= this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::get - Index greater than size() or negative" );
            }

            // Advance from begin and return the value at that location
            // after setting the value to the new value.
            typename std::list<E>::iterator iter = this->values.begin();
            std::advance( iter, index );
            E oldValue = *iter;
            *iter = element;

            return oldValue;
        }

        virtual void add( int index, const E& element ) {

            if( index < 0 || index > this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::add - Index greater than size()" );
            }

            // Advance from begin and insert the value at that location
            typename std::list<E>::iterator iter = this->values.begin();
            std::advance( iter, index );
            this->values.insert( iter, element );
        }

        virtual bool add( const E& value ) {
            values.insert( values.end(), value );
            return true;
        }

        virtual bool addAll( const Collection<E>& collection ) {

            if( collection.isEmpty() ) {
                return false;
            }

            std::vector<E> array = collection.toArray();
            typename std::vector<E>::const_iterator vecIter = array.begin();

            std::auto_ptr< ListIterator<E> > iter( this->listIterator( (int)this->values.size() ) );

            while( vecIter != array.end() ) {
                iter->add( *( vecIter++) );
            }

            return true;
        }

        /**
         * {@inheritDoc}
         */
        virtual bool addAll( int index, const Collection<E>& collection ) {

            if( index < 0 || index > this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::addAll - Index greater than size()" );
            }

            if( collection.isEmpty() ) {
                return false;
            }

            std::vector<E> array = collection.toArray();
            typename std::vector<E>::const_iterator vecIter = array.begin();

            std::auto_ptr< ListIterator<E> > iter( this->listIterator( index ) );

            while( vecIter != array.end() ) {
                iter->add( *( vecIter++) );
            }

            return true;
        }

        /**
         * {@inheritDoc}
         */
        virtual bool remove( const E& value ) {

            int origSize = this->size();
            this->values.remove( value );
            return origSize != this->size();
        }

        /**
         * {@inheritDoc}
         */
        virtual E removeAt( int index ) {

            if( index < 0 || index >= this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::removeAt - Index greater than size() or negative" );
            }

            // Advance from begin and insert the value at that location
            typename std::list<E>::iterator iter = this->values.begin();
            std::advance( iter, index );
            E oldValue = *iter;
            this->values.erase( iter );

            return oldValue;
        }

        virtual int indexOf( const E& value ) const {

            typename std::list<E>::const_iterator iter;
            iter = std::find( values.begin(), values.end(), value );

            if( iter == values.end() ) {
                return -1;
            }

            return (int)std::distance( values.begin(), iter );
        }

        virtual int lastIndexOf( const E& value ) const {

            typename std::list<E>::const_reverse_iterator iter;
            iter = std::find( values.rbegin(), values.rend(), value );

            if( iter == values.rend() ) {
                return -1;
            }

            // Now reverse the result to get the last index
            return (int)( this->size() - std::distance( values.rbegin(), iter ) - 1 );
        }

        virtual bool contains( const E& value ) const {
            typename std::list<E>::const_iterator iter;
            iter = std::find( values.begin(), values.end(), value );
            return iter != values.end();
        }

    };

}}

#endif /*_DECAF_UTIL_STLLIST_H_*/
