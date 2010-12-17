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

#ifndef _DECAF_UTIL_ARRAYLIST_H_
#define _DECAF_UTIL_ARRAYLIST_H_

#include <memory>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Integer.h>
#include <decaf/util/Config.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/ListIterator.h>
#include <decaf/util/List.h>
#include <decaf/util/AbstractList.h>

namespace decaf {
namespace util {

    using decaf::lang::System;

    template< typename E >
    class ArrayList : public decaf::util::AbstractList<E> {
    private:

        E* elements;
        int capacity;
        int head;
        int curSize;

    public:

        ArrayList() : AbstractList<E>(), elements( NULL ), capacity( 0 ), head( 0 ), curSize( 0 ) {
            this->ensureCapacity( 10 );
        }

        ArrayList( const Collection<E>& collection ) : AbstractList<E>(), elements( NULL ),
                                                       capacity( 0 ), head( 0 ), curSize( 0 ) {

            this->capacity = collection.size() + ( collection.size() / 10 );
            this->elements = new E[this->capacity];

            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                this->elements[this->head++] = iter->next();
                this->curSize++;
            }
        }

        ArrayList( const ArrayList<E>& arrayList ) : AbstractList<E>(), elements( NULL ),
                                                     capacity( 0 ), head( 0 ), curSize( 0 ) {

            this->capacity = arrayList.size() + ( arrayList.size() / 10 );
            this->elements = new E[this->capacity];

            std::auto_ptr< Iterator<E> > iter( arrayList.iterator() );
            while( iter->hasNext() ) {
                this->elements[this->head++] = iter->next();
                this->curSize++;
            }
        }

        ArrayList( int initialCapacity ) : AbstractList<E>(), elements( NULL ),
                                           capacity( initialCapacity ), head( 0 ), curSize( 0 ) {

            if( initialCapacity < 0 ) {
                throw decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__, "Initial Capacity argument cannot be negative." );
            }

            this->elements = new E[this->capacity];
        }

        virtual ~ArrayList() {
            try{
                delete [] elements;
            }
            DECAF_CATCHALL_NOTHROW()
        }

    public:

        ArrayList<E>& operator= ( const ArrayList<E>& list ) {
            this->clear();
            this->addAll( list );
            return *this;
        }

        ArrayList<E>& operator= ( const Collection<E>& collection ) {
            this->clear();
            this->addAll( 0, collection );
            return *this;
        }

    public:

        /**
         * Increases the capacity of this ArrayList instance, if necessary, to ensure that it can
         * hold at least the number of elements specified by the minimum capacity argument.  If the
         * capacity is already greater than or equal to the minimum capacity argument then the array
         * is left unchanged.
         *
         * @param minimumCapacity
         *      The desired minimum capacity for this ArrayList.
         */
        void ensureCapacity( int minimumCapacity ) {

            if( minimumCapacity < 0 || this->capacity >= minimumCapacity ) {
                return;
            }

            int newCapacity = minimumCapacity == 0 ? 10 : minimumCapacity;

            E* newElements = new E[newCapacity];
            if( this->curSize > 0 ) {
                decaf::lang::System::arraycopy( this->elements, this->head, newElements, 0, this->curSize );
            }
            delete [] this->elements;
            this->elements = newElements;
            this->capacity = newCapacity;
            AbstractList<E>::modCount++;
        }

        /**
         * Trims the internal array to match the current size of the ArrayList.  This compacts
         * the internal array to save storage space used by this ArrayList.
         */
        void trimToSize() {

            if( this->curSize < this->capacity ) {

                int newCapacity = this->curSize == 0 ? 10 : this->curSize;

                E* newElements = new E[newCapacity];
                if( this->curSize > 0 ) {
                    System::arraycopy( this->elements, 0, newElements, 0, this->curSize );
                }

                delete [] this->elements;
                this->elements = newElements;
                this->capacity = newCapacity;
            }

            AbstractList<E>::modCount++;
        }

    public:

        virtual void clear() {
            delete [] this->elements;
            this->curSize = 0;
            this->capacity = 10;
            this->elements = new E[this->capacity];
            AbstractList<E>::modCount++;
        }

        virtual bool isEmpty() const {
            return this->curSize == 0;
        }

        virtual int size() const {
            return this->curSize;
        }

        virtual E set( int index, const E& element ) {

            if( index < 0 || index >= this->curSize ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::get - Index greater than size() or negative" );
            }

            E oldValue = this->elements[index];
            this->elements[index] = element;

            return oldValue;
        }

        virtual E get( int index ) const {

            if( index < 0 || index >= this->curSize ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::get - Index greater than size() or negative" );
            }

            return this->elements[index];
        }

        virtual bool add( const E& value ) {

            this->expandEnd( 1 );
            this->elements[this->curSize++] = value;
            AbstractList<E>::modCount++;

            return true;
        }

