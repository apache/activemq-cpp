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
#include <decaf/lang/exceptions/NoSuchElementException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Config.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/ListIterator.h>
#include <decaf/util/List.h>

namespace decaf{
namespace util{

    /**
     * List class that wraps the STL list object to provide a simpler interface and
     * additional methods not provided by the STL type.
     */
    template <typename E>
    class StlList : public decaf::util::List<E> {
    private:

        std::list<E> values;

    private:

        class StlListIterator : public ListIterator<E> {
        private:

            typename std::list<E>::iterator current;
            typename std::list<E>::iterator prev;
            typename std::list<E>* list;

        public:

            StlListIterator( typename std::list<E>* list, std::size_t index ) {
                this->current = list->begin();
                std::advance( this->current, index );
                this->prev = list->end();
                this->list = list;
            }
            virtual ~StlListIterator() {}

            virtual E next() throw( lang::exceptions::NoSuchElementException ){
                if( this->current == list->end() ) {
                    throw lang::exceptions::NoSuchElementException(
                        __FILE__, __LINE__,
                        "List::Iterator::next - No more elements to return" );
                }

                this->prev = this->current;
                return *( this->current++ );
            }

            virtual bool hasNext() const {
                return ( this->current != list->end() );
            }

            virtual void remove() throw ( lang::exceptions::IllegalStateException,
                                          lang::exceptions::UnsupportedOperationException ){
                if( this->prev == list->end() ) {
                    throw lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "List::Iterator::remove - Invalid State to call remove" );
                }

                this->list->erase( this->prev );
                this->prev = this->list->end();
            }

            virtual void add( const E& e DECAF_UNUSED )
                throw ( decaf::lang::exceptions::UnsupportedOperationException,
                        decaf::lang::exceptions::IllegalArgumentException ) {

                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "List::ListIterator::add - Not Implemented Yet." );
            }

            virtual void set( const E& e )
                throw ( decaf::lang::exceptions::UnsupportedOperationException,
                        decaf::lang::exceptions::IllegalArgumentException,
                        decaf::lang::exceptions::IllegalStateException )
            {
                if( this->current == list->end() ) {
                    throw lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "List::Iterator::set - Not a valid state for set" );
                }

                *( this->current ) = e;
            }

            virtual bool hasPrevious() const {
                return ( this->current != this->list->begin() );
            }

            virtual E previous() {
                if( this->current == this->list->begin() ) {
                    throw lang::exceptions::NoSuchElementException(
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

        public:

            ConstStlListIterator( const typename std::list<E>* list, std::size_t index ) {
                this->current = list->begin();
                std::advance( this->current, index );
                this->prev = list->end();
                this->list = list;
            }
            virtual ~ConstStlListIterator() {}

            virtual E next() throw( lang::exceptions::NoSuchElementException ){
                if( this->current == list->end() ) {
                    throw lang::exceptions::NoSuchElementException(
                        __FILE__, __LINE__,
                        "List::Iterator::next - No more elements to return" );
                }

                this->prev = this->current;
                return *( this->current++ );
            }

            virtual bool hasNext() const {
                return ( this->current != list->end() );
            }

            virtual void remove() throw ( lang::exceptions::IllegalStateException,
                                          lang::exceptions::UnsupportedOperationException ){

                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "List::ListIterator::remove - Const Iterator." );
            }

            virtual void add( const E& e DECAF_UNUSED )
                throw ( decaf::lang::exceptions::UnsupportedOperationException,
                        decaf::lang::exceptions::IllegalArgumentException ) {

                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "List::ListIterator::add - Const Iterator." );
            }

            virtual void set( const E& e DECAF_UNUSED )
                throw ( decaf::lang::exceptions::UnsupportedOperationException,
                        decaf::lang::exceptions::IllegalArgumentException,
                        decaf::lang::exceptions::IllegalStateException )
            {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "List::ListIterator::set - Const Iterator." );
            }

            virtual bool hasPrevious() const {
                return ( this->current != this->list->begin() );
            }

