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

#ifndef _DECAF_LANG_ARRAYPOINTER_H_
#define _DECAF_LANG_ARRAYPOINTER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/System.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/concurrent/atomic/AtomicRefCounter.h>
#include <decaf/util/Comparator.h>
#include <decaf/util/Arrays.h>
#include <memory>
#include <typeinfo>
#include <algorithm>

namespace decaf {
namespace lang {

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
    class ArrayPointer : public REFCOUNTER {
    private:

        struct ArrayData {
            T* value;
            int length;

            ArrayData() : value( NULL ), length( 0 ) {}
            ArrayData( T* value, int length ) : value( value ), length( length ) {
                if( value != NULL && length <= 0 ) {
                    throw decaf::lang::exceptions::IllegalArgumentException(
                        __FILE__, __LINE__, "Non-NULL array pointer cannot have a size <= zero" );
                }

                if( value == NULL && length > 0 ) {
                    throw decaf::lang::exceptions::IllegalArgumentException(
                        __FILE__, __LINE__, "NULL array pointer cannot have a size > zero" );
                }
            }
        };

        typedef void (*deletionFuncPtr)(ArrayData* p);

    private:

        ArrayData* array;

        // Pointer to our internal delete function, allows incompletes.
        deletionFuncPtr onDelete;

    public:

        typedef T* PointerType;              // type returned by operator->
        typedef T& ReferenceType;            // type returned by operator*
        typedef const T& ConstReferenceType; // type returned by const operator*
        typedef REFCOUNTER CounterType;      // Type of the Reference Counter

    public:

        /**
         * Default Constructor
         *
         * Initialized the contained array pointer to NULL, using the subscript operator
         * results in an exception unless reset to contain a real value.
         */
        ArrayPointer() : REFCOUNTER(), array( new ArrayData() ), onDelete( onDeleteFunc ) {}

        /**
         * Create a new ArrayPointer instance and allocates an internal array that is sized
         * using the passed in size value.
         *
         * @param size
         *      The size of the array to allocate for this ArrayPointer instance.
         */
        ArrayPointer( int size ) :
            REFCOUNTER(), array( NULL ), onDelete( onDeleteFunc ) {

            if( size == 0 ) {
                return;
            }

            try{
                T* value = new T[size];
                this->array = new ArrayData( value, size );
            } catch( std::exception& ex ) {
                REFCOUNTER::release();
                throw ex;
            } catch(...) {
                REFCOUNTER::release();
                throw std::bad_alloc();
            }
        }

        /**
         * Create a new ArrayPointer instance and allocates an internal array that is sized
         * using the passed in size value.  The array elements are initialized with the given
         * value.
         *
         * @param size
         *      The size of the array to allocate for this ArrayPointer instance.
         * @param fillWith
         *      The value to initialize each element of the newly allocated array with.
         */
        ArrayPointer( int size, const T& fillWith ) :
            REFCOUNTER(), array( NULL ), onDelete( onDeleteFunc ) {

            if( size == 0 ) {
                return;
            }

            try{
                T* value = new T[size];
                decaf::util::Arrays::fill( value, size, 0, size, fillWith );
                this->array = new ArrayData( value, size );
            } catch( std::exception& ex ) {
                REFCOUNTER::release();
                throw ex;
            } catch(...) {
                REFCOUNTER::release();
                throw std::bad_alloc();
            }
        }

        /**
         * Explicit Constructor, creates an ArrayPointer that contains value with a
         * single reference.  This object now has ownership until a call to release.
         *
         * @param value
         *      The pointer to the instance of the array we are taking ownership of.
         * @param size
         *      The size of the array this object is taking ownership of.
         */
        explicit ArrayPointer( const PointerType value, int size ) :
            REFCOUNTER(), array( NULL ), onDelete( onDeleteFunc ) {

            try{
                this->array = new ArrayData( value, size );
            } catch( std::exception& ex ) {
                REFCOUNTER::release();
                throw ex;
            } catch(...) {
                REFCOUNTER::release();
                throw std::bad_alloc();
            }
        }

        /**
         * Copy constructor. Copies the value contained in the ArrayPointer to the new
         * instance and increments the reference counter.
         */
        ArrayPointer( const ArrayPointer& value ) throw() :
            REFCOUNTER( value ), array( value.array ), onDelete( onDeleteFunc ) {}

        virtual ~ArrayPointer() throw() {
            if( REFCOUNTER::release() == true ) {
                onDelete( this->array );
            }
        }

        /**
         * Resets the ArrayPointer to hold the new value.  Before the new value is stored
         * reset checks if the old value should be destroyed and if so calls delete.
         * Call reset with a value of NULL is supported and acts to set this Pointer
         * to a NULL pointer.
         *
         * @param value
         *      The new array pointer value to contain.
         * @param size
         *      The size of the new array value this object now contains.
         */
        void reset( T* value, int size = 0 ) {
            ArrayPointer( value, size ).swap( *this );
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
            T* temp = this->array->value;
            this->array->value = NULL;
            this->array->length = 0;
            return temp;
        }

        /**
         * Gets the real array pointer that is contained within this Pointer.  This
         * is not really safe since the caller could delete or alter the pointer but
         * it mimics the STL auto_ptr and gives access in cases where the caller
         * absolutely needs the real Pointer.  Use at your own risk.
         *
         * @return the contained pointer.
         */
        PointerType get() const {
            return this->array->value;
        }

        /**
         * Returns the current size of the contained array or zero if the array is
         * NULL.
         *
         * @returns the size of the array or zero if the array is NULL
         */
        int length() const {
            return this->array->length;
        }

