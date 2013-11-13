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

#ifndef _DECAF_LANG_ABSTRACTSTRINGBUILDER_H_
#define _DECAF_LANG_ABSTRACTSTRINGBUILDER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/String.h>

#include <decaf/internal/util/concurrent/SynchronizableImpl.h>

namespace decaf {
namespace lang {

    class AbstractStringBuilderImpl;

    /**
     * A modifiable sequence of characters for use in creating and modifying Strings.
     * This class is intended as a base class for StringBuffer and StringBuilder.
     *
     * @see StringBuffer
     * @see StringBuilder
     *
     * @since 1.0
     */
    class DECAF_API AbstractStringBuilder : public decaf::internal::util::concurrent::SynchronizableImpl {
    protected:

        static const int INITIAL_CAPACITY;

    private:

        AbstractStringBuilderImpl* impl;

    public:

        AbstractStringBuilder();

        AbstractStringBuilder(int capacity);

        AbstractStringBuilder(const String& source);

        AbstractStringBuilder(const std::string& source);

        AbstractStringBuilder(const CharSequence* source);

        virtual ~AbstractStringBuilder();

    public:

        /**
         * Returns the current capacity. The capacity indicates the amount of space available in
         * the internal character buffer, when the number of characters inserted exceeds the current
         * capacity the internal buffer is reallocated and old contents copied to the new buffer.
         *
         * @returns the current capacity value.
         */
        virtual int capacity() const;

        /**
         * Returns the character at the given index.
         *
         * @param index
         *      The index in this buffer where the character to return is located.
         *
         * @throws IndexOutOfBoundsException if index < 0 or index >= length().
         */
        char charAt(int index) const;

        /**
         * Ensures that the capacity is at least equal to the specified min value. If
         * the current capacity is less than the given value, then this buffer backing this
         * instance will be reallocated and the old contents copied into the new buffer. The
         * new capacity is set to either the given value or twice the old capacity + 2
         * depending on which is larger.
         *
         * If the minimumCapacity argument is negative this method does nothing.
         *
         * @param minCapacity
         *      The minimum capacity to ensure exists in this buffer.
         */
        virtual void ensureCapacity(int minCapacity);

        /**
         * Returns the current length of the String that has been built.
         *
         * @returns the current number of characters that have been inserted.
         */
        virtual int length() const;

        /**
         * Sets the length of this character buffer. The backing buffer is changed to a new
         * character buffer whose length is specified by the argument.  Each character in the
         * old buffer is copied into the new buffer up to the given length value.  If the new
         * length is greater than the old buffer length then the additional character are all
         * set to '\0'.
         *
         * @param length
         *      The new length to give this character buffer
         *
         * @throws IndexOutOfBoundsException if length is less than zero.
         */
        virtual void setLength(int length);

        /**
         * Sets the character at the specified index to the new char value given.
         *
         * @param index
         *      The index of the character to modify.
         * @param value
         *      The new char value to assign at the given index.
         *
         * @throws IndexOutOfBoundsException if index is negative or greater than length().
         */
        virtual void setCharAt(int index, char value);

        /**
         * Returns a String that represents the contents of this buffer.  Any changes
         * made to this buffer after calling this method will not be reflected in the
         * String value that is returned.
         */
        virtual String toString() const;

        /**
         * Attempts to reduce storage used for the character sequence. If the buffer
         * is larger than necessary to hold its current sequence of characters, then
         * it may be resized to become more space efficient. Calling this method may,
         * but is not required to, affect the value returned by a subsequent call to
         * the capacity() method.
         */
        virtual void trimToSize();

    protected:

        /**
         * Appends the string "null" to the current character buffer.
         */
        void doAppendNull();

        /**
         * Appends the given char to this buffer.
         *
         * @param value
         *      The char value to be appended into this buffer.
         */
        void doAppend(const char value);

        /**
         * Appends the given C string to this buffer.
         *
         * @param value
         *      The C string value to be appended into this buffer.
         *
         * @throws NullPointerException if the pointer is NULL.
         */
        void doAppend(const char* value);

        /**
         * Appends the given C string to this buffer starting at the given offset and
         * ending after the length number of characters has been appened.
         *
         * @param value
         *      The C string value to be appended into this buffer.
         * @param offset
         *      The starting position into the C string array.
         * @param length
         *      The number of characters to copy from the given array.
         *
         * @throws NullPointerException if the pointer is NULL.
         * @throws IndexOutOfBoundsException if offset or length is negative or the value of
         *         offset + length is greater than the strings length.
         */
        void doAppend(const char* value, int offset, int length);

        /**
         * Appends the given CharSequence to this buffer.
         *
         * @param value
         *      The CharSequence value to be appended into this buffer.
         *
         * @throws NullPointerException if the pointer is NULL.
         */
        void doAppend(const CharSequence* value);

        /**
         * Appends the given CharSequence to this buffer starting at the given offset and
         * ending after the length number of characters has been appened.
         *
         * @param value
         *      The CharSequence value to be appended into this buffer.
         * @param offset
         *      The starting position into the CharSequence.
         * @param length
         *      The number of characters to copy from the given CharSequence.
         *
         * @throws NullPointerException if the pointer is NULL.
         * @throws IndexOutOfBoundsException if offset or length is negative or the value of
         *         offset + length is greater than the strings length.
         */
        void doAppend(const CharSequence* value, int offset, int length);

        /**
         * Append the given std::string to this buffer.
         *
         * @param value
         *      The String instance to append into this buffer.
         */
        void doAppend(const std::string& value);

        /**
         * Append the given String to this buffer.
         *
         * @param value
         *      The String instance to append into this buffer.
         */
        void doAppend(const String& value);

        /**
         * Append the given AbstractStringBuilder to this buffer.
         *
         * @param value
         *      The String instance to append into this buffer.
         */
        void doAppend(const AbstractStringBuilder& value);

        /**
         * Delete the characters in the range start - end.
         *
         * @param start
         *      The starting index of the delete operation
         * @param end
         *      The ending index of the delete operation (exclusive).
         */
        void doDeleteRange(int start, int end);

        /**
         * Deletes the character at the given index from this buffer.
         *
         * @param index
         *      The index of the character to delete.
         */
        void doDeleteCharAt(int index);

    };

}}

#endif /* _DECAF_LANG_ABSTRACTSTRINGBUILDER_H_ */
