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

#ifndef _DECAF_NIO_CHARBUFFER_H_
#define _DECAF_NIO_CHARBUFFER_H_

#include <decaf/nio/Buffer.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/nio/BufferUnderflowException.h>
#include <decaf/nio/BufferOverflowException.h>
#include <decaf/nio/ReadOnlyBufferException.h>
#include <decaf/lang/CharSequence.h>
#include <decaf/lang/Appendable.h>

namespace decaf{
namespace nio{

    /**
     * This class defines four categories of operations upon character buffers:
     *
     * o Absolute and relative get and put methods that read and write single characters;
     * o Relative bulk get methods that transfer contiguous sequences of characters from
     *   this buffer into an array; and
     * o Relative bulk put methods that transfer contiguous sequences of characters from
     *   a character array, a string, or some other character buffer into this buffer.
     * o Methods for compacting, duplicating, and slicing a character buffer.
     *
     * Character buffers can be created either by allocation, which allocates space for
     * the buffer's content, by wrapping an existing character array or string into a
     * buffer, or by creating a view of an existing byte buffer
     *
     * This class implements the CharSequence interface so that character buffers may
     * be used wherever character sequences are accepted, for example in the
     * regular-expression package decaf.util.regex.
     *
     * Methods in this class that do not otherwise have a value to return are specified
     * to return the buffer upon which they are invoked. This allows method invocations
     * to be chained. The sequence of statements
     *
     *     cb.put("text/");
     *     cb.put(subtype);
     *     cb.put("; charset=");
     *     cb.put(enc);
     *
     * can, for example, be replaced by the single statement
     *
     *     cb.put("text/").put(subtype).put("; charset=").put(enc);
     */
    class DECAF_API CharBuffer : public Buffer,
                                 public lang::CharSequence,
                                 public lang::Appendable,
                                 public lang::Comparable<CharBuffer> {
    protected:

        /**
         * Creates a CharBuffer object that has its backing array allocated internally
         * and is then owned and deleted when this object is deleted.  The array is
         * initially created with all elements initialized to zero.
         * @param capacity - size of the array, this is the limit we read and write to.
         */
        CharBuffer( std::size_t capacity );

    public:

        virtual ~CharBuffer() {}

        /**
         * @returns a std::string describing this object
         */
        virtual std::string toString() const;

        /**
         * Appends the specified character to this buffer
         * @param value - the char to append.
         * @returns a reference to this modified CharBuffer
         * @throws BufferOverflowException if there is no more space
         * @throws ReadOnlyBufferException if this Buffer is read only.
         */
        CharBuffer& append( char value )
            throw ( BufferOverflowException, ReadOnlyBufferException );

        /**
         * Appends the specified character sequence to this buffer.
         * If value is Null the the string "null" is appended to the buffer.
         * @param value - the CharSequence to append.
         * @returns a reference to this modified CharBuffer
         * @throws BufferOverflowException if there is no more space
         * @throws ReadOnlyBufferException if this Buffer is read only.
         */
        CharBuffer& append( const lang::CharSequence* value )
            throw ( BufferOverflowException, ReadOnlyBufferException );

        /**
         * Appends a subsequence of the specified character sequence to this buffer
         * If value is Null the the string "null" is appended to the buffer.
         * @param value - the CharSequence to append.
         * @param start - the index to start appending from.
         * @param end - the index to append to.
         * @returns a reference to this modified CharBuffer
         * @throws BufferOverflowException if there is no more space
         * @throws ReadOnlyBufferException if this Buffer is read only.
         * @throws IndexOutOfBoundsException if start > end, or > length of sequence.
         */
        CharBuffer& append( const lang::CharSequence* value, std::size_t start, std::size_t end )
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException,
                    BufferOverflowException, ReadOnlyBufferException );

        /**
         * Returns the character array that backs this buffer  (optional operation).
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
        virtual char* array()
            throw( decaf::lang::exceptions::UnsupportedOperationException,
                   ReadOnlyBufferException ) = 0;

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
                   ReadOnlyBufferException ) = 0;

        /**
         * Creates a new, read-only char buffer that shares this buffer's content.
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
         * @return The new, read-only char buffer which the caller then owns.
         */
        virtual CharBuffer* asReadOnlyBuffer() const = 0;

