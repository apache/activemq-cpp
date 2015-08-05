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
#include <decaf/lang/ArrayPointer.h>

#include <string>
#include <ostream>

namespace decaf {
namespace lang {

    class Contents;
    class AbstractStringBuilder;

    /**
     * An immutable sequence of characters.
     *
     * This class is implemented using a char[]. The length of the array may exceed
     * the length of the string. For example, the string "Hello" may be backed by
     * the array {@code ['H', 'e', 'l', 'l', 'o', 'W'. 'o', 'r', 'l', 'd']} with
     * offset 0 and length 5.
     *
     * Multiple strings can share the same char[] because strings are immutable.
     *
     * The substring method always returns a string that shares the backing array of
     * its source string. Generally this is an optimization: fewer character arrays
     * need to be allocated, and less copying is necessary. But this can also lead
     * to unwanted heap retention. Taking a short substring of long string means that
     * the long shared char[] won't be garbage until both strings are destroyed. This
     * typically happens when parsing small substrings out of a large input. To avoid
     * this where necessary, call the compact method which allocates a new array that
     * is just big enough to store the String's content.
     *
     * @since 1.0
     */
    class DECAF_API String: public CharSequence {
    private:

        mutable Contents* contents;

    public:

        /**
         * Creates a new empty String object.  This value is equivalent to
         * calling String("") and all methods will behave as if the string is
         * an empty string.
         */
        String();

        /**
         * Create a new String instance that contains N copies of the given character
         * value.
         *
         * @param value
         *      The character to fill this String with.
         * @param count
         *      The number of copies of the character to fill.
         *
         * @throws IndexOutOfBoundsException if the count parameter is negative.
         */
        String(const char value, int count);

        /**
         * Create a new String object that represents the given STL string
         *
         * @param source
         *      The string to copy into this new String object.
         */
        String(const String& source);

        /**
         * Create a new String object that represents the given STL string
         *
         * @param source
         *      The string to copy into this new String object.
         */
        String(const std::string& source);

        /**
         * Create a new String object that represents the given array of characters, the C string
         * must be null terminated in order for a proper size calculation to work.  If the string
         * is not properly terminated than this method can overrun the array and cause a fault.
         *
         * @param array
         *      The character buffer to copy into this new String object.
         *
         * @throws NullPointerException if the character array parameter is NULL.
         */
        String(const char* array);

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
        String(const char* array, int size);

        /**
         * Create a new String object that represents the given array of characters, the C string
         * must be null terminated in order for a proper size calculation to work.  If the string
         * is not properly terminated than this method can overrun the array and cause a fault.
         *
         * @param array
         *      The character buffer to copy into this new String object.
         * @param offset
         *      The position to start copying from in the given buffer.
         * @param length
         *      The number of bytes to copy from the given buffer starting from the offset.
         *
         * @throws NullPointerException if the character array parameter is NULL.
         * @throws IndexOutOfBoundsException if the size, offset or length parameter is negative
         *         or if the length to copy is greater than the span of size - offset.
         */
        String(const char* array, int offset, int length);

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
        String(const char* array, int size, int offset, int length);

        virtual ~String();

    public:

        /**
         * Assignment from another String instance.  The internal contents of this string now
         * reference the same contents as the provided String.  The original contents of this
         * String are not altered in other String instances that reference it.
         *
         * @param other
         *      The other String to assign to this instance.
         *
         * @return a reference to this string with the new contents.
         */
        String& operator= (const String& other);

        /**
         * Assignment from another std::string instance.  The internal contents of this string now
         * reference a copy of the contents as the provided std::string.  The original contents of
         * this String are not altered in other String instances that reference it.
         *
         * @param other
         *      The other std::string to assign to this instance.
         *
         * @return a reference to this string with the new contents.
         */
        String& operator= (const std::string& other);

        /**
         * Assignment from another C string instance.  The internal contents of this string now
         * reference a copy of the contents as the provided C string.  The original contents of
         * this String are not altered in other String instances that reference it.
         *
         * @param other
         *      The other C string to assign to this instance.
         *
         * @return a reference to this string with the new contents.
         */
        String& operator= (const char* other);

        /**
         * Comparison operators for the various string types that uses the equals method
         * to determine equality.
         *
         * @param other
         *      The string value to compare to this one.
         *
         * @return true if the other string is equal to this one, false otherwise.
         */
        bool operator==(const char* other) const;
        bool operator==(const String& other) const;
        bool operator==(const std::string& other) const;

