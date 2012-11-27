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

#include "AbstractListTest.h"

#include <decaf/lang/Integer.h>
#include <decaf/util/AbstractList.h>
#include <vector>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    template<typename E>
    class SimpleList : public AbstractList<E> {
    private:

        std::vector<E> array;

    public:

        using AbstractList<E>::add;

    public:

        SimpleList() : AbstractList<E>(), array() {
        }

        virtual ~SimpleList() {}

        virtual E get( int index ) const {

            if( index < 0 || index >= (int)array.size() ) {
                throw IndexOutOfBoundsException();
            }

            return this->array[index];
        }

        virtual void add( int i, const E& value ) {
            if( i < 0 || i > (int)array.size() ) {
                throw IndexOutOfBoundsException();
            }

            this->array.insert( this->array.begin() + i, value );
        }

        virtual E removeAt( int i ) {
            if( i < 0 || i >= (int)array.size() ) {
                throw IndexOutOfBoundsException();
            }

            E oldValue = this->array[i];
            this->array.erase( this->array.begin() + i );
            return oldValue;
        }

        virtual int size() const {
            return (int)this->array.size();
        }
    };

    template<typename E>
    class MockArrayList : public AbstractList<E> {
    public:

        std::vector<E> array;

    public:

        using AbstractList<E>::add;

    public:

        MockArrayList() : AbstractList<E>(), array() {
        }

        virtual ~MockArrayList() {}

        virtual E get( int index ) const {
            if( index < 0 || index >= (int)array.size() ) {
                throw IndexOutOfBoundsException();
            }

            return this->array[index];
        }

        virtual void add( int i, const E& value ) {
            if( i < 0 || i > (int)array.size() ) {
                throw IndexOutOfBoundsException();
            }

            this->modCount += 10;
            this->array.insert( this->array.begin() + i, value );
        }

        virtual E removeAt( int i ) {
            if( i < 0 || i >= (int)array.size() ) {
                throw IndexOutOfBoundsException();
            }

            this->modCount++;
            E oldValue = this->array[i];
            this->array.erase( this->array.begin() + i );
            return oldValue;
        }

        virtual int size() const {
            return (int)this->array.size();
        }
    };

    template<typename E>
    class MockRemoveFailureArrayList : public AbstractList<E> {
    public:

        virtual ~MockRemoveFailureArrayList() {}

        virtual E get( int index ) const {
            return E();
        }

        virtual int size() const {
            return 0;
        }

        virtual E removeAt( int i ) {
            this->modCount += 2;
            return E();
        }

        int getModCount() const {
            return this->modCount;
        }

    };

    template<typename E>
    class MockList : public AbstractList<E> {
    private:

        std::vector<E> array;

    public:

        using AbstractList<E>::add;

    public:

        MockList() : AbstractList<E>(), array() {}
        virtual ~MockList() {}

        virtual E get( int index ) const {
            if( index < 0 || index >= (int)array.size() ) {
                throw IndexOutOfBoundsException();
            }

            return array[index];
        }

        virtual int size() const {
            return (int)array.size();
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
AbstractListTest::AbstractListTest() {
}

////////////////////////////////////////////////////////////////////////////////
AbstractListTest::~AbstractListTest() {
}

////////////////////////////////////////////////////////////////////////////////
void AbstractListTest::testIterator() {

    SimpleList<int> list;
    list.add( 10 );
    list.add( 20 );
    std::auto_ptr< Iterator<int> > iter( list.iterator() );

    CPPUNIT_ASSERT_EQUAL( 10, iter->next() );
    iter->remove();
    CPPUNIT_ASSERT_EQUAL( 20, iter->next() );
}

////////////////////////////////////////////////////////////////////////////////
void AbstractListTest::testListIterator() {

    int tempValue;
    SimpleList<int> list;

    list.add( 3 );
    list.add( 15 );
    list.add( 5 );
    list.add( 1 );
    list.add( 7 );

    std::auto_ptr< ListIterator<int> > iter( list.listIterator() );

    CPPUNIT_ASSERT_MESSAGE( "Should not have previous", !iter->hasPrevious() );
    CPPUNIT_ASSERT_MESSAGE( "Should have next", iter->hasNext() );
    tempValue = iter->next();
    CPPUNIT_ASSERT_MESSAGE( std::string( "next returned wrong value.  Wanted 3, got: " ) +
                            Integer::toString( tempValue ), tempValue == 3 );
    tempValue = iter->previous();

    SimpleList<std::string> list2;
    list2.add( std::string("1") );
    std::auto_ptr< ListIterator<std::string> > iter2( list2.listIterator() );
    iter2->add( std::string("2") );
    iter2->next();
    CPPUNIT_ASSERT_MESSAGE( "Should contain two elements", list2.size() == 2 );

    SimpleList<int> list3;
    std::auto_ptr< ListIterator<int> > it( list3.listIterator() );
    it->add( 1 );
    it->add( 2 );
    CPPUNIT_ASSERT_MESSAGE( "Should contain two elements", list3.size() == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void AbstractListTest::testIteratorNext() {

    MockArrayList<std::string> t;
    t.array.push_back( "a" );
    t.array.push_back( "b" );

    std::auto_ptr< Iterator<std::string> > it( t.iterator() );

    while( it->hasNext() ) {
        it->next();
    }

    try {
        it->next();
        CPPUNIT_FAIL( "Should throw NoSuchElementException" );
    } catch( NoSuchElementException& cme ) {
        // expected
    }

    t.add( "c" );
    try {
        it->remove();
        CPPUNIT_FAIL( "Should throw ConcurrentModificationException" );
    } catch( ConcurrentModificationException& cme ) {
        // expected
    }

    it.reset( t.iterator() );
    try {
        it->remove();
        CPPUNIT_FAIL( "Should throw IllegalStateException" );
    } catch( IllegalStateException& ise ) {
        // expected
    }

    std::string value = it->next();
    CPPUNIT_ASSERT_EQUAL( std::string("a"), value );
}

////////////////////////////////////////////////////////////////////////////////
void AbstractListTest::testRemove() {

    MockRemoveFailureArrayList<std::string> list;
    std::auto_ptr< Iterator<std::string> > iter( list.iterator() );

    iter->next();
    iter->remove();

    try {
        iter->remove();
    } catch( ConcurrentModificationException& e ) {
        CPPUNIT_FAIL("Excepted to catch IllegalStateException not ConcurrentModificationException");
    } catch( IllegalStateException& e ) {
        //Excepted to catch IllegalStateException here
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractListTest::testIndexOf() {

    SimpleList<int> array;
    for( int i = 1; i < 6; i++ ) {
        array.add(i);
    }

    MockArrayList<int> list;
    list.addAll( array );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "find 0 in the list do not contain 0", -1, list.indexOf( 0 ) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "did not return the right location of element 3", 2, list.indexOf( 3 ) );
}

////////////////////////////////////////////////////////////////////////////////
void AbstractListTest::testLastIndexOf() {

    SimpleList<int> array;
    for( int i = 1; i < 6; i++ ) {
        array.add(i);
    }
    for( int i = 5; i > 0; i-- ) {
        array.add(i);
    }

    CPPUNIT_ASSERT( array.size() == 10 );

    MockArrayList<int> list;
    list.addAll( array );

    CPPUNIT_ASSERT( list.size() == 10 );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "find 6 in the list do not contain 6",
                                  -1, list.lastIndexOf( 6 ) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "did not return the right location of element 4",
                                  6, list.lastIndexOf( 4 ) );
}

////////////////////////////////////////////////////////////////////////////////
void AbstractListTest::testRemoveAt() {

    MockList<int> list;

    try {
        list.removeAt( 0 );
        CPPUNIT_FAIL("should throw UnsupportedOperationException");
    } catch( UnsupportedOperationException& e ) {
        // expected
    }

    try {
        list.set( 0, 1 );
        CPPUNIT_FAIL("should throw UnsupportedOperationException");
    } catch( UnsupportedOperationException& e ) {
        // expected
    }
}
