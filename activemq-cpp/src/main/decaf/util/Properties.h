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

#ifndef _DECAF_UTIL_PROPERTIES_H_
#define _DECAF_UTIL_PROPERTIES_H_

#include <vector>
#include <string>
#include <decaf/util/Config.h>
#include <decaf/util/StlMap.h>
#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/io/IOException.h>

namespace decaf{
namespace io{
    class Reader;
    class Writer;
}
namespace util{

    class PropertiesInternal;

    /**
     * Java-like properties class for mapping string names to string values.
     * <p>
     * The Properties list contains a key value pair of properties that can be loaded and
     * stored to a stream.  Each Properties instance can contain an internal Properties list
     * that contains default values for keys not found in the Properties List.
     * <p>
     * The Properties list if a Thread Safe class, it can be shared amongst objects in
     * multiple threads without the need for additional synchronization.
     *
     * @since 1.0
     */
    class DECAF_API Properties{
    private:

        PropertiesInternal* internal;

    protected:

        /**
         * Default list used to answer for any keys not found in the properties list, can
         * be filled in by another implementation of this class.
         */
        decaf::lang::Pointer<Properties> defaults;

    public:

        Properties();

        Properties( const Properties& src );

        virtual ~Properties();

        /**
         * Assignment Operator
         *
         * @param src
         *      The Properties list to copy to this List.
         *
         * @return a reference to this List for use in chaining.
         */
        Properties& operator= ( const Properties& src );

        /**
         * Returns true if the properties object is empty.
         *
         * @return true if empty
         */
        bool isEmpty() const;

        /**
         * @return The number of Properties in this Properties Object.
         */
        int size() const;

        /**
         * Looks up the value for the given property.
         *
         * @param name
         *      The name of the property to be looked up.
         *
         * @return the value of the property with the given name, if it
         *         exists.  If it does not exist, returns NULL.
         */
        const char* getProperty( const std::string& name ) const;

        /**
         * Looks up the value for the given property.
         *
         * @param name
         *      The name of the property to be looked up.
         * @param defaultValue
         *      The value to be returned if the given property does not exist.
         *
         * @return The value of the property specified by <code>name</code>, if it
         *         exists, otherwise the <code>defaultValue</code>.
         */
        std::string getProperty( const std::string& name, const std::string& defaultValue ) const;

        /**
         * Sets the value for a given property.  If the property already
         * exists, overwrites the value.
         *
         * @param name
         *      The name of the value to be written.
         * @param value
         *      The value to be written.
         *
         * @returns the old value of the property or empty string if not set.
         */
        std::string setProperty( const std::string& name, const std::string& value );

        /**
         * Check to see if the Property exists in the set.
         *
         * @param name
         *      The property name to check for in this properties set.
         *
         * @return true if property exists, false otherwise.
         */
        bool hasProperty( const std::string& name ) const;

        /**
         * Removes the property with the given name.
         *
         * @param name
         *      The name of the property to remove.
         *
         * @returns the previous value of the property if set, or empty string.
         */
        std::string remove( const std::string& name );

        /**
         * Returns an enumeration of all the keys in this property list, including distinct keys
         * in the default property list if a key of the same name has not already been found from
         * the main properties list.
         *
         * @returns a set of keys in this property list where the key and its corresponding value are
         *          strings, including the keys in the default property list.
         */
        std::vector<std::string> propertyNames() const;

        /**
         * Method that serializes the contents of the property map to an array.
         *
         * @return list of pairs where the first is the name and the second
         *         is the value.
         */
        std::vector< std::pair< std::string, std::string > > toArray() const;

        /**
         * Copies the contents of the given properties object to this one, if the
         * given Properties instance in NULL then this List is not modified.
         *
         * @param source
         *      The source properties object.
         */
        void copy( const Properties& source );

        /**
         * Clones this object.
         *
         * @returns a replica of this object.
         */
        Properties* clone() const;

        /**
         * Clears all properties from the map.
         */
        void clear();

        /**
         * Test whether two Properties objects are equivalent.  Two Properties
         * Objects are considered equivalent when they each contain the same number
         * of elements and each key / value pair contained within the two are equal.
         *
         * This comparison does not check the contents of the Defaults instance.
         *
         * @param source
         *      The Properties object to compare this instance to.
         *
         * @return true if the contents of the two Properties objects are the same.
         */
        bool equals( const Properties& source ) const;