        /**
         * Comparison operators for the various string types that uses the equals method
         * to determine equality.
         *
         * @param other
         *      The string value to compare to this one.
         *
         * @return true if the other string is not equal to this one, false otherwise.
         */
        bool operator!=(const char* other) const;
        bool operator!=(const String& other) const;
        bool operator!=(const std::string& other) const;

        /**
         * Comparison operators for the various string types that uses the compareTo method
         * to determine if the string is lexicographically less than the other.
         *
         * @param other
         *      The string value to compare to this one.
         *
         * @return true if this string is lexicographically less than the other string.
         */
        bool operator< (const char* other) const;
        bool operator< (const String& other) const;
        bool operator< (const std::string& other) const;

        /**
         * Comparison operators for the various string types that uses the compareTo method
         * to determine if the string is lexicographically less than or equal to the other.
         *
         * @param other
         *      The string value to compare to this one.
         *
         * @return true if this string is lexicographically less than or equal to the other string.
         */
        bool operator<=(const char* other) const;
        bool operator<=(const String& other) const;
        bool operator<=(const std::string& other) const;

        /**
         * Comparison operators for the various string types that uses the compareTo method
         * to determine if the string is lexicographically greater than the other.
         *
         * @param other
         *      The string value to compare to this one.
         *
         * @return true if this string is lexicographically greater than the other string.
         */
        bool operator> (const char* other) const;
        bool operator> (const String& other) const;
        bool operator> (const std::string& other) const;

        /**
         * Comparison operators for the various string types that uses the compareTo method
         * to determine if the string is lexicographically greater than or equal to the other.
         *
         * @param other
         *      The string value to compare to this one.
         *
         * @return true if this string is lexicographically greater than the other string.
         */
        bool operator>=(const char* other) const;
        bool operator>=(const String& other) const;
        bool operator>=(const std::string& other) const;

        /**
         * Concatenation operators for the various string types.  The value of this string
         * and the given string are concatenated and returned in a new String instance.
         *
         * @param other
         *      The string whose value is to be concatenated with this one.
         *
         * @return a new String instance that is the concatenation of the two strings.
         */
        String operator+ (const String& other) const;
        String operator+ (const std::string& other) const;
        String operator+ (const char* other) const;

        /**
         * Returns a const char* value to allow easier coexistence with standard c++
         * string operations.
         *
         * This method can result in a compaction of the String's backing store into a
         * new character array in order to return a pointer value that is guaranteed to
         * be NULL terminated.
         *
         * @return a const char* value for this String.
         */
        const char* c_str() const;

    public:

        /**
         * If the String instance is holding a reference to a character array that is larger
         * than the string's view of the backing store a new array is allocated and the
         * characters from the substring this String represents are copied to the new backing
         * store and returned in the resulting String object.
         *
         * This can free up heap memory when a String is holding a large array but only
         * viewing a small portion of it and the original source String is no longer also
         * maintaining a reference to the backing store.
         *
         * @return a new String instance with a compacted backing store.
         */
        String compact() const;

        /**
         * Compares two strings lexicographically. The comparison is based on the value
         * of each character in the strings. The character sequence represented by this
         * String is compared lexicographically to the character sequence represented by
         * the provided string. The result is a negative number if this String
         * lexicographically precedes the argument string. The result is a positive value
         * if this String lexicographically follows the argument string. The result is
         * zero if the strings are equal; compareTo returns 0 exactly when the equals
         * method would return true.
         *
         * @param string
         *      the string to compare.
         *
         * @return 0 if the strings are equal, a negative integer if this string is
         *         before the specified string, or a positive integer if this string
         *         is after the specified string.
         */
        int compareTo(const String& string) const;

        /**
         * Compares two strings lexicographically. The comparison is based on the value
         * of each character in the strings. The character sequence represented by this
         * String is compared lexicographically to the character sequence represented by
         * the provided string. The result is a negative number if this String
         * lexicographically precedes the argument string. The result is a positive value
         * if this String lexicographically follows the argument string. The result is
         * zero if the strings are equal; compareTo returns 0 exactly when the equals
         * method would return true.
         *
         * @param string
         *      the STL string to compare.
         *
         * @return 0 if the strings are equal, a negative integer if this string is
         *         before the specified string, or a positive integer if this string
         *         is after the specified string.
         */
        int compareTo(const std::string& string) const;

