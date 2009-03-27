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

#ifndef _DECAF_INTERNAL_NIO_INTARRAYBUFFER_H_
#define _DECAF_INTERNAL_NIO_INTARRAYBUFFER_H_

#include <decaf/nio/IntBuffer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/nio/BufferUnderflowException.h>
#include <decaf/nio/BufferOverflowException.h>
#include <decaf/nio/ReadOnlyBufferException.h>
#include <decaf/internal/nio/ByteArrayPerspective.h>

namespace decaf{
namespace internal{
namespace nio{

    class DECAF_API IntArrayBuffer : public decaf::nio::IntBuffer {
    private:

        // Read / Write flag
        bool readOnly;

        // The reference array object that backs this buffer.
        internal::nio::ByteArrayPerspective* _array;

        // Offset into the array that we are to start from
        std::size_t offset;

    public:

        /**
         * Creates a IntArrayBuffer object that has its backing array allocated internally
         * and is then owned and deleted when this object is deleted.  The array is
         * initially created with all elements initialized to zero.
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param readOnly - should this buffer be read-only, default as false
         */
        IntArrayBuffer( std::size_t capactiy, bool readOnly = false );

        /**
         * Creates a IntArrayBuffer object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param offset - the position that is this buffers start pos.
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param readOnly - should this buffer be read-only, default as false
         * @throws NullPointerException if buffer is NULL
         */
        IntArrayBuffer( int* array, std::size_t offset,
                        std::size_t capacity, bool readOnly = false )
            throw( decaf::lang::exceptions::NullPointerException );

        /**
         * Creates a byte buffer that wraps the passed ByteArrayPerspective and
         * start at the given offset.  The capacity and limit of the new IntArrayBuffer
         * will be that of the remaining capacity of the passed buffer.
         * @param array - the ByteArrayPerspective to wrap
         * @param offset - the offset into array where the buffer starts
         * @param capacity - the length of the array we are wrapping or limit.
         * @param readOnly - is this a readOnly buffer.
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if offset is greater than array capacity.
         */
        IntArrayBuffer( ByteArrayPerspective& array,
                        std::size_t offset, std::size_t capacity,
                        bool readOnly = false )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Create a IntArrayBuffer that mirros this one, meaning it shares a
         * reference to this buffers ByteArrayPerspective and when changes
         * are made to that data it is reflected in both.
         * @param other - the IntArrayBuffer this one is to mirror.
         * @param readOnly - should this buffer be read-only, default as false
         */
        IntArrayBuffer( const IntArrayBuffer& other );

        virtual ~IntArrayBuffer();

    public:

