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

#include "LinkedBlockingQueueTest.h"

#include <decaf/util/LinkedList.h>
#include <decaf/util/concurrent/Executors.h>
#include <decaf/util/concurrent/LinkedBlockingQueue.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
const int LinkedBlockingQueueTest::SIZE = 256;

////////////////////////////////////////////////////////////////////////////////
namespace {

    void populate( LinkedBlockingQueue<int>& queue, int n ) {

        CPPUNIT_ASSERT( queue.isEmpty() );

        for( int i = 0; i < n; ++i ) {
            queue.add( i );
        }

        CPPUNIT_ASSERT( !queue.isEmpty());
        CPPUNIT_ASSERT_EQUAL( n, queue.size() );
    }

    void populate( LinkedList<int>& list, int n ) {

        CPPUNIT_ASSERT( list.isEmpty() );

        for( int i = 0; i < n; ++i ) {
            list.add( i );
        }

        CPPUNIT_ASSERT( !list.isEmpty());
        CPPUNIT_ASSERT_EQUAL( n, list.size() );
    }

    void populate( LinkedBlockingQueue<std::string>& queue, int n ) {

        CPPUNIT_ASSERT( queue.isEmpty() );

        for( int i = 0; i < n; ++i ) {
            queue.add( Integer::toString( i ) );
        }

        CPPUNIT_ASSERT( !queue.isEmpty());
        CPPUNIT_ASSERT_EQUAL( n, queue.size() );
    }

    void populate( std::vector<int>& list, int n ) {

        CPPUNIT_ASSERT( list.empty() );

        for( int i = 0; i < n; ++i ) {
            list.push_back( i );
        }

        CPPUNIT_ASSERT( !list.empty());
        CPPUNIT_ASSERT_EQUAL( n, (int)list.size() );
    }
}

///////////////////////////////////////////////////////////////////////////////
LinkedBlockingQueueTest::LinkedBlockingQueueTest() {
}

