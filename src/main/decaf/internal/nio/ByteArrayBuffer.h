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
#include <decaf/nio/BufferUnderflowException.h>
#include <decaf/nio/BufferOverflowException.h>
#include <decaf/nio/ReadOnlyBufferException.h>
#include <decaf/internal/nio/ByteArrayPerspective.h>

#include <decaf/nio/CharBuffer.h>
#include <decaf/nio/DoubleBuffer.h>
#include <decaf/nio/FloatBuffer.h>
#include <decaf/nio/ShortBuffer.h>
#include <decaf/nio/IntBuffer.h>
#include <decaf/nio/LongBuffer.h>

namespace decaf{
namespace internal{
namespace nio{

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
     */
    class DECAF_API ByteArrayBuffer : public decaf::nio::ByteBuffer {
    private:

        // Read / Write flag
        bool readOnly;

        // The reference array object that backs this buffer.
        internal::nio::ByteArrayPerspective* _array;

        // Offset into the array that we are to start from
        std::size_t offset;

    public:

        /**
         * Creates a ByteArrayBuffer object that has its backing array allocated internally
         * and is then owned and deleted when this object is deleted.  The array is
         * initially created with all elements initialized to zero.
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param readOnly - should this buffer be read-only, default as false
         */
        ByteArrayBuffer( std::size_t capactiy, bool readOnly = false );

        /**
         * Creates a ByteArrayBuffer object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param offset - the position that is this buffers start pos.
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param readOnly - should this buffer be read-only, default as false
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayBuffer( unsigned char* array, std::size_t offset,
                         std::size_t capacity, bool readOnly = false )
            throw( decaf::lang::exceptions::NullPointerException );

        /**
         * Creates a byte buffer that wraps the passed ByteArrayPerspective and
         * start at the given offset.  The capacity and limit of the new ByteArrayBuffer
         * will be that of the remaining capacity of the passed buffer.
         * @param array - the ByteArrayPerspective to wrap
         * @param offset - the offset into array where the buffer starts
         * @param length - the length of the array we are wrapping or limit.
         * @param readOnly - is this a readOnly buffer.
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if offset is greater than array capacity.
         */
        ByteArrayBuffer( ByteArrayPerspective& array,
                         std::size_t offset, std::size_t length,
                         bool readOnly = false )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Create a ByteArrayBuffer that mirros this one, meaning it shares a
         * reference to this buffers ByteArrayPerspective and when changes
         * are made to that data it is reflected in both.
         * @param other - the ByteArrayBuffer this one is to mirror.
         * @param readOnly - should this buffer be read-only, default as false
         */
        ByteArrayBuffer( const ByteArrayBuffer& other );

        virtual ~ByteArrayBuffer();

        /**
         * Tells whether or not this buffer is read-only.
         * @returns true if, and only if, this buffer is read-only
         */
        virtual bool isReadOnly() const {
            return this->readOnly;
        }

