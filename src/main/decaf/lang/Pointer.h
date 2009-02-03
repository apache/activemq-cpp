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

#ifndef _DECAF_LANG_POINTER_H_
#define _DECAF_LANG_POINTER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <memory>

namespace decaf {
namespace lang {

    /**
     * Decaf's implementation of a Smart Pointer that is a template on a Type
     * and is Thread Safe.
     */
    template< typename T >
    class DECAF_API Pointer {
    private:

        T* value;
        decaf::util::concurrent::atomic::AtomicInteger* refCount;

    public:

        typedef T* StoredType;    // the type of the object we point to
        typedef T* PointerType;   // type returned by operator->
        typedef T& ReferenceType; // type returned by operator*

    public:

        /**
         * Default Constructor
         *
         * Initialized the contained pointer to NULL, using the -> operator
         * results in an exception unless reset to contain a real value.
         */
        Pointer() : value( NULL ), refCount( new decaf::util::concurrent::atomic::AtomicInteger( 1 ) ) {}

        /**
         * Explicit Constructor, creates a Pointer that contains value with a
         * single reference.  This object now has ownership until a call to release.
         *
         * @param value - instance of the type we are containing here.
         */
        explicit Pointer( const StoredType& value ) :
            value( value ), refCount( new decaf::util::concurrent::atomic::AtomicInteger( 1 ) ) {
        }

        /**
         * Copy constructor. Copies the value contained in the pointer to the new
         * instance and increments the reference counter.
         */
        Pointer( const Pointer<T>& value ) throw() : value( value.value ), refCount( value.refCount ) {
            this->refCount->incrementAndGet();
        }

        virtual ~Pointer() throw() {
            if( this->refCount->decrementAndGet() == 0 ) {
                delete this->value;
                delete this->refCount;
            }
        }

        /**
         * Resets the Pointer to hold the new value.  Before the new value is stored
         * reset checks if the old value should be destroyed and if so calls delete.
         * Call reset with a value of NULL is supported and acts to set this Pointer
         * to a NULL pointer.
         *
         * @param value - The new value to contain.
         */
        void reset( T* value ) {
            Pointer( value ).swap( *this );
        }

        /**
         * Gets the real pointer that is contained within this Pointer.  This is
         * not really safe since the caller could delete or alter the pointer but
         * it mimics the STL auto_ptr and gives access in cases where the caller
         * absolutely needs the real Pointer.  Use at your own risk.
         *
         * @return the contained pointer.
         */
        PointerType get() const {
            return this->value;
        }

        /**
         * Exception Safe Swap Function
         * @param value - the value to swap with this.
         */
        void swap( Pointer<T>& value ) throw() {
            std::swap( this->value, value.value );
            std::swap( this->refCount, value.refCount );
        }

        /**
         * Assigns the value of right to this Pointer and increments the reference Count.
         * @param right - Pointer on the right hand side of an operator= call to this.
         */
        Pointer& operator= ( const Pointer<T>& right ) throw() {
            if( this == &right ) {
                return *this;
            }

            Pointer temp( right );
            temp.swap( *this );
            return *this;
        }

        /**
         * Dereference Operator, returns a reference to the Contained value.  This
         * method throws an NullPointerException if the contained value is NULL.
         *
         * @return reference to the contained pointer.
         * @throws NullPointerException if the contained value is Null
         */
        ReferenceType operator*() {
            if( this->value == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(
                    __FILE__, __LINE__, "Pointer operator& - Pointee is NULL." );
            }

            return *( this->value );
        }
        ReferenceType operator*() const {
            if( this->value == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(
                    __FILE__, __LINE__, "Pointer operator& - Pointee is NULL." );
            }

            return *( this->value );
        }

        /**
         * Indirection Operator, returns a pointer to the Contained value.  This
         * method throws an NullPointerException if the contained value is NULL.
         *
         * @return reference to the contained pointer.
         * @throws NullPointerException if the contained value is Null
         */
        PointerType operator->() {
            if( this->value == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(
                    __FILE__, __LINE__, "Pointer operator-> - Pointee is NULL." );
            }
            return this->value;
        }
        PointerType operator->() const {
            if( this->value == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(
                    __FILE__, __LINE__, "Pointer operator-> - Pointee is NULL." );
            }
            return this->value;
        }

        bool operator!() const {
            return this->value == NULL;
        }

        inline friend bool operator==( const Pointer& left, const T* right ) {
            return left.get() == right;
        }

        inline friend bool operator==( const T* left, const Pointer& right ) {
            return left == right.get();
        }

        inline friend bool operator!=( const Pointer& left, const T* right ) {
            return left.get() != right;
        }

        inline friend bool operator!=( const T* left, const Pointer& right ) {
            return left != right.get();
        }

        template< typename U >
        bool operator==( const Pointer<U>& right ) const {
            return this->value == right.get();
        }

        template< typename U >
        bool operator!=( const Pointer<U>& right ) const {
            return !( this->value == right.get() );
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
namespace std{

    /**
     * An override of the less function object so that the Pointer objects
     * can be stored in STL Maps, etc.
     */
    template< typename T >
    struct less< decaf::lang::Pointer<T> > :
        public binary_function< decaf::lang::Pointer<T>,
                                decaf::lang::Pointer<T>, bool>
    {
        bool operator()( const decaf::lang::Pointer<T>& left,
                         const decaf::lang::Pointer<T>& right ) const
        {
            return less<T*>()( left.get(), right.get() );
        }
    };
}

#endif /*_DECAF_LANG_POINTER_H_*/