///////////////////////////////////////////////////////////////////////////////
LinkedBlockingQueueTest::~LinkedBlockingQueueTest() {
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testConstructor1() {

    LinkedBlockingQueue<int> queue;

    CPPUNIT_ASSERT_EQUAL(0, queue.size());
    CPPUNIT_ASSERT(queue.isEmpty());
    CPPUNIT_ASSERT_EQUAL((int)Integer::MAX_VALUE, queue.remainingCapacity());
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testConstructor2() {

    LinkedBlockingQueue<int> queue(SIZE);

    CPPUNIT_ASSERT_EQUAL(0, queue.size());
    CPPUNIT_ASSERT(queue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(SIZE, queue.remainingCapacity());
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testConstructor3() {

    LinkedList<int> list;
    populate(list, SIZE);

    LinkedBlockingQueue<int> q(list);

    for (int i = 0; i < SIZE; ++i) {
        int result;
        CPPUNIT_ASSERT(q.poll(result));
        CPPUNIT_ASSERT_EQUAL(list.get(i), result);
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testConstructor4() {

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IllegalArgumentException",
        LinkedBlockingQueue<int>(-1),
        IllegalArgumentException);
}

//////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testEquals() {

    LinkedBlockingQueue<int> q1;
    populate( q1, 7 );
    LinkedBlockingQueue<int> q2;
    populate( q2, 7 );

    CPPUNIT_ASSERT( q1.equals( q2 ) );
    CPPUNIT_ASSERT( q2.equals( q1 ) );

    q1.add( 42 );
    CPPUNIT_ASSERT( !q1.equals( q2 ) );
    CPPUNIT_ASSERT( !q2.equals( q1 ) );
    q2.add( 42 );
    CPPUNIT_ASSERT( q1.equals( q2 ) );
    CPPUNIT_ASSERT( q2.equals( q1 ) );
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testEmptyFull() {

    LinkedBlockingQueue<int> q(2);
    CPPUNIT_ASSERT(q.isEmpty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("should have room for 2", 2, q.remainingCapacity());
    q.add(1);
    CPPUNIT_ASSERT(!q.isEmpty());
    q.add(2);
    CPPUNIT_ASSERT(!q.isEmpty());
    CPPUNIT_ASSERT_EQUAL(0, q.remainingCapacity());
    CPPUNIT_ASSERT(!q.offer(3));
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testRemainingCapacity() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT_EQUAL(i, q.remainingCapacity());
        CPPUNIT_ASSERT_EQUAL(SIZE - i, q.size());
        q.remove();
    }
    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT_EQUAL(SIZE - i, q.remainingCapacity());
        CPPUNIT_ASSERT_EQUAL(i, q.size());
        q.add(i);
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testOffer() {

    LinkedBlockingQueue<int> q(1);
    CPPUNIT_ASSERT(q.offer(0));
    CPPUNIT_ASSERT(!q.offer(1));
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testAdd() {

    LinkedBlockingQueue<int> q(SIZE);
    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT(q.add(i));
    }
    CPPUNIT_ASSERT_EQUAL(0, q.remainingCapacity());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IllegalStateException",
        q.add(SIZE),
        IllegalStateException);
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testAddAllSelf() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IllegalArgumentException",
        q.addAll(q),
        IllegalArgumentException);
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testAddAll1() {

    LinkedBlockingQueue<int> q(1);
    LinkedList<int> list;

    populate(list, SIZE);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IllegalStateException",
        q.addAll(list),
        IllegalStateException);
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testAddAll2() {

    LinkedBlockingQueue<int> q(SIZE);
    LinkedList<int> empty;
    LinkedList<int> list;
    populate(list, SIZE);

    CPPUNIT_ASSERT(!q.addAll(empty));
    CPPUNIT_ASSERT(q.addAll(list));

    for (int i = 0; i < SIZE; ++i) {
        int result;
        CPPUNIT_ASSERT(q.poll(result));
        CPPUNIT_ASSERT_EQUAL(list.get(i), result);
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testPut() {

    LinkedBlockingQueue<int> q(SIZE);
    for(int i = 0; i < SIZE; ++i) {
        q.put(i);
        CPPUNIT_ASSERT(q.contains(i));
    }
    CPPUNIT_ASSERT_EQUAL(0, q.remainingCapacity());
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testTake() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT_EQUAL(i, q.take());
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testPoll() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);
    int result;

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT(q.poll(result));
        CPPUNIT_ASSERT_EQUAL(i, result);
    }

    CPPUNIT_ASSERT(!q.poll(result));
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testTimedPoll1() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);
    int result;

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT(q.poll(result, 0, TimeUnit::MILLISECONDS));
        CPPUNIT_ASSERT_EQUAL(i, result);
    }

    CPPUNIT_ASSERT(!q.poll(result, 0, TimeUnit::MILLISECONDS));
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testTimedPoll2() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);
    int result;

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT(q.poll(result, 100, TimeUnit::MILLISECONDS));
        CPPUNIT_ASSERT_EQUAL(i, result);
    }

    CPPUNIT_ASSERT(!q.poll(result, 100, TimeUnit::MILLISECONDS));
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testPeek() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);
    int result;

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT(q.peek(result));
        CPPUNIT_ASSERT_EQUAL(i, result);
        q.poll(result);
        CPPUNIT_ASSERT(q.peek(result) == false || i != result);
    }

    CPPUNIT_ASSERT(!q.peek(result));
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testElement() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);
    int result;

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT_EQUAL(i, q.element());
        q.poll(result);
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NoSuchElementException",
        q.element(),
        NoSuchElementException);
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testRemove() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT_EQUAL(i, q.remove());
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NoSuchElementException",
        q.remove(),
        NoSuchElementException);
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testRemoveElement() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);

    for(int i = 1; i < SIZE; i += 2) {
        CPPUNIT_ASSERT(q.remove(i));
    }

    for(int i = 0; i < SIZE; i += 2) {
        CPPUNIT_ASSERT(q.remove(i));
        CPPUNIT_ASSERT(!q.remove(i + 1));
    }

    CPPUNIT_ASSERT(q.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testRemoveElement2() {

    LinkedBlockingQueue<int> q;
    populate( q, SIZE );

    CPPUNIT_ASSERT_MESSAGE( "Failed to remove valid Object", q.remove(42) );
    CPPUNIT_ASSERT_MESSAGE( "Removed invalid object", !q.remove(999) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Found Object after removal", false, q.contains(42) );
    q.add(SIZE+1);
    q.remove(SIZE+1);
    CPPUNIT_ASSERT_MESSAGE( "Should not contain null afrer removal", !q.contains(SIZE+1) );
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testRemoveElementAndAdd() {

    LinkedBlockingQueue<int> q;

    CPPUNIT_ASSERT(q.add(1));
    CPPUNIT_ASSERT(q.add(2));
    CPPUNIT_ASSERT(q.remove(1));
    CPPUNIT_ASSERT(q.remove(2));
    CPPUNIT_ASSERT(q.add(3));
    CPPUNIT_ASSERT(q.take() == 3);
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testContains() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT(q.contains(i));
        q.remove();
        CPPUNIT_ASSERT(!q.contains(i));
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testClear() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);

    q.clear();
    CPPUNIT_ASSERT(q.isEmpty());
    CPPUNIT_ASSERT_EQUAL(0, q.size());
    CPPUNIT_ASSERT_EQUAL(SIZE, q.remainingCapacity());
    q.add(1);
    CPPUNIT_ASSERT(!q.isEmpty());
    CPPUNIT_ASSERT(q.contains(1));
    q.clear();
    CPPUNIT_ASSERT(q.isEmpty());
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testContainsAll() {

    LinkedBlockingQueue<int> q;
    LinkedBlockingQueue<int> p;
    populate(q, SIZE);

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT(q.containsAll(p));
        CPPUNIT_ASSERT(!p.containsAll(q));
        p.add(i);
    }
    CPPUNIT_ASSERT(p.containsAll(q));
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testRetainAll() {
    LinkedBlockingQueue<int> q;
    LinkedBlockingQueue<int> p;
    populate(q, SIZE);
    populate(p, SIZE);

    for(int i = 0; i < SIZE; ++i) {
        bool changed = q.retainAll(p);
        if(i == 0) {
            CPPUNIT_ASSERT(!changed);
        } else {
            CPPUNIT_ASSERT(changed);
        }

        CPPUNIT_ASSERT(q.containsAll(p));
        CPPUNIT_ASSERT_EQUAL(SIZE-i, q.size());
        p.remove();
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testRemoveAll() {

    for (int i = 1; i < SIZE; ++i) {
        LinkedBlockingQueue<int> q;
        LinkedBlockingQueue<int> p;
        populate(q, SIZE);
        populate(p, i);

        CPPUNIT_ASSERT(q.removeAll(p));
        CPPUNIT_ASSERT_EQUAL(SIZE-i, q.size());
        for (int j = 0; j < i; ++j) {
            int result = p.remove();
            CPPUNIT_ASSERT(!q.contains(result));
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testToArray() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);

    std::vector<int> array = q.toArray();
    for(int i = 0; i < (int)array.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(array[i], q.take());
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testDrainToSelf() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IllegalArgumentException",
        q.drainTo(q),
        IllegalArgumentException);
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testDrainTo() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);
    LinkedList<int> list;

    q.drainTo(list);

    CPPUNIT_ASSERT_EQUAL(q.size(), 0);
    CPPUNIT_ASSERT_EQUAL(list.size(), SIZE);

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT_EQUAL(list.get(i), i);
    }

    q.add(0);
    q.add(1);
    CPPUNIT_ASSERT(!q.isEmpty());
    CPPUNIT_ASSERT(q.contains(0));
    CPPUNIT_ASSERT(q.contains(1));
    list.clear();

    q.drainTo(list);

    CPPUNIT_ASSERT_EQUAL(q.size(), 0);
    CPPUNIT_ASSERT_EQUAL(list.size(), 2);
    for(int i = 0; i < 2; ++i) {
        CPPUNIT_ASSERT_EQUAL(list.get(i), i);
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testDrainToSelfN() {

    LinkedBlockingQueue<int> q(SIZE);
    populate(q, SIZE);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IllegalArgumentException",
        q.drainTo(q, SIZE),
        IllegalArgumentException);
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testDrainToN() {

    LinkedBlockingQueue<int> q;

    for(int i = 0; i < SIZE + 2; ++i) {

        for(int j = 0; j < SIZE; j++) {
            CPPUNIT_ASSERT(q.offer(j));
        }
        LinkedList<int> list;
        q.drainTo(list, i);
        int k = (i < SIZE) ? i : SIZE;
        CPPUNIT_ASSERT_EQUAL(list.size(), k);
        CPPUNIT_ASSERT_EQUAL(q.size(), SIZE - k);
        for(int j = 0; j < k; ++j) {
            CPPUNIT_ASSERT_EQUAL(list.get(j), j);
        }

        int temp;
        while(q.poll(temp) != false);
    }
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class PutThread : public Thread {
    private:

        LinkedBlockingQueue<int>* theQ;
        int putValue;

    private:

        PutThread(const PutThread&);
        PutThread operator= (const PutThread&);

    public:

        PutThread(LinkedBlockingQueue<int>* q, int putValue) : theQ(q), putValue(putValue) {}

        virtual void run() {
            try {
                theQ->put(putValue);
            } catch(InterruptedException& ie){
                // TODO deal with exceptions in threads.
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testDrainToWithActivePut() {

    LinkedBlockingQueue<int> q;
    populate(q, SIZE);

    PutThread t(&q, SIZE+1);

    t.start();

    LinkedList<int> list;
    q.drainTo(list);
    CPPUNIT_ASSERT(list.size() >= SIZE);

    for(int i = 0; i < SIZE; ++i) {
        CPPUNIT_ASSERT_EQUAL(list.get(i), i);
    }

    t.join();

    CPPUNIT_ASSERT(q.size() + list.size() >= SIZE);
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testIterator() {

    LinkedBlockingQueue<int> q;
    populate(q, SIZE);

    Pointer< Iterator<int> > iter(q.iterator());

    while(iter->hasNext()) {
        CPPUNIT_ASSERT_EQUAL(iter->next(), q.take());
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testIteratorRemove () {

    LinkedBlockingQueue<int> q(3);

    q.add(2);
    q.add(1);
    q.add(3);

    Pointer< Iterator<int> > iter(q.iterator());
    iter->next();
    iter->remove();

    iter.reset(q.iterator());

    CPPUNIT_ASSERT_EQUAL(iter->next(), 1);
    CPPUNIT_ASSERT_EQUAL(iter->next(), 3);
    CPPUNIT_ASSERT(!iter->hasNext());
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testIteratorOrdering() {

    LinkedBlockingQueue<int> q(3);
    q.add(1);
    q.add(2);
    q.add(3);

    CPPUNIT_ASSERT_EQUAL(0, q.remainingCapacity());
    int k = 0;

    Pointer< Iterator<int> > iter(q.iterator());

    while(iter->hasNext()) {
        int i = iter->next();
        CPPUNIT_ASSERT_EQUAL(++k, i);
    }
    CPPUNIT_ASSERT_EQUAL(3, k);
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testWeaklyConsistentIteration () {

    LinkedBlockingQueue<int> q(3);
    q.add(1);
    q.add(2);
    q.add(3);

    Pointer< Iterator<int> > iter(q.iterator());

    while(iter->hasNext()) {
        q.remove();
        iter->next();
    }

    CPPUNIT_ASSERT_EQUAL(0, q.size());
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class PuttingThread : public Thread {
    private:

        LinkedBlockingQueue<int>* theQ;
        int count;

    private:

        PuttingThread(const PuttingThread&);
        PuttingThread operator= (const PuttingThread&);

    public:

        PuttingThread(LinkedBlockingQueue<int>* q, int count) : theQ(q), count(count) {}

        virtual ~PuttingThread() {}

        virtual void run() {
            try {
                for(int i = 0; i < count; ++i) {
                    theQ->put(i);
                    Thread::sleep(1);
                }
            } catch(InterruptedException& ie){
                // TODO deal with exceptions in threads.
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class TakingThread : public Thread {
    private:

        LinkedBlockingQueue<int>* theQ;
        int count;
        LinkedList<int>* list;

    private:

        TakingThread(const TakingThread&);
        TakingThread operator= (const TakingThread&);

    public:

        TakingThread(LinkedBlockingQueue<int>* q, LinkedList<int>* list, int count) :
            theQ(q), count(count), list(list) {}

        virtual void run() {
            try {
                for(int i = 0; i < count; ++i) {
                    list->add(theQ->take());
                    Thread::sleep(1);
                }
            } catch(InterruptedException& ie){
                // TODO deal with exceptions in threads.
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testConcurrentPut() {

    {
        LinkedBlockingQueue<int> q;
        PuttingThread t(&q, SIZE);
        LinkedList<int> list;

        t.start();

        for(int i = 0; i < SIZE; ++i) {
            list.add(q.take());
        }

        t.join();

        CPPUNIT_ASSERT(list.size() == SIZE);

        for(int i = 0; i < SIZE; ++i) {
            CPPUNIT_ASSERT_EQUAL(list.get(i), i);
        }
    }
    {
        LinkedBlockingQueue<int> q;
        PuttingThread t1(&q, SIZE);
        PuttingThread t2(&q, SIZE);
        PuttingThread t3(&q, SIZE);
        PuttingThread t4(&q, SIZE);
        LinkedList<int> list;

        t1.start();
        t2.start();
        t3.start();
        t4.start();

        for(int i = 0; i < SIZE*4; ++i) {
            list.add(q.take());
        }

        t1.join();
        t2.join();
        t3.join();
        t4.join();

        CPPUNIT_ASSERT(list.size() == SIZE*4);
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testConcurrentTake() {

    {
        LinkedBlockingQueue<int> q;
        LinkedList<int> list;
        TakingThread t(&q, &list, SIZE);

        t.start();

        for(int i = 0; i < SIZE; ++i) {
            q.put(i);
        }

        t.join();

        CPPUNIT_ASSERT(list.size() == SIZE);

        for(int i = 0; i < SIZE; ++i) {
            CPPUNIT_ASSERT_EQUAL(list.get(i), i);
        }
    }
    {
        LinkedBlockingQueue<int> q;
        LinkedList<int> list1;
        TakingThread t1(&q, &list1, SIZE);
        LinkedList<int> list2;
        TakingThread t2(&q, &list2, SIZE);
        LinkedList<int> list3;
        TakingThread t3(&q, &list3, SIZE);
        LinkedList<int> list4;
        TakingThread t4(&q, &list4, SIZE);

        t1.start();
        t2.start();
        t3.start();
        t4.start();

        for(int i = 0; i < SIZE*4; ++i) {
            q.put(i);
        }

        t1.join();
        t2.join();
        t3.join();
        t4.join();

        CPPUNIT_ASSERT(list1.size() == SIZE);
        CPPUNIT_ASSERT(list2.size() == SIZE);
        CPPUNIT_ASSERT(list3.size() == SIZE);
        CPPUNIT_ASSERT(list4.size() == SIZE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testConcurrentPutAndTake() {

    {
        const int SCOPED_SIZE = SIZE * 5;
        LinkedBlockingQueue<int> q;
        LinkedList<int> list;
        PuttingThread p(&q, SCOPED_SIZE);
        TakingThread t(&q, &list, SCOPED_SIZE);

        t.start();
        Thread::sleep(20);
        p.start();

        p.join();
        t.join();

        CPPUNIT_ASSERT(list.size() == SCOPED_SIZE);

        for(int i = 0; i < SCOPED_SIZE; ++i) {
            CPPUNIT_ASSERT_EQUAL(list.get(i), i);
        }
    }

    {
        LinkedBlockingQueue<int> q;
        LinkedList<int> list1;
        LinkedList<int> list2;
        LinkedList<int> list3;
        LinkedList<int> list4;
        PuttingThread p1(&q, SIZE);
        PuttingThread p2(&q, SIZE);
        PuttingThread p3(&q, SIZE);
        PuttingThread p4(&q, SIZE);
        TakingThread t1(&q, &list1, SIZE);
        TakingThread t2(&q, &list2, SIZE);
        TakingThread t3(&q, &list3, SIZE);
        TakingThread t4(&q, &list4, SIZE);

        t1.start();
        Thread::sleep(20);
        p1.start();
        t2.start();
        Thread::sleep(20);
        p2.start();
        t3.start();
        Thread::sleep(20);
        p3.start();
        t4.start();
        Thread::sleep(20);
        p4.start();

        p1.join();
        t1.join();
        p2.join();
        t2.join();
        p3.join();
        t3.join();
        p4.join();
        t4.join();

        CPPUNIT_ASSERT(list1.size() == SIZE);
        CPPUNIT_ASSERT(list2.size() == SIZE);
        CPPUNIT_ASSERT(list3.size() == SIZE);
        CPPUNIT_ASSERT(list4.size() == SIZE);
    }
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class TestBlockingPutRunnable : public Runnable {
    private:

        LinkedBlockingQueueTest* test;

    private:

        TestBlockingPutRunnable(const TestBlockingPutRunnable&);
        TestBlockingPutRunnable operator= (const TestBlockingPutRunnable&);

    public:

        TestBlockingPutRunnable(LinkedBlockingQueueTest* test) : Runnable(), test(test) {

        }

        virtual ~TestBlockingPutRunnable() {}

        virtual void run() {
            int added = 0;
            try {
                LinkedBlockingQueue<int> q(LinkedBlockingQueueTest::SIZE);
                for (int i = 0; i < LinkedBlockingQueueTest::SIZE; ++i) {
                    q.put(i);
                    ++added;
                }
                q.put(LinkedBlockingQueueTest::SIZE);
                test->threadShouldThrow();
            } catch (InterruptedException& ie){
                test->threadAssertEquals(added, LinkedBlockingQueueTest::SIZE);
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testBlockingPut() {
    TestBlockingPutRunnable runnable(this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join();
    } catch (InterruptedException& ie) {
        unexpectedException();
    }
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class TestTimedOfferRunnable : public Runnable {
    private:

        LinkedBlockingQueue<int>* queue;
        LinkedBlockingQueueTest* test;

    private:

        TestTimedOfferRunnable(const TestTimedOfferRunnable&);
        TestTimedOfferRunnable operator= (const TestTimedOfferRunnable&);

    public:

        TestTimedOfferRunnable(LinkedBlockingQueue<int>* queue, LinkedBlockingQueueTest* test) :
            Runnable(), queue(queue), test(test) {
        }

        virtual ~TestTimedOfferRunnable() {}

        virtual void run() {
            try {
                queue->put(1);
                queue->put(2);
                test->threadAssertFalse(
                    queue->offer(3, LinkedBlockingQueueTest::SHORT_DELAY_MS, TimeUnit::MILLISECONDS));
                queue->offer(4, LinkedBlockingQueueTest::LONG_DELAY_MS, TimeUnit::MILLISECONDS);
                test->threadShouldThrow();
            } catch (InterruptedException& success) {
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testTimedOffer() {

    LinkedBlockingQueue<int> q(2);
    TestTimedOfferRunnable runnable(&q, this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SMALL_DELAY_MS);
        t.interrupt();
        t.join();
    } catch (Exception& ex) {
        unexpectedException(ex);
    }
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class TestTakeFromEmptyRunnable : public Runnable {
    private:

        LinkedBlockingQueue<int>* queue;
        LinkedBlockingQueueTest* test;

    private:

        TestTakeFromEmptyRunnable(const TestTakeFromEmptyRunnable&);
        TestTakeFromEmptyRunnable operator= (const TestTakeFromEmptyRunnable&);

    public:

        TestTakeFromEmptyRunnable(LinkedBlockingQueue<int>* queue, LinkedBlockingQueueTest* test) :
            Runnable(), queue(queue), test(test) {
        }

        virtual ~TestTakeFromEmptyRunnable() {}

        virtual void run() {
            try {
                queue->take();
                test->threadShouldThrow();
            } catch (InterruptedException& success) {
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testTakeFromEmpty() {

    LinkedBlockingQueue<int> q(2);
    TestTakeFromEmptyRunnable runnable(&q, this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join();
    } catch (Exception& e){
        unexpectedException();
    }
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class TestBlockingTakeRunnable : public Runnable {
    private:

        LinkedBlockingQueueTest* test;

    private:

        TestBlockingTakeRunnable(const TestBlockingTakeRunnable&);
        TestBlockingTakeRunnable operator= (const TestBlockingTakeRunnable&);

    public:

        TestBlockingTakeRunnable(LinkedBlockingQueueTest* test) :
            Runnable(), test(test) {
        }

        virtual ~TestBlockingTakeRunnable() {}

        virtual void run() {
            try {
                LinkedBlockingQueue<int> queue;
                populate(queue, LinkedBlockingQueueTest::SIZE);

                for (int i = 0; i < LinkedBlockingQueueTest::SIZE; ++i) {
                    test->threadAssertEquals(i, queue.take());
                }
                queue.take();
                test->threadShouldThrow();
            } catch (InterruptedException& success) {
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testBlockingTake() {

    TestBlockingTakeRunnable runnable(this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join();
    } catch (InterruptedException& ie) {
        unexpectedException(ie);
    }
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class TestInterruptedTimedPollRunnable : public Runnable {
    private:

        LinkedBlockingQueueTest* test;

    private:

        TestInterruptedTimedPollRunnable(const TestInterruptedTimedPollRunnable&);
        TestInterruptedTimedPollRunnable operator= (const TestInterruptedTimedPollRunnable&);

    public:

        TestInterruptedTimedPollRunnable(LinkedBlockingQueueTest* test) :
            Runnable(), test(test) {
        }

        virtual ~TestInterruptedTimedPollRunnable() {}

        virtual void run() {
            try {
                LinkedBlockingQueue<int> queue;
                populate(queue, LinkedBlockingQueueTest::SIZE);

                for (int i = 0; i < LinkedBlockingQueueTest::SIZE; ++i) {
                    test->threadAssertEquals(i, queue.take());
                }
                queue.take();
                int result = 0;
                test->threadAssertFalse(queue.poll(result, LinkedBlockingQueueTest::SHORT_DELAY_MS, TimeUnit::MILLISECONDS));
                test->threadShouldThrow();
            } catch (InterruptedException& success) {
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testInterruptedTimedPoll() {

    TestInterruptedTimedPollRunnable runnable(this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SHORT_DELAY_MS);
        t.interrupt();
        t.join();
    } catch (InterruptedException& ie) {
        unexpectedException();
    }
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class TestTimedPollWithOfferRunnable : public Runnable {
    private:

        LinkedBlockingQueue<int>* queue;
        LinkedBlockingQueueTest* test;

    private:

        TestTimedPollWithOfferRunnable(const TestTimedPollWithOfferRunnable&);
        TestTimedPollWithOfferRunnable operator= (const TestTimedPollWithOfferRunnable&);

    public:

        TestTimedPollWithOfferRunnable(LinkedBlockingQueue<int>* queue, LinkedBlockingQueueTest* test) :
            Runnable(), queue(queue), test(test) {
        }

        virtual ~TestTimedPollWithOfferRunnable() {}

        virtual void run() {
            try {
                int result = 0;
                test->threadAssertFalse(queue->poll(result, LinkedBlockingQueueTest::SHORT_DELAY_MS, TimeUnit::MILLISECONDS));
                test->threadAssertTrue(queue->poll(result, LinkedBlockingQueueTest::LONG_DELAY_MS, TimeUnit::MILLISECONDS));
                queue->poll(result, LinkedBlockingQueueTest::LONG_DELAY_MS, TimeUnit::MILLISECONDS);
                test->threadShouldThrow();
            } catch (InterruptedException& success) {
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testTimedPollWithOffer() {

    LinkedBlockingQueue<int> q(2);
    TestTimedPollWithOfferRunnable runnable(&q, this);
    Thread t(&runnable);

    try {
        t.start();
        Thread::sleep(SMALL_DELAY_MS);
        CPPUNIT_ASSERT(q.offer(0, SHORT_DELAY_MS, TimeUnit::MILLISECONDS));
        t.interrupt();
        t.join();
    } catch (Exception& e) {
        unexpectedException(e);
    }
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class TestOfferInExecutor1 : public Runnable {
    private:

        LinkedBlockingQueue<int>* queue;
        LinkedBlockingQueueTest* test;

    private:

        TestOfferInExecutor1(const TestOfferInExecutor1&);
        TestOfferInExecutor1 operator= (const TestOfferInExecutor1&);

    public:

        TestOfferInExecutor1(LinkedBlockingQueue<int>* queue, LinkedBlockingQueueTest* test) :
            Runnable(), queue(queue), test(test) {
        }

        virtual ~TestOfferInExecutor1() {}

        virtual void run() {
            test->threadAssertFalse(queue->offer(3));
            try {
                test->threadAssertTrue(queue->offer(3, LinkedBlockingQueueTest::MEDIUM_DELAY_MS, TimeUnit::MILLISECONDS));
                test->threadAssertEquals(0, queue->remainingCapacity());
            } catch (InterruptedException& e) {
                test->threadUnexpectedException(e);
            }
        }
    };

    class TestOfferInExecutor2 : public Runnable {
    private:

        LinkedBlockingQueue<int>* queue;
        LinkedBlockingQueueTest* test;

    private:

        TestOfferInExecutor2(const TestOfferInExecutor2&);
        TestOfferInExecutor2 operator= (const TestOfferInExecutor2&);

    public:

        TestOfferInExecutor2(LinkedBlockingQueue<int>* queue, LinkedBlockingQueueTest* test) :
            Runnable(), queue(queue), test(test) {
        }

        virtual ~TestOfferInExecutor2() {}

        virtual void run() {
            test->threadAssertFalse(queue->offer(3));
            try {
                Thread::sleep(LinkedBlockingQueueTest::SMALL_DELAY_MS);
                test->threadAssertEquals(1, queue->take());
            } catch (InterruptedException& e) {
                test->threadUnexpectedException(e);
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testOfferInExecutor() {

    LinkedBlockingQueue<int> q(2);
    TestOfferInExecutor1* runnable1 = new TestOfferInExecutor1(&q, this);
    TestOfferInExecutor2* runnable2 = new TestOfferInExecutor2(&q, this);
    q.add(1);
    q.add(2);
    Pointer<ExecutorService> executor(Executors::newFixedThreadPool(2));
    executor->execute(runnable1);
    executor->execute(runnable2);

    joinPool(executor.get());
}

///////////////////////////////////////////////////////////////////////////////
namespace {

    class TestPollInExecutor1 : public Runnable {
    private:

        LinkedBlockingQueue<int>* queue;
        LinkedBlockingQueueTest* test;

    private:

        TestPollInExecutor1(const TestPollInExecutor1&);
        TestPollInExecutor1 operator= (const TestPollInExecutor1&);

    public:

        TestPollInExecutor1(LinkedBlockingQueue<int>* queue, LinkedBlockingQueueTest* test) :
            Runnable(), queue(queue), test(test) {
        }

        virtual ~TestPollInExecutor1() {}

        virtual void run() {
            int result = 0;
            test->threadAssertFalse(queue->poll(result));
            try {
                test->threadAssertTrue(queue->poll(result, LinkedBlockingQueueTest::MEDIUM_DELAY_MS, TimeUnit::MILLISECONDS));
                test->threadAssertTrue(queue->isEmpty());
            } catch (InterruptedException& e) {
                test->threadUnexpectedException(e);
            }
        }
    };

    class TestPollInExecutor2 : public Runnable {
    private:

        LinkedBlockingQueue<int>* queue;
        LinkedBlockingQueueTest* test;

    private:

        TestPollInExecutor2(const TestPollInExecutor2&);
        TestPollInExecutor2 operator= (const TestPollInExecutor2&);

    public:

        TestPollInExecutor2(LinkedBlockingQueue<int>* queue, LinkedBlockingQueueTest* test) :
            Runnable(), queue(queue), test(test) {
        }

        virtual ~TestPollInExecutor2() {}

        virtual void run() {
            try {
                Thread::sleep(LinkedBlockingQueueTest::SMALL_DELAY_MS);
                queue->put(1);
            } catch (InterruptedException& e) {
                test->threadUnexpectedException(e);
            }
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
void LinkedBlockingQueueTest::testPollInExecutor() {

    LinkedBlockingQueue<int> q(2);
    TestPollInExecutor1* runnable1 = new TestPollInExecutor1(&q, this);
    TestPollInExecutor2* runnable2 = new TestPollInExecutor2(&q, this);
    Pointer<ExecutorService> executor(Executors::newFixedThreadPool(2));
    executor->execute(runnable1);
    executor->execute(runnable2);

    joinPool(executor.get());
}