            virtual E previous() {
                if( this->current == this->list->begin() ) {
                    throw lang::exceptions::NoSuchElementException(
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
        StlList() {}

        /**
         * Copy constructor - copies the content of the given set into this
         * one.
         * @param source The source set.
         */
        StlList( const StlList& source ) : List<E>() {
            copy( source );
        }

        /**
         * Copy constructor - copies the content of the given set into this
         * one.
         * @param source The source set.
         */
        StlList( const Collection<E>& source ) : List<E>() {
            List<E>::copy( source );
        }

        virtual ~StlList() {}

        /**
         * {@inheritDoc}
         */
        virtual bool equals( const StlList& source ) const {
            return this->values == source.values;
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
        virtual ListIterator<E>* listIterator( std::size_t index )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException ) {

            if( index >= this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::listIterator - Index greater than size()" );
            }

            return new StlListIterator( &values, index );
        }
        virtual ListIterator<E>* listIterator( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException ) {

            if( index >= this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::listIterator - Index greater than size()" );
            }

            return new ConstStlListIterator( &values, index );
        }

        /**
         * {@inheritDoc}
         */
        virtual void copy( const StlList& source ) {
            this->values.clear();
            this->values = source.values;
        }

        /**
         * {@inheritDoc}
         */
        virtual void clear() throw ( lang::exceptions::UnsupportedOperationException ) {
            values.clear();
        }

        /**
         * {@inheritDoc}
         */
        virtual bool contains( const E& value ) const throw ( lang::Exception ) {
            typename std::list<E>::const_iterator iter;
            iter = std::find( values.begin(), values.end(), value );
            return iter != values.end();
        }

        /**
         * {@inheritDoc}
         */
        virtual std::size_t indexOf( const E& value )
            throw ( decaf::lang::exceptions::NoSuchElementException ) {

            typename std::list<E>::iterator iter;
            iter = std::find( values.begin(), values.end(), value );

            if( iter == values.end() ) {
                throw decaf::lang::exceptions::NoSuchElementException(
                    __FILE__, __LINE__,
                    "List::indexOf - No matching element in list" );
            }

            return std::distance( values.begin(), iter );
        }

        /**
         * {@inheritDoc}
         */
        virtual std::size_t lastIndexOf( const E& value )
            throw ( decaf::lang::exceptions::NoSuchElementException ) {

            typename std::list<E>::reverse_iterator iter;
            iter = std::find( values.rbegin(), values.rend(), value );

            if( iter == values.rend() ) {
                throw decaf::lang::exceptions::NoSuchElementException(
                    __FILE__, __LINE__,
                    "List::lastIndexOf - No matching element in list" );
            }

            // Now reverse the result to get the last index
            return this->size() - std::distance( values.rbegin(), iter ) - 1;
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
        virtual std::size_t size() const {
            return values.size();
        }

        /**
         * {@inheritDoc}
         */
        virtual E get( std::size_t index ) const
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException ) {

            if( index >= this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::get - Index greater than size()" );
            }

            // Advance from begin and return the value at that location.
            typename std::list<E>::const_iterator iter = this->values.begin();
            std::advance( iter, index );
            return *( iter );
        }

        /**
         * {@inheritDoc}
         */
        virtual E set( std::size_t index, const E& element )
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException ){

            if( index >= this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::get - Index greater than size()" );
            }

            // Advance from begin and return the value at that location
            // after setting the value to the new value.
            typename std::list<E>::iterator iter = this->values.begin();
            std::advance( iter, index );
            E oldValue = *iter;
            *iter = element;

            return oldValue;
        }

        /**
         * {@inheritDoc}
         */
        virtual bool add( const E& value )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException,
                    lang::exceptions::IllegalStateException ) {

            values.insert( values.end(), value );

            return true;
        }

        /**
         * {@inheritDoc}
         */
        virtual void add( std::size_t index, const E& element )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IndexOutOfBoundsException ) {

            if( index > this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::add - Index greater than size()" );
            }

            // Advance from begin and insert the value at that location
            typename std::list<E>::iterator iter = this->values.begin();
            std::advance( iter, index );
            this->values.insert( iter, element );
        }

        /**
         * {@inheritDoc}
         */
        virtual bool addAll( std::size_t index, const Collection<E>& source )
            throw ( decaf::lang::exceptions::UnsupportedOperationException,
                    decaf::lang::exceptions::IndexOutOfBoundsException ) {

            if( index != 0 && index > this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::addAll - Index greater than size()" );
            }

            std::auto_ptr< Iterator<E> > srcIter( source.iterator() );
            while( srcIter->hasNext() ) {
                this->add( index++, srcIter->next() );
            }

            return !source.isEmpty();
        }

        /**
         * {@inheritDoc}
         */
        virtual bool remove( const E& value )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException ) {

            std::size_t origSize = this->size();
            this->values.remove( value );
            return origSize != this->size();
        }

        /**
         * {@inheritDoc}
         */
        virtual E remove( std::size_t index )
            throw ( decaf::lang::exceptions::UnsupportedOperationException,
                    decaf::lang::exceptions::IndexOutOfBoundsException ) {

            if( index > this->size() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::add - Index greater than size()" );
            }

            // Advance from begin and insert the value at that location
            typename std::list<E>::iterator iter = this->values.begin();
            std::advance( iter, index );
            E oldValue = *iter;
            this->values.erase( iter );

            return oldValue;
        }

    };

}}

#endif /*_DECAF_UTIL_STLLIST_H_*/