        /**
         * Returns the byte array that backs this buffer
         * <p>
         * Modifications to this buffer's content will cause the returned array's
         * content to be modified, and vice versa.
         * <p>
         * Invoke the hasArray method before invoking this method in order to ensure
         * that this buffer has an accessible backing array.
         * @returns The array that backs this buffer
         * @throws ReadOnlyBufferException - If this buffer is backed by an array but
         * is read-only
         * @throws UnsupportedOperationException - If this buffer is not backed by an
         * accessible array
         */
        virtual unsigned char* array()
            throw( decaf::nio::ReadOnlyBufferException,
                   decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Returns the offset within this buffer's backing array of the first element
         * of the buffer.
         * <p>
         * If this buffer is backed by an array then buffer position p corresponds to
         * array index p + arrayOffset().
         * <p>
         * Invoke the hasArray method before invoking this method in order to ensure
         * that this buffer has an accessible backing array.
         * @returns The offset within this buffer's array of the first element of
         * the buffer
         * @throws ReadOnlyBufferException - If this buffer is backed by an array but
         * is read-only
         * @throws UnsupportedOperationException - If this buffer is not backed by an
         * accessible array
         */
        virtual std::size_t arrayOffset() const
            throw( decaf::nio::ReadOnlyBufferException,
                   lang::exceptions::UnsupportedOperationException );

        /**
         * Tells whether or not this buffer is backed by an accessible byte array.
         * If this method returns true then the array and arrayOffset methods may safely
         * be invoked.  Subclasses should override this method if they do not have a
         * backing array as this class always returns true.
         * @returns true if, and only if, this buffer is backed by an array and is not
         * read-only
         */
        virtual bool hasArray() const { return true; }

    public:   // Abstract Methods

        /**
         * Creates a view of this byte buffer as a char buffer.
         * <p>
         * The content of the new buffer will start at this buffer's current position.
         * Changes to this buffer's content will be visible in the new buffer, and vice
         * versa; the two buffers' position, limit, and mark values will be independent.
         * <p>
         * The new buffer's position will be zero, its capacity and its limit will be
         * the number of bytes remaining in this buffer, and its mark will be undefined.
         * The new buffer will be read-only if, and only if, this buffer is read-only.
         * @returns the new Char Buffer, which the caller then owns.
         */
        virtual decaf::nio::CharBuffer* asCharBuffer() const { return NULL; } //TODO

        /**
         * Creates a view of this byte buffer as a double buffer.
         * <p>
         * The content of the new buffer will start at this buffer's current position.
         * Changes to this buffer's content will be visible in the new buffer, and vice
         * versa; the two buffers' position, limit, and mark values will be independent.
         * <p>
         * The new buffer's position will be zero, its capacity and its limit will be
         * the number of bytes remaining in this buffer divided by eight, and its mark
         * will be undefined. The new buffer will be read-only if, and only if, this
         * buffer is read-only.
         * @returns the new double Buffer, which the caller then owns.
         */
        virtual decaf::nio::DoubleBuffer* asDoubleBuffer() const { return NULL; } //TODO

        /**
         * Creates a view of this byte buffer as a float buffer.
         * <p>
         * The content of the new buffer will start at this buffer's current position.
         * Changes to this buffer's content will be visible in the new buffer, and vice
         * versa; the two buffers' position, limit, and mark values will be independent.
         * <p>
         * The new buffer's position will be zero, its capacity and its limit will be
         * the number of bytes remaining in this buffer divided by four, and its mark
         * will be undefined. The new buffer will be read-only if, and only if, this
         * buffer is read-only.
         * @returns the new float Buffer, which the caller then owns.
         */
        virtual decaf::nio::FloatBuffer* asFloatBuffer() const { return NULL; } //TODO

        /**
         * Creates a view of this byte buffer as a int buffer.
         * <p>
         * The content of the new buffer will start at this buffer's current position.
         * Changes to this buffer's content will be visible in the new buffer, and vice
         * versa; the two buffers' position, limit, and mark values will be independent.
         * <p>
         * The new buffer's position will be zero, its capacity and its limit will be
         * the number of bytes remaining in this buffer divided by four, and its mark
         * will be undefined. The new buffer will be read-only if, and only if, this
         * buffer is read-only.
         * @returns the new int Buffer, which the caller then owns.
         */
        virtual decaf::nio::IntBuffer* asIntBuffer() const { return NULL; } //TODO

        /**
         * Creates a view of this byte buffer as a long buffer.
         * <p>
         * The content of the new buffer will start at this buffer's current position.
         * Changes to this buffer's content will be visible in the new buffer, and vice
         * versa; the two buffers' position, limit, and mark values will be independent.
         * <p>
         * The new buffer's position will be zero, its capacity and its limit will be
         * the number of bytes remaining in this buffer divided by eight, and its mark
         * will be undefined. The new buffer will be read-only if, and only if, this
         * buffer is read-only.
         * @returns the new long Buffer, which the caller then owns.
         */
        virtual decaf::nio::LongBuffer* asLongBuffer() const { return NULL; } //TODO

        /**
         * Creates a view of this byte buffer as a short buffer.
         * <p>
         * The content of the new buffer will start at this buffer's current position.
         * Changes to this buffer's content will be visible in the new buffer, and vice
         * versa; the two buffers' position, limit, and mark values will be independent.
         * <p>
         * The new buffer's position will be zero, its capacity and its limit will be
         * the number of bytes remaining in this buffer divided by two, and its mark
         * will be undefined. The new buffer will be read-only if, and only if, this
         * buffer is read-only.
         * @returns the new short Buffer, which the caller then owns.
         */
        virtual decaf::nio::ShortBuffer* asShortBuffer() const { return NULL; } //TODO

        /**
         * Creates a new, read-only byte buffer that shares this buffer's content.
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
         * @return The new, read-only byte buffer which the caller then owns.
         */
        virtual ByteArrayBuffer* asReadOnlyBuffer() const;

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
         * @returns a reference to this ByteArrayBuffer
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& compact() throw( decaf::nio::ReadOnlyBufferException );

        /**
         * Creates a new byte buffer that shares this buffer's content.
         * <p>
         * The content of the new buffer will be that of this buffer. Changes to this
         * buffer's content will be visible in the new buffer, and vice versa; the two
         * buffers' position, limit, and mark values will be independent.
         * <p>
         * The new buffer's capacity, limit, position, and mark values will be identical
         * to those of this buffer. The new buffer will be read-only if, and only if,
         * this buffer is read-only.
         * @returns a new Byte Buffer which the caller owns.
         */
        virtual ByteArrayBuffer* duplicate();

        /**
         * Relative get method. Reads the byte at this buffer's current position, and
         * then increments the position.
         * @returns The byte at the buffer's current position
         * @throws BufferUnderflowException - If the buffer's current position is not
         * smaller than its limit
         */
        virtual unsigned char get() const throw( decaf::nio::BufferUnderflowException );

        /**
         * Absolute get method. Reads the byte at the given index.
         * @param index - the index in the Buffer where the byte is to be read
         * @returns the byte that is located at the given index
         * @throws IndexOutOfBoundsException - If index is not smaller than the
         * buffer's limit
         */
        virtual unsigned char get( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads the next byte at this buffer's current position, and then increments
         * the position by one
         * @returns the next char in the buffer..
         * @throws BufferUnderflowException - If there are no more bytes remaining in
         * this buffer, meaning we have reached the set limit.
         */
        virtual char getChar() throw( decaf::nio::BufferUnderflowException ) {
            return (char)this->get();
        }

        /**
         * Reads one byte at the given index and returns it
         * @param index - the index in the Buffer where the byte is to be read
         * @returns the char at the given index in the buffer
         * @throws IndexOutOfBoundsException - If index is not smaller than the
         * buffer's limit
         */
        virtual char getChar( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException ) {

            return (char)this->get( index );
        }

        /**
         * Reads the next eight bytes at this buffer's current position, and then
         * increments the position by that amount.
         * @returns the next double in the buffer..
         * @throws BufferUnderflowException - If there are no more bytes remaining in
         * this buffer, meaning we have reached the set limit.
         */
        virtual double getDouble() throw( decaf::nio::BufferUnderflowException );

        /**
         * Reads eight bytes at the given index and returns it
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the double at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual double getDouble( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads the next four bytes at this buffer's current position, and then
         * increments the position by that amount.
         * @returns the next float in the buffer..
         * @throws BufferUnderflowException - If there are no more bytes remaining in
         * this buffer, meaning we have reached the set limit.
         */
        virtual float getFloat() throw( decaf::nio::BufferUnderflowException );

        /**
         * Reads four bytes at the given index and returns it
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the float at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual float getFloat( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads the next eight bytes at this buffer's current position, and then
         * increments the position by that amount.
         * @returns the next long long in the buffer..
         * @throws BufferUnderflowException - If there are no more bytes remaining in
         * this buffer, meaning we have reached the set limit.
         */
        virtual long long getLong() throw( decaf::nio::BufferUnderflowException );

        /**
         * Reads eight bytes at the given index and returns it
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the long long at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual long long getLong( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads the next four bytes at this buffer's current position, and then
         * increments the position by that amount.
         * @returns the next int in the buffer..
         * @throws BufferUnderflowException - If there are no more bytes remaining in
         * this buffer, meaning we have reached the set limit.
         */
        virtual int getInt() throw( decaf::nio::BufferUnderflowException );

        /**
         * Reads four bytes at the given index and returns it
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the int at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual int getInt( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads the next two bytes at this buffer's current position, and then
         * increments the position by that amount.
         * @returns the next short in the buffer..
         * @throws BufferUnderflowException - If there are no more bytes remaining in
         * this buffer, meaning we have reached the set limit.
         */
        virtual short getShort() throw( decaf::nio::BufferUnderflowException );

        /**
         * Reads two bytes at the given index and returns it
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the short at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual short getShort( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes the given byte into this buffer at the current position, and then
         * increments the position.
         * @param value - the byte value to be written
         * @returns a reference to this buffer
         * @throws BufferOverflowException - If this buffer's current position is not
         * smaller than its limit
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& put( unsigned char value )
            throw( decaf::nio::BufferOverflowException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes the given byte into this buffer at the given index.
         * @param index - position in the Buffer to write the data
         * @param value - the byte to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& put( std::size_t index, unsigned char value )
            throw( lang::exceptions::IndexOutOfBoundsException,
                    decaf::nio::ReadOnlyBufferException );

        /**
         * Writes one byte containing the given value, into this buffer at the
         * current position, and then increments the position by one.
         * @param value - The value to be written
         * @returns a reference to this buffer
         * @throw BufferOverflowException - If there are fewer than bytes remaining
         * in this buffer than the size of the data to be written
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putChar( char value )
            throw( decaf::nio::BufferOverflowException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes one byte containing the given value, into this buffer at the
         * given index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putChar( std::size_t index, char value )
            throw( lang::exceptions::IndexOutOfBoundsException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * current position, and then increments the position by eight.
         * @param value - The value to be written
         * @returns a reference to this buffer
         * @throw BufferOverflowException - If there are fewer than bytes remaining
         * in this buffer than the size of the data to be written
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putDouble( double value )
            throw( decaf::nio::BufferOverflowException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * given index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putDouble( std::size_t index, double value )
            throw( lang::exceptions::IndexOutOfBoundsException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * current position, and then increments the position by eight.
         * @param value - The value to be written
         * @returns a reference to this buffer
         * @throw BufferOverflowException - If there are fewer than bytes remaining
         * in this buffer than the size of the data to be written
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putFloat( float value )
            throw( decaf::nio::BufferOverflowException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * given index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putFloat( std::size_t index, float value )
            throw( lang::exceptions::IndexOutOfBoundsException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * current position, and then increments the position by eight.
         * @param value - The value to be written
         * @returns a reference to this buffer
         * @throw BufferOverflowException - If there are fewer than bytes remaining
         * in this buffer than the size of the data to be written
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putLong( long long value )
            throw( decaf::nio::BufferOverflowException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * given index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putLong( std::size_t index, long long value )
            throw( lang::exceptions::IndexOutOfBoundsException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * current position, and then increments the position by eight.
         * @param value - The value to be written
         * @returns a reference to this buffer
         * @throw BufferOverflowException - If there are fewer than bytes remaining
         * in this buffer than the size of the data to be written
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putInt( int value )
            throw( decaf::nio::BufferOverflowException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * given index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putInt( std::size_t index, int value )
            throw( lang::exceptions::IndexOutOfBoundsException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes two bytes containing the given value, into this buffer at the
         * current position, and then increments the position by eight.
         * @param value - The value to be written
         * @returns a reference to this buffer
         * @throw BufferOverflowException - If there are fewer than bytes remaining
         * in this buffer than the size of the data to be written
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putShort( short value )
            throw( decaf::nio::BufferOverflowException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Writes two bytes containing the given value, into this buffer at the
         * given index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         * @throw ReadOnlyBufferException - If this buffer is read-only
         */
        virtual ByteArrayBuffer& putShort( std::size_t index, short value )
            throw( lang::exceptions::IndexOutOfBoundsException,
                   decaf::nio::ReadOnlyBufferException );

        /**
         * Creates a new byte buffer whose content is a shared subsequence of this
         * buffer's content.  The content of the new buffer will start at this buffer's
         * current position. Changes to this buffer's content will be visible in the new
         * buffer, and vice versa; the two buffers' position, limit, and mark values will
         * be independent.
         * <p>
         * The new buffer's position will be zero, its capacity and its limit will be the
         * number of bytes remaining in this buffer, and its mark will be undefined. The
         * new buffer will be read-only if, and only if, this buffer is read-only.
         * @returns the newly create ByteArrayBuffer which the caller owns.
         */
        virtual ByteArrayBuffer* slice() const;

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

#endif /*_DECAF_INTERNAL_NIO_BYTEBUFFER_H_*/
