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
#include <decaf/lang/Comparable.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <apr_uuid.h>
#include <string>

namespace decaf{
namespace util{

    /**
     * A class that represents an immutable universally unique identifier (UUID).
     * A UUID represents a 128-bit value.
     *
     * There exist different variants of these global identifiers. The methods of
     * this class are for manipulating the Leach-Salz variant, although the
     * constructors allow the creation of any variant of UUID (described below).
     *
     * The layout of a variant 2 (Leach-Salz) UUID is as follows: The most
     * significant long consists of the following unsigned fields:
     *
     *  0xFFFFFFFF00000000 time_low
     *  0x00000000FFFF0000 time_mid
     *  0x000000000000F000 version
     *  0x0000000000000FFF time_hi
     *
     * The least significant long consists of the following unsigned fields:
     *
     *  0xC000000000000000 variant
     *  0x3FFF000000000000 clock_seq
     *  0x0000FFFFFFFFFFFF node
     *
     * The variant field contains a value which identifies the layout of the UUID.
     * The bit layout described above is valid only for a UUID with a variant value
     * of 2, which indicates the Leach-Salz variant.
     *
     * The version field holds a value that describes the type of this UUID. There
     * are four different basic types of UUIDs: time-based, DCE security, name-based,
     * and randomly generated UUIDs. These types have a version value of 1, 2, 3 and
     * 4, respectively.
     *
     * For more information including algorithms used to create UUIDs, see the
     * Internet-Draft UUIDs and GUIDs or the standards body definition at
     * ISO/IEC 11578:1996.
     */
    class DECAF_API UUID : public lang::Comparable<UUID> {
    private:

        // APR Uuid Type
        apr_uuid_t apr_uuid;

        // Copy of the High part of the data
        unsigned long long mostSigBits;

        // Copy of the Low part of the data
        unsigned long long leastSigBits;

        // Version indicator, set when a UUID is generated
        int uuidVersion;

    public:

        /**
         * Static factory to retrieve a type 4 (pseudo randomly generated) UUID.
         * The UUID is generated using a cryptographically strong pseudo random
         * number generator.
         *
         * @return type 4 UUID
         */
        static UUID randomUUID();

        /**
         * Static factory to retrieve a type 3 (name based) UUID based on the
         * specified byte array.
         *
         * @param name
         *      A byte array to be used to construct a UUID.
         *
         * @return type 3 UUID
         */
        static UUID nameUUIDFromBytes( const std::vector<char>& name );

        /**
         * Static factory to retrieve a type 3 (name based) UUID based on the
         * specified byte array.
         *
         * @param name
         *      A byte array to be used to construct a UUID.
         * @param size
         *      The size of the byte array, or number of bytes to use.
         *
         * @return type 3 UUID
         */
        static UUID nameUUIDFromBytes( const char* name, std::size_t size );

        /**
         * Creates a UUID from the string standard representation as described
         * in the toString() method.
         *
         * @param name
         *      A string to be used to construct a UUID.
         *
         * @return type 3 UUID
         *
         * @throws IllegalArgumentException if the UUID string given is invalid.
         */
        static UUID fromString( const std::string& name );

    public:

        /**
         * Constructs a new UUID using the specified data. mostSigBits is used
         * for the most significant 64 bits of the UUID and leastSigBits becomes
         * the least significant 64 bits of the UUID.
         *
         * @param mostSigBits
         * @param leastSigBits
         */
        UUID( long long mostSigBits, long long leastSigBits );

        virtual ~UUID();

        /**
         * Compare the given UUID to this one
         * @param value - the UUID to compare to
         */
        virtual int compareTo( const UUID& value ) const;

        /**
         * Compares this UUID to the one given, returns true if they are equal.
         *
         * @param value
         *      The UUID to compare to.
         *
         * @return true if UUIDs are the same.
         */
        virtual bool equals( const UUID& value ) const;

        /**
         * Compares equality between this object and the one passed.
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const UUID& value ) const;

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const UUID& value ) const;

        /**
         * Returns a String object representing this UUID.  UUID's are formatted
         * as: 00112233-4455-6677-8899-AABBCCDDEEFF whose length is 36.
         * @returns formatted string for this UUID
         */
        virtual std::string toString() const;

        /**
         * @returns the most significant 64 bits of this UUID's 128 bit value.
         */
        virtual long long getLeastSignificantBits() const;

        /**
         * @returns the most significant 64 bits of this UUID's 128 bit value.
         */
        virtual long long getMostSignificantBits() const;

        /**
         * The node value associated with this UUID.
         *
         * The 48 bit node value is constructed from the node field of this UUID.
         * This field is intended to hold the IEEE 802 address of the machine that
         * generated this UUID to guarantee spatial uniqueness.
         *
         * The node value is only meaningful in a time-based UUID, which has
         * version type 1. If this UUID is not a time-based UUID then this method
         * throws UnsupportedOperationException.
         *
         * @returns the node value of this UUID
         *
         * @throws UnsupportedOperationException if this UUID version does not support this operation.
         */
        virtual long long node();

        /**
         * The timestamp value associated with this UUID.
         *
         * The 60 bit timestamp value is constructed from the time_low, time_mid,
         * and time_hi fields of this UUID. The resulting timestamp is measured in
         * 100-nanosecond units since midnight, October 15, 1582 UTC.
         *
         * The timestamp value is only meaningful in a time-based UUID, which has
         * version type 1. If this UUID is not a time-based UUID then this method
         * throws UnsupportedOperationException.
         * @returns the timestamp associated with a V1 UUID
         * @throws UnsupportedOperationException if this UUID version does not support this operation.
         */
        virtual long long timestamp();

        /**
         * The clock sequence value associated with this UUID.
         *
         * The 14 bit clock sequence value is constructed from the clock sequence
         * field of this UUID. The clock sequence field is used to guarantee temporal
         * uniqueness in a time-based UUID.
         *
         * The clockSequence value is only meaningful in a time-based UUID, which
         * has version type 1. If this UUID is not a time-based UUID then this
         * method throws UnsupportedOperationException.
         * @returns the clockSequeunce associated with a V1 UUID
         * @throws UnsupportedOperationException if this UUID version does not support this operation.
         */
        virtual int clockSequence();

        /**
         * The variant number associated with this UUID. The variant number describes
         * the layout of the UUID. The variant number has the following meaning:
         *
         *     * 0 Reserved for NCS backward compatibility
         *     * 2 The Leach-Salz variant (used by this class)
         *     * 6 Reserved, Microsoft Corporation backward compatibility
         *     * 7 Reserved for future definition
         *
         * @returns the variant associated with a V1 UUID
         *
         * @throws UnsupportedOperationException if this UUID version does not support this operation.
         */
        virtual int variant();

        /**
         * The version number associated with this UUID. The version number describes
         * how this UUID was generated. The version number has the following meaning:
         *
         *     * 1 Time-based UUID
         *     * 2 DCE security UUID
         *     * 3 Name-based UUID
         *     * 4 Randomly generated UUID
         *
         * @returns the version associated with a V1 UUID
         *
         * @throws UnsupportedOperationException if this UUID version does not support this operation.
         */
        virtual int version();

    };

}}

#endif /*_DECAF_UTIL_UUID_H_*/
