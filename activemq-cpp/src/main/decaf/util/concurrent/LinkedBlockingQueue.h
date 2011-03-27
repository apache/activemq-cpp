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

#ifndef _DECAF_UTIL_CONCURRENT_LINKEDBLOCKINGQUEUE_H_
#define _DECAF_UTIL_CONCURRENT_LINKEDBLOCKINGQUEUE_H_

#include <decaf/util/Config.h>

#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/Lock.h>
#include <decaf/util/concurrent/BlockingQueue.h>
#include <decaf/util/AbstractQueue.h>
#include <decaf/util/Iterator.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

namespace decaf {
namespace util {
namespace concurrent {

    using decaf::lang::Pointer;

    /**
     * A BlockingQueue derivative that allows for a bound to be placed on the number of elements
     * that can be enqueued at any one time.  Elements are inserted and removed in FIFO order.
     * The internal structure of the queue is based on a linked nodes which provides for better
     * performance over their array based versions but the performance is less predictable.
     *
     * The capacity bound of this class default to Integer::MAX_VALUE.
     *
     * @since 1.0
     */
    template<typename E>
    class LinkedBlockingQueue : public BlockingQueue<E> {
    private:

        template< typename U >
        class QueueNode {
        private:

            U value;
            bool unlinked;
            bool dequeued;

        public:

            Pointer< QueueNode<E> > next;

        private:

            QueueNode( const QueueNode& );
            QueueNode& operator= ( const QueueNode& );

        public:

            QueueNode() : value(), unlinked(false), dequeued(false), next() {}
            QueueNode(const U& value) : value(value), unlinked(false), dequeued(false), next() {}

            void set(Pointer< QueueNode<U> > next, const U& value) {
                this->next = next;
                this->value = value;
                this->unlinked = false;
                this->dequeued = false;
            }

            E get() const {
                return this->value;
            }

            E getAndDequeue() {
                E result = this->value;
                this->value = E();
                this->dequeued = true;

                return result;
            }

            void unlink() {
                this->value = E();
                this->unlinked = true;
            }

            bool isUnlinked() const {
                return this->unlinked;
            }

            bool isDequeued() const {
                return this->dequeued;
            }
        };

        class TotalLock {
        private:

            TotalLock( const TotalLock& src );
            TotalLock& operator= ( const TotalLock& src );

        private:

            const LinkedBlockingQueue<E>* parent;

        public:

            TotalLock( const LinkedBlockingQueue<E>* parent ) : parent(parent) {
                parent->putLock.lock();
                parent->takeLock.lock();
            }

            ~TotalLock() {
                parent->putLock.unlock();
                parent->takeLock.unlock();
            }

        };

    private:

        int capacity;
        decaf::util::concurrent::atomic::AtomicInteger count;

        mutable decaf::util::concurrent::Mutex putLock;
        mutable decaf::util::concurrent::Mutex takeLock;

        Pointer< QueueNode<E> > head;
        Pointer< QueueNode<E> > tail;

    public:

        /**
         * Create a new instance with a Capacity of Integer::MAX_VALUE
         */
        LinkedBlockingQueue() : BlockingQueue<E>(), capacity(lang::Integer::MAX_VALUE), count(),
                                putLock(), takeLock(), head(new QueueNode<E>()), tail() {

            this->tail = this->head;
        }

        /**
         * Create a new instance with the given initial capacity value.
         *
         * @param capacity
         *      The initial capacity value to assign to this Queue.
         *
         * @throws IllegalArgumentException if the specified capacity is not greater than zero.
         */
        LinkedBlockingQueue(int capacity) : BlockingQueue<E>(), capacity(capacity), count(),
                                            putLock(), takeLock(), head(new QueueNode<E>()), tail() {
            if(capacity <= 0) {
                throw decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__, "Capacity value must be greater than zero.");
            }

            this->tail = this->head;
        }

