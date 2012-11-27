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
#include <decaf/util/concurrent/locks/ReentrantReadWriteLock.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/List.h>

namespace decaf {
namespace util {
namespace concurrent {

    template< typename E >
    class CopyOnWriteArrayList : public List<E> {
    private:

        struct Array {

            int size;
            int capacity;
            E* elements;

            Array() : size(0), capacity(0), elements(NULL) {
            }

            Array(int capacity) : size(0), capacity(0), elements(NULL) {
                reserve(capacity);
            }

            Array(const Array& src, int capacity) : size(0), capacity(0), elements(NULL) {
                reserve(decaf::lang::Math::max(src.size, capacity));
                if (src.size > 0) {
                    decaf::lang::System::arraycopy<E>(src.elements, 0, this->elements, 0, src.size);
                }
                this->size = src.size;
            }

            ~Array() {
                delete [] elements;
            }

            void reserve(int requested) {
                if (capacity < requested) {
                    // growth as a factor of 1.5
                    int newlen = decaf::lang::Math::max((int)(capacity*1.5), requested);
                    E* newbuf = newlen ? new E[newlen] : NULL;

                    if (this->elements != NULL) {
                        decaf::lang::System::arraycopy<E>(this->elements, 0, newbuf, 0, size);
                    }

                    delete[] this->elements;
                    this->elements = newbuf;
                    capacity = newlen;
                }
            }

        private:

            Array(const Array&);
            Array operator= (const Array&);

        };

    private:

        mutable decaf::util::concurrent::locks::ReentrantReadWriteLock arrayLock;
        decaf::lang::Pointer<decaf::util::concurrent::locks::Condition> condition;
        decaf::lang::Pointer<Array> array;

    public:

        class ArrayListIterator : public ListIterator<E> {
        private:

            decaf::lang::Pointer<Array> array;
            int position;

        private:

            ArrayListIterator(const ArrayListIterator&);
            ArrayListIterator& operator=(const ArrayListIterator&);

        public:

            ArrayListIterator(decaf::lang::Pointer<Array> array, int index) :
                ListIterator<E> (), array(array), position(index) {

                if (position < 0 || position > array->size) {
                    throw decaf::lang::exceptions::IndexOutOfBoundsException(
                        __FILE__, __LINE__, "Iterator created with invalid index.");
                }
            }

            virtual ~ArrayListIterator() {
                this->array.reset(NULL);
            };

            virtual E next() {
                if (position >= array->size) {
                    throw NoSuchElementException();
                }

                return this->array->elements[position++];
            }

            virtual bool hasNext() const {
                return this->position < array->size;
            }

            virtual void remove() {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "CopyOnWriteArrayList Iterator cannot remove elements.");
            }

            virtual void add(const E& e DECAF_UNUSED ) {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "CopyOnWriteArrayList Iterator cannot add elements.");
            }

            virtual void set(const E& e DECAF_UNUSED ) {
                throw decaf::lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "CopyOnWriteArrayList Iterator cannot add elements.");
            }

            virtual bool hasPrevious() const {
                return this->position > 0;
            }

            virtual E previous() {
                if (position <= 0) {
                    throw NoSuchElementException();
                }

                return this->array->elements[position--];
            }

            virtual int nextIndex() const {
                return this->position;
            }

            virtual int previousIndex() const {
                return this->position - 1;
            }

        };

    public:

        CopyOnWriteArrayList() : List<E>(), arrayLock(), condition(), array(new Array()) {
            this->condition.reset(arrayLock.writeLock().newCondition());
        }

        CopyOnWriteArrayList(const Collection<E>& collection) : List<E>(), arrayLock(), condition(), array(new Array()) {
            this->condition.reset(arrayLock.writeLock().newCondition());
            this->doCopyCollection(collection);
        }

        CopyOnWriteArrayList(const CopyOnWriteArrayList<E>& collection) : List<E>(), arrayLock(), condition(), array(new Array()) {
            this->condition.reset(arrayLock.writeLock().newCondition());
            this->doCopyCollection(collection);
        }

        CopyOnWriteArrayList(const E* array, int size) : List<E>(), arrayLock(), condition(), array(new Array()) {
            this->condition.reset(arrayLock.writeLock().newCondition());
            decaf::lang::Pointer<Array> temp(new Array(size));
            for (int i = 0; i < size; ++i) {
                temp->elements[i] = array[i];
                temp->size++;
            }
            this->array.swap(temp);
        }

