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

#ifndef _DECAF_INTERNAL_NIO_CHARARRAYBUFFER_H_
#define _DECAF_INTERNAL_NIO_CHARARRAYBUFFER_H_

#include <decaf/nio/CharBuffer.h>
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

    class DECAF_API CharArrayBuffer : public decaf::nio::CharBuffer {
    protected:

        // The reference array object that backs this buffer.
        decaf::lang::Pointer<ByteArrayAdapter> _array;

        // Offset into the array that we are to start from
        int offset;

        // The length of the sub-array, or limit
        int length;

        // Read / Write flag
        bool readOnly;

    public:

        /**
         * Creates a CharArrayBuffer object that has its backing array allocated internally
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
        CharArrayBuffer( int size, bool readOnly = false );

        /**
         * Creates a CharArrayBuffer object that wraps the given array.  If the own flag
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
        CharArrayBuffer( char* array, int size, int offset, int length, bool readOnly = false );

        /**
         * Creates a byte buffer that wraps the passed ByteArrayAdapter and
         * start at the given offset.  The capacity and limit of the new CharArrayBuffer
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
        CharArrayBuffer( const decaf::lang::Pointer<ByteArrayAdapter>& array,
                         int offset, int length, bool readOnly = false );

        /**
         * Create a CharArrayBuffer that mirrors this one, meaning it shares a
         * reference to this buffers ByteArrayAdapter and when changes
         * are made to that data it is reflected in both.
         *
         * @param other
         *      The CharArrayBuffer this one is to mirror.
         */
        CharArrayBuffer( const CharArrayBuffer& other );

        virtual ~CharArrayBuffer();

    public:  // Overrides

        /**
         * {@inheritDoc}
         */
        virtual char* array();

        /**
         * {@inheritDoc}
         */
        virtual int arrayOffset();

        /**
         * {@inheritDoc}
         */
        virtual CharBuffer* asReadOnlyBuffer() const;

        /**
         * {@inheritDoc}
         */
        virtual CharBuffer& compact();

        /**
         * {@inheritDoc}
         */
        virtual CharBuffer* duplicate();

        /**
         * {@inheritDoc}
         */
        virtual char get();

        /**
         * {@inheritDoc}
         */
        virtual char get( int index ) const;

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
        virtual CharBuffer& put( char value );

        /**
         * {@inheritDoc}
         */
        virtual CharBuffer& put( int index, char value );

        /**
         * {@inheritDoc}
         */
        virtual CharBuffer* slice() const;

        /**
         * {@inheritDoc}
         */
        virtual lang::CharSequence* subSequence( int start, int end ) const;

    protected:

        /**
         * Sets this CharArrayBuffer as Read-Only.
         *
         * @param value
         *      Boolean value, true if this buffer is to be read-only, false otherwise.
         */
        virtual void setReadOnly( bool value ) {
            this->readOnly = value;
        }

    };

}}}

#endif /*_DECAF_INTERNAL_NIO_CHARARRAYBUFFER_H_*/
