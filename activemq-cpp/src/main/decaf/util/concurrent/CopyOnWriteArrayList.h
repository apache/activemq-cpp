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

#ifndef _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYLIST_H_
#define _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYLIST_H_

#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/ArrayPointer.h>
#include <decaf/lang/System.h>
#include <decaf/util/List.h>

namespace decaf {
namespace util {
namespace concurrent {

    using decaf::lang::ArrayPointer;
    using decaf::lang::System;

    template< typename E >
    class CopyOnWriteArrayList : public List<E> {
    private:

        Mutex mutex;
        ArrayPointer<E> array;

    public:

        class ArrayListIterator : public ListIterator<E> {
        private:

            ArrayPointer<E> array;
            int position;

        private:

            ArrayListIterator( const ArrayListIterator& );
            ArrayListIterator& operator= ( const ArrayListIterator& );

        public:

            ArrayListIterator( const ArrayPointer<E>& array, int index ) :
                ListIterator<E>(), array( array ), position( index ) {

                if( position < 0 || position > array.length() ) {
                    throw decaf::lang::exceptions::IndexOutOfBoundsException(
                        __FILE__, __LINE__, "Iterator created with invalid index." );
                }
            }

            virtual ~ArrayListIterator() {};

            virtual E next() {

                if( position >= array.length() ) {
                    throw NoSuchElementException();
                }

                return this->array[position++];
            }

            virtual bool hasNext() const {
                return this->position < array.length();
            }

            virtual void remove() {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "CopyOnWriteArrayList Iterator cannot remove elements." );
            }

            virtual void add( const E& e DECAF_UNUSED ) {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "CopyOnWriteArrayList Iterator cannot add elements." );
            }

            virtual void set( const E& e DECAF_UNUSED ) {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "CopyOnWriteArrayList Iterator cannot add elements." );
            }

            virtual bool hasPrevious() const {
                return this->position > 0;
            }

            virtual E previous() {
                if( position <= 0 ) {
                    throw NoSuchElementException();
                }

                return this->array[position--];
            }

            virtual int nextIndex() const {
                return this->position;
            }

            virtual int previousIndex() const {
                return this->position - 1;
            }

        };

    public:

        CopyOnWriteArrayList() : List<E>(), mutex(), array() {
        }

        CopyOnWriteArrayList( const Collection<E>& collection ) : List<E>(), mutex(), array() {
            this->doCopyCollection( collection );
        }

        CopyOnWriteArrayList( const CopyOnWriteArrayList<E>& collection ) : List<E>(), mutex(), array() {
            this->doCopyCollection( collection );
        }

        CopyOnWriteArrayList( const E* array, int size ) : List<E>(), mutex(), array() {

            E* elements = new E[size];
            for( int i = 0; i < size; ++i ) {
                elements[i] = array[i];
            }

            this->array.reset( elements, size );
        }

        virtual ~CopyOnWriteArrayList() {}

    public:

        CopyOnWriteArrayList<E>& operator= ( const CopyOnWriteArrayList<E>& list ) {
            synchronized( &mutex ) {
                this->clear();
                this->doCopyCollection( list );
            }
            return *this;
        }

        CopyOnWriteArrayList<E>& operator= ( const Collection<E>& list ) {
            synchronized( &mutex ) {
                this->clear();
                this->doCopyCollection( list );
            }
            return *this;
        }

    public:  // Collections API

        virtual void copy( const Collection<E>& collection ) {
            this->doCopyCollection( collection );
        }

        virtual bool add( const E& value ) {

            synchronized( &this->mutex ) {

                ArrayPointer<E> oldData = this->array;
                int size = oldData.length();
                E* elements = new E[size + 1];
                if( size > 0 ) {
                    System::arraycopy( oldData.get(), 0, elements, 0, size );
                }
                elements[size] = value;
                this->array.reset( elements, size + 1 );

                return true;
            }

            return false;
        }