        virtual ~CopyOnWriteArrayList() {
            this->array.reset(NULL);
        }

    public:

        CopyOnWriteArrayList<E>& operator=(const CopyOnWriteArrayList<E>& list) {
            this->arrayLock.writeLock().lock();
            try {
                this->clear();
                this->doCopyCollection(list);
            } catch (decaf::lang::Exception& ex) {
                this->writeLock.unlock();
                throw;
            }

            this->arrayLock.writeLock().unlock();
            return *this;
        }

        CopyOnWriteArrayList<E>& operator=(const Collection<E>& list) {
            this->arrayLock.writeLock().lock();
            try {
                this->clear();
                this->doCopyCollection(list);
            } catch (decaf::lang::Exception& ex) {
                this->writeLock.unlock();
                throw;
            }

            this->arrayLock.writeLock().unlock();
            return *this;
        }

    public:  // Collections API

        virtual void copy(const Collection<E>& collection) {
            this->arrayLock.writeLock().lock();
            try {
                this->clear();
                this->doCopyCollection(collection);
                this->arrayLock.writeLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        virtual bool add(const E& value) {
            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;
                decaf::lang::Pointer<Array> newArray(new Array(*oldArray, size + 1));
                newArray->elements[size] = value;
                newArray->size++;
                this->array.swap(newArray);
                this->arrayLock.writeLock().unlock();
                return true;
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        virtual bool addAll(const Collection<E>& collection) {
            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;
                decaf::lang::Pointer<Array> newArray(new Array(*oldArray, size + collection.size()));
                std::auto_ptr<Iterator<E> > iter(collection.iterator());
                while (iter->hasNext()) {
                    newArray->elements[newArray->size++] = iter->next();
                }
                this->array.swap(newArray);
                this->arrayLock.writeLock().unlock();
                return true;
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        virtual void clear() {
            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> newArray(new Array());
                this->array.swap(newArray);
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }

            this->arrayLock.writeLock().unlock();
        }

        virtual bool contains(const E& value) const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            for (int i = 0; i < current->size; ++i) {
                if (current->elements[i] == value) {
                    return true;
                }
            }

            return false;
        }

        virtual bool containsAll(const Collection<E>& collection) const {
            std::auto_ptr<Iterator<E> > iter(collection.iterator());
            while (iter->hasNext()) {
                E next = iter->next();
                if (!this->contains(next)) {
                    return false;
                }
            }

            return true;
        }

        virtual bool equals(const Collection<E>& collection) const {

            if ((void*) this == &collection) {
                return true;
            }

            const List<E>* asList = dynamic_cast<const List<E>*> (&collection);
            if (asList == NULL) {
                return false;
            }

            if (this->size() != asList->size()) {
                return false;
            }

            std::auto_ptr<Iterator<E> > thisIter(this->iterator());
            std::auto_ptr<Iterator<E> > otherIter(asList->iterator());

            while (thisIter->hasNext()) {
                if (!otherIter->hasNext()) {
                    return false;
                }

                E myNext = thisIter->next();
                E otherNext = otherIter->next();

                if (myNext != otherNext) {
                    return false;
                }
            }

            if (otherIter->hasNext()) {
                return false;
            }

            return true;
        }

        virtual bool isEmpty() const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            return current->size == 0;
        }

        virtual bool remove(const E& value) {
            this->arrayLock.writeLock().lock();
            try {
                int index = this->indexOf(value);
                if (index == -1) {
                    this->arrayLock.writeLock().unlock();
                    return false;
                }
                this->removeAt(index);
                this->arrayLock.writeLock().unlock();
                return true;
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        virtual bool removeAll(const Collection<E>& collection) {
            if (collection.isEmpty()) {
                return false;
            }

            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;

                if (size == 0) {
                    this->arrayLock.writeLock().unlock();
                    return false;
                }

                decaf::lang::Pointer<Array> buffer(new Array(size));
                int count = 0;
                for (int i = 0; i < size; ++i) {
                    E value = oldArray->elements[i];
                    if (!collection.contains(value)) {
                        buffer->elements[count++] = value;
                        buffer->size++;
                    }
                }

                if (count == 0) {
                    this->array.reset(new Array());
                } else {
                    decaf::lang::Pointer<Array> newArray(new Array(*buffer, count));
                    this->array.swap(newArray);
                }

                this->arrayLock.writeLock().unlock();
                return true;
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        virtual bool retainAll(const Collection<E>& collection) {
            this->arrayLock.writeLock().lock();
            try {

                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;

                if (size == 0) {
                    this->arrayLock.writeLock().unlock();
                    return false;
                }

                if (collection.isEmpty()) {
                    this->array.reset(new Array());
                    this->arrayLock.writeLock().unlock();
                    return true;
                }

                decaf::lang::Pointer<Array> buffer(new Array(size));
                int count = 0;

                for (int i = 0; i < size; ++i) {
                    E value = oldArray->elements[i];
                    if (collection.contains(value)) {
                        buffer->elements[count++] = value;
                        buffer->size++;
                    }
                }

                if (count == 0) {
                    this->array.reset(new Array());
                } else {
                    this->array.swap(buffer);
                }

                this->arrayLock.writeLock().unlock();
                return true;
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        virtual int size() const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            return current->size;
        }

        virtual std::vector<E> toArray() const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }
            std::vector<E> result( current->size );
            for( int i = 0; i < current->size; ++i ) {
                result[i] = current->elements[i];
            }

            return result;
        }

    public:  // Iterable API

        virtual decaf::util::Iterator<E>* iterator() {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            return new ArrayListIterator(current, 0);
        }
        virtual decaf::util::Iterator<E>* iterator() const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            return new ArrayListIterator(current, 0);
        }

    public:  // List API

        virtual ListIterator<E>* listIterator()  {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            return new ArrayListIterator(current, 0);
        }
        virtual ListIterator<E>* listIterator() const  {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            return new ArrayListIterator(current, 0);
        }

        virtual ListIterator<E>* listIterator(int index) {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            return new ArrayListIterator(current, index);
        }
        virtual ListIterator<E>* listIterator(int index) const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            return new ArrayListIterator(this->array, index);
        }

        virtual int indexOf(const E& value) const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            for (int i = 0; i < current->size; ++i) {
                if (current->elements[i] == value) {
                    return i;
                }
            }

            return -1;
        }

