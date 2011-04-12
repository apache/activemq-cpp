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

#ifndef _DECAF_LANG_STRING_H_
#define _DECAF_LANG_STRING_H_

#include <decaf/util/Config.h>

#include <decaf/lang/CharSequence.h>
#include <decaf/lang/Comparable.h>

#include <string>

namespace decaf {
namespace lang {

    class Contents;

    /**
     * The String class represents an immutable sequence of chars.
     *
     * @since 1.0
     */
    class DECAF_API String : public CharSequence {
    private:

        Contents* contents;

    public:  // Constructors

        /**
         * Creates a new empty String object.
         */
        String();

        /**
         * Create a new String object that represents the given STL string
         *
         * @param source
         *      The string to copy into this new String object.
         */
        String( const String& source );

        /**
         * Create a new String object that represents the given STL string
         *
         * @param source
         *      The string to copy into this new String object.
         */
        String( const std::string& source );

        /**
         * Create a new String object that represents the given array of characters.  The method
         * takes the size of the array as a parameter to allow for strings that are not NULL
         * terminated, the caller can pass strlen(array) in the case where the array is properly
         * NULL terminated.
         *
         * @param array
         *      The character buffer to copy into this new String object.
         * @param size
         *      The size of the string buffer given, in case the string is not NULL terminated.
         *
         * @throws NullPointerException if the character array parameter is NULL.
         * @throws IndexOutOfBoundsException if the size parameter is negative.
         */
        String( const char* array, int size );

        /**
         * Create a new String object that represents the given array of characters.  The method
         * takes the size of the array as a parameter to allow for strings that are not NULL
         * terminated, the caller can pass strlen(array) in the case where the array is properly
         * NULL terminated.
         *
         * @param array
         *      The character buffer to copy into this new String object.
         * @param size
         *      The size of the string buffer given, in case the string is not NULL terminated.
         * @param offset
         *      The position to start copying from in the given buffer.
         * @param length
         *      The number of bytes to copy from the given buffer starting from the offset.
         *
         * @throws NullPointerException if the character array parameter is NULL.
         * @throws IndexOutOfBoundsException if the size, offset or length parameter is negative
         *         or if the length to copy is greater than the span of size - offset.
         */
        String( const char* array, int size, int offset, int length );

        virtual ~String();

    public:

        // TODO
        String& operator= ( const String& );
        String& operator= ( const std::string& );

    public:  // String API

        /**
         * @returns true if the length of this String is zero.
         */
        bool isEmpty() const;

    public:  // CharSequence Implementation

        /**
         * {@inheritDoc}
         */
        virtual int length() const;

        /**
         * {@inheritDoc}
         */
        virtual char charAt( int index ) const;

        /**
         * {@inheritDoc}
         */
        virtual CharSequence* subSequence( int start, int end ) const;

        /**
         * {@inheritDoc}
         */
        virtual std::string toString() const;

    public:  // Static methods.

        /**
         * Returns a String that represents the value of the given boolean value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @returns "true" if the boolean is true, "false" otherwise.
         */
        static String valueOf(bool value);

        /**
         * Returns a String that represents the value of the given char value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @returns a String that contains the single character value given.
         */
        static String valueOf(char value);

        /**
         * Returns a String that represents the value of the given float value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @returns a String that contains the string representation of the float value given.
         */
        static String valueOf(float value);

        /**
         * Returns a String that represents the value of the given double value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @returns a String that contains the string representation of the double value given.
         */
        static String valueOf(double value);

        /**
         * Returns a String that represents the value of the given short value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @returns a String that contains the string representation of the short value given.
         */
        static String valueOf(short value);

        /**
         * Returns a String that represents the value of the given integer value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @returns a String that contains the string representation of the integer value given.
         */
        static String valueOf(int value);

        /**
         * Returns a String that represents the value of the given 64bit long value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @returns a String that contains the string representation of the 64 bit long value given.
         */
        static String valueOf(long long value);

    };

}}

#endif /* _DECAF_LANG_STRING_H_ */