        /**
         * Reads the character at the given index relative to the current position.
         * <p>
         * @param index - The index of the character to be read relative to position
         * @returns The character at index position() + index
         * @throws IndexOutOfBoundsException
         */
        char charAt( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

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
         * @returns a reference to this CharBuffer
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        virtual CharBuffer& compact() throw( ReadOnlyBufferException ) = 0;

        /**
         * Creates a new char buffer that shares this buffer's content.
         * <p>
         * The content of the new buffer will be that of this buffer. Changes to this
         * buffer's content will be visible in the new buffer, and vice versa; the two
         * buffers' position, limit, and mark values will be independent.
         * <p>
         * The new buffer's capacity, limit, position, and mark values will be identical
         * to those of this buffer. The new buffer will be read-only if, and only if,
         * this buffer is read-only.
         * @returns a new char Buffer which the caller owns.
         */
        virtual CharBuffer* duplicate() = 0;

        /**
         * Relative get method. Reads the character at this buffer's current position,
         * and then increments the position.
         * @returns the char at the current position
         * @throws BufferUnderflowException if there no more data to return
         */
        virtual char get() throw ( BufferUnderflowException ) = 0;

        /**
         * Absolute get method. Reads the char at the given index.
         * @param index - the index in the Buffer where the char is to be read
         * @returns the char that is located at the given index
         * @throws IndexOutOfBoundsException - If index is not smaller than the
         * buffer's limit
         */
        virtual char get( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException ) = 0;

        /**
         * Relative bulk get method.
         * <p>
         * This method transfers chars from this buffer into the given destination
         * vector. An invocation of this method of the form src.get(a) behaves in
         * exactly the same way as the invocation.  The vector must be sized to the
         * amount of data that is to be read, that is to say, the caller should call
         * buffer.resize( N ) before calling this get method.
         * @returns a reference to this CharBuffer
         * @throws BufferUnderflowException - If there are fewer than length chars
         * remaining in this buffer
         */
        CharBuffer& get( std::vector<char> buffer )
            throw ( BufferUnderflowException );

        /**
         * Relative bulk get method.
         * <p>
         * This method transfers chars from this buffer into the given destination array.
         * If there are fewer chars remaining in the buffer than are required to satisfy
         * the request, that is, if length > remaining(), then no bytes are transferred
         * and a BufferUnderflowException is thrown.
         * <p>
         * Otherwise, this method copies length chars from this buffer into the given
         * array, starting at the current position of this buffer and at the given offset
         * in the array. The position of this buffer is then incremented by length.
         * <p>
         * @param buffer - pointer to an allocated buffer to fill
         * @param offset - position in the buffer to start filling
         * @param length - amount of data to put in the passed buffer
         * @returns a reference to this Buffer
         * @throws BufferUnderflowException - If there are fewer than length chars
         * remaining in this buffer
         * @throws NullPointerException if the passed buffer is null.
         */
        CharBuffer& get( char* buffer, std::size_t offset, std::size_t length )
            throw( BufferUnderflowException,
                   lang::exceptions::NullPointerException );

        /**
         * Tells whether or not this buffer is backed by an accessible char array.
         * If this method returns true then the array and arrayOffset methods may safely
         * be invoked.  Subclasses should override this method if they do not have a
         * backing array as this class always returns true.
         * @returns true if, and only if, this buffer is backed by an array and is not
         * read-only
         */
        virtual bool hasArray() const = 0;

        /**
         * Returns the length of this character buffer.
         * @returns the length of this buffer from the position to the limit.
         */
        std::size_t length() const {
            return this->remaining();
        }

        /**
         * This method transfers the chars remaining in the given source buffer into
         * this buffer. If there are more chars remaining in the source buffer than in
         * this buffer, that is, if src.remaining() > remaining(), then no chars are
         * transferred and a BufferOverflowException is thrown.
         * <p>
         * Otherwise, this method copies n = src.remaining() chars from the given
         * buffer into this buffer, starting at each buffer's current position. The
         * positions of both buffers are then incremented by n.
         * @param src - the buffer to take chars from an place in this one.
         * @returns a reference to this buffer
         * @throws BufferOverflowException - If there is insufficient space in this
         * buffer for the remaining chars in the source buffer
         * @throws IllegalArgumentException - If the source buffer is this buffer
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        CharBuffer& put( CharBuffer& src )
            throw( BufferOverflowException, ReadOnlyBufferException,
                   lang::exceptions::IllegalArgumentException );

        /**
         * This method transfers chars into this buffer from the given source array.
         * If there are more chars to be copied from the array than remain in this buffer,
         * that is, if length > remaining(), then no chars are transferred and a
         * BufferOverflowException is thrown.
         * <p>
         * Otherwise, this method copies length bytes from the given array into this
         * buffer, starting at the given offset in the array and at the current position
         * of this buffer. The position of this buffer is then incremented by length.
         * @param buffer- The array from which chars are to be read
         * @param offset- The offset within the array of the first char to be read;
         * @param length - The number of chars to be read from the given array
         * @returns a reference to this buffer
         * @throws BufferOverflowException - If there is insufficient space in this buffer
         * @throws ReadOnlyBufferException - If this buffer is read-only
         * @throws NullPointerException if the passed buffer is null.
         */
        CharBuffer& put( const char* buffer, std::size_t offset, std::size_t length )
            throw( BufferOverflowException, ReadOnlyBufferException,
                   lang::exceptions::NullPointerException );

        /**
         * This method transfers the entire content of the given source char array into
         * this buffer.  This is the same as calling put( &buffer[0], 0, buffer.size()
         * @pparam buffer - The buffer whose contents are copied to this CharBuffer
         * @returns a reference to this buffer
         * @throws BufferOverflowException - If there is insufficient space in this buffer
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        CharBuffer& put( std::vector<char>& buffer )
            throw( BufferOverflowException, ReadOnlyBufferException );

        /**
         * Writes the given char into this buffer at the current position, and then
         * increments the position.
         * @param value - the char value to be written
         * @returns a reference to this buffer
         * @throws BufferOverflowException - If this buffer's current position is not
         * smaller than its limit
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        virtual CharBuffer& put( char value )
            throw( BufferOverflowException, ReadOnlyBufferException ) = 0;

        /**
         * Writes the given char into this buffer at the given index.
         * @param index - position in the Buffer to write the data
         * @param value - the char to write.
         * @returns a reference to this buffer
         * @throws IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        virtual CharBuffer& put( std::size_t index, char value )
            throw( lang::exceptions::IndexOutOfBoundsException,
                   ReadOnlyBufferException ) = 0;

        /**
         * Relative bulk put method  (optional operation).
         * <p>
         * This method transfers characters from the given string into this buffer. If
         * there are more characters to be copied from the string than remain in this
         * buffer, that is, if end - start > remaining(), then no characters are
         * transferred and a BufferOverflowException is thrown.
         * @returns a reference to this buffer
         * <p>
         * Otherwise, this method copies n = end - start characters from the given string
         * into this buffer, starting at the given start index and at the current position
         * of this buffer. The position of this buffer is then incremented by n.
         * @param src - the string to copy from
         * @param start - position in src to start from
         * @param end - the position in src to stop at
         * @returns a reference to this CharBuffer
         * @throws BufferOverflowException - If this buffer's current position is not
         * @throws IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        CharBuffer& put( const std::string& src, std::size_t start, std::size_t end )
            throw( BufferOverflowException, ReadOnlyBufferException,
                   decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Relative bulk put method  (optional operation).
         * <p>
         * This method transfers the entire content of the given source string into this
         * buffer. An invocation of this method of the form dst.put(s) behaves in exactly
         * the same way as the invocation
         * @param src - the string to copy from
         * @returns a reference to this CharBuffer
         * @throws BufferOverflowException - If this buffer's current position is not
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        CharBuffer& put( const std::string& src )
            throw( BufferOverflowException, ReadOnlyBufferException );

        /**
         * Attempts to read characters into the specified character buffer. The buffer is
         * used as a repository of characters as-is: the only changes made are the results
         * of a put operation. No flipping or rewinding of the buffer is performed.
         * @param target - the buffer to read characters into
         * @returns The number of characters added to the buffer, or string::npos if this
         * source of characters is at its end
         * @throws NullPointerException - If target is Null
         * @throws IllegalArgumentException - If target is this
         * @throws ReadOnlyBufferException - If this buffer is read-only
         */
        virtual std::size_t read( CharBuffer* target )
            throw ( decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IllegalArgumentException,
                    ReadOnlyBufferException );

        /**
         * Creates a new character buffer that represents the specified subsequence of
         * this buffer, relative to the current position.
         * <p>
         * The new buffer will share this buffer's content; that is, if the content of
         * this buffer is mutable then modifications to one buffer will cause the other
         * to be modified. The new buffer's capacity will be that of this buffer, its
         * position will be position() + start, and its limit will be position() + end.
         * The new Buffer will be read-only if, and only if, this buffer is read-only.
         * @param start - The index, relative to the current position, of the first
         * character in the subsequence; must be non-negative and no larger than
         * remaining()
         * @param end - The index, relative to the current position, of the character
         * following the last character in the subsequence; must be no smaller than start
         * and no larger than remaining()
         * @return The new character buffer, caller owns
         * @throws IndexOutOfBoundsException - If the preconditions on start and end fail
         */
        virtual lang::CharSequence* subSequence( std::size_t start, std::size_t end ) const
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException ) = 0;