        /**
         * Create a new instance with a Capacity of Integer::MAX_VALUE and adds all the
         * values contained in the specified collection to this Queue.
         *
         * @param collection
         *      The Collection whose elements are to be copied to this Queue.
         *
         * @throws IllegalStateException if the number of elements in the collection exceeds
         *         this Queue's capacity.
         */
        LinkedBlockingQueue(const Collection<E>& collection) : BlockingQueue<E>(),
                                                               capacity(lang::Integer::MAX_VALUE),
                                                               count(), putLock(), takeLock(),
                                                               head(new QueueNode<E>()), tail() {

            this->tail = this->head;

            Pointer< Iterator<E> > iter(collection.iterator());

            try {

                int count = 0;

                while(iter->hasNext()) {
                    if(count == this->capacity) {
                        throw decaf::lang::exceptions::IllegalStateException( __FILE__, __LINE__,
                            "Number of elements in the Collection exceeds this Queue's Capacity.");
                    }

                    this->enqueue(iter->next());
                    ++count;
                }

                this->count.set(count);
            }
            DECAF_CATCH_RETHROW(decaf::lang::exceptions::IllegalStateException)
            DECAF_CATCH_RETHROW(decaf::lang::Exception)
            DECAF_CATCHALL_THROW(decaf::lang::Exception)
        }

        virtual ~LinkedBlockingQueue() {
            try{
                this->purgeList();
            } catch(...) {}
        }

    public:

        LinkedBlockingQueue<E>& operator= ( const LinkedBlockingQueue<E>& queue ) {
            this->clear();
            this->addAll(queue);
            return *this;
        }

        LinkedBlockingQueue<E>& operator= ( const Collection<E>& collection ) {
            this->clear();
            this->addAll(collection);
            return *this;
        }

    public:

        virtual int size() const {
            return this->count.get();
        }

        virtual void clear() {

            TotalLock lock(this);

            this->purgeList();
            this->tail = this->head;
            this->count.set(0);

            if(this->count.getAndSet(0) == this->capacity) {
                this->putLock.notify();
            }
        }

        virtual int remainingCapacity() const {
            return this->capacity - this->count.get();
        }

        virtual void put( const E& value ) {

            int c = -1;

            synchronized(&this->putLock) {

                while(this->count.get() == this->capacity) {
                    this->putLock.wait();
                }

                // This method now owns the putLock so we know we have at least
                // enough capacity for one put, if we enqueue an item and there's
                // still more room we should signal a waiting put to ensure that
                // threads don't wait forever.
                enqueue(value);
                c = this->count.getAndIncrement();

                if(c + 1 < this->capacity) {
                    this->putLock.notify();
                }
            }

            // When c is zero it means we at least incremented once so there was
            // something in the Queue, another take could have already happened but
            // we don't know so wake up a waiting taker.
            if(c == 0) {
                this->signalNotEmpty();
            }
        }

        virtual bool offer( const E& value, long long timeout, const TimeUnit& unit ) {

            int c = -1;

            synchronized(&this->putLock) {

                while(this->count.get() == this->capacity) {
                    this->putLock.wait(unit.toMillis(timeout));
                }

                enqueue(value);
                c = this->count.getAndIncrement();

                if(c + 1 < this->capacity) {
                    this->putLock.notify();
                }
            }

            if(c == 0) {
                this->signalNotEmpty();
            }

            return true;
        }

        virtual bool offer( const E& value ) {

            if(this->count.get() == this->capacity) {
                return false;
            }

            int c = -1;
            synchronized(&this->putLock) {
                if(this->count.get() < this->capacity) {

                    enqueue(value);
                    c = this->count.getAndIncrement();

                    if(c + 1 < this->capacity) {
                        this->putLock.notify();
                    }
                }
            }

            if(c == 0) {
                this->signalNotEmpty();
            }

            return c >= 0;
        }

        virtual E take() {
            E value;
            int c = -1;
            synchronized(&this->takeLock) {

                while(this->count.get() == 0) {
                     this->takeLock.wait();
                }

                // Since this methods owns the takeLock and count != 0 we know that
                // its safe to take one element.  if c is greater than one then there
                // is at least one more so we try to wake up another taker if any.
                value = dequeue();
                c = this->count.getAndDecrement();

                if(c > 1) {
                    this->takeLock.notify();
                }
            }

            // When c equals capacity we have removed at least one element
            // from the Queue so we wake a blocked put operation if there is
            // one to prevent a deadlock.
            if(c == this->capacity) {
                this->signalNotFull();
            }

            return value;
        }

        virtual bool poll( E& result, long long timeout, const TimeUnit& unit ) {
            int c = -1;
            synchronized(&this->takeLock) {
                if(this->count.get() == 0) {
                    if(timeout <= 0) {
                        return false;
                    }
                    this->takeLock.wait(unit.toMillis(timeout));
                    if(this->count.get() == 0) {
                        return false;
                    }
                }

                result = dequeue();
                c = this->count.getAndDecrement();

                if(c > 1) {
                    this->takeLock.notify();
                }
            }

            if(c == this->capacity) {
                this->signalNotFull();
            }

            return true;
        }

