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

#ifndef _DECAF_INTERNAL_NIO_BYTEBUFFER_H_
#define _DECAF_INTERNAL_NIO_BYTEBUFFER_H_

#include <decaf/nio/ByteBuffer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/nio/BufferUnderflowException.h>
#include <decaf/nio/BufferOverflowException.h>
#include <decaf/nio/ReadOnlyBufferException.h>
#include <decaf/internal/util/ByteArrayAdapter.h>

#include <decaf/nio/CharBuffer.h>
#include <decaf/nio/DoubleBuffer.h>
#include <decaf/nio/FloatBuffer.h>
#include <decaf/nio/ShortBuffer.h>
#include <decaf/nio/IntBuffer.h>
#include <decaf/nio/LongBuffer.h>

#include <decaf/lang/Pointer.h>

namespace decaf{
namespace internal{
namespace nio{

    using decaf::internal::util::ByteArrayAdapter;

    /**
     * This class defines six categories of operations upon byte buffers:
     *
     *  1. Absolute and relative get and put methods that read and write single bytes;
     *  2. Relative bulk get methods that transfer contiguous sequences of bytes from
     *     this buffer into an array;
     *  3. Relative bulk put methods that transfer contiguous sequences of bytes from
     *     a byte array or some other byte buffer into this buffer;
     *  4. Absolute and relative get and put methods that read and write values of other
     *     primitive types, translating them to and from sequences of bytes in a
     *     particular byte order;
     *  5. Methods for creating view buffers, which allow a byte buffer to be viewed as
     *     a buffer containing values of some other primitive type; and
     *  6. Methods for compacting, duplicating, and slicing a byte buffer.
     *
     * Byte buffers can be created either by allocation, which allocates space for the
     * buffer's content, or by wrapping an existing byte array into a buffer.
     *
     * Access to binary data:
     *
     * This class defines methods for reading and writing values of all other primitive
     * types, except boolean. Primitive values are translated to (or from) sequences of
     * bytes according to the buffer's current byte order.
     *
     * For access to heterogeneous binary data, that is, sequences of values of
     * different types, this class defines a family of absolute and relative get and
     * put methods for each type. For 32-bit floating-point values, for example, this
     * class defines:
     *
     *   float getFloat()
     *   float getFloat(int index)
     *   void  putFloat(float f)
     *   void  putFloat(int index, float f)
     *
     * Corresponding methods are defined for the types char, short, int, long, and
     * double. The index parameters of the absolute get and put methods are in terms
     * of bytes rather than of the type being read or written.
     *
     * For access to homogeneous binary data, that is, sequences of values of the same
     * type, this class defines methods that can create views of a given byte buffer.
     * A view buffer is simply another buffer whose content is backed by the byte buffer.
     * Changes to the byte buffer's content will be visible in the view buffer, and vice
     * versa; the two buffers' position, limit, and mark values are independent. The
     * asFloatBuffer method, for example, creates an instance of the FloatBuffer class
     * that is backed by the byte buffer upon which the method is invoked. Corresponding
     * view-creation methods are defined for the types char, short, int, long, and double.
     *
     * View buffers have two important advantages over the families of type-specific
     * get and put methods described above:
     *
     *   A view buffer is indexed not in terms of bytes but rather in terms of the
     *   type-specific size of its values;
     *
     *   A view buffer provides relative bulk get and put methods that can transfer
     *   contiguous sequences of values between a buffer and an array or some other
     *   buffer of the same type; and
     *
     * @since 1.0
     */
    class DECAF_API ByteArrayBuffer : public decaf::nio::ByteBuffer {
    private:

        // The reference array object that backs this buffer.
        decaf::lang::Pointer<ByteArrayAdapter> _array;

        // Offset into the array that we are to start from
        int offset;

        // The number of bytes we are limited to.
        int length;

        // Read / Write flag
        bool readOnly;

    public:

        /**
         * Creates a ByteArrayBuffer object that has its backing array allocated internally
         * and is then owned and deleted when this object is deleted.  The array is
         * initially created with all elements initialized to zero.
         *
         * @param capacity
         *      The size of the array, this is the limit we read and write to.
         * @param readOnly
         *      Should this buffer be read-only, default as false
         *
         * @throws IllegalArguementException if the capacity value is negative.
         */
        ByteArrayBuffer( int capacity, bool readOnly = false );

        /**
         * Creates a ByteArrayBuffer object that wraps the given array.
         *
         * @param array
         *      The array to wrap.
         * @param size
         *      The size of the array passed.
         * @param offset
         *      The position that is this buffers start position.
         * @param length
         *      The size of the sub-array, this is the limit we read and write to.
         * @param readOnly
         *      Should this buffer be read-only, default as false.
         *
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if the preconditions of size, offset and
         *         length are violated.
         */
        ByteArrayBuffer( unsigned char* array, int size, int offset, int length,
                         bool readOnly = false );