        /**
         * Formats the contents of the Properties Object into a string that can be logged, etc.
         *
         * @returns string value of this object.
         */
        std::string toString() const;

        /**
         * Reads a property list (key and element pairs) from the input byte stream. The
         * input stream is in a simple line-oriented format as specified in load(Reader) and
         * is assumed to use the ISO 8859-1 character encoding.
         *
         * This method does not close the stream upon its return.
         *
         * @param stream
         *      The stream to read the properties data from.
         *
         * @throw IOException if there is an error while reading from the stream.
         * @throw IllegalArgumentException if malformed data is found while reading the properties.
         * @throw NullPointerException if the passed stream is Null.
         */
        void load( decaf::io::InputStream* stream );

        /**
         * Reads a property list (key and element pairs) from the input character stream in a
         * simple line-oriented format.
         * <p>
         * Properties are processed in terms of lines. There are two kinds of line, natural lines
         * and logical lines. A natural line is defined as a line of characters that is terminated
         * either by a set of line terminator characters (\n or \r or \r\n) or by the end of the
         * stream. A natural line may be either a blank line, a comment line, or hold all or some
         * of a key-element pair. A logical line holds all the data of a key-element pair, which
         * may be spread out across several adjacent natural lines by escaping the line terminator
         * sequence with a backslash character \. Note that a comment line cannot be extended in
         * this manner; every natural line that is a comment must have its own comment indicator,
         * as described below. Lines are read from input until the end of the stream is reached.
         * <p>
         * A natural line that contains only white space characters is considered blank and is
         * ignored. A comment line has an ASCII '#' or '!' as its first non-white space character;
         * comment lines are also ignored and do not encode key-element information. In addition
         * to line terminators, this format considers the characters space (' '), tab ('\t'),
         * and form feed ('\f') to be white space.
         * <p>
         * If a logical line is spread across several natural lines, the backslash escaping the
         * line terminator sequence, the line terminator sequence, and any white space at the
         * start of the following line have no affect on the key or element values. The remainder
         * of the discussion of key and element parsing (when loading) will assume all the
         * characters constituting the key and element appear on a single natural line after line
         * continuation characters have been removed. Note that it is not sufficient to only
         * examine the character preceding a line terminator sequence to decide if the line
         * terminator is escaped; there must be an odd number of contiguous backslashes for the
         * line terminator to be escaped. Since the input is processed from left to right, a
         * non-zero even number of 2n contiguous backslashes before a line terminator (or
         * elsewhere) encodes n backslashes after escape processing.
         * <p>
         * The key contains all of the characters in the line starting with the first non-white
         * space character and up to, but not including, the first unescaped '=', ':', or white
         * space character other than a line terminator. All of these key termination characters
         * may be included in the key by escaping them with a preceding backslash character; for
         * example,
         *
         *    \:\=
         *
         * would be the two-character key ":=". Line terminator characters can be included using
         * \r and \n escape sequences. Any white space after the key is skipped; if the first
         * non-white space character after the key is '=' or ':', then it is ignored and any white
         * space characters after it are also skipped. All remaining characters on the line become
         * part of the associated element string; if there are no remaining characters, the element
         * is the empty string "". Once the raw character sequences constituting the key and
         * element are identified, escape processing is performed as described above.
         * <p>
         * As an example, each of the following three lines specifies the key "Truth" and the
         * associated element value "Beauty":
         * <p>
         *     Truth = Beauty
         *             Truth:Beauty
         *         Truth                  :Beauty
         * <p>
         * As another example, the following three lines specify a single property:
         * <p>
         * fruits                           apple, banana, pear, \
         *                                  cantaloupe, watermelon, \
         *                                  kiwi, mango
         * <p>
         * The key is "fruits" and the associated element is:
         *     "apple, banana, pear, cantaloupe, watermelon, kiwi, mango"
         * <p>
         * Note that a space appears before each \ so that a space will appear after each comma
         * in the final result; the \, line terminator, and leading white space on the continuation
         * line are merely discarded and are not replaced by one or more other characters.
         * <p>
         * As a third example, the line:
         *
         *   cheeses
         *
         * specifies that the key is "cheeses" and the associated element is the empty string "".
         * <p>
         * Characters in keys and elements can be represented in escape sequences similar to those
         * used for character and string literals (see §3.3 and §3.10.6 of the Java Language
         * Specification). The differences from the character escape sequences and Unicode escapes
         * used for characters and strings are:
         *
         *   - Octal escapes are not recognized.
         *   - The character sequence \b does not represent a backspace character.
         *   - The method does not treat a backslash character, \, before a non-valid escape
         *     character as an error; the backslash is silently dropped. For example, in a C++
         *     string the sequence "\z" would cause a compile time error. In contrast, this
         *     method silently drops the backslash. Therefore, this method treats the two
         *     character sequence "\b" as equivalent to the single character 'b'.
         *   - Escapes are not necessary for single and double quotes; however, by the rule above,
         *     single and double quote characters preceded by a backslash still yield single and
         *     double quote characters, respectively.
         *
         * This method does not close the Reader upon its return.
         *
         * @param reader
         *      The Reader that provides an character stream as input.
         *
         * @throw IOException if there is an error while reading from the stream.
         * @throw IllegalArgumentException if malformed data is found while reading the properties.
         * @throw NullPointerException if the passed stream is Null.
         */
        void load( decaf::io::Reader* reader );