        /**
         * Creates a new CharBuffer whose content is a shared subsequence of this
         * buffer's content.  The content of the new buffer will start at this buffer's
         * current position. Changes to this buffer's content will be visible in the new
         * buffer, and vice versa; the two buffers' position, limit, and mark values will
         * be independent.
         * <p>
         * The new buffer's position will be zero, its capacity and its limit will be the
         * number of bytes remaining in this buffer, and its mark will be undefined. The
         * new buffer will be read-only if, and only if, this buffer is read-only.
         * @returns the newly create CharBuffer which the caller owns.
         */
        virtual CharBuffer* slice() const = 0;

    public:  // Comparable

        /**
         * Compares this object with the specified object for order. Returns a
         * negative integer, zero, or a positive integer as this object is less
         * than, equal to, or greater than the specified object.
         * @param value - the Object to be compared.
         * @returns a negative integer, zero, or a positive integer as this
         * object is less than, equal to, or greater than the specified object.
         */
        virtual int compareTo( const CharBuffer& value ) const;

        /**
         * @return true if this value is considered equal to the passed value.
         */
        virtual bool equals( const CharBuffer& value ) const;

        /**
         * Compares equality between this object and the one passed.
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const CharBuffer& value ) const;

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const CharBuffer& value ) const;

    public:   // Statics

        /**
         * Allocates a new character buffer.
         * <p>
         * The new buffer's position will be zero, its limit will be its capacity, and
         * its mark will be undefined. It will have a backing array, and its array offset
         * will be zero.
         * @param capacity - The size of the Char buffer in chars ( 1 byte ).
         * @returns the CharBuffer that was allocated, caller owns.
         */
        static CharBuffer* allocate( std::size_t capacity );

