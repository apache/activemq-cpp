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

#ifndef _DECAF_UTIL_ABSTRACTCOLLECTION_H_
#define _DECAF_UTIL_ABSTRACTCOLLECTION_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/Iterable.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/Collection.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <memory>

namespace decaf {
namespace util {

    /**
     * This class provides a skeletal implementation of the Collection  interface, to
     * minimize the effort required to implement this interface.
     *
     * To implement an unmodifiable collection, the programmer needs only to extend this
     * class and provide implementations for the iterator and size methods. (The iterator
     * returned by the iterator method must implement hasNext and next.)
     *
     * To implement a modifiable collection, the programmer must additionally override
     * this class's add method (which otherwise throws an UnsupportedOperationException),
     * and the iterator returned by the iterator method must additionally implement its
     * remove method.
     *
     * The programmer should generally provide a void (no argument) and Collection
     * constructor, as per the recommendation in the Collection interface specification.
     *
     * The documentation for each non-abstract method in this class describes its
     * implementation in detail. Each of these methods may be overridden if the collection
     * being implemented admits a more efficient implementation.
     *
     * @since 1.0
     */
    template< typename E >
    class AbstractCollection : public decaf::util::Collection<E> {
    public:

        virtual ~AbstractCollection() {}

        virtual bool add( const E& value DECAF_UNUSED )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException ) {

            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "AbstractCollection add is not implemented.");
        }

        virtual bool addAll( const Collection<E>& collection )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException ) {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                if( this->add( iter->next() ) ) {
                    result = true;
                }
            }

            return result;
        }

        virtual void clear()
            throw ( lang::exceptions::UnsupportedOperationException ) {

            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                iter->next();
                iter->remove();
            }
        }

        virtual void copy( const Collection<E>& collection ) {
            this->clear();

            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                this->add( iter->next() );
            }
        }

        virtual bool contains( const E& value ) const throw ( lang::Exception ) {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                if( iter->next() == value ) {
                    result = true;
                }
            }

            return result;
        }

        virtual bool containsAll( const Collection<E>& collection ) const
            throw ( lang::Exception ) {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                if( this->contains( iter->next() ) ) {
                    result = true;
                }
            }

            return result;
        }

        virtual bool equals( const Collection<E>& collection ) const {
            if( this->size() != collection.size() ) {
                return false;
            }

            std::auto_ptr< Iterator<E> > srcIter( collection.iterator() );
            std::auto_ptr< Iterator<E> > thisIter( this->iterator() );

            while( srcIter->hasNext() ) {
                if( !( thisIter->next() == srcIter->next() ) ) {
                    return false;
                }
            }

            return true;
        }

        virtual bool isEmpty() const {
            return this->size() == 0;
        }

        virtual bool remove( const E& value DECAF_UNUSED )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException ) {

            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "AbstractCollection add is not implemented.");
        }

        virtual bool removeAll( const Collection<E>& collection )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException ) {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( collection.iterator() );
            while( iter->hasNext() ) {
                if( this->remove( iter->next() ) ) {
                    result = true;
                }
            }

            return result;
        }

        virtual bool retainAll( const Collection<E>& collection )
            throw ( lang::exceptions::UnsupportedOperationException,
                    lang::exceptions::IllegalArgumentException ) {

            bool result = false;
            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                if( !collection.contains( iter->next() ) ) {
                    iter->remove();
                    result = true;
                }
            }

            return result;
        }

        virtual std::vector<E> toArray() const {
            std::vector<E> valueArray;
            valueArray.reserve( this->size() );

            std::auto_ptr< Iterator<E> > iter( this->iterator() );
            while( iter->hasNext() ) {
                valueArray.push_back( iter->next() );
            }

            return valueArray;
        }

    };

}}

#endif /*_DECAF_UTIL_ABSTRACTCOLLECTION_H_*/
