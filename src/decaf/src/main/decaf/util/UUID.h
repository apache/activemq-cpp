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

#ifndef _DECAF_UTIL_UUID_H_
#define _DECAF_UTIL_UUID_H_

#include <decaf/util/Config.h>
#include <decaf/util/Comparable.h>
#include <apr_pools.h>
#include <apr_uuid.h>

namespace decaf{
namespace util{

    /**
     * A class that represents an immutable universally unique identifier (UUID).
     * A UUID represents a 128-bit value.
     */
    class UUID : public Comparable<UUID> {
    private:

        // APR Pool to allocate from
        apr_pool_t* pool;

        // APR Uuid Type
        apr_uuid_t* uuid;

    public:

        /**
         * Static factory to retrieve a type 4 (pseudo randomly generated) UUID.
         * The UUID is generated using a cryptographically strong pseudo random
         * number generator.
         * @return type 4 UUID
         */
        static UUID randomUUID();

        /**
         * Static factory to retrieve a type 3 (name based) UUID based on the
         * specified byte array.
         * @param name - a byte array to be used to construct a UUID.
         * @return type 3 UUID
         */
        static UUID nameUUIDFromBytes( const char* name );

        /**
         * Creates a UUID from the string standard representation as described
         * in the toString() method.
         * @param name - a string to be used to construct a UUID.
         * @return type 3 UUID
         */
        static UUID fromString( const std::string& name );

    public:

        /**
         * Constructs a new UUID using the specified data. mostSigBits is used
         * for the most significant 64 bits of the UUID and leastSigBits becomes
         * the least significant 64 bits of the UUID.
         * @param mostSigBits
         * @param leastSigBits
         */
        UUID( long mostSigBits, long leastSigBits );

        virtual ~UUID();

        /**
         * Compare the given UUID to this one
         * @param value - the UUID to compare to
         */
        virtual int compareTo( const UUID& value );

        /**
         * @returns the most significant 64 bits of this UUID's 128 bit value.
         */
        virtual long getLeastSignificantBits() const;

        /**
         * @returns the most significant 64 bits of this UUID's 128 bit value.
         */
        virtual long getMostSignificantBits() const;

    };

}}

#endif /*_DECAF_UTIL_UUID_H_*/
