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

#ifndef _DECAF_INTERNAL_NIO_BYTEARRAYPERSPECTIVE_H_
#define _DECAF_INTERNAL_NIO_BYTEARRAYPERSPECTIVE_H_

#include <decaf/internal/util/ByteArrayAdapter.h>

namespace decaf{
namespace internal{
namespace nio{

    /**
     * This class extends ByteArray to create a reference counted byte array
     * that can be held and used by several differen ByteBuffers and allow them
     * to know on destruction whose job it is to delete the perspective.
     * <p>
     * Creating an instance of this class implicity takes a reference to it, so
     * a creator must return its ref before the count will be zero.
     */
    class DECAF_API ByteArrayPerspective : public util::ByteArrayAdapter {
    private:

        // Count of the number of perspectives into this array.
        int references;

    public:

        /**
         * Creates a byte array object that is allocated internally and is then owned
         * and deleted when this object is deleted.  The array is initially created
         * with all elements initialized to zero.
         * @param capacity - size of the array, this is the limit we read and write to.
         */
        ByteArrayPerspective( std::size_t capacity );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayPerspective( unsigned char* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayPerspective( char* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayPerspective( double* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayPerspective( float* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayPerspective( long long* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayPerspective( int* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayPerspective( short* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        virtual ~ByteArrayPerspective() {}

        /**
         * Takes a reference to this Perspective and returns a pointer to this
         * so that a caller can take a ref and get a ref in one call.
         * @returns this
         */
        ByteArrayPerspective* takeRef() {
            this->references++;
            return this;
        }

        /**
         * Returns a reference to this Perspective, when the count is zero
         * it should be deleted
         */
        void returnRef() { this->references--; }

        /**
         * @returns the current number of reference on this perspective
         */
        int getReferences() const { return this->references; }

    };

}}}

#endif /*_DECAF_INTERNAL_NIO_BYTEARRAYPERSPECTIVE_H_*/
