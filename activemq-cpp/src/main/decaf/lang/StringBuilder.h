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
    class DECAF_API StringBuilder : public AbstractStringBuilder {
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

        /**
         * Appends the string representation of the given object pointer.  If the pointer
         * is NULL then the value "null" is appended to this StringBuilder.
         *
         * @param pointer
         *      A pointer to some object that must define a toString method.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
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
         * @returns a reference to this StringBuilder so that operations can be chained.
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
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(bool value);

        /**
         * Appends the given char value into the internal char buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(char value);

        /**
         * Appends the given short value into the internal char buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(short value);

        /**
         * Appends the given int value into the internal char buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(int value);

        /**
         * Appends the given long long value into the internal char buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(long long value);

        /**
         * Appends the given float value into the internal char buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(float value);

        /**
         * Appends the given double value into the internal char buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(double value);

        /**
         * Appends the contents of the given C string into this buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(const char* value);

        /**
         * Appends the given subsequence of the given C string into this buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(const char* value, int offset, int length);

        /**
         * Appends the contents of the CharSequence into this buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(const CharSequence* value);

        /**
         * Appends the given subsequence of the given CharSequence into this buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(const CharSequence* value, int offset, int length);

        /**
         * Appends the contents of the String into this buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
         */
        StringBuilder& append(const String& value);

        /**
         * Appends the contents of the StringBuffer into this buffer.
         *
         * @returns a reference to this StringBuilder so that operations can be chained.
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
         * @returns a reference to this StringBuilder so that operations can be chained.
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
         * @throws StringIndexOutOfBoundsException
         *      if the index is negative or greater than or equal to length().
         */
        StringBuilder& deleteCharAt(int index);

    };

}}

#endif /* _DECAF_LANG_STRINGBUILDER_H_ */
