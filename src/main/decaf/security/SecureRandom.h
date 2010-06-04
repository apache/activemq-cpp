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

#ifndef _DECAF_SECURITY_SECURERANDOM_H_
#define _DECAF_SECURITY_SECURERANDOM_H_

#include <decaf/util/Config.h>

#include <decaf/util/Random.h>
#include <decaf/security/SecureRandomSpi.h>

#include <memory>

namespace decaf {
namespace security {

    /**
     * @since 1.0
     */
    class DECAF_API SecureRandom : public decaf::util::Random {
    private:

        std::auto_ptr<SecureRandomSpi> secureRandom;

    public:

        /**
         * Creates a new instance of a secure random number generator that implements the
         * default random number algorithm.
         *
         * The SecureRandom instance that is created with this constructor is unseeded and
         * can be seeded by calling the setSeed method.  Calls to nextBytes on an unseeded
         * SecureRandom result in the object seeding itself.
         */
        SecureRandom();

        /**
         * Creates a new instance of a secure random number generator that implements the
         * default random number algorithm.
         *
         * The SecureRandom instance created by this constructor is seeded using the passed
         * byte array.
         *
         * @param seed
         *      The seed bytes to use to seed this secure random number generator.
         */
        SecureRandom( const std::vector<unsigned char>& seed );

        /**
         * Creates a new instance of a secure random number generator that implements the
         * default random number algorithm.
         *
         * The SecureRandom instance created by this constructor is seeded using the passed
         * byte array.
         *
         * @param seed
         *      The seed bytes to use to seed this secure random number generator.
         * @param size
         *      The number of bytes in the seed buffer.
         *
         * @throw NullPointerException if the seed buffer is NULL.
         * @throw IllegalArgumentException if the size value is negative.
         */
        SecureRandom( const unsigned char* seed, int size );

        virtual ~SecureRandom();

    public:  // Virtual Methods

        /**
         * {@inheritDoc}
         */
        virtual void nextBytes( std::vector<unsigned char>& buf );

        /**
         * {@inheritDoc}
         */
        virtual void nextBytes( unsigned char* buf, int size );

        /**
         * {@inheritDoc}
         */
        virtual void setSeed( unsigned long long seed );

        /**
         * Supplements or sets the seed of this secure random number generator, calls to this
         * method never reduces randomness.
         *
         * @param seed
         *      A vector of bytes that is used update the seed of the RNG.
         */
        virtual void setSeed( const std::vector<unsigned char>& seed );

        /**
         * Supplements or sets the seed of this secure random number generator, calls to this
         * method never reduces randomness.
         *
         * @param seed
         *      The seed bytes to use to seed this secure random number generator.
         * @param size
         *      The number of bytes in the seed buffer.
         *
         * @throw NullPointerException if the seed buffer is NULL.
         * @throw IllegalArgumentException if the size value is negative.
         */
        virtual void setSeed( const unsigned char* seed, int size );

    protected:  // Virtual method used by all non-virtual methods in Random.

        /**
         * {@inheritDoc}
         */
        virtual int next( int bits );

    };

}}

#endif /* _DECAF_SECURITY_SECURERANDOM_H_ */