        /**
         * Compares two strings lexicographically. The comparison is based on the value
         * of each character in the strings. The character sequence represented by this
         * String is compared lexicographically to the character sequence represented by
         * the provided string. The result is a negative number if this String
         * lexicographically precedes the argument string. The result is a positive value
         * if this String lexicographically follows the argument string. The result is
         * zero if the strings are equal; compareTo returns 0 exactly when the equals
         * method would return true.
         *
         * @param string
         *      the C string to compare.
         *
         * @return 0 if the strings are equal, a negative integer if this string is
         *         before the specified string, or a positive integer if this string
         *         is after the specified string.
         *
         * @throws NullPointerException if the passed in C String value is NULL.
         */
        int compareTo(const char* string) const;

        /**
         * Compares two strings lexicographically, ignoring case differences. This method
         * returns an integer whose sign is that of calling compareTo with normalized
         * versions of the strings where case differences have been eliminated by calling
         * Character::toLowerCase() on each character.
         *
         * @param string
         *      the string to compare.
         *
         * @return 0 if the strings are equal, a negative integer if this string is
         *         before the specified string, or a positive integer if this string
         *         is after the specified string.
         */
        int compareToIgnoreCase(const String& string) const;

        /**
         * Compares two strings lexicographically, ignoring case differences. This method
         * returns an integer whose sign is that of calling compareTo with normalized
         * versions of the strings where case differences have been eliminated by calling
         * Character::toLowerCase() on each character.
         *
         * @param string
         *      the STL string to compare.
         *
         * @return 0 if the strings are equal, a negative integer if this string is
         *         before the specified string, or a positive integer if this string
         *         is after the specified string.
         */
        int compareToIgnoreCase(const std::string& string) const;

        /**
         * Compares two strings lexicographically, ignoring case differences. This method
         * returns an integer whose sign is that of calling compareTo with normalized
         * versions of the strings where case differences have been eliminated by calling
         * Character::toLowerCase() on each character.
         *
         * @param string
         *      the C string to compare.
         *
         * @return 0 if the strings are equal, a negative integer if this string is
         *         before the specified string, or a positive integer if this string
         *         is after the specified string.
         *
         * @throws NullPointerException if the passed in C String value is NULL.
         */
        int compareToIgnoreCase(const char* string) const;

        /**
         * Concatenates this string and the specified string.
         *
         * @param string
         *      the string to concatenate onto this String
         *
         * @return a new string which is the concatenation of this string and the
         *         specified string.
         */
        String concat(const String& string) const;

        /**
         * Concatenates this string and the specified std::string.
         *
         * @param string
         *      the STL string to concatenate onto this String
         *
         * @return a new string which is the concatenation of this string and the
         *         specified string.
         */
        String concat(const std::string& string) const;

        /**
         * Concatenates this string and the specified C string.
         *
         * @param string
         *      the C string to concatenate onto this String
         *
         * @return a new string which is the concatenation of this string and the
         *         specified string.
         */
        String concat(const char* string) const;

        /**
         * Determines if this String contains the sequence of characters in the String
         * passed in.
         *
         * @param string
         *      the String value to search for.
         *
         * @return true if the sequence of characters are contained in this String,
         *         otherwise returns false.
         */
        bool contains(const String& string) const;

        /**
         * Determines if this String contains the sequence of characters in the std::string
         * passed in.
         *
         * @param string
         *      the STL String value to search for.
         *
         * @return true if the sequence of characters are contained in this String,
         *         otherwise returns false.
         */
        bool contains(const std::string& string) const;

        /**
         * Determines if this String contains the sequence of characters in the C String
         * passed in.  If the value given is null the method always returns false.
         *
         * @param string
         *      the C String value to search for.
         *
         * @return true if the sequence of characters are contained in this String,
         *         otherwise returns false.
         */
        bool contains(const char* string) const;

        /**
         * Compares the specified string to this string to determine if the
         * specified string is a suffix.
         *
         * @param suffix
         *            the suffix to look for.
         *
         * @return true if the specified string is a suffix of this string, false otherwise.
         */
        bool endsWith(const String& suffix) const;

