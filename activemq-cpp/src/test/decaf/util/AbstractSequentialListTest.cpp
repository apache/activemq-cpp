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

#include "AbstractSequentialListTest.h"

#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/util/AbstractSequentialList.h>
#include <decaf/util/LinkedList.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    template< typename E >
    class SimpleList : public AbstractSequentialList<E> {
    private:

        LinkedList<E> list;

    public:

        SimpleList() : AbstractSequentialList<E>(), list() {}
        virtual ~SimpleList() {}

        virtual ListIterator<E>* listIterator( int index ) {
            return list.listIterator( index );
        }
        virtual ListIterator<E>* listIterator( int index ) const {
            return list.listIterator( index );
        }

        virtual int size() const {
            return list.size();
        }

    };

    template< typename E >
    class MockListIterator : public ListIterator<E> {
    public:

        virtual ~MockListIterator() {}

        virtual E next() {
            throw UnsupportedOperationException();
        }

        virtual bool hasNext() const {
            throw UnsupportedOperationException();
        }

        virtual void remove() {
            throw UnsupportedOperationException();
        }

        virtual void add( const E& e ) {
            throw UnsupportedOperationException();
        }

        virtual void set( const E& e ) {
            throw UnsupportedOperationException();
        }

        virtual bool hasPrevious() const {
            throw UnsupportedOperationException();
        }

        virtual E previous() {
            throw UnsupportedOperationException();
        }

        virtual int nextIndex() const {
            throw UnsupportedOperationException();
        }

        virtual int previousIndex() const {
            throw UnsupportedOperationException();
        }

    };

    template< typename E >
    class MockAbstractSequentialList : public AbstractSequentialList<E> {
    public:

        virtual ~MockAbstractSequentialList() {}

        virtual ListIterator<E>* listIterator( int index ) {
            return new MockListIterator<E>();
        }
        virtual ListIterator<E>* listIterator( int index ) const {
            return new MockListIterator<E>();
        }

        virtual int size() const {
            return 0;
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
AbstractSequentialListTest::AbstractSequentialListTest() {
}

////////////////////////////////////////////////////////////////////////////////
AbstractSequentialListTest::~AbstractSequentialListTest() {
}

////////////////////////////////////////////////////////////////////////////////
void AbstractSequentialListTest::testAddAll() {

    LinkedList<int> collection;
    for( int i = 0; i < 50; ++i ) {
        collection.add( i );
    }

    SimpleList<int> list;
    list.addAll( collection );
    CPPUNIT_ASSERT_MESSAGE( "Should return true", list.addAll( 2, collection ) );
}

////////////////////////////////////////////////////////////////////////////////
void AbstractSequentialListTest::testGet() {

    SimpleList<int> list;

    list.add( 1 );
    list.add( 2 );

    CPPUNIT_ASSERT_EQUAL( 1, list.get( 0 ) );
    CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 ) );

    // get value by index which is out of bounds
    try {
        list.get( list.size() );
        CPPUNIT_FAIL("Should throw IndexOutOfBoundsException.");
    } catch( IndexOutOfBoundsException& e ) {
        // expected
    }

    try {
        list.get( -1 );
        CPPUNIT_FAIL("Should throw IndexOutOfBoundsException.");
    } catch( IndexOutOfBoundsException& e ) {
        // expected
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractSequentialListTest::testRemove() {

    SimpleList<int> list;
    list.add(1);

    CPPUNIT_ASSERT_EQUAL( 1, list.removeAt( 0 ) );

    list.add( 2 );
    CPPUNIT_ASSERT_EQUAL( 2, list.removeAt( 0 ) );

    // remove index is out of bounds
    try {
        list.removeAt( list.size() );
        CPPUNIT_FAIL("Should throw IndexOutOfBoundsException.");
    } catch( IndexOutOfBoundsException& e ) {
        // expected
    }
    try {
        list.removeAt( -1 );
        CPPUNIT_FAIL("Should throw IndexOutOfBoundsException.");
    } catch( IndexOutOfBoundsException& e ) {
        // expected
    }

    // list dont't support remove operation
    try {
        MockAbstractSequentialList<int> mylist;
        mylist.removeAt( 0 );
        CPPUNIT_FAIL("Should throw UnsupportedOperationException.");
    } catch( UnsupportedOperationException& e ) {
        // expected
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractSequentialListTest::testSet() {

    SimpleList<int> list;

    try {
        list.set( 0, 12 );
        CPPUNIT_FAIL("should throw IndexOutOfBoundsException");
    } catch( IndexOutOfBoundsException& e ) {
        // expected
    }
}