        virtual bool poll(E& result) {

            if(this->count.get() == 0) {
                return false;
            }

            int c = -1;
            synchronized(&this->takeLock) {

                if(this->count.get() > 0) {
                    result = dequeue();
                    c = this->count.getAndDecrement();

                    if(c > 1) {
                        this->takeLock.notify();
                    }
                }
            }

            if(c == this->capacity) {
                this->signalNotFull();
            }

            return true;
        }

        virtual bool peek(E& result) const {

            if(this->count.get() == 0) {
                return false;
            }

            synchronized(&this->takeLock) {
                Pointer< QueueNode<E> > front = this->head->next;
                if(front == NULL) {
                    return false;
                } else {
                    result = front->get();
                }
            }

            return true;
        }

        using AbstractQueue<E>::remove;

        virtual bool remove(const E& value) {

            TotalLock lock(this);

            for(Pointer< QueueNode<E> > predicessor = this->head, p = predicessor->next; p != NULL;
                predicessor = p, p = p->next) {

                if(value == p->get()) {
                    unlink(p, predicessor);
                    return true;
                }
            }

            return false;
        }

        virtual std::vector<E> toArray() const {

            TotalLock lock(this);

            int size = this->count.get();
            std::vector<E> array;
            array.reserve(size);

            for(Pointer< QueueNode<E> > p = this->head->next; p != NULL; p = p->next) {
                array.push_back(p->get());
            }

            return array;
        }

        virtual std::string toString() const {
            return std::string("LinkedBlockingQueue [ current size = ") +
                   decaf::lang::Integer::toString(this->count.get()) + "]";
        }

        virtual int drainTo( Collection<E>& c ) {
            return this->drainTo(c, decaf::lang::Integer::MAX_VALUE);
        }

        virtual int drainTo( Collection<E>& sink, int maxElements ) {

            if(&sink == this) {
                throw decaf::lang::exceptions::IllegalArgumentException(__FILE__, __LINE__,
                    "Cannot drain this Collection to itself.");
            }

            bool signalNotFull = false;
            bool shouldThrow = false;
            decaf::lang::Exception delayed;
            int result = 0;

            synchronized(&this->takeLock) {

                // We get the count of Nodes that exist now, any puts that are done
                // after this are not drained and since we hold the lock nothing can
                // get taken so state should remain consistent.
                result = decaf::lang::Math::min(maxElements, this->count.get());
                Pointer< QueueNode<E> > node = this->head;
                int i = 0;
                try {

                    while(i < result) {
                        Pointer< QueueNode<E> > p = node->next;
                        sink.add( p->getAndDequeue() );
                        node = p;
                        ++i;
                    }

                } catch(decaf::lang::Exception& e) {
                    delayed = e;
                    shouldThrow = true;
                }

                if(i > 0) {
                    this->head = node;
                    signalNotFull = (this->count.getAndAdd(-i) == this->capacity);
                }
            }

            if(signalNotFull) {
                this->signalNotFull();
            }

            if(shouldThrow) {
                throw delayed;
            }

            return result;
        }

    private:

        class LinkedIterator : public Iterator<E> {
        private:

            Pointer< QueueNode<E> > current;
            Pointer< QueueNode<E> > last;
            E currentElement;
            LinkedBlockingQueue<E>* parent;

        public:

            LinkedIterator(LinkedBlockingQueue<E>* parent) : current(), last(),
                                                             currentElement(), parent(parent) {
                TotalLock lock(parent);

                this->current = parent->head->next;
                if(this->current != NULL) {
                    this->currentElement = current->get();
                }
            }

            virtual bool hasNext() const {
                return this->current != NULL;
            }

            virtual E next() {

                TotalLock lock(this->parent);

                if(this->current == NULL) {
                    throw decaf::util::NoSuchElementException(__FILE__, __LINE__,
                        "Iterator next called with no matching next element.");
                }

                E result = this->currentElement;
                this->last = this->current;
                this->current = this->nextNode(this->current);
                this->currentElement = (this->current == NULL) ? E() : this->current->get();

                return result;
            }

            virtual void remove() {

                if(this->last == NULL) {
                    throw decaf::lang::exceptions::IllegalStateException(__FILE__, __LINE__,
                        "Iterator remove called without having called next().");
                }

                TotalLock lock(this->parent);

                Pointer< QueueNode<E> > node;
                node.swap(this->last);

                for(Pointer< QueueNode<E> > trail = this->parent->head, p = trail->next; p != NULL;
                    trail = p, p = p->next) {

                    if(p == node) {
                        this->parent->unlink(p, trail);
                        break;
                    }
                }
            }