        virtual bool addAll( const Collection<E>& collection ) {

            synchronized( &this->mutex ) {

                ArrayPointer<E> oldData = this->array;
                int size = oldData.length();
                E* elements = new E[size + collection.size()];

                if( size > 0 ) {
                    System::arraycopy( oldData.get(), 0, elements, 0, size );
                }

                std::auto_ptr< Iterator<E> > iter( collection.iterator() );
                while( iter->hasNext() ) {
                    elements[size++] = iter->next();
                }

                this->array.reset( elements, size );

                return true;
            }

            return false;
        }

        virtual void clear() {
            synchronized( &this->mutex ) {
                this->array.reset( NULL );
            }
        }

        virtual bool contains( const E& value ) const {

            ArrayPointer<E> elements = this->array;
            for( int i = 0; i < elements.length(); ++i ) {
                if( elements[i] == value ) {
                    return true;
                }
            }

            return false;
        }

        virtual bool containsAll( const Collection<E>& collection ) const {

            ArrayPointer<E> elements = this->array;
            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                E next = iter->next();
                if( !this->contains( next ) ) {
                    return false;
                }
            }

            return true;
        }

        virtual bool equals( const Collection<E>& collection ) const {

            if( (void*)this == &collection ) {
                return true;
            }

            const List<E>* asList = dynamic_cast<const List<E>*>( &collection );
            if( asList == NULL ) {
                return false;
            }

            if( this->size() != asList->size() ) {
                return false;
            }

            std::auto_ptr< Iterator<E> > thisIter( this->iterator() );
            std::auto_ptr< Iterator<E> > otherIter( asList->iterator() );

            while( thisIter->hasNext() ) {
                if( !otherIter->hasNext() ) {
                    return false;
                }

                E myNext = thisIter->next();
                E otherNext = otherIter->next();

                if( myNext != otherNext ) {
                    return false;
                }
            }

            if( otherIter->hasNext() ) {
                return false;
            }

            return true;
        }

        virtual bool isEmpty() const {
            return this->getArray().length() == 0;
        }

        virtual bool remove( const E& value ) {

            synchronized( &this->mutex ) {

                int index = this->indexOf( value );
                if( index == -1 ) {
                    return false;
                }

                this->removeAt( index );

                return true;
            }

            return false;
        }

        virtual bool removeAll( const Collection<E>& collection ) {

            if( collection.isEmpty() ) {
                return false;
            }

            synchronized( &this->mutex ) {

                ArrayPointer<E> oldData = this->array;
                int size = oldData.length();

                if( size == 0 ) {
                    return false;
                }

                ArrayPointer<E> elements( size );
                int count = 0;

                for( int i = 0; i < size; ++i ) {
                    E value = oldData[i];
                    if( !collection.contains( value ) ) {
                        elements[count++] = value;
                    }
                }

                if( count == 0 ) {
                    this->array.reset( NULL );
                } else {
                    E* newArray = new E[count];
                    System::arraycopy( elements.get(), 0, newArray, 0, count );
                    this->array.reset( newArray, count );
                }

                return true;

            }

            return false;
        }

        virtual bool retainAll( const Collection<E>& collection ) {

            synchronized( &this->mutex ) {

                ArrayPointer<E> oldData = this->array;
                int size = oldData.length();

                if( size == 0 ) {
                    return false;
                }

                if( collection.isEmpty() ) {
                    this->array.reset( NULL );
                    return true;
                }

                ArrayPointer<E> elements( size );
                int count = 0;

                for( int i = 0; i < size; ++i ) {
                    E value = oldData[i];
                    if( collection.contains( value ) ) {
                        elements[count++] = value;
                    }
                }

                if( count == 0 ) {
                    this->array.reset( NULL );
                } else {
                    E* newArray = new E[count];
                    System::arraycopy( elements.get(), 0, newArray, 0, count );
                    this->array.reset( newArray, count );
                }

                return true;

            }

            return false;
        }

        virtual int size() const {
            return this->getArray().length();
        }

        virtual std::vector<E> toArray() const {

            ArrayPointer<E> data = this->getArray();
            std::vector<E> result( data.length() );
            for( int i = 0; i < data.length(); ++i ) {
                result[i] = data[i];
            }

            return result;
        }

    public:  // Iterable API

        virtual decaf::util::Iterator<E>* iterator() {
            return new ArrayListIterator( this->getArray(), 0 );
        }
        virtual decaf::util::Iterator<E>* iterator() const {
            return new ArrayListIterator( this->getArray(), 0 );
        }

    public:  // List API

        virtual ListIterator<E>* listIterator()  {
            return new ArrayListIterator( this->getArray(), 0 );
        }
        virtual ListIterator<E>* listIterator() const  {
            return new ArrayListIterator( this->getArray(), 0 );
        }

        virtual ListIterator<E>* listIterator( int index )  {
            return new ArrayListIterator( this->getArray(), index );
        }
        virtual ListIterator<E>* listIterator( int index ) const  {
            return new ArrayListIterator( this->getArray(), index );
        }

        virtual int indexOf( const E& value ) const {

            ArrayPointer<E> elements = this->array;
            for( int i = 0; i < elements.length(); ++i ) {
                if( elements[i] == value ) {
                    return i;
                }
            }

            return -1;
        }

        virtual int lastIndexOf( const E& value ) const {

            ArrayPointer<E> elements = this->array;
            for( int i = elements.length() - 1; i >= 0; --i ) {
                if( elements[i] == value ) {
                    return i;
                }
            }

            return -1;
        }

        virtual E get( int index ) const {
            ArrayPointer<E> array = this->getArray();
            checkIndexExclusive( index, array.length() );
            return array[index];
        }

        virtual E set( int index, const E& element ) {

            synchronized( &this->mutex ) {

                ArrayPointer<E> previous = this->getArray();
                int size = previous.length();
                this->checkIndexExclusive( index, size );
                E* newArray = new E[size];
                System::arraycopy( previous.get(), 0, newArray, 0, size );
                E old = newArray[index];
                newArray[index] = element;
                this->array.reset( newArray, size );

                return old;
            }

            return E();
        }

        virtual void add( int index, const E& element ) {

            synchronized( &this->mutex ) {

                ArrayPointer<E> previous = this->getArray();
                int size = previous.length();
                checkIndexInclusive( index, size );
                E* newArray = new E[size+1];

                if( size > 0 ) {
                    System::arraycopy( previous.get(), 0, newArray, 0, index);
                }

                if( size > index ) {
                    System::arraycopy( previous.get(), index, newArray, index + 1, size - index );
                }

                newArray[index] = element;

                this->array.reset( newArray, size+1 );
            }
        }

        virtual bool addAll( int index, const Collection<E>& collection ) {

            synchronized( &this->mutex ) {

                ArrayPointer<E> previous = this->getArray();
                int size = previous.length();
                checkIndexInclusive( index, size );
                int csize = collection.size();

                if( csize == 0 ) {
                    return false;
                }

                E* newArray = new E[size+csize];

                if( size > 0 ) {
                    System::arraycopy( previous.get(), 0, newArray, 0, index);
                }

                std::auto_ptr< Iterator<E> > iter( collection.iterator() );
                int pos = index;
                while( iter->hasNext() ) {
                    newArray[pos++] = iter->next();
                }

                if( size > index ) {
                    System::arraycopy( previous.get(), index, newArray, index + csize, size - index );
                }

                this->array.reset( newArray, size+csize );
            }

            return true;
        }

        virtual E removeAt( int index ) {

            synchronized( &this->mutex ) {

                ArrayPointer<E> previous = this->getArray();
                int size = previous.length();
                this->checkIndexExclusive( index, size );
                E old = previous[index];

                if( size == 1 ) {
                    this->array.reset( NULL );
                    return old;
                }

                E* newArray = new E[size-1];
                System::arraycopy( previous.get(), 0, newArray, 0, index );

                if( size > index ) {
                    System::arraycopy( previous.get(), index + 1, newArray, index, size - index - 1 );
                }

                this->array.reset( newArray, size - 1 );

                return old;
            }

            return E();
        }

        virtual std::string toString() const {
            ArrayPointer<E> current = this->getArray();

            std::string result;

            return result;
        }

    public:

        /**
         * Adds the given value to the end of this List if it is not already contained
         * in this List.
         *
         * @param value
         *      The element to be added if not already contained in this List.
         *
         * @returns true if the element is added to this List.
         */
        bool addIfAbsent( const E& value ) {

            synchronized( &this->mutex ) {

                ArrayPointer<E> previous = this->getArray();
                int size = previous.length();

                if( size != 0 ) {
                    if( this->indexOf( value ) != -1 ) {
                        return false;
                    }
                }

                E* newArray = new E[size+1];
                if( size > 0 ) {
                    System::arraycopy( previous.get(), 0, newArray, 0, size );
                }
                newArray[size] = value;

                this->array.reset( newArray, size+1 );

                return true;
            }

            return false;
        }

        /**
         * Every element in the given collection that is not already contained in this Collection
         * is added to the end of this collection.  The order that the elements are added is ditacted
         * by the order that the collection's iterator returns them.
         *
         * @param collection
         *      The collection whose elements are to be added if not already in this List.
         *
         * @returns the number of elements that are added to this List.
         */
        int addAllAbsent( const Collection<E>& collection ) {

            if( collection.size() == 0 ) {
                return 0;
            }

            synchronized( &this->mutex ) {

                ArrayPointer<E> previous = this->getArray();
                int size = previous.length();

                ArrayPointer<E> add( collection.size() );
                int count = 0;

                std::auto_ptr< Iterator<E> > iter( collection.iterator() );
                while( iter->hasNext() ) {
                    E value = iter->next();
                    if( this->indexOf( value ) == -1 ) {
                        add[count++] = value;
                    }
                }

                E* newArray = new E[size+count];
                System::arraycopy( previous.get(), 0, newArray, 0, size );
                System::arraycopy( add.get(), 0, newArray, size, count );

                this->array.reset( newArray, size+count );

                return count;
            }

            return 0;
        }

        /**
         * Searches backwards through the List for the given element starting at the index
         * specified.
         *
         * @param value
         *      The value to search for in the List.
         * @param index
         *      The index in the list to begin the search from.
         *
         * @returns the index in the list that matches the value given, or -1 if not found.
         *
         * @throws IndexOutOfBoundsException if the given index is greater than or equal to the List size.
         */
        int lastIndexOf( const E& value, int index ) {

            ArrayPointer<E> elements = this->array;

            if( index >= elements.length() ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index given %d, actual size %d", index, elements.length() );
            }

            for( int i = index - 1; i >= 0; --i ) {
                if( elements[i] == value ) {
                    return i;
                }
            }

            return -1;
        }

        /**
         * Searches the List starting from the specified index and returns the index of the
         * first item in the list that is equal to the given value.
         *
         * @param value
         *      The value to search for in the List.
         * @param index
         *      The index in the List to begin the search from.
         *
         * @returns the index in the List that matches the given element or -1 if not found.
         *
         * @throws IndexOutOfBoundsException if the given index is negative.
         */
        int indexOf( const E& value, int index ) const {

            ArrayPointer<E> elements = this->array;

            if( index < 0 ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index given %d, actual size %d", index, elements.length() );
            }

            for( int i = index; i < elements.length(); ++i ) {
                if( elements[i] == value ) {
                    return i;
                }
            }

            return -1;
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

        virtual void wait()  {
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

    private:

        template<typename T>
        friend class CopyOnWriteArraySet;

        void doCopyCollection( const Collection<E>& collection ) {

            if( (void*)this == &collection ) {
                return;
            }

            synchronized( &this->mutex ) {

                ArrayPointer<E> data( collection.size() );

                std::auto_ptr< Iterator<E> > iter( collection.iterator() );
                int index = 0;

                while( iter->hasNext() ) {
                    data[index++] = iter->next();
                }

                this->array = data;
            }
        }

        ArrayPointer<E> getArray() const {
            return this->array;
        }

        static void checkIndexInclusive( int index, int size ) {
            if( index < 0 || index > size ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index is %d, size is %d", index, size );
            }
        }

        static void checkIndexExclusive( int index, int size ) {
            if( index < 0 || index >= size ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index is %d, size is %d", index, size );
            }
        }

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYLIST_H_ */
