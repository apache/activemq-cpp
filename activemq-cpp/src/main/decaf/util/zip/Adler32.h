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

#ifndef _DECAF_UTIL_ZIP_ADLER32_H_
#define _DECAF_UTIL_ZIP_ADLER32_H_

#include <decaf/util/Config.h>
#include <decaf/util/zip/Checksum.h>

namespace decaf {
namespace util {
namespace zip {

    /**
     * Clas that can be used to compute an Adler-32 Checksum for a data stream.  The
     * Alder-32 checksum trades reliability for speed over the CRC-32 algorithm.
     *
     * @since 1.0
     */
    class DECAF_API Adler32 : public Checksum {
    private:

        long long value;

    public:

        Adler32();

        virtual ~Adler32();

        /**
         * @returns the current checksum value.
         */
        virtual long long getValue() const;

        /**
         * Reset the checksum to its initial value.
         */
        virtual void reset();

        /**
         * Updates the current checksum with the specified vector of bytes.
         *
         * @param buffer
         *      The buffer to read the updated bytes from.
         */
        virtual void update( const std::vector<unsigned char>& buffer );

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
        virtual void update( const std::vector<unsigned char>& buffer, int offset, int length );

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
        virtual void update( const unsigned char* buffer, int size, int offset, int length );

        /**
         * Updates the current checksum with the specified byte value.
         *
         * @param byte
         *      The byte value to update the current Checksum with (0..255).
         */
        virtual void update( int byte );

    };

}}}

#endif /* _DECAF_UTIL_ZIP_ADLER32_H_ */
