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

#ifndef _DECAF_LANG_STRINGBUFFER_H_
#define _DECAF_LANG_STRINGBUFFER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/AbstractStringBuilder.h>
#include <decaf/lang/String.h>
#include <decaf/lang/CharSequence.h>
#include <decaf/lang/Appendable.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/concurrent/Mutex.h>

namespace decaf {
namespace lang {

    class StringBuilder;

    /**
     * StringBuffer is a variable size contiguous indexable array of characters. The
     * length of the StringBuffer is the number of characters it contains. The capacity
     * of the StringBuffer is the number of characters it can hold.
     *
     * Characters may be inserted at any position up to the length of the StringBuffer,
     * increasing the length of the StringBuffer. Characters at any position in the
     * StringBuffer may be replaced, which does not affect the StringBuffer length.
     *
     * The capacity of a StringBuffer may be specified when the StringBuffer is
     * created. If the capacity of the StringBuffer is exceeded, the capacity is
     * increased.
     *
     * StringBuffer objects are safe for use by multiple threads. The methods are
     * synchronized where necessary so that all the operations on any particular instance
     * behave as if they occur in some serial order that is consistent with the order of
     * the method calls made by each of the individual threads involved.
     *
     * @see String
     * @see StringBuffer
     *
     * @since 1.0
     */
    class DECAF_API StringBuffer : public AbstractStringBuilder,
                                   public Appendable {
    public:

        /**
         * Creates an empty StringBuffer instance with a capacity of 16.
         */
        StringBuffer();

        /**
         * Creates an empty StringBuffer instance with the given capacity
         *
         * @param capacity
         *      The initial capacity to give this new instance.
         *
         * @throws NegativeArraySizeException if the given capacity is less than zero.
         */
        StringBuffer(int capacity);

        /**
         * Constructs a string buffer initialized to the contents of the specified string.
         * The initial capacity of the string builder is 16 plus the length of the string
         * argument.
         *
         * @param source
         *      The String whose contents are to be copied into this StringBuffer.
         */
        StringBuffer(const String& source);

        /**
         * Constructs a string buffer initialized to the contents of the specified string.
         * The initial capacity of the string builder is 16 plus the length of the string
         * argument.
         *
         * @param source
         *      The CharSequence whose contents are to be copied into this StringBuffer.
         *
         * @throws NullPointerException if the CharSequence pointer is NULL.
         */
        StringBuffer(const CharSequence* source);

        virtual ~StringBuffer();

    public:

        virtual int capacity() const;

        virtual char charAt(int index) const;

        virtual void ensureCapacity(int minCapacity);

        virtual void getChars(int start, int end, char* dst, int dstSize, int dstStart) const;

        virtual int indexOf(const String& value) const;

        virtual int indexOf(const String& value, int start) const;

        virtual int lastIndexOf(const String& value) const;

        virtual int lastIndexOf(const String& value, int start) const;

        virtual int length() const;

        virtual void setLength(int length);

        virtual void setCharAt(int index, char value);

        virtual String substring(int start) const;

        virtual String substring(int start, int end) const;

        virtual CharSequence* subSequence(int start, int end) const;

        virtual String toString() const;

        virtual void trimToSize();

    public:

        /**
         * Appends the string representation of the given object pointer.  If the pointer
         * is NULL then the value "null" is appended to this StringBuffer.
         *
         * @param pointer
         *      A pointer to some object that must define a toString method.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        template<typename POINTER>
        StringBuffer& append(const POINTER* pointer) {

            if (pointer == NULL) {
                doAppendNull();
            } else {
                doAppend(pointer->toString());
            }

            return *this;
        }

        /**
         * Appends the string representation of the given object pointer.  If the pointer
         * is NULL then the value "null" is appended to this StringBuffer.
         *
         * @param pointer
         *      A pointer to some object that must define a toString method.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        template<typename TYPE>
        StringBuffer& append(const Pointer<TYPE> pointer) {

            if (pointer == NULL) {
                doAppendNull();
            } else {
                doAppend(pointer->toString());
            }

            return *this;
        }

        /**
         * Appends the string representation of the given boolean value.
         *
         * @param value
         *      The value to append to the contents of the StringBuffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(bool value);

        /**
         * Appends the given char value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(char value);

        /**
         * Appends the given short value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(short value);

        /**
         * Appends the given int value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(int value);

        /**
         * Appends the given long long value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(long long value);

        /**
         * Appends the given float value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(float value);

        /**
         * Appends the given double value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(double value);

        /**
         * Appends the contents of the given C string into this buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(const char* value);

        /**
         * Appends the given subsequence of the given C string into this buffer.
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
        StringBuffer& append(const char* value, int offset, int length);

        /**
         * Appends the contents of the CharSequence into this buffer, if the CharSequence
         * pointer is NULL then this method appends the string "null" to this Buffer.
         *
         * @param value
         *      The CharSequence value to be appended into this buffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(const CharSequence* value);

        /**
         * Appends the given CharSequence to this buffer starting at the given offset and
         * ending after the length number of characters has been append.  If the given
         * CharSequence pointer is NULL then this method appends the string "null".
         *
         * @param value
         *      The CharSequence value to be appended into this buffer.
         * @param offset
         *      The starting position into the CharSequence.
         * @param length
         *      The number of characters to copy from the given CharSequence.
         *
         * @throws IndexOutOfBoundsException if offset or length is negative or the value of
         *         offset + length is greater than the strings length.
         */
        StringBuffer& append(const CharSequence* value, int offset, int length);

        /**
         * Appends the contents of the String into this buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(const String& value);

        /**
         * Appends the contents of the StringBuffer into this buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& append(const StringBuilder& value);

        /**
         * Removes the characters in a substring of this buffer. The substring begins at
         * the specified start and extends to the character at index end - 1 or to the
         * end of the sequence if end is greater than the current length() value. If
         * start is equal to end, no changes are made.
         *
         * @param start
         *      The starting index to delete from this buffer.
         * @param end
         *      The ending index (exclusive) to delete from this buffer.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws StringIndexOutOfBoundsException
         *      if start is negative, greater than length(), or greater than end.
         */
        StringBuffer& deleteRange(int start, int end);

        /**
         * Deletes the char at the specified position in this buffer, length decreases by one.
         *
         * @param index
         *      The index in this buffer where the character to delete is located.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws StringIndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& deleteCharAt(int index);

        /**
         * Inserts the string representation of the given object pointer.  If the pointer
         * is NULL then the value "null" is inserted to this StringBuffer.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param pointer
         *      A pointer to some object that must define a toString method.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        template<typename POINTER>
        StringBuffer& insert(int index, const POINTER* pointer) {

            if (pointer == NULL) {
                doInsert(index, "null");
            } else {
                doInsert(index, pointer->toString());
            }

            return *this;
        }

        /**
         * Inserts the string representation of the given object pointer.  If the pointer
         * is NULL then the value "null" is inserted to this StringBuffer.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param pointer
         *      A pointer to some object that must define a toString method.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        template<typename TYPE>
        StringBuffer& insert(int index, const Pointer<TYPE> pointer) {

            if (pointer == NULL) {
                doInsert(index, "null");
            } else {
                doInsert(index, pointer->toString());
            }

            return *this;
        }

        /**
         * Inserts the given char into the character buffer at the given index.  The contents
         * of the buffer are shifted up by one from the given index prior to insertion.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, char value);

        /**
         * Inserts the given boolean into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(bool).
         *
         * @param index
         *      The position in the buffer to insert the boolean value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, bool value);

        /**
         * Inserts the given short into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(short).
         *
         * @param index
         *      The position in the buffer to insert the short value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, short value);

        /**
         * Inserts the given int into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(int).
         *
         * @param index
         *      The position in the buffer to insert the int value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, int value);

        /**
         * Inserts the given long long into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(long long).
         *
         * @param index
         *      The position in the buffer to insert the long long value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, long long value);

        /**
         * Inserts the given float into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(float).
         *
         * @param index
         *      The position in the buffer to insert the float value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, float value);

        /**
         * Inserts the given double into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(double).
         *
         * @param index
         *      The position in the buffer to insert the double value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, double value);

        /**
         * Inserts the given C string into the character buffer at the given index.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws NullPointerException if the target C string pointer is NULL.
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, const char* value);

        /**
         * Inserts the given String into the character buffer at the given index.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws NullPointerException if the target C string pointer is NULL.
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, const String& value);

        /**
         * Inserts the given std::string into the character buffer at the given index.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws NullPointerException if the target std::string pointer is NULL.
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, const std::string& value);

        /**
         * Inserts the given C string into the character buffer at the given index starting
         * from the given offset into the string and copying up to length chars from the string
         * into this buffer.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param value
         *      The value to insert at the given index.
         * @param offset
         *      The offset into the C string to start the copy from.
         * @param length
         *      The number of characters to copy from the given C string.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws NullPointerException if the target C string pointer is NULL.
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         *      If offset or length is negative or offset > the string length + length.
         */
        StringBuffer& insert(int index, const char* value, int offset, int length);

        /**
         * Inserts the given CharSequence into the character buffer at the given index starting
         * from the given offset into the string and copying up to length chars from the string
         * into this buffer.  If the CharSequence pointer is NULL then this method inserts the
         * string "null" into this Buffer.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuffer& insert(int index, const CharSequence* value);

        /**
         * Inserts the given CharSequence into the character buffer at the given index starting
         * from the given offset into the string and copying up to length chars from the string
         * into this buffer.  If the CharSequence pointer is NULL then this method inserts the
         * string "null" into this Buffer.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param value
         *      The value to insert at the given index.
         * @param offset
         *      The offset into the CharSequence to start the copy from.
         * @param length
         *      The number of characters to copy from the given CharSequence.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         *      If offset or length is negative or offset > the string length + length.
         */
        StringBuffer& insert(int index, const CharSequence* value, int offset, int length);

        /**
         * Replace some number of characters in this Buffer with the value given.
         *
         * The characters replaced start at the given index and end at the given end value
         * (exclusive).  If the replacement string value is longer the internal buffer is
         * lengthened to accommodate the new value.
         *
         * @param start
         *      The starting index to replace in the buffer (inclusive).
         * @param end
         *      The ending index of the replacement operation (exclusive).
         * @param value
         *      The new string value to replace the older value.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException if start is negative, greater than end or greater than length().
         */
        StringBuffer& replace(int start, int end, const String& value);

        /**
         * Reverses the order of characters in this builder.
         *
         * @return a reference to this StringBuffer so that operations can be chained.
         */
        StringBuffer& reverse();

    };

}}

#endif /* _DECAF_LANG_STRINGBUFFER_H_ */
