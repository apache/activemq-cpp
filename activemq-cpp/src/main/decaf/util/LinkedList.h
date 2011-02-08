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

#ifndef _DECAF_UTIL_LINKEDLIST_H_
#define _DECAF_UTIL_LINKEDLIST_H_

#include <list>
#include <memory>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Integer.h>
#include <decaf/util/Config.h>
#include <decaf/util/Deque.h>
#include <decaf/util/ArrayList.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/ListIterator.h>
#include <decaf/util/AbstractSequentialList.h>

namespace decaf {
namespace util {

    using decaf::lang::System;

    /**
     * A complete implementation of the List interface using a doubly linked list data structure.
     *
     * This class also implements the Deque interface providing a common interface for additions
     * into the list at the front and end as well as allowing insertions anywhere in between.
     * This class can be used then to implement other data structures such as Stacks, Queue's or
     * double ended Queue's.
     *
     * The operations on this List object that index a particular element involve iterating over
     * the links of the list from beginning to end, starting from whichever end is closer to the
     * location the operation is to be performed on.
     *
     * @since 1.0
     */
    template< typename E >
    class LinkedList : public AbstractSequentialList<E>, public Deque<E> {
    private:

        template< typename U >
        class ListNode {
        public:

            U value;
            ListNode<U>* prev;
            ListNode<U>* next;

        private:

            ListNode( const ListNode& );
            ListNode& operator= ( const ListNode& );

        public:

            ListNode() : value(), prev(NULL), next(NULL) {
            }

            ListNode( const U& value ) : value(value), prev(NULL), next(NULL) {
            }

            ListNode( ListNode<U>* prev, ListNode<U>* next, const U& value ) :
                value(value), prev(prev), next(next) {
            }

        };

    private:

        int listSize;
        ListNode<E> head;
        ListNode<E> tail;

    public:

        LinkedList() :
            AbstractSequentialList<E>(), listSize(0), head(), tail() {

            this->head.next = &this->tail;
            this->tail.prev = &this->head;
        }

        LinkedList( const LinkedList<E>& list ) :
            AbstractSequentialList<E>(), listSize(0), head(), tail() {

            this->head.next = &this->tail;
            this->tail.prev = &this->head;

            this->addAllAtLocation( 0, list );
        }

        LinkedList( const Collection<E>& collection ) :
            AbstractSequentialList<E>(), listSize(0), head(), tail() {

            this->head.next = &this->tail;
            this->tail.prev = &this->head;

            this->addAllAtLocation( 0, collection );
        }

        virtual ~LinkedList() {
            try{
                this->purgeList();
            } catch(...) {}
        }

    public:

        LinkedList<E>& operator= ( const LinkedList<E>& list ) {
            this->clear();
            this->addAllAtLocation( 0, list );
            return *this;
        }

        LinkedList<E>& operator= ( const Collection<E>& collection ) {
            this->clear();
            this->addAllAtLocation( 0, collection );
            return *this;
        }

    public:

        virtual E get( int index ) const {

            if( index < 0 || index >= this->listSize ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index given is outside bounds of this list {%d}", index );
            }

            const ListNode<E>* location = NULL;

            if( index < this->listSize / 2 ) {
                location = &this->head;
                for( int i = 0; i <= index; ++i ) {
                    location = location->next;
                }
            } else {
                location = &this->tail;
                for( int i = this->listSize; i > index; --i ) {
                    location = location->prev;
                }
            }

            return location->value;
        }

        virtual E set( int index, const E& element ) {

            if( index < 0 || index >= this->listSize ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index given is outside bounds of this list {%d}", index );
            }

            ListNode<E>* location = NULL;

            if( index < this->listSize / 2 ) {
                location = &this->head;
                for( int i = 0; i <= index; ++i ) {
                    location = location->next;
                }
            } else {
                location = &this->tail;
                for( int i = this->listSize; i > index; --i ) {
                    location = location->prev;
                }
            }

            E oldValue = location->value;
            location->value = element;

            return oldValue;
        }