        /**
         * Writes this property list (key and element pairs) in this Properties table to the
         * output stream in a format suitable for loading into a Properties table using the
         * load(InputStream) method.
         * <p>
         * Properties from the defaults table of this Properties table (if any) are not
         * written out by this method.
         * <p>
         * This method outputs the comments, properties keys and values in the same format
         * as specified in store(Writer), with the following differences:
         *
         *   - The stream is written using the ISO 8859-1 character encoding.
         *   - Characters not in Latin-1 in the comments are written as \uxxxx for their
         *     appropriate unicode hexadecimal value xxxx.
         *   - Characters less than \u0020 and characters greater than \u007E in property keys
         *     or values are written as \uxxxx for the appropriate hexadecimal value xxxx.
         *
         * After the entries have been written, the output stream is flushed. The output stream
         * remains open after this method returns.
         *
         * @param out
         *      The OutputStream instance to write the properties to.
         * @param comment
         *      A description of these properties that is written to the output stream.
         *
         * @throw IOException if there is an error while writing from the stream.
         * @throw NullPointerException if the passed stream is Null.
         */
        void store( decaf::io::OutputStream* out, const std::string& comment );

        /**
         * Writes this property list (key and element pairs) in this Properties table to the output
         * character stream in a format that can be read by the load(Reader) method.
         * <p>
         * Properties from the defaults table of this Properties table (if any) are not written
         * out by this method.
         * <p>
         * If the comments argument is not empty, then an ASCII # character, the comments string,
         * and a line separator are first written to the output stream. Thus, the comments can
         * serve as an identifying comment. Any one of a line feed ('\n'), a carriage return ('\r'),
         * or a carriage return followed immediately by a line feed in comments is replaced by a
         * line separator generated by the Writer and if the next character in comments is not
         * character # or character ! then an ASCII # is written out after that line separator.
         * <p>
         * Next, a comment line is always written, consisting of an ASCII # character, the current
         * date and time (as if produced by the toString method of Date for the current time), and
         * a line separator as generated by the Writer.
         * <p>
         * Then every entry in this Properties table is written out, one per line. For each entry
         * the key string is written, then an ASCII =, then the associated element string. For the
         * key, all space characters are written with a preceding \ character. For the element,
         * leading space characters, but not embedded or trailing space characters, are written
         * with a preceding \ character. The key and element characters #, !, =, and : are written
         * with a preceding backslash to ensure that they are properly loaded.
         * <p>
         * After the entries have been written, the output stream is flushed. The output stream
         * remains open after this method returns.
         *
         * @param writer
         *      The Writer instance to use to output the properties.
         * @param comments
         *      A description of these properties that is written before writing the properties.
         *
         * @throw IOException if there is an error while writing from the stream.
         * @throw NullPointerException if the passed stream is Null.
         */
        void store( decaf::io::Writer* writer, const std::string& comments );

    private:

        void selectProperties( decaf::util::StlMap<std::string, std::string>& selectProperties ) const;

    };

}}

#endif /*_DECAF_UTIL_PROPERTIES_H_*/
