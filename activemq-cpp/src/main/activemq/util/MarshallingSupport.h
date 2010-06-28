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

#ifndef _ACTIVENQ_UTIL_MARSHALLINGSUPPORT_H_
#define _ACTIVENQ_UTIL_MARSHALLINGSUPPORT_H_

#include <activemq/util/Config.h>

#include <decaf/io/IOException.h>
#include <decaf/io/UTFDataFormatException.h>

#include <decaf/io/DataOutputStream.h>
#include <decaf/io/DataInputStream.h>

#include <string>

namespace activemq {
namespace util {

    class AMQCPP_API MarshallingSupport {
    public:

        MarshallingSupport();
        virtual ~MarshallingSupport();

    public:  // Openwire Type Marshalling methods

        /**
         * Write the string object to the given DataOutputStream as Raw bytes, no string
         * encoding is done on this char values in the string.  User must encode to Modified
         * UTF-8 as needed.
         *
         * @param dataOut
         *      The DataOutputStream to write the String data to.
         * @param value
         *      Thre String value to write in Openwire form.
         *
         * @throws IOException if an I/O error occurs while writing the string.
         */
        static void writeString( decaf::io::DataOutputStream& dataOut, const std::string& value );

        /**
         * Write the string object to the given DataOutputStream as Raw bytes, no string
         * encoding is done on this char values in the string.  User must encode to Modified
         * UTF-8 as needed.  This method write out only the size as a short and the string data
         * no Openwire Type tag is appended.
         *
         * @param dataOut
         *      The DataOutputStream to write the String data to.
         * @param value
         *      Thre String value to write in Openwire form.
         *
         * @throws IOException if an I/O error occurs while writing the string.
         */
        static void writeString16( decaf::io::DataOutputStream& dataOut, const std::string& value );

        /**
         * Write the string object to the given DataOutputStream as Raw bytes, no string
         * encoding is done on this char values in the string.  User must encode to Modified
         * UTF-8 as needed.  This method write out only the size as a int and the string data
         * no Openwire Type tag is appended.
         *
         * @param dataOut
         *      The DataOutputStream to write the String data to.
         * @param value
         *      Thre String value to write in Openwire form.
         *
         * @throws IOException if an I/O error occurs while writing the string.
         */
        static void writeString32( decaf::io::DataOutputStream& dataOut, const std::string& value );

        /**
         * Reads an Openwire encoded string from the provided DataInputStream.  No string
         * processing is performed by this method, clients that know the data contains UTF-8
         * encoded content must use one of the utility methods of this class to decode the UTF-8
         * data.
         *
         * This version assumes a size prefix of 16bits.
         *
         * @param dataIn
         *      The DataInputStream to read the String data from.
         *
         * @returns the String value.
         *
         * @throws IOException if an I/O error occurs while writing the string.
         */
        static std::string readString16( decaf::io::DataInputStream& dataIn );

        /**
         * Reads an Openwire encoded string from the provided DataInputStream.  No string
         * processing is performed by this method, clients that know the data contains UTF-8
         * encoded content must use one of the utility methods of this class to decode the UTF-8
         * data.
         *
         * This version assumes a size prefix of 32bits.
         *
         * @param dataIn
         *      The DataInputStream to read the String data from.
         *
         * @returns the String value.
         *
         * @throws IOException if an I/O error occurs while writing the string.
         */
        static std::string readString32( decaf::io::DataInputStream& dataIn );

    public:  // Static Utility Methods.

        /**
         * Given an ASCII String with byte values [0..255] convert the string to a
         * string containing the modified UTF-8 form of that same string.  This allows
         * an ASCII string containing values greater than 127 as well as embedded NULLs
         * to be sent to a Java client.
         *
         * @param asciiString
         *      The ASCII string to encode as Modified UTF-8
         *
         * @return a string containing the Modified UTF-8 encoded form of the provided string.
         *
         * @throws UTFDataFormatException if the length of the encoded string would exceed the
         *         size of an signed integer.
         */
        static std::string asciiToModifiedUtf8( const std::string& asciiString );

        /**
         * Given a string that contains bytes in the Java Modified UTF-8 format convert
         * that string back into ASCII values from [0..255].  This will handle any string
         * sent from a Java client which contains values within the [0..255] range or has
         * embedded Nulls.  Strings that have encoded values greater than 255 will cause
         * an exception to be thrown.
         *
         * @param modifiedUtf8String
         *      The string to convert from Modified UTF-8 to ASCII.
         *
         * @return the ASCII encoded version of the provided string.
         *
         * @throws UTFDataFormatException if the provided string contains invalid data or the
         *         character values encoded in the string exceed ASCII value 255.
         */
        static std::string modifiedUtf8ToAscii( const std::string modifiedUtf8String );

    };

}}

#endif /* _ACTIVENQ_UTIL_MARSHALLINGSUPPORT_H_ */