        virtual int lastIndexOf(const E& value) const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            for (int i = current->size - 1; i >= 0; --i) {
                if (current->elements[i] == value) {
                    return i;
                }
            }

            return -1;
        }

        virtual E get(int index) const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

            checkIndexExclusive(index, current->size);
            return current->elements[index];
        }

        virtual E set(int index, const E& element) {
            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;
                this->checkIndexExclusive(index, size);
                decaf::lang::Pointer<Array> newArray(new Array(*oldArray, size));
                E old = newArray->elements[index];
                newArray->elements[index] = element;
                this->array.swap(newArray);
                this->arrayLock.writeLock().unlock();
                return old;
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        virtual void add(int index, const E& element) {
            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;
                this->checkIndexInclusive(index, size);
                decaf::lang::Pointer<Array> newArray(new Array(size + 1));

                if (size > 0) {
                    decaf::lang::System::arraycopy<E>(oldArray->elements, 0, newArray->elements, 0, index);
                }

                if (size > index) {
                    decaf::lang::System::arraycopy<E>(oldArray->elements, index, newArray->elements, index + 1, size - index);
                }

                newArray->elements[index] = element;
                newArray->size = size + 1;
                this->array.swap(newArray);
                this->arrayLock.writeLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        virtual bool addAll(int index, const Collection<E>& collection) {
            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;
                this->checkIndexInclusive(index, size);
                int csize = collection.size();

                if (csize == 0) {
                    this->arrayLock.writeLock().unlock();
                    return false;
                }

                decaf::lang::Pointer<Array> newArray(new Array(size + csize));
                if (size > 0) {
                    decaf::lang::System::arraycopy(oldArray->elements, 0, newArray->elements, 0, index);
                }

                std::auto_ptr<Iterator<E> > iter(collection.iterator());
                int pos = index;
                while (iter->hasNext()) {
                    newArray->elements[pos++] = iter->next();
                }

                if (size > index) {
                    decaf::lang::System::arraycopy(oldArray->elements, index, newArray->elements, index + csize, size - index);
                }
                newArray->size = size + csize;
                this->array.swap(newArray);
                this->arrayLock.writeLock().unlock();
                return true;
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        virtual E removeAt(int index) {
            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;
                this->checkIndexExclusive(index, size);
                E old = oldArray->elements[index];

                if (size == 1) {
                    this->array.reset(new Array());
                    this->arrayLock.writeLock().unlock();
                    return old;
                }

                decaf::lang::Pointer<Array> newArray(new Array(size - 1));
                decaf::lang::System::arraycopy<E>(oldArray->elements, 0, newArray->elements, 0, index);

                if (size > index) {
                    decaf::lang::System::arraycopy<E>(oldArray->elements, index + 1, newArray->elements, index, size - index - 1);
                }

                newArray->size = size - 1;
                this->array.swap(newArray);
                this->arrayLock.writeLock().unlock();
                return old;
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        virtual std::string toString() const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }

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
        bool addIfAbsent(const E& value) {
            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;

                if (size != 0) {
                    if (this->indexOf(value) != -1) {
                        this->arrayLock.writeLock().unlock();
                        return false;
                    }
                }

                decaf::lang::Pointer<Array> newArray(new Array(*oldArray, size + 1));
                newArray->elements[size] = value;
                newArray->size++;
                this->array.swap(newArray);
                this->arrayLock.writeLock().unlock();
                return true;
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
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
        int addAllAbsent(const Collection<E>& collection) {

            if (collection.size() == 0) {
                return 0;
            }

            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;
                decaf::lang::Pointer<Array> buffer(new Array(collection.size()));
                int count = 0;

                std::auto_ptr<Iterator<E> > iter(collection.iterator());
                while (iter->hasNext()) {
                    E value = iter->next();
                    if (this->indexOf(value) == -1) {
                        buffer->elements[count++] = value;
                        buffer->size++;
                    }
                }

                decaf::lang::Pointer<Array> newArray(new Array(*oldArray, size + count));
                decaf::lang::System::arraycopy(buffer->elements, 0, newArray->elements, size, count);
                newArray->size = size + count;
                this->array.swap(newArray);
                this->arrayLock.writeLock().unlock();
                return count;
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
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
        int lastIndexOf(const E& value, int index) {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }


            if (index >= current->size) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index given %d, actual size %d", index, current->size);
            }

            for (int i = index - 1; i >= 0; --i) {
                if (current->elements[i] == value) {
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
        int indexOf(const E& value, int index) const {
            decaf::lang::Pointer<Array> current;
            this->arrayLock.readLock().lock();
            try {
                current = this->array;
                this->arrayLock.readLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.readLock().unlock();
                throw;
            }


            if (index < 0) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index given %d, actual size %d", index, current->size);
            }

            for (int i = index; i < current->size; ++i) {
                if (current->elements[i] == value) {
                    return i;
                }
            }

            return -1;
        }

    public:  // Synchronizable

        virtual void lock() {
            this->arrayLock.writeLock().lock();
        }

        virtual bool tryLock() {
            return this->arrayLock.writeLock().tryLock();
        }

        virtual void unlock() {
            this->arrayLock.writeLock().unlock();
        }

        virtual void wait()  {
            this->condition->await();
        }

        virtual void wait(long long millisecs) {
            this->condition->await(millisecs, decaf::util::concurrent::TimeUnit::MILLISECONDS);
        }

        virtual void wait( long long millisecs, int nanos ) {
            long long timeout = decaf::util::concurrent::TimeUnit::MILLISECONDS.toNanos(millisecs) + nanos;
            this->condition->awaitNanos(timeout);
        }

        virtual void notify() {
            this->condition->signal();
        }

        virtual void notifyAll() {
            this->condition->signalAll();
        }

    private:

        void doCopyCollection(const Collection<E>& collection) {

            if ((void*) this == &collection || collection.isEmpty()) {
                return;
            }

            this->arrayLock.writeLock().lock();
            try {
                decaf::lang::Pointer<Array> oldArray = this->array;
                int size = oldArray->size;

                decaf::lang::Pointer<Array> buffer(new Array(*oldArray, size + collection.size()));
                std::auto_ptr<Iterator<E> > iter(collection.iterator());
                int index = 0;
                while (iter->hasNext()) {
                    buffer->elements[size + index++] = iter->next();
                    buffer->size++;
                }

                this->array.swap(buffer);
                this->arrayLock.writeLock().unlock();
            } catch (decaf::lang::Exception& ex) {
                this->arrayLock.writeLock().unlock();
                throw;
            }
        }

        static void checkIndexInclusive(int index, int size) {
            if (index < 0 || index > size) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index is %d, size is %d", index, size);
            }
        }

        static void checkIndexExclusive(int index, int size) {
            if (index < 0 || index >= size) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Index is %d, size is %d", index, size);
            }
        }

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYLIST_H_ */