        /**
         * Returns true if this String is equal to the given String instance.
         *
         * @param other
         *      A String instance to compare to this string.
         *
         * @return true if this String is equal to the given String instance.
         */
        bool equals(const String& other) const;

        /**
         * Returns true if this String is equal to the given std::string instance.
         *
         * @param other
         *      A standard string instance to compare to this String.
         *
         * @return true if this String is equal to the given std::string instance.
         */
        bool equals(const std::string& other) const;

        /**
         * Returns true if this String is equal to the given C string instance.  This method
         * treats the NULL pointer case as equivalent to the empty string case and returns
         * true if this String instance is also empty.
         *
         * @param other
         *      A C string instance to compare to this String.
         *
         * @return true if this String is equal to the given C string instance.
         */
        bool equals(const char* other) const;

        /**
         * Compares the specified string to this string ignoring the case of the
         * characters and returns true if they are equal.
         *
         * @param string
         *      the string to compare.
         *
         * @return true if the specified string is equal to this string, false otherwise.
         */
        bool equalsIgnoreCase(const String& string) const;

        /**
         * Compares the specified std::string to this String ignoring the case of the
         * characters and returns true if they are equal.
         *
         * @param string
         *      the std::string to compare.
         *
         * @return true if the specified string is equal to this String, false otherwise.
         */
        bool equalsIgnoreCase(const std::string& string) const;

        /**
         * Compares the specified C string to this string ignoring the case of the
         * characters and returns true if they are equal.
         *
         * @param string
         *      the C string to compare.
         *
         * @return true if the specified C string is equal to this string, false otherwise.
         */
        bool equalsIgnoreCase(const char* string) const;

        /**
         * Searches in this string for the first index of any character in the specified
         * String. The search for the matching characters starts at the beginning and moves
         * towards the end of this string.
         *
         * @param string
         *      the characters to find the first of within this String.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if none of the characters in the String exist in this String.
         */
        int findFirstOf(const String& chars) const;

        /**
         * Searches in this string for the first index of any character in the specified
         * String. The search for the matching characters starts at the given index and moves
         * towards the end of this string.
         *
         * @param chars
         *      the characters to find the first of within this String.
         * @param start
         *      the starting offset.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if none of the characters in the String exist in this String.
         */
        int findFirstOf(const String& chars, int start) const;

        /**
         * Searches in this string for the first index of any character that is not in the
         * specified String. The search for the non-matching characters starts at the beginning
         * and moves towards the end of this string.
         *
         * @param chars
         *      the characters to find the first non-matching index of within this String.
         *
         * @return the index of the first character not in the specified string in this
         *         string, -1 if all of the characters in the given String exist in this String.
         */
        int findFirstNotOf(const String& chars) const;

        /**
         * Searches in this string for the first index of any character that is not in the
         * specified String. The search for the non-matching characters starts at the given index
         * and moves towards the end of this string.
         *
         * @param chars
         *      the characters to find the first non-matching index of within this String.
         * @param start
         *      the starting offset.
         *
         * @return the index of the first character not in the specified string in this
         *         string, -1 if all of the characters in the given String exist in this String.
         */
        int findFirstNotOf(const String& chars, int start) const;

        /**
         * Copies characters from this String into the destination char array, starting from
         * the given index.
         *
         * @param srcBegin
         *      Starting index in this String for the copy operation.
         * @param srcEnd
         *      The index at which the copy ends, this value is not copied.
         * @param dest
         *      The destination character array to copy the data to.
         * @param destSize
         *      The size of the destination array.
         * @param destBegin
         *      The offset into the destination array to start copying to.
         *
         * @throws IndexOutOfBoundsException if any of the following conditions are met:
         *           srcBegin or srcEnd are negative.
         *           srcBegin is greater than src end.
         *           srcEnd is greater than the length()
         *           destSize or destBegin are negative.
         *           destBegin + (srcEnd - srcBegin) is greater than destSize.
         */
        void getChars(int srcBegin, int srcEnd, char* dest, int destSize, int destBegin) const;

        /**
         * Returns a hash code for this String instance, the hash code for an empty
         * String will always be zero.
         *
         * @return a hash code for this String instance.
         */
        int hashCode() const;

