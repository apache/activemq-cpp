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
#include <decaf/lang/exceptions/ClassCastException.h>
#include <decaf/util/concurrent/atomic/AtomicRefCounter.h>
#include <decaf/util/Comparator.h>
#include <memory>
#include <typeinfo>
#include <algorithm>
#include <functional>

namespace decaf {
namespace lang {

    // Used internally in Pointer.
    struct STATIC_CAST_TOKEN {};
    struct DYNAMIC_CAST_TOKEN {};

    /**
     * Decaf's implementation of a Smart Pointer that is a template on a Type
     * and is Thread Safe if the default Reference Counter is used.  This Pointer
     * type allows for the substitution of different Reference Counter implementations
     * which provide a means of using invasive reference counting if desired using
     * a custom implementation of <code>ReferenceCounter</code>.
     * <p>
     * The Decaf smart pointer provide comparison operators for comparing Pointer
     * instances in the same manner as normal pointer, except that it does not provide
     * an overload of operators ( <, <=, >, >= ).  To allow use of a Pointer in a STL
     * container that requires it, Pointer provides an implementation of std::less.
     *
     * @since 1.0
     */
    template< typename T, typename REFCOUNTER = decaf::util::concurrent::atomic::AtomicRefCounter >
    class Pointer : public REFCOUNTER {
    private:

        typedef void (*deletionFuncPtr)(T* p);

    private:

        T* value;

        // Pointer to our internal delete function, allows incompletes.
        deletionFuncPtr onDelete;

    public:

        typedef T* PointerType;          // type returned by operator->
        typedef T& ReferenceType;        // type returned by operator*
        typedef REFCOUNTER CounterType;  // Type of the Reference Counter

    public:

        /**
         * Default Constructor
         *
         * Initialized the contained pointer to NULL, using the -> operator
         * results in an exception unless reset to contain a real value.
         */
        Pointer() : REFCOUNTER(), value( NULL ), onDelete( onDeleteFunc ) {}

        /**
         * Explicit Constructor, creates a Pointer that contains value with a
         * single reference.  This object now has ownership until a call to release.
         *
         * @param value - instance of the type we are containing here.
         */
        explicit Pointer( const PointerType value ) : REFCOUNTER(), value( value ), onDelete( onDeleteFunc ) {
        }

        /**
         * Copy constructor. Copies the value contained in the pointer to the new
         * instance and increments the reference counter.
         */
        Pointer( const Pointer& value ) throw() :
            REFCOUNTER( value ), value( value.value ), onDelete( onDeleteFunc ) {}

        /**
         * Copy constructor. Copies the value contained in the pointer to the new
         * instance and increments the reference counter.
         */
        template< typename T1, typename R1 >
        Pointer( const Pointer<T1, R1>& value ) throw() :
            REFCOUNTER( value ), value( value.get() ), onDelete( onDeleteFunc ) {}

        /**
         * Static Cast constructor. Copies the value contained in the pointer to the new
         * instance and increments the reference counter performing a static cast on the
         * value contained in the source Pointer object.
         *
         * @param value - Pointer instance to cast to this type.
         */
        template< typename T1, typename R1 >
        Pointer( const Pointer<T1, R1>& value, const STATIC_CAST_TOKEN& ) throw() :
            REFCOUNTER( value ), value( static_cast<T*>( value.get() ) ), onDelete( onDeleteFunc ) {
        }

        /**
         * Dynamic Cast constructor. Copies the value contained in the pointer to the new
         * instance and increments the reference counter performing a dynamic cast on the
         * value contained in the source Pointer object.  If the cast fails and return NULL
         * then this method throws a ClassCastException.
         *
         * @param value - Pointer instance to cast to this type.
         *
         * @throw ClassCastException if the dynamic cast returns NULL
         */
        template< typename T1, typename R1 >
        Pointer( const Pointer<T1, R1>& value, const DYNAMIC_CAST_TOKEN& ) :
                REFCOUNTER( value ), value( dynamic_cast<T*>( value.get() ) ), onDelete( onDeleteFunc ) {

            if( this->value == NULL ) {

                // Remove the reference we took in the Reference Counter's ctor since we
                // didn't actually create one as the dynamic cast failed.
                REFCOUNTER::release();
                throw decaf::lang::exceptions::ClassCastException(
                    __FILE__, __LINE__,
                    "Failed to cast source pointer of type %s to this type: %s.",
                    typeid( T1 ).name(), typeid( T ).name() );
            }
        }

