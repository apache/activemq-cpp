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

#ifndef _DECAF_SECURITY_MESSAGEDIGESTSPI_H_
#define _DECAF_SECURITY_MESSAGEDIGESTSPI_H_

#include <decaf/util/Config.h>

#include <decaf/nio/ByteBuffer.h>
#include <decaf/security/SecuritySpi.h>

#include <vector>

namespace decaf {
namespace security {

    class MessageDigest;

    /**
     * This class defines the Service Provider Interface (SPI) for the MessageDigest class,
     * which provides the functionality of a message digest algorithm, such as MD5 or SHA.
     * Message digests are secure one-way hash functions that take arbitrary-sized data and
     * output a fixed-length hash value.
     *
     * All the pure virtual methods in this class must be implemented by a cryptographic
     * service provider who wishes to supply the implementation of a particular message digest
     * algorithm.
     *
     * Implementations are free to implement clone method or throw a CloneNotSupportedException..
     *
     * @since 1.0
     */
    class DECAF_API MessageDigestSpi : public SecuritySpi {
    public:

        MessageDigestSpi();

        virtual ~MessageDigestSpi();

        /**
         * Queries the SPI implementation and returns true if the SPI can be
         * cloned.
         *
         * @returns true if the SPI is clonable.
         */
        virtual bool isCloneable() const;

        /**
         * Returns a clone if the implementation supports being cloned.
         *
         * @returns a new pointer that is a copy of this object.
         *
         * @throws CloneNotSupportedException
         *      if this is called on an implementation that does not support cloning.
         */
        virtual MessageDigestSpi* clone();

    protected:

        /**
         * Returns the digest length in bytes.
         *
         * @returns The digest length in bytes.
         */
        virtual int engineGetDigestLength() = 0;

        /**
         * Updates the digest using the specified byte.
         *
         * @param input
         *      The byte to use for the update.
         */
        virtual void engineUpdate(unsigned char input) = 0;

        /**
         * Updates the digest using the specified array of bytes, starting at the specified offset.
         *
         * @param input
         *      The array of bytes to use for the update.
         * @param size
         *      The size of the given input buffer..
         * @param offset
         *      The offset to start from in the array of bytes.
         * @param length
         *      The number of bytes to use, starting at offset.
         *
         * @throws NullPointerException if the input array pointer is NULL.
         */
        virtual void engineUpdate(const unsigned char* input, int size, int offset, int length) = 0;

        /**
         * Resets the digest for further use.
         */
        virtual void engineReset() = 0;

        /**
         * Update the digest using the specified Vector of Bytes.
         *
         * @param input
         *      The vector of bytes that will be used to update the digest.
         */
        virtual void engineUpdate(const std::vector<unsigned char>& input) = 0;

        /**
         * Update the digest using the specified ByteBuffer. The digest is updated using
         * the input.remaining() bytes starting at input.position(). Upon return, the
         * buffer's position will be equal to its limit; its limit will not have changed.
         *
         * @param input
         *      The ByteBuffer instance that will be used to update the digest.
         */
        virtual void engineUpdate(decaf::nio::ByteBuffer& input) = 0;

        /**
         * Completes the hash computation by performing final operations such as padding.
         * Once engineDigest has been called, the engine should be reset (see engineReset).
         * Resetting is the responsibility of the engine implementor.
         *
         * @returns an STL vector of bytes containing the resulting hash value.
         */
        virtual std::vector<unsigned char> engineDigest() = 0;

        /**
         * Completes the hash computation by performing final operations such as padding.
         * Once engineDigest has been called, the engine should be reset (see engineReset).
         * Resetting is the responsibility of the engine implementor.
         *
         * @param buffer
         *      The output buffer in which to store the digest
         * @param size
         *      The size of the given input buffer
         * @param offset
         *      The offset to start from in the output buffer
         * @param length
         *      The number of bytes within buffer allotted for the digest.
         *      Both this default implementation and the SUN provider do not
         *      return partial digests. The presence of this parameter is solely
         *      for consistency in our API's. If the value of this parameter is
         *      less than the actual digest length, the method will throw a
         *      DigestException. This parameter is ignored if its value is greater
         *      than or equal to the actual digest length.
         *
         * @returns the length of the digest stored in the output buffer.
         *
         * @throws DigestException if an error occurs.
         * @throws NullPointerException if the buffer pointer is NULL.
         */
        virtual int engineDigest(unsigned char* buffer, int size, int offset, int length) = 0;

    private:

        friend class MessageDigest;

    };

}}

#endif /* _DECAF_SECURITY_MESSAGEDIGESTSPI_H_ */
