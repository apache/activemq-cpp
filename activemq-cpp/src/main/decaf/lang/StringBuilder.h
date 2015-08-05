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

#ifndef _DECAF_LANG_STRINGBUILDER_H_
#define _DECAF_LANG_STRINGBUILDER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/AbstractStringBuilder.h>
#include <decaf/lang/String.h>
#include <decaf/lang/CharSequence.h>
#include <decaf/lang/Appendable.h>
#include <decaf/lang/Pointer.h>

namespace decaf {
namespace lang {

    class StringBuffer;

    /**
     * A modifiable sequence of characters for use in creating and modifying Strings.
     * This class is intended as a direct replacement of StringBuffer for non-concurrent
     * use; unlike StringBuffer this class is not synchronized for thread safety.
     *
     * The majority of the modification methods on this class return a reference to this
     * StringBuilder, so that, like StringBuffers, they can be used in chaining method
     * calls together. For example:
     *
     *  StringBuilder("One should ").append("always strive ").append("to achieve Harmony")}.
     *
     * @see CharSequence
     * @see Appendable
     * @see StringBuffer
     * @see String
     *
     * @since 1.0
     */
    class DECAF_API StringBuilder : public AbstractStringBuilder,
                                    public Appendable {
    public:

        /**
         * Creates an empty StringBuilder instance with a capacity of 16.
         */
        StringBuilder();

        /**
         * Creates an empty StringBuilder instance with the given capacity.
         *
         * @param capacity
         *      The initial capacity to give this new instance.
         *
         * @throws NegativeArraySizeException if the given capacity is less than zero.
         */
        StringBuilder(int capacity);

        /**
         * Constructs a string builder initialized to the contents of the specified string.
         * The initial capacity of the string builder is 16 plus the length of the string
         * argument.
         *
         * @param source
         *      The String whose contents are to be copied into this StringBuilder.
         */
        StringBuilder(const String& source);

        /**
         * Constructs a string builder initialized to the contents of the specified string.
         * The initial capacity of the string builder is 16 plus the length of the string
         * argument.
         *
         * @param source
         *      The CharSequence whose contents are to be copied into this StringBuilder.
         *
         * @throws NullPointerException if the CharSequence pointer is NULL.
         */
        StringBuilder(const CharSequence* source);

        virtual ~StringBuilder();

    public:

        virtual int length() const;

        /**
         * Appends the string representation of the given object pointer.  If the pointer
         * is NULL then the value "null" is appended to this StringBuilder.
         *
         * @param pointer
         *      A pointer to some object that must define a toString method.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        template<typename POINTER>
        StringBuilder& append(const POINTER* pointer) {

            if (pointer == NULL) {
                doAppendNull();
            } else {
                doAppend(pointer->toString());
            }

            return *this;
        }

        /**
         * Appends the string representation of the given object pointer.  If the pointer
         * is NULL then the value "null" is appended to this StringBuilder.
         *
         * @param pointer
         *      A pointer to some object that must define a toString method.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        template<typename TYPE>
        StringBuilder& append(const Pointer<TYPE> pointer) {

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
         *      The value to append to the contents of the StringBuilder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(bool value);

        /**
         * Appends the given char value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuilder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(char value);

        /**
         * Appends the given short value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuilder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(short value);

        /**
         * Appends the given int value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuilder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(int value);

        /**
         * Appends the given long long value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuilder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(long long value);

        /**
         * Appends the given float value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuilder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(float value);

        /**
         * Appends the given double value into the internal char buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuilder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(double value);

        /**
         * Appends the contents of the given C string into this buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuilder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(const char* value);

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
        StringBuilder& append(const char* value, int offset, int length);

        /**
         * Appends the contents of the CharSequence into this buffer, if the CharSequence
         * pointer is NULL then this method appends the string "null" to this Buffer.
         *
         * @param value
         *      The CharSequence value to be appended into this buffer.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(const CharSequence* value);

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
        StringBuilder& append(const CharSequence* value, int offset, int length);

        /**
         * Appends the contents of the String into this buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuilder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(const String& value);

        /**
         * Appends the contents of the StringBuffer into this buffer.
         *
         * @param value
         *      The value to append to the contents of the StringBuilder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(const StringBuffer& value);

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
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws StringIndexOutOfBoundsException
         *      if start is negative, greater than length(), or greater than end.
         */
        StringBuilder& deleteRange(int start, int end);

        /**
         * Deletes the char at the specified position in this buffer, length decreases by one.
         *
         * @param index
         *      The index in this buffer where the character to delete is located.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws StringIndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& deleteCharAt(int index);

        /**
         * Inserts the string representation of the given object pointer.  If the pointer
         * is NULL then the value "null" is inserted to this StringBuilder.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param pointer
         *      A pointer to some object that must define a toString method.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        template<typename POINTER>
        StringBuilder& insert(int index, const POINTER* pointer) {

            if (pointer == NULL) {
                doInsert(index, "null");
            } else {
                doInsert(index, pointer->toString());
            }

            return *this;
        }

        /**
         * Inserts the string representation of the given object pointer.  If the pointer
         * is NULL then the value "null" is inserted to this StringBuilder.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param pointer
         *      A pointer to some object that must define a toString method.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        template<typename TYPE>
        StringBuilder& insert(int index, const Pointer<TYPE> pointer) {

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
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, char value);

        /**
         * Inserts the given boolean into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(bool).
         *
         * @param index
         *      The position in the buffer to insert the boolean value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, bool value);

        /**
         * Inserts the given short into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(short).
         *
         * @param index
         *      The position in the buffer to insert the short value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, short value);

        /**
         * Inserts the given int into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(int).
         *
         * @param index
         *      The position in the buffer to insert the int value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, int value);

        /**
         * Inserts the given long long into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(long long).
         *
         * @param index
         *      The position in the buffer to insert the long long value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, long long value);

        /**
         * Inserts the given float into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(float).
         *
         * @param index
         *      The position in the buffer to insert the float value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, float value);

        /**
         * Inserts the given double into the character buffer at the given index.  The value
         * is converted to a String in the same fashion as calling String::valueOf(double).
         *
         * @param index
         *      The position in the buffer to insert the double value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, double value);

        /**
         * Inserts the given C string into the character buffer at the given index.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws NullPointerException if the target C string pointer is NULL.
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, const char* value);

        /**
         * Inserts the given String into the character buffer at the given index.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws NullPointerException if the target C string pointer is NULL.
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, const String& value);

        /**
         * Inserts the given std::string into the character buffer at the given index.
         *
         * @param index
         *      The position in the buffer to insert the char value.
         * @param value
         *      The value to insert at the given index.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws NullPointerException if the target std::string pointer is NULL.
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, const std::string& value);

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
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws NullPointerException if the target C string pointer is NULL.
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         *      If offset or length is negative or offset > the string length + length.
         */
        StringBuilder& insert(int index, const char* value, int offset, int length);

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
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& insert(int index, const CharSequence* value);

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
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         *      If offset or length is negative or offset > the string length + length.
         */
        StringBuilder& insert(int index, const CharSequence* value, int offset, int length);

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
         * @return a reference to this StringBuilder so that operations can be chained.
         *
         * @throws IndexOutOfBoundsException if start is negative, greater than end or greater than length().
         */
        StringBuilder& replace(int start, int end, const String& value);

        /**
         * Reverses the order of characters in this builder.
         *
         * @return a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& reverse();

    };

}}

#endif /* _DECAF_LANG_STRINGBUILDER_H_ */