        /**
         * Searches in this string for the first index of the specified character.
         * The search for the character starts at the beginning and moves towards
         * the end of this string.
         *
         * @param c
         *      the character to find.
         *
         * @return the index in this string of the specified character, -1 if the
         *         character isn't found.
         */
        int indexOf(char value) const;

        /**
         * Searches in this string for the index of the specified character. The
         * search for the character starts at the specified offset and moves towards
         * the end of this string.
         *
         * If the start value given is less than zero the search starts at the beginning
         * of the string.  If the start value is greater than the length of the string
         * minus one is returned.
         *
         * @param value
         *      the character to find.
         * @param start
         *      the starting offset.
         *
         * @return the index in this string of the specified character, -1 if the
         *         character isn't found.
         */
        int indexOf(char value, int start) const;

        /**
         * Searches in this string for the first index of the specified string. The
         * search for the string starts at the beginning and moves towards the end
         * of this string.
         *
         * @param string
         *      the string to find within this String.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if the specified string is not a substring.
         */
        int indexOf(const String& string) const;

        /**
         * Searches in this string for the index of the specified string. The search
         * for the string starts at the specified offset and moves towards the end
         * of this string.
         *
         * @param subString
         *      the string to find within this String.
         * @param start
         *      the starting offset.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if the specified string is not a substring.
         */
        int indexOf(const String& subString, int start) const;

        /**
         * Searches in this String for the first index of the specified std::string. The
         * search for the string starts at the beginning and moves towards the end
         * of this string.
         *
         * @param string
         *      the STL string to find within this String.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if the specified string is not a substring.
         */
        int indexOf(const std::string& string) const;

        /**
         * Searches in this string for the index of the specified std::string. The search
         * for the string starts at the specified offset and moves towards the end
         * of this string.
         *
         * @param subString
         *      the STL string to find within this String.
         * @param start
         *      the starting offset.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if the specified string is not a substring.
         */
        int indexOf(const std::string& subString, int start) const;

        /**
         * Searches in this String for the first index of the specified C string. The
         * search for the string starts at the beginning and moves towards the end
         * of this string.  If the given string pointer is NULL this method returns -1.
         *
         * @param string
         *      the C string to find within this String.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if the specified string is not a substring.
         */
        int indexOf(const char* string) const;

        /**
         * Searches in this string for the index of the specified C string. The search
         * for the string starts at the specified offset and moves towards the end
         * of this string.  If the given string pointer is NULL this method returns -1.
         *
         * @param subString
         *      the C string to find within this String.
         * @param start
         *      the starting offset.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if the specified string is not a substring.
         */
        int indexOf(const char* subString, int start) const;

        /**
         * @return true if the length of this String is zero.
         */
        bool isEmpty() const;

        /**
         * Searches in this string for the last index of the specified character.
         * The search for the character starts at the end and moves towards the
         * beginning of this string.
         *
         * @param value
         *      the character to find.
         *
         * @return the index in this string of the specified character, -1 if the
         *         character isn't found.
         */
        int lastIndexOf(char value) const;

        /**
         * Searches in this string for the index of the specified character. The
         * search for the character starts at the specified offset and moves towards
         * the beginning of this string.
         *
         * @param value
         *      the character to find.
         * @param start
         *      the starting offset.
         *
         * @return the index in this string of the specified character, -1 if the
         *         character isn't found.
         */
        int lastIndexOf(char value, int start) const;

        /**
         * Searches in this string for the last index of the specified string. The
         * search for the string starts at the end and moves towards the beginning
         * of this string.
         *
         * @param string
         *      the string to find.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if the specified string is not a substring.
         */
        int lastIndexOf(const String& string) const;

        /**
         * Searches in this string for the index of the specified string. The search
         * for the string starts at the specified offset and moves towards the
         * beginning of this string.
         *
         * @param subString
         *      the string to find.
         * @param start
         *      the starting offset.
         *
         * @return the index of the first character of the specified string in this
         *         string , -1 if the specified string is not a substring.
         */
        int lastIndexOf(const String& subString, int start) const;

        /**
         * Searches in this string for the last index of the specified std::string. The
         * search for the string starts at the end and moves towards the beginning
         * of this string.
         *
         * @param string
         *      the STL string to find.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if the specified string is not a substring.
         */
        int lastIndexOf(const std::string& string) const;

