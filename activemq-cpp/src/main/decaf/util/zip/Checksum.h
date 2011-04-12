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

#ifndef _DECAF_UTIL_ZIP_CHECKSUM_H_
#define _DECAF_UTIL_ZIP_CHECKSUM_H_

#include <decaf/util/Config.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

#include <vector>

namespace decaf {
namespace util {
namespace zip {

    /**
     * An interface used to represent Checksum values in the Zip package.
     *
     * @since 1.0
     */
    class DECAF_API Checksum {
    public:

        virtual ~Checksum() {}

        /**
         * @returns the current checksum value.
         */
        virtual long long getValue() const = 0;

        /**
         * Reset the checksum to its initial value.
         */
        virtual void reset() = 0;

        /**
         * Updates the current checksum with the specified vector of bytes.
         *
         * @param buffer
         *      The buffer to read the updated bytes from.
         */
        virtual void update( const std::vector<unsigned char>& buffer ) = 0;

        /**
         * Updates the current checksum with the specified array of bytes.
         *
         * @param buffer
         *      The buffer to read the updated bytes from.
         * @param offset
         *      The position in the buffer to start reading.
         * @param length
         *      The amount of data to read from the byte buffer.
         *
         * @throw IndexOutOfBoundsException if offset + length > size of the buffer.
         */
        virtual void update( const std::vector<unsigned char>& buffer, int offset, int length ) = 0;

        /**
         * Updates the current checksum with the specified array of bytes.
         *
         * @param buffer
         *      The buffer to read the updated bytes from.
         * @param size
         *      The size of the passed buffer.
         * @param offset
         *      The position in the buffer to start reading.
         * @param length
         *      The amount of data to read from the byte buffer.
         *
         * @throw NullPointerException if the passed buffer is NULL.
         * @throw IndexOutOfBoundsException if offset + length > size of the buffer.
         */
        virtual void update( const unsigned char* buffer, int size, int offset, int length ) = 0;

        /**
         * Updates the current checksum with the specified byte value.
         *
         * @param byte
         *      The byte value to update the current Checksum with (0..255).
         */
        virtual void update( int byte ) = 0;

    };

}}}

#endif /* _DECAF_UTIL_ZIP_CHECKSUM_H_ */
