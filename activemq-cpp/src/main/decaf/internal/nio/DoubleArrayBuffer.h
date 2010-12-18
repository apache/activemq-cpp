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

#ifndef _DECAF_INTERNAL_NIO_DOUBLEARRAYBUFFER_H_
#define _DECAF_INTERNAL_NIO_DOUBLEARRAYBUFFER_H_

#include <decaf/nio/DoubleBuffer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/nio/BufferUnderflowException.h>
#include <decaf/nio/BufferOverflowException.h>
#include <decaf/nio/ReadOnlyBufferException.h>
#include <decaf/internal/util/ByteArrayAdapter.h>

#include <decaf/lang/Pointer.h>

namespace decaf{
namespace internal{
namespace nio{

    using decaf::internal::util::ByteArrayAdapter;

    class DECAF_API DoubleArrayBuffer : public decaf::nio::DoubleBuffer{
    private:

        // The reference array object that backs this buffer.
        decaf::lang::Pointer<ByteArrayAdapter> _array;

        // Offset into the array that we are to start from
        int offset;

        // Number of bytes past offset to read to, or Limit.
        int length;

        // Read / Write flag
        bool readOnly;

    public:

        /**
         * Creates a DoubleArrayBuffer object that has its backing array allocated internally
         * and is then owned and deleted when this object is deleted.  The array is
         * initially created with all elements initialized to zero.
         *
         * @param size
         *      The size of the array, this is the limit we read and write to.
         * @param readOnly
         *      Boolean indicating if this buffer should be read-only, default as false.
         *
         * @throws IllegalArguementException if the capacity value is negative.
         */
        DoubleArrayBuffer( int capacity, bool readOnly = false );

        /**
         * Creates a DoubleArrayBuffer object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         *
         * @param array
         *      The actual array to wrap.
         * @param size
         *      The size of the given array.
         * @param offset
         *      The position that is this buffers start position.
         * @param length
         *      The limit of how many bytes into the array this Buffer can write.
         * @param readOnly
         *      Boolean indicating if this buffer should be read-only, default as false.
         *
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if offset is greater than array capacity.
         */
        DoubleArrayBuffer( double* array, int size, int offset, int length, bool readOnly = false );

        /**
         * Creates a byte buffer that wraps the passed ByteArrayAdapter and
         * start at the given offset.  The capacity and limit of the new DoubleArrayBuffer
         * will be that of the remaining capacity of the passed buffer.
         *
         * @param array
         *      The ByteArrayAdapter to wrap.
         * @param offset
         *      The position that is this buffers start position.
         * @param length
         *      The limit of how many bytes into the array this Buffer can write.
         * @param readOnly
         *      Boolean indicating if this buffer should be read-only, default as false.
         *
         * @throws NullPointerException if array is NULL
         * @throws IndexOutOfBoundsException if offset + length is greater than array size.
         */
        DoubleArrayBuffer( const decaf::lang::Pointer<ByteArrayAdapter>& array,
                           int offset, int length, bool readOnly = false );

        /**
         * Create a DoubleArrayBuffer that mirrors this one, meaning it shares a
         * reference to this buffers ByteArrayAdapter and when changes
         * are made to that data it is reflected in both.
         *
         * @param other
         *      The DoubleArrayBuffer this one is to mirror.
         */
        DoubleArrayBuffer( const DoubleArrayBuffer& other );

        virtual ~DoubleArrayBuffer();

    public:

        /**
         * {@inheritDoc}
         */
        virtual double* array();

        /**
         * {@inheritDoc}
         */
        virtual int arrayOffset();

        /**
         * {@inheritDoc}
         */
        virtual DoubleBuffer* asReadOnlyBuffer() const;

        /**
         * {@inheritDoc}
         */
        virtual DoubleBuffer& compact();

        /**
         * {@inheritDoc}
         */
        virtual DoubleBuffer* duplicate();

        /**
         * {@inheritDoc}
         */
        virtual double get();

        /**
         * {@inheritDoc}
         */
        virtual double get( int index ) const;

        /**
         * {@inheritDoc}
         */
        virtual bool hasArray() const { return true; }

        /**
         * {@inheritDoc}
         */
        virtual bool isReadOnly() const {
            return this->readOnly;
        }

        /**
         * {@inheritDoc}
         */
        virtual DoubleBuffer& put( double value );

        /**
         * {@inheritDoc}
         */
        virtual DoubleBuffer& put( int index, double value );

        /**
         * {@inheritDoc}
         */
        virtual DoubleBuffer* slice() const;

    protected:

        /**
         * Sets this DoubleArrayBuffer as Read-Only or not Read-Only.
         *
         * @param value
         *      Boolean value, true if this buffer is to be read-only, false otherwise.
         */
        virtual void setReadOnly( bool value ) {
            this->readOnly = value;
        }

    };

}}}

#endif /*_DECAF_INTERNAL_NIO_DOUBLEARRAYBUFFER_H_*/