        /**
         * Searches in this string for the index of the specified std::string. The search
         * for the string starts at the specified offset and moves towards the
         * beginning of this string.
         *
         * @param subString
         *      the STL string to find.
         * @param start
         *      the starting offset.
         *
         * @return the index of the first character of the specified string in this
         *         string , -1 if the specified string is not a substring.
         */
        int lastIndexOf(const std::string& subString, int start) const;

        /**
         * Searches in this string for the last index of the specified C string. The
         * search for the string starts at the end and moves towards the beginning
         * of this string.
         *
         * @param string
         *      the C string to find.
         *
         * @return the index of the first character of the specified string in this
         *         string, -1 if the specified string is not a substring.
         */
        int lastIndexOf(const char* string) const;

        /**
         * Searches in this string for the index of the specified C string. The search
         * for the string starts at the specified offset and moves towards the
         * beginning of this string.
         *
         * @param subString
         *      the C string to find.
         * @param start
         *      the starting offset.
         *
         * @return the index of the first character of the specified string in this
         *         string , -1 if the specified string is not a substring.
         */
        int lastIndexOf(const char* subString, int start) const;

        /**
         * Compares the specified string to this string and compares the specified
         * range of characters to determine if they are the same.  The method returns
         * false is any of the index values are negative or result in a span that would
         * exceed the length of either string.
         *
         * @param thisStart
         *      the starting offset in this string.
         * @param string
         *      the string to compare.
         * @param start
         *      the starting offset in the specified string.
         * @param length
         *      the number of characters to compare.
         *
         * @return true if the ranges of characters are equal, false otherwise
         */
        bool regionMatches(int thisStart, const String& string, int start, int length) const;

        /**
         * Compares the specified string to this string and compares the specified
         * range of characters to determine if they are the same. When ignoreCase is
         * true, the case of the characters is ignored during the comparison.
         *
         * @param ignoreCase
         *      specifies if case should be ignored.
         * @param thisStart
         *      the starting offset in this string.
         * @param string
         *      the string to compare.
         * @param start
         *      the starting offset in the specified string.
         * @param length
         *      the number of characters to compare.
         *
         * @return true if the ranges of characters are equal, false otherwise.
         */
        bool regionMatches(bool ignoreCase, int thisStart, const String& string, int start, int length) const;

        /**
         * Copies this string replacing occurrences of the specified character with
         * another character.
         *
         * @param oldChar
         *      the character to replace.
         * @param newChar
         *      the replacement character.
         *
         * @return a new string with occurrences of oldChar replaced by newChar.
         */
        String replace(char oldChar, char newChar) const;

        /**
         * Compares the specified string to this string to determine if the specified
         * string is a prefix.  If the prefix string is empty or is equal to this String
         * than true is returned.
         *
         * @param prefix
         *      the string to look for.
         *
         * @return if the specified string is a prefix of this string, false otherwise
         */
        bool startsWith(const String& prefix) const;

        /**
         * Compares the specified string to this string, starting at the specified
         * offset, to determine if the specified string is a prefix.
         *
         * @param prefix
         *      the string to look for.
         * @param start
         *      the starting offset.
         *
         * @return true if the specified string occurs in this string at the specified
         *         offset, false otherwise.
         */
        bool startsWith(const String& prefix, int start) const;

        /**
         * Copies a range of characters into a new string starting from the given offset and
         * extending to the end of this String.
         *
         * @param start
         *      the offset of the first character.
         *
         * @return a new string containing the characters from start to the end of the string.
         *
         * @throws IndexOutOfBoundsException if start < 0 or start > length().
         */
        String substring(int start) const;

        /**
         * Copies a range of characters into a new string.  The length of the returned String
         * is end - start meaning that the characters in the new string include only start to
         * end - 1.
         *
         * @param start
         *      the offset of the first character. (inclusive)
         * @param end
         *      the offset one past the last character. (exclusive)
         *
         * @return a new string containing the characters from start to end - 1.
         *
         * @throws IndexOutOfBoundsException if start < 0, start > end or end > length().
         */
        String substring(int start, int end) const;

        /**
         * Copies the characters in this string to a newly allocated character array.  The
         * returned array is the property of the caller and must be deleted by them.  If the
         * String is empty then a NULL is returned.
         *
         * The array returned is not guaranteed to be null terminated as the array is
         * sized according to the result of calling length().
         *
         * @return a character array containing the characters of this string.
         */
        char* toCharArray() const;