        /**
         * Creates a byte buffer that wraps the passed ByteArrayAdapter and
         * start at the given offset.  The capacity and limit of the new ByteArrayBuffer
         * will be that of the remaining capacity of the passed buffer.
         *
         * @param array
         *      The ByteArrayAdapter to wrap
         * @param offset
         *      The offset into array where the buffer starts
         * @param length
         *      The length of the array we are wrapping or limit.
         * @param readOnly
         *      Boolean indicating if this a readOnly buffer.
         *
         * @throws NullPointerException if array is NULL
         * @throws IndexOutOfBoundsException if offset is greater than array capacity.
         */
        ByteArrayBuffer( const decaf::lang::Pointer<ByteArrayAdapter>& array,
                         int offset, int length, bool readOnly = false );

        /**
         * Create a ByteArrayBuffer that mirrors this one, meaning it shares a
         * reference to this buffers ByteArrayAdapter and when changes
         * are made to that data it is reflected in both.
         *
         * @param other
         *      The ByteArrayBuffer this one is to mirror.
         */
        ByteArrayBuffer( const ByteArrayBuffer& other );

        virtual ~ByteArrayBuffer();

    public:

        /**
         * {@inheritDoc}
         */
        virtual bool isReadOnly() const {
            return this->readOnly;
        }

        /**
         * {@inheritDoc}
         */
        virtual unsigned char* array();

        /**
         * {@inheritDoc}
         */
        virtual int arrayOffset() const;

        /**
         * {@inheritDoc}
         */
        virtual bool hasArray() const { return true; }

    public:   // Abstract Methods

        /**
         * {@inheritDoc}
         */
        virtual decaf::nio::CharBuffer* asCharBuffer() const { return NULL; } //TODO

        /**
         * {@inheritDoc}
         */
        virtual decaf::nio::DoubleBuffer* asDoubleBuffer() const { return NULL; } //TODO

        /**
         * {@inheritDoc}
         */
        virtual decaf::nio::FloatBuffer* asFloatBuffer() const { return NULL; } //TODO

        /**
         * {@inheritDoc}
         */
        virtual decaf::nio::IntBuffer* asIntBuffer() const { return NULL; } //TODO

        /**
         * {@inheritDoc}
         */
        virtual decaf::nio::LongBuffer* asLongBuffer() const { return NULL; } //TODO

        /**
         * {@inheritDoc}
         */
        virtual decaf::nio::ShortBuffer* asShortBuffer() const { return NULL; } //TODO

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer* asReadOnlyBuffer() const;

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& compact();

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer* duplicate();

        /**
         * {@inheritDoc}
         */
        virtual unsigned char get() const;

        /**
         * {@inheritDoc}
         */
        virtual unsigned char get( int index ) const;

        /**
         * {@inheritDoc}
         */
        virtual char getChar() {
            return (char)this->get();
        }

        /**
         * {@inheritDoc}
         */
        virtual char getChar( int index ) const {

            return (char)this->get( index );
        }

        /**
         * {@inheritDoc}
         */
        virtual double getDouble();

        /**
         * {@inheritDoc}
         */
        virtual double getDouble( int index ) const;

        /**
         * {@inheritDoc}
         */
        virtual float getFloat();

        /**
         * {@inheritDoc}
         */
        virtual float getFloat( int index ) const;

        /**
         * {@inheritDoc}
         */
        virtual long long getLong();

        /**
         * {@inheritDoc}
         */
        virtual long long getLong( int index ) const;

        /**
         * {@inheritDoc}
         */
        virtual int getInt();

        /**
         * {@inheritDoc}
         */
        virtual int getInt( int index ) const;

        /**
         * {@inheritDoc}
         */
        virtual short getShort();

        /**
         * {@inheritDoc}
         */
        virtual short getShort( int index ) const;

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& put( unsigned char value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& put( int index, unsigned char value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putChar( char value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putChar( int index, char value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putDouble( double value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putDouble( int index, double value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putFloat( float value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putFloat( int index, float value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putLong( long long value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putLong( int index, long long value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putInt( int value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putInt( int index, int value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putShort( short value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer& putShort( int index, short value );

        /**
         * {@inheritDoc}
         */
        virtual ByteArrayBuffer* slice() const;

    protected:

        /**
         * Sets this ByteArrayBuffer as Read-Only or not Read-Only.
         *
         * @param value
         *      Boolean value, true if this buffer is to be read-only, false otherwise.
         */
        virtual void setReadOnly( bool value ) {
            this->readOnly = value;
        }

    };

}}}

#endif /*_DECAF_INTERNAL_NIO_BYTEBUFFER_H_*/