        private:

            Pointer< QueueNode<E> > nextNode(Pointer< QueueNode<E> >& p) {

                // Handle the case of a dequeued Node, the new head of Queue
                // will be parent->head->next() even if the Queue is empty.
                if(p->isDequeued()) {
                    return this->parent->head->next;
                }

                Pointer< QueueNode<E> > s = p->next;

                // Handle Nodes that have been removed from the interior of the
                // Queue, these are tagged but still retain their next() value
                // in order to account for multiple removes.  If all nodes were
                // removed from the last call then eventually we reach next() == NULL
                // which is the old tail.
                while(s != NULL && s->isUnlinked()) {
                    s = s->next;
                }

                return s;
            }

        };

        class ConstLinkedIterator : public Iterator<E> {
        private:

            Pointer< QueueNode<E> > current;
            Pointer< QueueNode<E> > last;
            E currentElement;
            const LinkedBlockingQueue<E>* parent;

        public:

            ConstLinkedIterator(const LinkedBlockingQueue<E>* parent) : current(), last(),
                                                                        currentElement(),
                                                                        parent(parent) {
                TotalLock lock(parent);

                this->current = parent->head->next;
                if(this->current != NULL) {
                    this->currentElement = current->get();
                }
            }

            virtual bool hasNext() const {
                return this->current != NULL;
            }

            virtual E next() {

                TotalLock lock(this->parent);

                if(this->current == NULL) {
                    throw decaf::util::NoSuchElementException(__FILE__, __LINE__,
                        "Iterator next called with no matching next element.");
                }

                E result = this->currentElement;
                this->last = this->current;
                this->current = this->nextNode(this->current);
                this->currentElement = (this->current == NULL) ? E() : this->current->get();

                return result;
            }

            virtual void remove() {
                throw lang::exceptions::UnsupportedOperationException(
                    __FILE__, __LINE__, "Cannot write to a const ListIterator." );
            }

        private:

            Pointer< QueueNode<E> > nextNode(Pointer< QueueNode<E> >& p) {

                // Handle the case of a dequeued Node, the new head of Queue
                // will be parent->head->next() even if the Queue is empty.
                if(p->isDequeued()) {
                    return this->parent->head->next;
                }

                Pointer< QueueNode<E> > s = p->next;

                // Handle Nodes that have been removed from the interior of the
                // Queue, these are tagged but still retain their next() value
                // in order to account for multiple removes.  If all nodes were
                // removed from the last call then eventually we reach next() == NULL
                // which is the old tail.
                while(s != NULL && s->isUnlinked()) {
                    s = s->next;
                }

                return s;
            }

        };

    public:

        virtual decaf::util::Iterator<E>* iterator() {
            return new LinkedIterator(this);
        }

        virtual decaf::util::Iterator<E>* iterator() const {
            return new ConstLinkedIterator(this);
        }

    private:

        void unlink(Pointer< QueueNode<E> >& p, Pointer< QueueNode<E> >& predicessor) {

            // In order to prevent Iterators from losing their ability to provide
            // weakly consistent iteration the next value of p is left intact but
            // the node is marked as unlinked and it value is reset to default.
            p->unlink();

            predicessor->next = p->next;

            if(this->tail == p) {
                this->tail = predicessor;
            }

            if(this->count.getAndDecrement() == capacity) {
                this->signalNotFull();
            }
        }

        void signalNotEmpty() {
            synchronized(&this->takeLock) {
                this->takeLock.notify();
            }
        }

        void signalNotFull() {
            synchronized(&this->putLock) {
                this->putLock.notify();
            }
        }

        // Must be called with the putLock locked.
        void enqueue(E value) {
            Pointer< QueueNode<E> > newTail( new QueueNode<E>(value) );
            this->tail->next = newTail;
            this->tail = newTail;
        }

        // Must be called with the takeLock locked.
        E dequeue() {
            Pointer< QueueNode<E> > temp = this->head;
            Pointer< QueueNode<E> > newHead = temp->next;
            this->head = newHead;

            return newHead->getAndDequeue();
        }

        void purgeList() {
            Pointer< QueueNode<E> > current = this->head->next;
            Pointer< QueueNode<E> > temp;
            while(current != NULL) {
                temp = current;
                current = current->next;
                temp.reset(NULL);
            }
        }
    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_LINKEDBLOCKINGQUEUE_H_ */
