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

#ifndef _DECAF_SECURITY_SECURERANDOMSPI_H_
#define _DECAF_SECURITY_SECURERANDOMSPI_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace security {

    /**
     * Interface class used by Security Service Providers to implement a source of secure random
     * bytes.
     *
     * @since 1.0
     */
    class DECAF_API SecureRandomSpi {
    public:

        SecureRandomSpi();

        virtual ~SecureRandomSpi();

        /**
         * Reseed the Random Number Generator, the value given supplements the previous seed value
         * if any instead of replacing it.
         *
         * @param seed
         *      The array of bytes used to update the generators seed.
         * @param size
         *      The size of the passed byte array.
         */
        virtual void providerSetSeed( const unsigned char* seed, int size ) = 0;

        /**
         * Generates the number of random bytes specified by the size parameter and write them to
         * the passed bytes array.  The array must have already been allocated and be of the correct
         * size to prevent segmentation faults.
         *
         * @param bytes
         *      The array that will be filled with random bytes equal to size.
         * @param numBytes
         *      The number of bytes to generate and write into the bytes array.
         */
        virtual void providerNextBytes( unsigned char* bytes, int numBytes ) = 0;

        /**
         * Generates a new set of seed bytes, the returned value may be used to seed another
         * Random Number Generator.  The caller owns the returned array and must delete it.
         *
         * @param numBytes
         *      The number of bytes that should be generated for the new seed array.
         */
        virtual unsigned char* providerGenerateSeed( int numBytes ) = 0;

    };

}}

#endif /* _DECAF_SECURITY_SECURERANDOMSPI_H_ */