        virtual void add( int index, const E& element ) {

            if( index < 0 || index > this->curSize ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "Index was negative or greater than size()" );
            }

            if( index == 0 ) {
                this->expandFront( 1 );
            } else if( index == this->curSize ) {
                this->expandEnd( 1 );
            } else {
                this->expandMiddle( index, 1 );
            }

            this->elements[index] = element;
            this->curSize++;
            AbstractList<E>::modCount++;
        }

        virtual bool addAll( const Collection<E>& collection ) {

            int csize = collection.size();
            if( csize == 0 ) {
                return false;
            }

            std::vector<E> array = collection.toArray();

            this->expandEnd( csize );

            for( int i = 0; i < csize; ++i ) {
                this->elements[this->curSize++] = array[i];
            }

            AbstractList<E>::modCount++;

            return true;
        }

        virtual bool addAll( int index, const Collection<E>& collection ) {

            if( index < 0 || index > this->curSize ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::addAll - Index greater than size()" );
            }

            int csize = collection.size();
            if( csize == 0 ) {
                return false;
            }

            std::vector<E> array = collection.toArray();

            if( index == 0 ) {
                this->expandFront( csize );
            } else if( index == this->curSize ) {
                this->expandEnd( csize );
            } else {
                this->expandMiddle( index, csize );
            }

            for( int i = 0; i < csize; ++i, ++this->curSize ) {
                this->elements[index++] = array[i];
            }

            AbstractList<E>::modCount++;

            return true;
        }

        virtual bool remove( const E& value ) {

            int result = indexOf( value );
            if( result != -1 ) {
                this->removeAt( result );
                return true;
            }

            return false;
        }

        virtual E removeAt( int index ) {

            if( index < 0 || index >= this->curSize ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__,
                    "List::removeAt - Index greater than size() or negative" );
            }

            E old = this->elements[index];

            System::arraycopy( this->elements, 0, this->elements, 0, index );

            if( this->curSize > index ) {
                System::arraycopy( this->elements, index + 1, this->elements, index, this->curSize - index - 1 );
            }

            this->elements[--this->curSize] = E();
            AbstractList<E>::modCount++;

            return old;
        }

        virtual bool contains( const E& value ) const {
            return this->indexOf( value ) != -1;
        }

        virtual int indexOf( const E& value ) const {

            for( int i = 0; i < this->curSize; ++i ) {
                if( this->elements[i] == value ) {
                    return i;
                }
            }

            return -1;
        }

        virtual int lastIndexOf( const E& value ) const {

            for( int i = this->curSize - 1; i >= 0 ; --i ) {
                if( this->elements[i] == value ) {
                    return i;
                }
            }

            return -1;
        }

        virtual std::vector<E> toArray() const {

            std::vector<E> result;

            for( int i = 0; i < this->curSize; ++i ) {
                result.push_back( this->elements[i] );
            }

            return result;
        }

        virtual std::string toString() const {

            std::string result;

            result.append( "decaf::util::ArrayList [  size = " );
            result.append( decaf::lang::Integer::toString( this->curSize ) );
            result.append( " ]");

            return result;
        }

    private:

        void expandFront( int amount ) {

            if( amount == 0 ) {
                return;
            }

            E* previous = this->elements;

            if( amount > this->capacity - this->curSize ) {
                this->capacity = this->capacity + amount + 11;
                this->elements = new E[this->capacity];
            }

            if( this->curSize > 0 ) {
                System::arraycopy( previous, 0, this->elements, amount, this->curSize );
            }

            if( previous != this->elements ) {
                delete [] previous;
            }
        }

        void expandEnd( int amount ) {

            if( amount == 0 ) {
                return;
            }

            E* previous = this->elements;

            if( amount > this->capacity - this->curSize ) {
                this->capacity = this->capacity + amount + 11;
                this->elements = new E[this->capacity];
                System::arraycopy( previous, 0, this->elements, 0, this->curSize );
            }

            if( previous != this->elements ) {
                delete [] previous;
            }
        }

        void expandMiddle( int index, int amount ) {

            if( amount == 0 ) {
                return;
            }

            E* previous = this->elements;

            if( amount > this->capacity - this->curSize ) {
                this->capacity = this->capacity + amount + 11;
                this->elements = new E[this->capacity];
            }

            if( this->curSize > 0 ) {
                System::arraycopy( previous, 0, this->elements, 0, index );
            }

            if( this->curSize > index ) {
                System::arraycopy( previous, index, this->elements, index + amount, this->curSize - index );
            }

            if( previous != this->elements ) {
                delete [] previous;
            }
        }

    };

}}

#endif /* _DECAF_UTIL_ARRAYLIST_H_ */
