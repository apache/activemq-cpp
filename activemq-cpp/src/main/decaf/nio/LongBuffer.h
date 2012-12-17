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

#ifndef _DECAF_NIO_LONGBUFFER_H_
#define _DECAF_NIO_LONGBUFFER_H_

#include <decaf/nio/Buffer.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/nio/BufferUnderflowException.h>
#include <decaf/nio/BufferOverflowException.h>
#include <decaf/nio/ReadOnlyBufferException.h>

namespace decaf{
namespace nio{

    /**
     * This class defines four categories of operations upon long long buffers:
     *
     * o Absolute and relative get and put methods that read and write single long longs;
     * o Relative bulk get methods that transfer contiguous sequences of long longs from
     *   this buffer long longo an array; and
     * o Relative bulk put methods that transfer contiguous sequences of long longs from a
     *   long long array or some other long long buffer long longo this buffer
     * o Methods for compacting, duplicating, and slicing a long long buffer.
     *
     * Double buffers can be created either by allocation, which allocates space for the
     * buffer's content, by wrapping an existing long long array long longo a buffer, or by
     * creating a view of an existing byte buffer
     *
     * Methods in this class that do not otherwise have a value to return are specified
     * to return the buffer upon which they are invoked. This allows method invocations
     * to be chained.
     */
    class DECAF_API LongBuffer : public Buffer,
                                 public lang::Comparable<LongBuffer> {
    protected:

        /**
         * Creates a LongBuffer object that has its backing array allocated long longernally
         * and is then owned and deleted when this object is deleted.  The array is
         * initially created with all elements initialized to zero.
         *
         * @param capacity
         *       The size and limit of the Buffer in long longs.
         *
         * @throws IllegalArguementException if capacity is negative.
         */
        LongBuffer( int capacity );

    public:

        virtual ~LongBuffer() {}

        /**
         * @returns a std::string describing this object
         */
        virtual std::string toString() const;

        /**
         * Returns the long long array that backs this buffer  (optional operation).
         *
         * Modifications to this buffer's content will cause the returned array's content
         * to be modified, and vice versa.
         *
         * Invoke the hasArray method before invoking this method in order to ensure that
         * this buffer has an accessible backing array.
         *
         * @returns the array that backs this Buffer.
         *
         * @throws ReadOnlyBufferException if this Buffer is read only.
         * @throws UnsupportedOperationException if the underlying store has no array.
         */
        virtual long long* array() = 0;

        /**
         * Returns the offset within this buffer's backing array of the first element of
         * the buffer  (optional operation).
         *
         * Invoke the hasArray method before invoking this method in order to ensure that
         * this buffer has an accessible backing array.
         *
         * @returns The offset long longo the backing array where index zero starts.
         *
         * @throws ReadOnlyBufferException if this Buffer is read only.
         * @throws UnsupportedOperationException if the underlying store has no array.
         */
        virtual int arrayOffset() = 0;

        /**
         * Creates a new, read-only long long buffer that shares this buffer's content.
         *
         * The content of the new buffer will be that of this buffer. Changes to this
         * buffer's content will be visible in the new buffer; the new buffer itself,
         * however, will be read-only and will not allow the shared content to be
         * modified. The two buffers' position, limit, and mark values will be
         * independent.
         *
         * If this buffer is itself read-only then this method behaves in exactly the
         * same way as the duplicate method.
         *
         * The new buffer's capacity, limit, position, and mark values will be
         * identical to those of this buffer.
         *
         * @return The new, read-only long long buffer which the caller then owns.
         */
        virtual LongBuffer* asReadOnlyBuffer() const = 0;

        /**
         * Compacts this buffer
         *
         * The bytes between the buffer's current position and its limit, if any, are
         * copied to the beginning of the buffer. That is, the byte at index
         * p = position() is copied to index zero, the byte at index p + 1 is copied
         * to index one, and so forth until the byte at index limit() - 1 is copied
         * to index n = limit() - 1 - p. The buffer's position is then set to n+1 and
         * its limit is set to its capacity. The mark, if defined, is discarded.
         *
         * The buffer's position is set to the number of bytes copied, rather than to
         * zero, so that an invocation of this method can be followed immediately by
         * an invocation of another relative put method.
         *
         * @returns a reference to this LongBuffer.
         *
         * @throws ReadOnlyBufferException if this buffer is read-only.
         */
        virtual LongBuffer& compact() = 0;

        /**
         * Creates a new long long buffer that shares this buffer's content.
         *
         * The content of the new buffer will be that of this buffer. Changes to this
         * buffer's content will be visible in the new buffer, and vice versa; the two
         * buffers' position, limit, and mark values will be independent.
         *
         * The new buffer's capacity, limit, position, and mark values will be identical
         * to those of this buffer. The new buffer will be read-only if, and only if,
         * this buffer is read-only.
         *
         * @returns a new long long Buffer which the caller owns.
         */
        virtual LongBuffer* duplicate() = 0;

        /**
         * Relative get method. Reads the value at this buffer's current position,
         * and then increments the position.
         *
         * @returns the long long at the current position.
         *
         * @throws BufferUnderflowException if there no more data to return.
         */
        virtual long long get() = 0;

        /**
         * Absolute get method. Reads the value at the given index.
         *
         * @param index
         *      The index in the Buffer where the long long is to be read.
         *
         * @returns the long long that is located at the given index.
         *
         * @throws IndexOutOfBoundsException if index is not smaller than the
         *         buffer's limit, or index is negative.
         */
        virtual long long get( int index ) const = 0;

        /**
         * Relative bulk get method.
         *
         * This method transfers values from this buffer long longo the given destination
         * vector. An invocation of this method of the form src.get(a) behaves in
         * exactly the same way as the invocation.  The vector must be sized to the
         * amount of data that is to be read, that is to say, the caller should call
         * buffer.resize( N ) before calling this get method.
         *
         * @returns a reference to this Buffer.
         *
         * @throws BufferUnderflowException if there are fewer than length long longs
         *         remaining in this buffer.
         */
        LongBuffer& get( std::vector<long long> buffer );

        /**
         * Relative bulk get method.
         *
         * This method transfers long longs from this buffer long longo the given destination array.
         * If there are fewer long longs remaining in the buffer than are required to satisfy
         * the request, that is, if length > remaining(), then no bytes are transferred
         * and a BufferUnderflowException is thrown.
         *
         * Otherwise, this method copies length long longs from this buffer long longo the given
         * array, starting at the current position of this buffer and at the given offset
         * in the array. The position of this buffer is then incremented by length.
         *
         * @param buffer
         *      The pointer to an allocated long long buffer to fill.
         * @param size
         *      The size of the passed in buffer.
         * @param offset
         *      The position in the buffer to start filling.
         * @param length
         *      The amount of data to put in the passed buffer.
         *
         * @returns a reference to this Buffer.
         *
         * @throws BufferUnderflowException if there are fewer than length long longs
         *         remaining in this buffer
         * @throws NullPolong longerException if the passed buffer is null.
         * @throws IndexOutOfBoundsException if the preconditions of size, offset, or length
         *         are not met.
         */
        LongBuffer& get( long long* buffer, int size, int offset, int length );

        /**
         * Tells whether or not this buffer is backed by an accessible long long array.
         * If this method returns true then the array and arrayOffset methods may safely
         * be invoked.  Subclasses should override this method if they do not have a
         * backing array as this class always returns true.
         *
         * @returns true if, and only if, this buffer is backed by an array and is not
         *          read-only.
         */
        virtual bool hasArray() const = 0;

        /**
         * This method transfers the long longs remaining in the given source buffer long longo
         * this buffer. If there are more long longs remaining in the source buffer than in
         * this buffer, that is, if src.remaining() > remaining(), then no long longs are
         * transferred and a BufferOverflowException is thrown.
         *
         * Otherwise, this method copies n = src.remaining() long longs from the given
         * buffer long longo this buffer, starting at each buffer's current position. The
         * positions of both buffers are then incremented by n.
         *
         * @param src
         *      The buffer to take long longs from an place in this one.
         *
         * @returns a reference to this buffer.
         *
         * @throws BufferOverflowException if there is insufficient space in this
         *         buffer for the remaining long longs in the source buffer
         * @throws IllegalArgumentException if the source buffer is this buffer
         * @throws ReadOnlyBufferException if this buffer is read-only
         */
        LongBuffer& put( LongBuffer& src );

        /**
         * This method transfers long longs long longo this buffer from the given source array.
         * If there are more long longs to be copied from the array than remain in this buffer,
         * that is, if length > remaining(), then no long longs are transferred and a
         * BufferOverflowException is thrown.
         *
         * Otherwise, this method copies length bytes from the given array long longo this
         * buffer, starting at the given offset in the array and at the current position
         * of this buffer. The position of this buffer is then incremented by length.
         *
         * @param buffer
         *      The array from which long longs are to be read.
         * @param size
         *      The size of the buffer passed.
         * @param offset
         *      The offset within the array of the first char to be read.
         * @param length
         *      The number of long longs to be read from the given array.
         *
         * @returns a reference to this buffer.
         *
         * @throws BufferOverflowException if there is insufficient space in this buffer
         * @throws ReadOnlyBufferException if this buffer is read-only
         * @throws NullPolong longerException if the passed buffer is null.
         * @throws IndexOutOfBoundsException if the preconditions of size, offset, or length
         *         are not met.
         */
        LongBuffer& put( const long long* buffer, int size, int offset, int length );

        /**
         * This method transfers the entire content of the given source long longs array long longo
         * this buffer.  This is the same as calling put( &buffer[0], 0, buffer.size().
         *
         * @param buffer
         *      The buffer whose contents are copied to this LongBuffer.
         *
         * @returns a reference to this buffer.
         *
         * @throws BufferOverflowException if there is insufficient space in this buffer.
         * @throws ReadOnlyBufferException if this buffer is read-only.
         */
        LongBuffer& put( std::vector<long long>& buffer );

        /**
         * Writes the given long longs long longo this buffer at the current position, and then
         * increments the position.
         *
         * @param value
         *      The long longs value to be written.
         *
         * @returns a reference to this buffer.
         *
         * @throws BufferOverflowException if this buffer's current position is not
         *         smaller than its limit
         * @throws ReadOnlyBufferException if this buffer is read-only
         */
        virtual LongBuffer& put( long long value ) = 0;

        /**
         * Writes the given long longs long longo this buffer at the given index.
         *
         * @param index
         *      The position in the Buffer to write the data
         * @param value
         *      The long longs to write.
         *
         * @returns a reference to this buffer.
         *
         * @throws IndexOutOfBoundsException if index greater than the buffer's limit
         *         minus the size of the type being written.
         * @throws ReadOnlyBufferException if this buffer is read-only
         */
        virtual LongBuffer& put( int index, long long value ) = 0;

        /**
         * Creates a new LongBuffer whose content is a shared subsequence of this
         * buffer's content.  The content of the new buffer will start at this buffer's
         * current position. Changes to this buffer's content will be visible in the new
         * buffer, and vice versa; the two buffers' position, limit, and mark values will
         * be independent.
         *
         * The new buffer's position will be zero, its capacity and its limit will be the
         * number of bytes remaining in this buffer, and its mark will be undefined. The
         * new buffer will be read-only if, and only if, this buffer is read-only.
         *
         * @returns the newly create LongBuffer which the caller owns.
         */
        virtual LongBuffer* slice() const = 0;

    public:  // Comparable

        /**
         * {@inheritDoc}
         */
        virtual int compareTo( const LongBuffer& value ) const;

        /**
         * {@inheritDoc}
         */
        virtual bool equals( const LongBuffer& value ) const;

        /**
         * {@inheritDoc}
         */
        virtual bool operator==( const LongBuffer& value ) const;

        /**
         * {@inheritDoc}
         */
        virtual bool operator<( const LongBuffer& value ) const;

    public:   // Statics

        /**
         * Allocates a new Double buffer.
         *
         * The new buffer's position will be zero, its limit will be its capacity, and
         * its mark will be undefined. It will have a backing array, and its array offset
         * will be zero.
         *
         * @param capacity
         *      The size of the Double buffer in long longs.
         *
         * @returns the LongBuffer that was allocated, caller owns.
         */
        static LongBuffer* allocate( int capacity );

        /**
         * Wraps the passed buffer with a new LongBuffer.
         *
         * The new buffer will be backed by the given long long array; that is, modifications
         * to the buffer will cause the array to be modified and vice versa. The new
         * buffer's capacity will be array.length, its position will be offset, its limit
         * will be offset + length, and its mark will be undefined. Its backing array
         * will be the given array, and its array offset will be zero.
         *
         * @param array
         *      The array that will back the new buffer.
         * @param size
         *      The size of the passed in array.
         * @param offset
         *      The offset of the subarray to be used.
         * @param length
         *      The length of the subarray to be used.
         *
         * @returns a new LongBuffer that is backed by buffer, caller owns.
         *
         * @throws NullPointerException if the array pointer is NULL.
         * @throws IndexOutOfBoundsException if the preconditions of size, offset, or length
         *         are not met.
         */
        static LongBuffer* wrap( long long* array, int size, int offset, int length );

        /**
         * Wraps the passed STL long long Vector in a LongBuffer.
         *
         * The new buffer will be backed by the given long long array; modifications to the
         * buffer will cause the array to be modified and vice versa. The new buffer's
         * capacity and limit will be buffer.size(), its position will be zero, and its
         * mark will be undefined. Its backing array will be the given array, and its
         * array offset will be zero.
         *
         * @param buffer
         *      The vector that will back the new buffer, the vector must have been sized
         *      to the desired size already by calling vector.resize( N ).
         *
         * @returns a new LongBuffer that is backed by buffer, caller owns.
         */
        static LongBuffer* wrap( std::vector<long long>& buffer );

    };

}}

#endif /*_DECAF_NIO_LONGBUFFER_H_*/