        /**
         * Exception Safe Swap Function
         * @param value - the value to swap with this.
         */
        void swap( ArrayPointer& value ) throw() {
            std::swap( this->array, value.array );
            REFCOUNTER::swap( value );
        }

        /**
         * Creates a new ArrayPointer instance that is a clone of the value contained in this
         * ArrayPointer.
         *
         * @return an ArrayPointer that contains a copy of the data in this ArrayPointer.
         */
        ArrayPointer clone() const {

            if( this->array->length == 0 ){
                return ArrayPointer();
            }

            ArrayPointer copy( this->array->length );
            decaf::lang::System::arraycopy( this->array->value, 0, copy.get(), 0, this->array->length );
            return copy;
        }

        /**
         * Assigns the value of right to this Pointer and increments the reference Count.
         * @param right - Pointer on the right hand side of an operator= call to this.
         */
        ArrayPointer& operator= ( const ArrayPointer& right ) throw() {
            if( this == (void*)&right ) {
                return *this;
            }

            ArrayPointer temp( right );
            temp.swap( *this );
            return *this;
        }
        template< typename T1, typename R1>
        ArrayPointer& operator= ( const ArrayPointer<T1, R1>& right ) throw() {
            if( this == (void*)&right ) {
                return *this;
            }

            ArrayPointer temp( right );
            temp.swap( *this );
            return *this;
        }

        /**
         * Dereference Operator, returns a reference to the Contained value.  This
         * method throws an NullPointerException if the contained value is NULL.
         *
         * @return reference to the contained pointer.
         *
         * @throws NullPointerException if the contained value is Null
         */
        ReferenceType operator[]( int index ) {
            if( this->array->value == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(
                    __FILE__, __LINE__, "ArrayPointer operator& - Pointee is NULL." );
            }

            if( index < 0 || this->array->length <= index ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Array Index %d is out of bounds for this array.", this->array->length );
            }

            return this->array->value[index];
        }
        ConstReferenceType operator[]( int index ) const {
            if( this->array->value == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(
                    __FILE__, __LINE__, "ArrayPointer operator& - Pointee is NULL." );
            }

            if( index < 0 || this->array->length <= index ) {
                throw decaf::lang::exceptions::IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Array Index %d is out of bounds for this array.", this->array->length );
            }

            return this->array->value[index];
        }

        bool operator!() const {
            return this->array->value == NULL;
        }

        inline friend bool operator==( const ArrayPointer& left, const T* right ) {
            return left.get() == right;
        }

        inline friend bool operator==( const T* left, const ArrayPointer& right ) {
            return left == right.get();
        }

        inline friend bool operator!=( const ArrayPointer& left, const T* right ) {
            return left.get() != right;
        }

        inline friend bool operator!=( const T* left, const ArrayPointer& right ) {
            return left != right.get();
        }

        template< typename T1, typename R1 >
        bool operator==( const ArrayPointer<T1, R1>& right ) const {
            return this->array->value == right.get();
        }

        template< typename T1, typename R1 >
        bool operator!=( const ArrayPointer<T1, R1>& right ) const {
            return this->array->value != right.get();
        }

    private:

        // Internal Static deletion function.
        static void onDeleteFunc( ArrayData* value ) {
            delete [] value->value;
            delete value;
        }

    };

    ////////////////////////////////////////////////////////////////////////////
    template< typename T, typename R, typename U >
    inline bool operator==( const ArrayPointer<T, R>& left, const U* right ) {
        return left.get() == right;
    }

    ////////////////////////////////////////////////////////////////////////////
    template< typename T, typename R, typename U >
    inline bool operator==( const U* left, const ArrayPointer<T, R>& right ) {
        return right.get() == left;
    }

    ////////////////////////////////////////////////////////////////////////////
    template< typename T, typename R, typename U >
    inline bool operator!=( const ArrayPointer<T, R>& left, const U* right ) {
        return !( left.get() == right );
    }

    ////////////////////////////////////////////////////////////////////////////
    template< typename T, typename R, typename U >
    inline bool operator!=( const U* left, const ArrayPointer<T, R>& right ) {
        return right.get() != left;
    }

    /**
     * This implementation of Comparator is designed to allows objects in a Collection
     * to be sorted or tested for equality based on the value of the value of the actual
     * pointer to the array being contained in this ArrayPointer.  This allows for a basic
     * ordering to be acheived in Decaf containers.
     *
     * Custom implementations are possible where an array of some type has a logical natural
     * ordering such as array of integers where the sum of all ints in the array is used.
     */
    template< typename T, typename R = decaf::util::concurrent::atomic::AtomicRefCounter >
    class ArrayPointerComparator : public decaf::util::Comparator< ArrayPointer<T,R> > {
    public:

        virtual ~ArrayPointerComparator() {}

        // Allows for operator less on types that implement Comparable or provide
        // a workable operator <
        virtual bool operator() ( const ArrayPointer<T,R>& left, const ArrayPointer<T,R>& right ) const {
            return left.get() < right.get();
        }

        // Requires that the type in the pointer is an instance of a Comparable.
        virtual int compare( const ArrayPointer<T,R>& left, const ArrayPointer<T,R>& right ) const {
            return left.get() < right.get() ? -1 : right.get() < left.get() ? 1 : 0;
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
    struct less< decaf::lang::ArrayPointer<T> > :
        public binary_function< decaf::lang::ArrayPointer<T>,
                                decaf::lang::ArrayPointer<T>, bool>
    {
        bool operator()( const decaf::lang::ArrayPointer<T>& left,
                         const decaf::lang::ArrayPointer<T>& right ) const
        {
            return less<T*>()( left.get(), right.get() );
        }
    };
}

#endif /*_DECAF_LANG_ARRAYPOINTER_H_*/