        virtual bool add( const E& value ) {
            this->addToEnd( value );
            return true;
        }

        virtual void add( int index, const E& value ) {

            if( index < 0 || index > this->listSize ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index given is outside bounds of this list {%d}", index );
            }

            this->addAtLocation( index, value );
        }

        virtual bool addAll( const Collection<E>& collection ) {
            return this->addAllAtLocation( this->listSize, collection );
        }

        virtual bool addAll( int index, const Collection<E>& collection ) {
            return this->addAllAtLocation( index, collection );
        }

        virtual void copy( const Collection<E>& collection ) {
            this->clear();
            this->addAllAtLocation( 0, collection );
        }

        virtual bool remove( const E& value ) {
            return this->removeFirstOccurrence( value );
        }

        virtual bool isEmpty() const {
            return this->listSize == 0;
        }

        virtual int size() const {
            return this->listSize;
        }

        virtual void clear() {
            this->purgeList();
            this->head.next = &this->tail;
            this->tail.prev = &this->head;
            this->listSize = 0;
            AbstractList<E>::modCount++;
        }

        virtual bool contains( const E& value ) const {
            return this->indexOf( value ) != -1;
        }

        virtual int indexOf( const E& value ) const {

            if( this->listSize == 0 ) {
                return -1;
            }

            const ListNode<E>* location = this->head.next;

            for( int i = 0; location != &this->tail; ++i, location = location->next ) {
                if( location->value == value ) {
                    return i;
                }
            }

            return -1;
        }

        virtual int lastIndexOf( const E& value ) const {

            if( this->listSize == 0 ) {
                return -1;
            }

            const ListNode<E>* location = this->tail.prev;

            for( int i = this->listSize - 1; location != &this->head; --i, location = location->prev ) {
                if( location->value == value ) {
                    return i;
                }
            }

            return -1;
        }

        virtual std::vector<E> toArray() const {

            std::vector<E> result;
            result.reserve( this->listSize );

            const ListNode<E>* current = this->head.next;

            while( current != &this->tail ) {
                result.push_back( current->value );
                current = current->next;
            }

            return result;
        }

    public:  // Deque interface implementation.

        virtual bool offer( const E& value ) {
            this->addLast( value );
            return true;
        }

        virtual bool poll( E& result ) {

            if( this->listSize == 0 ) {
                return false;
            }

            result = this->head.next->value;
            this->removeAtFront();
            return true;
        }

        virtual E remove() {
            return this->removeAtFront();
        }

        virtual bool peek( E& result ) const {

            if( this->listSize == 0 ) {
                return false;
            }

            result = this->head.next->value;
            return true;
        }

        virtual E element() const {
            if( this->listSize == 0 ) {
                throw decaf::util::NoSuchElementException(
                    __FILE__, __LINE__, "The list is Empty" );
            }

            return this->head.next->value;
        }

        virtual void addFirst( const E& value ) {
            this->addToFront( value );
        }

        virtual void addLast( const E& value ) {
            this->addToEnd( value );
        }

        virtual E& getFirst() {
            if( this->listSize == 0 ) {
                throw decaf::util::NoSuchElementException(
                    __FILE__, __LINE__, "The list is Empty" );
            }

            return this->head.next->value;
        }

        virtual const E& getFirst() const {
            if( this->listSize == 0 ) {
                throw decaf::util::NoSuchElementException(
                    __FILE__, __LINE__, "The list is Empty" );
            }

            return this->head.next->value;
        }

        virtual E& getLast() {
            if( this->listSize == 0 ) {
                throw decaf::util::NoSuchElementException(
                    __FILE__, __LINE__, "The list is Empty" );
            }

            return this->tail.prev->value;
        }