        virtual ~Pointer() throw() {
            if( REFCOUNTER::release() == true ) {
                onDelete( this->value );
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
         * Releases the Pointer held and resets the internal pointer value to Null.  This method
         * is not guaranteed to be safe if the Pointer is held by more than one object or this
         * method is called from more than one thread.
         *
         * @param value - The new value to contain.
         *
         * @returns The pointer instance that was held by this Pointer object, the pointer is
         *          no longer owned by this Pointer and won't be freed when this Pointer goes
         *          out of scope.
         */
        T* release() {
            T* temp = this->value;
            this->value = NULL;
            return temp;
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
        void swap( Pointer& value ) throw() {
            std::swap( this->value, value.value );
            REFCOUNTER::swap( value );
        }

        /**
         * Assigns the value of right to this Pointer and increments the reference Count.
         * @param right - Pointer on the right hand side of an operator= call to this.
         */
        Pointer& operator= ( const Pointer& right ) throw() {
            if( this == (void*)&right ) {
                return *this;
            }

            Pointer temp( right );
            temp.swap( *this );
            return *this;
        }
        template< typename T1, typename R1>
        Pointer& operator= ( const Pointer<T1, R1>& right ) throw() {
            if( this == (void*)&right ) {
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

        template< typename T1, typename R1 >
        bool operator==( const Pointer<T1, R1>& right ) const {
            return this->value == right.get();
        }

        template< typename T1, typename R1 >
        bool operator!=( const Pointer<T1, R1>& right ) const {
            return !( this->value == right.get() );
        }

        template< typename T1 >
        Pointer<T1, CounterType> dynamicCast() const {
            return Pointer<T1, CounterType>( *this, DYNAMIC_CAST_TOKEN() );
        }

        template< typename T1 >
        Pointer<T1, CounterType> staticCast() const {
            return Pointer<T1, CounterType>( *this, STATIC_CAST_TOKEN() );
        }

    private:

        // Internal Static deletion function.
        static void onDeleteFunc(T* value) {
            delete value;
        }

    };

    ////////////////////////////////////////////////////////////////////////////
    template< typename T, typename R, typename U >
    inline bool operator==( const Pointer<T, R>& left, const U* right ) {
        return left.get() == right;
    }

    ////////////////////////////////////////////////////////////////////////////
    template< typename T, typename R, typename U >
    inline bool operator==( const U* left, const Pointer<T, R>& right ) {
        return right.get() == left;
    }

    ////////////////////////////////////////////////////////////////////////////
    template< typename T, typename R, typename U >
    inline bool operator!=( const Pointer<T, R>& left, const U* right ) {
        return !( left.get() == right );
    }

    ////////////////////////////////////////////////////////////////////////////
    template< typename T, typename R, typename U >
    inline bool operator!=( const U* left, const Pointer<T, R>& right ) {
        return right.get() != left;
    }

    /**
     * This implementation of Comparator is designed to allows objects in a Collection
     * to be sorted or tested for equality based on the value of the Object being Pointed
     * to and not the value of the contained pointer in the Pointer instance.  This can
     * be useful in the case where a series of values in a Collection is more efficiently
     * accessed in the Objects Natural Order and not the underlying pointers location in
     * memory.
     * <p>
     * Also this allows Pointer objects that Point to different instances of the same type
     * to be compared based on the comparison of the object itself and not just the value of
     * the pointer.
     */
    template< typename T, typename R = decaf::util::concurrent::atomic::AtomicRefCounter >
    class PointerComparator : public decaf::util::Comparator< Pointer<T,R> > {
    public:

        virtual ~PointerComparator() {}

        // Allows for operator less on types that implement Comparable or provide
        // a workable operator <
        virtual bool operator() ( const Pointer<T,R>& left, const Pointer<T,R>& right ) const {
            return *left < *right;
        }

        // Requires that the type in the pointer is an instance of a Comparable.
        virtual int compare( const Pointer<T,R>& left, const Pointer<T,R>& right ) const {
            return *left < *right ? -1 : *right < *left ? 1 : 0;
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