        /**
         * Wraps the passed buffer with a new CharBuffer.
         * <p>
         * The new buffer will be backed by the given char array; that is, modifications
         * to the buffer will cause the array to be modified and vice versa. The new
         * buffer's capacity will be array.length, its position will be offset, its limit
         * will be offset + length, and its mark will be undefined. Its backing array
         * will be the given array, and its array offset will be zero.
         * @param buffer - The array that will back the new buffer
         * @param offset - The offset of the subarray to be used
         * @param length - The length of the subarray to be used
         * @returns a new CharBuffer that is backed by buffer, caller owns.
         */
        static CharBuffer* wrap( char* array, std::size_t offset, std::size_t length )
            throw( lang::exceptions::NullPointerException );

        /**
         * Wraps the passed STL char Vector in a CharBuffer.
         * <p>
         * The new buffer will be backed by the given char array; modifications to the
         * buffer will cause the array to be modified and vice versa. The new buffer's
         * capacity and limit will be buffer.size(), its position will be zero, and its
         * mark will be undefined. Its backing array will be the given array, and its
         * array offset will be zero.
         * @param buffer - The vector that will back the new buffer, the vector must
         * have been sized to the desired size already by calling vector.resize( N ).
         * @returns a new CharBuffer that is backed by buffer, caller owns.
         */
        static CharBuffer* wrap( std::vector<char>& buffer );

        /**
         * Wraps a character sequence into a buffer.
         * <p>
         * The content of the new, read-only buffer will be the content of the given
         * character sequence. The buffer's capacity will be csq.length(), its position
         * will be start, its limit will be end, and its mark will be undefined.
         * @param csq - The CharSequence that will back the new buffer
         * @param start - The index of the first character to be used; must be non-
         * negative and no larger than csq.length(). The new buffer's position will be
         * set to this value.
         * @param end - The index of the character following the last character to be
         * used; must be no smaller than start and no larger than
         * @returns a nre ReadOnly CharBuffer, caller owns
         * @throws NullPointerException if csq is null.
         * @throws IndexOutOfBoundsException if the preconditions on start and end fail
         */
// TODO
//        static CharBuffer* wrap( lang::CharSequence* csq, std::size_t start, std::size_t end )
//            throw( lang::exceptions::NullPointerException );

        /**
         * Wraps a full CharSequence into a buffer.
         * <p>
         * The content of the new, read-only buffer will be the content of the given
         * string. The new buffer's capacity and limit will be csq.length(), its position
         * will be zero, and its mark will be undefined.
         * <p>
         * @param csq - The character sequence from which the new character buffer is to
         * be created, cannot be null
         * @returns the newly created CharBuffer, caller owns.
         * @throws NullPointerException if csq is null.
         */
// TODO
//        static CharBuffer* wrap( lang::CharSequence* csq )
//            throw ( decaf::lang::exceptions::NullPointerException );

    };

}}

#endif /*_DECAF_NIO_CHARBUFFER_H_*/