        virtual const E& getLast() const {
            if( this->listSize == 0 ) {
                throw decaf::util::NoSuchElementException(
                    __FILE__, __LINE__, "The list is Empty" );
            }

            return this->tail.prev->value;
        }

        virtual bool offerFirst( const E& element ) {
            this->addToFront( element );
            return true;
        }

        virtual bool offerLast( const E& element ) {
            this->addToEnd( element );
            return true;
        }

        virtual E removeFirst() {
            return this->removeAtFront();
        }

        virtual E removeLast() {
            return this->removeAtEnd();
        }

        virtual bool pollFirst( E& result ) {
            if( this->listSize == 0 ) {
                return false;
            }

            result = this->head.next->value;
            this->removeAtFront();
            return true;
        }

        virtual bool pollLast( E& result ) {
            if( this->listSize == 0 ) {
                return false;
            }

            result = this->tail.prev->value;
            this->removeAtEnd();
            return true;
        }

        virtual bool peekFirst( E& result ) const {
            if( this->listSize == 0 ) {
                return false;
            }

            result = this->head.next->value;
            return true;
        }

        virtual bool peekLast( E& result ) const {
            if( this->listSize == 0 ) {
                return false;
            }

            result = this->tail.prev->value;
            return true;
        }

        virtual E pop() {
            return this->removeAtFront();
        }

        virtual void push( const E& element ) {
            this->addToFront( element );
        }

        virtual bool removeFirstOccurrence( const E& value ) {
            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                if( iter->next() == value ) {
                    iter->remove();
                    return true;
                }
            }

            return false;
        }
        virtual bool removeLastOccurrence( const E& value ) {
            std::auto_ptr< Iterator<E> > iter( this->descendingIterator() );
            while( iter->hasNext() ) {
                if( iter->next() == value ) {
                    iter->remove();
                    return true;
                }
            }

            return false;
        }

    private:

        class LinkedListIterator : public ListIterator<E> {
        private:

            mutable LinkedList<E>* list;
            ListNode<E>* current;
            ListNode<E>* lastReturned;
            int index;
            int expectedModCount;

        private:

            LinkedListIterator( const LinkedListIterator& );
            LinkedListIterator operator= ( const LinkedListIterator& );

        public:

            LinkedListIterator( LinkedList<E>* list, int index ) :
                ListIterator<E>(), list(list), current(NULL), lastReturned(NULL), index(-1), expectedModCount(0) {

                if( list == NULL ) {
                    throw decaf::lang::exceptions::NullPointerException(
                        __FILE__, __LINE__, "Parent LinkedList pointer was Null." );
                }

                if( index < 0 || index > list->listSize ) {
                    throw decaf::lang::exceptions::IndexOutOfBoundsException(
                        __FILE__, __LINE__, "Given index {%d} is out of range.", index );
                }

                this->expectedModCount = list->modCount;

                // index starts at -1 to indicate that we are before begin or that the
                // list is empty.  We always want to start out one before so that the call
                // to next moves us onto the element in question;

                if( index < this->list->listSize / 2 ) {
                    this->current = &this->list->head;
                    for( this->index = -1; this->index + 1 < index; ++this->index ) {
                        this->current = this->current->next;
                    }
                } else {
                    this->current = &this->list->tail;
                    for( this->index = this->list->listSize; this->index >= index; --this->index ) {
                        this->current = this->current->prev;
                    }
                }
            }

            virtual ~LinkedListIterator() {}

            virtual E next() {

                if( this->expectedModCount != this->list->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "List modified outside of this Iterator." );
                }

                if( this->current->next == &(this->list->tail) ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__, "No more elements to return from next()" );
                }

                this->current = this->current->next;
                this->lastReturned = this->current;
                this->index++;

                return this->current->value;
            }

            virtual bool hasNext() const {
                return ( this->current->next != &(this->list->tail) );
            }


            virtual E previous() {
                if( this->expectedModCount != this->list->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "List modified outside of this Iterator." );
                }