        /**
         * Returns the int array that backs this buffer  (optional operation).
         * <p>
         * Modifications to this buffer's content will cause the returned array's content
         * to be modified, and vice versa.
         * <p>
         * Invoke the hasArray method before invoking this method in order to ensure that
         * this buffer has an accessible backing array.
         * @returns the array that backs this Buffer
         * @throws ReadOnlyBufferException if this Buffer is read only.
         * @throws UnsupportedOperationException if the underlying store has no array.
         */
        virtual int* array()
            throw( decaf::lang::exceptions::UnsupportedOperationException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Returns the offset within this buffer's backing array of the first element of
         * the buffer  (optional operation).
         * <p>
         * Invoke the hasArray method before invoking this method in order to ensure that
         * this buffer has an accessible backing array.
         * @returns The offset into the backing array where index zero starts.
         * @throws ReadOnlyBufferException if this Buffer is read only.
         * @throws UnsupportedOperationException if the underlying store has no array.
         */
        virtual std::size_t arrayOffset()
            throw( decaf::lang::exceptions::UnsupportedOperationException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Creates a new, read-only int buffer that shares this buffer's content.
         * <p>
         * The content of the new buffer will be that of this buffer. Changes to this
         * buffer's content will be visible in the new buffer; the new buffer itself,
         * however, will be read-only and will not allow the shared content to be
         * modified. The two buffers' position, limit, and mark values will be
         * independent.
         * <p>
         * If this buffer is itself read-only then this method behaves in exactly the
         * same way as the duplicate method.
         * <p>
         * The new buffer's capacity, limit, position, and mark values will be
         * identical to those of this buffer.
         * @return The new, read-only int buffer which the caller then owns.
         */
        virtual IntBuffer* asReadOnlyBuffer() const;

        /**
         * Compacts this buffer
         * <p>
         * The bytes between the buffer's current position and its limit, if any, are
         * copied to the beginning of the buffer. That is, the byte at index
         * p = position() is copied to index zero, the byte at index p + 1 is copied
         * to index one, and so forth until the byte at index limit() - 1 is copied
         * to index n = limit() - 1 - p. The buffer's position is then set to n+1 and
         * its limit is set to its capacity. The mark, if defined, is discarded.
         * <p>
         * The buffer's position is set to the number of bytes copied, rather than to
         * zero, so that an invocation of this method can be followed immediately by
         * an invocation of another relative put method.
         * @returns a reference to this IntBuffer
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        virtual IntBuffer& compact() throw( decaf::nio::ReadOnlyBufferException );

        /**
         * Creates a new int buffer that shares this buffer's content.
         * <p>
         * The content of the new buffer will be that of this buffer. Changes to this
         * buffer's content will be visible in the new buffer, and vice versa; the two
         * buffers' position, limit, and mark values will be independent.
         * <p>
         * The new buffer's capacity, limit, position, and mark values will be identical
         * to those of this buffer. The new buffer will be read-only if, and only if,
         * this buffer is read-only.
         * @returns a new int Buffer which the caller owns.
         */
        virtual IntBuffer* duplicate();

        /**
         * Relative get method. Reads the value at this buffer's current position,
         * and then increments the position.
         * @returns the int at the current position
         * @throws BufferUnderflowException if there no more data to return
         */
        virtual int get() throw ( decaf::nio::BufferUnderflowException );

        /**
         * Absolute get method. Reads the value at the given index.
         * @param index - the index in the Buffer where the int is to be read
         * @returns the int that is located at the given index
         * @throws IndexOutOfBoundsException - If index is not smaller than the
         * buffer's limit
         */
        virtual int get( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Tells whether or not this buffer is backed by an accessible int array.
         * If this method returns true then the array and arrayOffset methods may safely
         * be invoked.  Subclasses should override this method if they do not have a
         * backing array as this class always returns true.
         * @returns true if, and only if, this buffer is backed by an array and is not
         * read-only
         */
        virtual bool hasArray() const { return true; }

        /**
         * Tells whether or not this buffer is read-only.
         * @returns true if, and only if, this buffer is read-only
         */
        virtual bool isReadOnly() const {
            return this->readOnly;
        }

        /**
         * Writes the given doubles into this buffer at the current position, and then
         * increments the position.
         * @param value - the doubles value to be written
         * @returns a reference to this buffer
         * @throws BufferOverflowException - If this buffer's current position is not
         * smaller than its limit
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        virtual IntBuffer& put( int value )
            throw( decaf::nio::BufferOverflowException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes the given doubles into this buffer at the given index.
         * @param index - position in the Buffer to write the data
         * @param value - the doubles to write.
         * @returns a reference to this buffer
         * @throws IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        virtual IntBuffer& put( std::size_t index, int value )
            throw( lang::exceptions::IndexOutOfBoundsException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Creates a new IntBuffer whose content is a shared subsequence of this
         * buffer's content.  The content of the new buffer will start at this buffer's
         * current position. Changes to this buffer's content will be visible in the new
         * buffer, and vice versa; the two buffers' position, limit, and mark values will
         * be independent.
         * <p>
         * The new buffer's position will be zero, its capacity and its limit will be the
         * number of bytes remaining in this buffer, and its mark will be undefined. The
         * new buffer will be read-only if, and only if, this buffer is read-only.
         * @returns the newly create IntBuffer which the caller owns.
         */
        virtual IntBuffer* slice() const;

    protected:

        /**
         * Sets this ByteArrayBuffer as Read-Only.
         * @param value - true if this buffer is to be read-only.
         */
        virtual void setReadOnly( bool value ) {
            this->readOnly = value;
        }

    };

}}}

#endif /*_DECAF_INTERNAL_NIO_INTARRAYBUFFER_H_*/