        /**
         * Converts the characters in this string to lower case.  The resulting value
         * is returned in a new String instance and this one is left unchanged.
         *
         * @return a new string containing the lower case characters equivalent to
         *         the characters in this string.
         */
        String toLowerCase() const;

        /**
         * Converts the characters in this string to upper case.  The resulting value
         * is returned in a new String instance and this one is left unchanged.
         *
         * @return a new string containing the upper case characters equivalent to
         *         the characters in this string.
         */
        String toUpperCase() const;

        /**
         * Returns a copy of the string, with leading and trailing whitespace omitted.
         *
         * @return a copy of the string, with leading and trailing whitespace omitted.
         */
        String trim() const;

    public:

        /**
         * {@inheritDoc}
         */
        virtual int length() const;

        /**
         * {@inheritDoc}
         */
        virtual char charAt(int index) const;

        /**
         * {@inheritDoc}
         */
        virtual CharSequence* subSequence(int start, int end) const;

        /**
         * {@inheritDoc}
         */
        virtual std::string toString() const;

    public:

        /**
         * Creates a new string containing the characters in the specified character
         * array. Modifying the character array after creating the string has no
         * effect on the string.
         *
         * @param data
         *      the array of characters.
         *
         * @return the new string.
         *
         * @throws NullPointerException if the C string pointer is NULL
         */
        static String copyValueOf(const char* data);

        /**
         * Creates a new string containing the specified characters in the character
         * array. Modifying the character array after creating the string has no
         * effect on the string.
         *
         * @param data
         *      the array of characters.
         * @param start
         *      the starting offset in the character array.
         * @param length
         *      the number of characters to use.
         *
         * @return the new string.
         *
         * @throws NullPointerException if the C string is NULL
         * @throws IndexOutOfBoundsException
         *         if length < 0, start < 0 or start + length > the C string's length.
         */
        static String copyValueOf(char* data, int start, int length);

        /**
         * Given a C String pointer return true if the value is either NULL or the
         * string contained is empty.
         *
         * @return true if the C string is either a NULL or an Empty string.
         */
        static bool isNullOrEmpty(const char*);

        /**
         * Returns a String that represents the value of the given boolean value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @return "true" if the boolean is true, "false" otherwise.
         */
        static String valueOf(bool value);

        /**
         * Returns a String that represents the value of the given char value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @return a String that contains the single character value given.
         */
        static String valueOf(char value);

        /**
         * Returns a String that represents the value of the given float value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @return a String that contains the string representation of the float value given.
         */
        static String valueOf(float value);

        /**
         * Returns a String that represents the value of the given double value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @return a String that contains the string representation of the double value given.
         */
        static String valueOf(double value);

        /**
         * Returns a String that represents the value of the given short value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @return a String that contains the string representation of the short value given.
         */
        static String valueOf(short value);

        /**
         * Returns a String that represents the value of the given integer value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @return a String that contains the string representation of the integer value given.
         */
        static String valueOf(int value);

        /**
         * Returns a String that represents the value of the given 64bit long value.
         *
         * @param value
         *      The value whose string representation is to be returned.
         *
         * @return a String that contains the string representation of the 64 bit long value given.
         */
        static String valueOf(long long value);

    private:

        /**
         * Version of getChars without bounds checks, for use by other classes
         * within the java.lang package only.  The caller is responsible for
         * ensuring that start >= 0 && start <= end && end <= count.
         */
        void getChars(int start, int end, char* buffer, int index) const;

        String(Contents* content);
        String(int offset, int length, const ArrayPointer<char> content);

        friend class AbstractStringBuilder;
    };

    std::ostream& operator<<(std::ostream &out, const String& target);

    bool operator==(const std::string& left, const String& right);
    bool operator==(const char* left, const String& right);
    bool operator!=(const std::string& left, const String& right);
    bool operator!=(const char* left, const String& right);
    bool operator<=(const std::string& left, const String& right);
    bool operator<=(const char* left, const String& right);
    bool operator>=(const std::string& left, const String& right);
    bool operator>=(const char* left, const String& right);
    bool operator< (const std::string& left, const String& right);
    bool operator< (const char* left, const String& right);
    bool operator> (const std::string& left, const String& right);
    bool operator> (const char* left, const String& right);

}}

#endif /* _DECAF_LANG_STRING_H_ */
