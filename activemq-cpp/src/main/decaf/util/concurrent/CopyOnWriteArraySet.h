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

#ifndef _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYSET_H_
#define _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYSET_H_

#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/ArrayPointer.h>
#include <decaf/util/concurrent/CopyOnWriteArrayList.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/Set.h>
#include <decaf/util/Arrays.h>
#include <decaf/util/AbstractSet.h>

namespace decaf {
namespace util {
namespace concurrent {

    using decaf::lang::ArrayPointer;

    /**
     * Since the CopyOnWriteArraySet and the CopyOnWriteArrayList share much of the same
     * operational semantics this class uses the CopyOnWriteArrayList for all its underlying
     * operations.
     *
     * This collection is best used in applications where the Set size is usually small and
     * write operations are minimal as they result in a copy of the underlying array being
     * created.  Reads are generally fast and the iterators provided by this collection
     * do not block as they operate on a snapshot of the data taken at the time of their
     * creation.
     *
     * @since 1.0
     */
    template< typename E >
    class CopyOnWriteArraySet : public AbstractSet<E> {
    private:

        CopyOnWriteArrayList<E> array;

    public:

        CopyOnWriteArraySet() : AbstractSet<E>(), array() {
        }

        CopyOnWriteArraySet( const Collection<E>& collection ) : AbstractSet<E>(), array() {
            this->copy( collection );
        }

        CopyOnWriteArraySet( const E* array, int size ) : AbstractSet<E>(), array() {

            for( int i = 0; i < size; ++i ) {
                this->array.addIfAbsent( array[i] );
            }
        }

        virtual ~CopyOnWriteArraySet() {}

    public:

        virtual void copy( const Collection<E>& collection ) {
            this->array.copy( collection );
        }

        virtual decaf::util::Iterator<E>* iterator() {
            return this->array.iterator();
        }

        virtual decaf::util::Iterator<E>* iterator() const {
            return this->array.iterator();
        }

        virtual int size() const {
            return this->array.size();
        }

        virtual bool isEmpty() const {
            return this->array.isEmpty();
        }

        virtual bool add( const E& value ) {
            return this->array.addIfAbsent( value );
        }

        virtual bool addAll( const Collection<E>& collection ) {
            return this->array.addAllAbsent( collection ) > 0 ? true : false;
        }

        virtual void clear() {
            this->array.clear();
        }

        virtual bool contains( const E& value ) const {
            return this->array.contains( value );
        }

        virtual bool containsAll( const Collection<E>& collection ) const {
            return this->array.containsAll( collection );
        }

        virtual bool remove( const E& value ) {
            return this->array.remove( value );
        }

        virtual bool removeAll( const Collection<E>& collection ) {
            return this->array.removeAll( collection );
        }

        virtual bool retainAll( const Collection<E>& collection ) {
            return this->array.retainAll( collection );
        }

        virtual std::vector<E> toArray() const {
            return this->array.toArray();
        }

        virtual bool equals( const Collection<E>& collection ) const {

            if( (void*)this == &collection ) {
                return true;
            }

            const Set<E>* asSet = dynamic_cast<const Set<E>*>( &collection );
            if( asSet == NULL ) {
                return false;
            }

            if( this->size() != asSet->size() ) {
                return false;
            }

            std::auto_ptr< Iterator<E> > setIter( asSet->iterator() );

            //  Use a single snapshot of underlying array
            ArrayPointer<E> elements = this->array.getArray();
            int length = elements.length();

            ArrayPointer<bool> matched( length, false );

            while( setIter->hasNext() ) {

                E value = setIter->next();
                for( int i = 0; i < length; ++i ) {
                    if( !matched[i] && value == elements[i] ) {
                        matched[i] = true;
                        break;
                    }
                }
            }

            for( int i = 0; i < length; ++i ) {
                if( matched[i] == false ) {
                    return false;
                }
            }

            return true;
        }

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_COPYONWRITEARRAYSET_H_ */