                if( this->current == &(this->list->head) ) {
                    throw decaf::lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "No previous element, must call next() before calling previous()." );
                }

                this->lastReturned = this->current;
                this->current = this->current->prev;
                this->index--;

                return this->lastReturned->value;
            }

            virtual bool hasPrevious() const {
                return ( this->current != &(this->list->head) );
            }

            virtual void remove() {
                if( this->expectedModCount != this->list->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "List modified outside of this Iterator." );
                }

                if( this->lastReturned == NULL ) {
                    throw lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "Invalid State to call remove, must call next() before remove()" );
                }

                ListNode<E>* next = this->lastReturned->next;
                ListNode<E>* previous = this->lastReturned->prev;

                next->prev = previous;
                previous->next = next;

                // When iterating in reverse this would not be true
                if( this->current == this->lastReturned ) {
                    this->index--;
                }
                this->current = previous;

                delete this->lastReturned;
                this->lastReturned = NULL;

                this->list->listSize--;
                this->list->modCount++;

                this->expectedModCount++;
            }

            virtual void add( const E& e ) {
                if( this->expectedModCount != this->list->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "List modified outside of this Iterator." );
                }

                ListNode<E>* newNode = new ListNode<E>( this->current, this->current->next, e );

                this->current->next->prev = newNode;
                this->current->next = newNode;

                this->current = newNode;
                this->lastReturned = NULL;

                this->index++;
                this->expectedModCount++;
                this->list->modCount++;
                this->list->listSize++;
            }

            virtual void set( const E& e ) {

                if( this->expectedModCount != this->list->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "List modified outside of this Iterator." );
                }

                if( this->lastReturned != NULL ) {
                    this->lastReturned->value = e;
                } else {
                    throw decaf::lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__, "Iterator next has not been called." );
                }
            }

            virtual int nextIndex() const {
                return this->index + 1;
            }

            virtual int previousIndex() const {
                return this->index;
            }
        };

        class ConstLinkedListIterator : public ListIterator<E> {
        private:

            const LinkedList<E>* list;
            const ListNode<E>* current;
            const ListNode<E>* lastReturned;
            int index;

        private:

            ConstLinkedListIterator( const ConstLinkedListIterator& );
            ConstLinkedListIterator operator= ( const ConstLinkedListIterator& );

        public:

            ConstLinkedListIterator( const LinkedList<E>* list, int index ) :
                ListIterator<E>(), list(list), current(NULL), lastReturned(NULL), index(-1) {

                if( list == NULL ) {
                    throw decaf::lang::exceptions::NullPointerException(
                        __FILE__, __LINE__, "Parent LinkedList pointer was Null." );
                }

                if( index < 0 || index > list->listSize ) {
                    throw decaf::lang::exceptions::IndexOutOfBoundsException(
                        __FILE__, __LINE__, "Given index {%d} is out of range.", index );
                }

                // index starts at -1 to indicate that we are before begin or that the
                // list is empty.  We always want to start out one before so that the call
                // to next moves us onto the element in question;

                if( index < this->list->listSize / 2 ) {
                    this->current = &this->list->head;
                    for( this->index = -1; this->index + 1 < index; ++this->index ) {
                        this->current = this->current->next;
                    }
                } else {
                    this->current = &this->list->tail;
                    for( this->index = this->list->listSize; this->index >= index; --this->index ) {
                        this->current = this->current->prev;
                    }
                }
            }

            virtual ~ConstLinkedListIterator() {}

            virtual E next() {

                if( this->current->next == &(this->list->tail) ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__, "No more elements to return from this ListIterator" );
                }

                this->current = this->current->next;
                this->lastReturned = this->current;
                this->index++;

                return this->current->value;
            }

            virtual bool hasNext() const {
                return ( this->current->next != &(this->list->tail) );
            }

            virtual E previous() {

                if( this->current == &(this->list->head) ) {
                    throw decaf::lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "No previous element, must call next() before calling previous()." );
                }

                this->lastReturned = this->current;
                this->current = this->current->prev;
                this->index--;

                return this->lastReturned->value;
            }

            virtual bool hasPrevious() const {
                return ( this->current != &(this->list->head) );
            }

            virtual void remove() {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "Cannot write to a const ListIterator." );
            }

            virtual void add( const E& e DECAF_UNUSED ) {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "Cannot write to a const ListIterator." );
            }

            virtual void set( const E& e DECAF_UNUSED ) {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "Cannot write to a const ListIterator." );
            }

            virtual int nextIndex() const {
                return this->index + 1;
            }

            virtual int previousIndex() const {
                return this->index;
            }
        };

        class ReverseIterator : public Iterator<E> {
        private:

            LinkedList<E>* list;
            ListNode<E>* current;
            int expectedModCount;
            bool canRemove;

        private:

            ReverseIterator( const ReverseIterator& );
            ReverseIterator operator= ( const ReverseIterator& );

        public:

            ReverseIterator( LinkedList<E>* list ) :
                Iterator<E>(), list( list ), current(NULL), expectedModCount(0), canRemove(false) {

                if( list == NULL ) {
                    throw decaf::lang::exceptions::NullPointerException(
                        __FILE__, __LINE__, "Parent LinkedList pointer was Null." );
                }

                this->expectedModCount = this->list->modCount;
                this->current = &list->tail;
            }

            virtual ~ReverseIterator() {}

            virtual bool hasNext() const {
                return this->current->prev != &(this->list->head);
            }

            virtual E next() {

                if( this->expectedModCount != this->list->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "List modified outside of this Iterator." );
                }

                if( this->current->prev == &(this->list->head) ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__, "No more elements to return from next()" );
                }

                this->current = this->current->prev;
                this->canRemove = true;

                return this->current->value;
            }

            virtual void remove() {
                if( this->expectedModCount != this->list->modCount ) {
                    throw ConcurrentModificationException(
                        __FILE__, __LINE__, "List modified outside of this Iterator." );
                }

                if( !this->canRemove ) {
                    throw lang::exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "Invalid State to call remove, must call next() before remove()" );
                }

                ListNode<E>* next = this->current->prev;
                ListNode<E>* prev = this->current->next;

                next->next = prev;
                prev->prev = next;

                delete this->current;

                this->current = prev;

                this->list->listSize--;
                this->list->modCount++;
                this->expectedModCount++;
                this->canRemove = false;
            }
        };

        class ConstReverseIterator : public Iterator<E> {
        private:

            const LinkedList<E>* list;
            const ListNode<E>* current;

        private:

            ConstReverseIterator( const ConstReverseIterator& );
            ConstReverseIterator operator= ( const ConstReverseIterator& );

        public:

            ConstReverseIterator( const LinkedList<E>* list ) :
                Iterator<E>(), list( list ), current(NULL) {

                if( list == NULL ) {
                    throw decaf::lang::exceptions::NullPointerException(
                        __FILE__, __LINE__, "Parent LinkedList pointer was Null." );
                }

                this->current = &list->tail;
            }

            virtual ~ConstReverseIterator() {}

            virtual bool hasNext() const {
                return this->current->prev != &(this->list->head);
            }

            virtual E next() {

                if( this->current->prev == &(this->list->head) ) {
                    throw NoSuchElementException(
                        __FILE__, __LINE__, "No more elements to return from next()" );
                }

                this->current = this->current->prev;

                return this->current->value;
            }

            virtual void remove() {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "Cannot write to a const Iterator." );
            }
        };

    public:

        using AbstractSequentialList<E>::listIterator;

        virtual ListIterator<E>* listIterator( int index ) {
            return new LinkedListIterator( this, index );
        }
        virtual ListIterator<E>* listIterator( int index ) const {
            return new ConstLinkedListIterator( this, index );
        }

        virtual Iterator<E>* descendingIterator() {
            return new ReverseIterator( this );
        }
        virtual Iterator<E>* descendingIterator() const {
            return new ConstReverseIterator( this );
        }

    private:

        E removeAtFront() {

            if( this->head.next == &this->tail ) {
                throw NoSuchElementException(
                    __FILE__, __LINE__, "The Collection is empty." );
            }

            ListNode<E>* oldNode = this->head.next;
            E result = oldNode->value;

            this->head.next = oldNode->next;
            this->head.next->prev = &this->head;

            delete oldNode;

            this->listSize--;
            AbstractList<E>::modCount++;

            return result;
        }

        E removeAtEnd() {

            if( this->head.next == &this->tail ) {
                throw NoSuchElementException(
                    __FILE__, __LINE__, "The Collection is empty." );
            }

            ListNode<E>* oldNode = this->tail.prev;
            E result = oldNode->value;

            this->tail.prev = oldNode->prev;
            this->tail.prev->next = &this->tail;

            delete oldNode;

            this->listSize--;
            AbstractList<E>::modCount++;

            return result;
        }

        void addToFront( const E& value ) {

            ListNode<E>* newHead = new ListNode<E>( &this->head, this->head.next, value );

            (this->head.next)->prev = newHead;
            this->head.next = newHead;

            this->listSize++;
            AbstractList<E>::modCount++;
        }

        void addToEnd( const E& value ) {

            ListNode<E>* newTail = new ListNode<E>( this->tail.prev, &this->tail, value );

            (this->tail.prev)->next = newTail;
            this->tail.prev = newTail;

            this->listSize++;
            AbstractList<E>::modCount++;
        }

        void addAtLocation( int index, const E& value ) {

            ListNode<E>* location = NULL;

            if( index <= this->listSize / 2 ) {
                location = this->head.next;
                for( int i = 0; i < index; ++i ) {
                    location = location->next;
                }
            } else {
                location = &this->tail;
                for( int i = this->listSize; i > index; --i ) {
                    location = location->prev;
                }
            }

            ListNode<E>* newNode = new ListNode<E>( location->prev, location, value );

            (location->prev)->next = newNode;
            location->prev = newNode;

            this->listSize++;
            AbstractList<E>::modCount++;
        }

        bool addAllAtLocation( int index, const Collection<E>& collection ) {

            if( index < 0 || index > this->listSize ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index for add is outside bounds of this LinkedList." );
            }

            int csize = collection.size();
            if( csize == 0 ) {
                return false;
            }

            std::auto_ptr< ArrayList<E> > copy;
            std::auto_ptr< Iterator<E> > iter;

            if( this == &collection ) {
                copy.reset( new ArrayList<E>( collection ) );
                iter.reset( copy->iterator() );
            } else {
                iter.reset( collection.iterator() );
            }

            ListNode<E>* newNode = NULL;
            ListNode<E>* previous = NULL;

            if( index < this->listSize / 2 ) {
                previous = &this->head;
                for( int i = 0; i < index; ++i ) {
                    previous = previous->next;
                }
            } else {
                previous = &this->tail;
                for( int i = this->listSize; i >= index; --i ) {
                    previous = previous->prev;
                }
            }

            while( iter->hasNext() ) {
                newNode = new ListNode<E>( previous, previous->next, iter->next() );
                previous->next->prev = newNode;
                previous->next = newNode;
                previous = newNode;
            }

            this->listSize += csize;
            AbstractList<E>::modCount++;

            return true;
        }

        void purgeList() {
            ListNode<E>* current = this->head.next;
            ListNode<E>* temp = NULL;
            while( current != &this->tail ) {
                temp = current;
                current = current->next;
                delete temp;
            }
        }
    };

}}

#endif /* _DECAF_UTIL_LINKEDLIST_H_ */
