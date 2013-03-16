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

#ifndef _DECAF_SECURITY_MESSAGEDIGEST_H_
#define _DECAF_SECURITY_MESSAGEDIGEST_H_

#include <decaf/util/Config.h>

#include <decaf/security/MessageDigestSpi.h>
#include <decaf/nio/ByteBuffer.h>

#include <string>

namespace decaf {
namespace security {

    class MessageDigestSpi;
    class MessageDigestImpl;
    class Provider;

    /**
     * This MessageDigest class provides applications the functionality of a message digest
     * algorithm, such as MD5 or SHA. Message digests are secure one-way hash functions that
     * take arbitrary-sized data and output a fixed-length hash value.
     *
     * A MessageDigest object starts out initialized. The data is processed through it using
     * the update methods. At any point reset can be called to reset the digest. Once all the
     * data to be updated has been updated, one of the digest methods should be called to
     * complete the hash computation.
     *
     * The digest method can be called once for a given number of updates. After digest has
     * been called, the MessageDigest object is reset to its initialized state.
     *
     * Implementations are free to implement the clone method.  Client applications can test
     * cloneability by attempting cloning and catching the CloneNotSupportedException:
     *
     *   MessageDigest* md = MessageDigest::getInstance("SHA");
     *
     *   try {
     *       md->update(toChapter1);
     *       MessageDigest* tc1 = md.clone();
     *       byte[] toChapter1Digest = tc1.digest();
     *       md.update(toChapter2);
     *       ...etc.
     *   } catch (CloneNotSupportedException& ex) {
     *       throw DigestException("couldn't make digest of partial content");
     *   }
     *
     * Note that if a given implementation is not cloneable, it is still possible to compute
     * intermediate digests by instantiating several instances, if the number of digests is
     * known in advance.
     *
     * @see MessageDigestSpi
     *
     * @since 1.0
     */
    class DECAF_API MessageDigest {
    private:

        MessageDigestImpl* impl;

        MessageDigestSpi* spi;
        const Provider* provider;
        std::string algorithm;

    private:

        MessageDigest(const MessageDigest&);
        MessageDigest& operator= (const MessageDigest&);

    protected:

        MessageDigest(const std::string& name);

    public:

        virtual ~MessageDigest();

        /**
         * Completes the hash computation by performing final operations such as padding.
         * The digest is reset after this call is made.
         */
        std::vector<unsigned char> digest();

        /**
         * Completes the hash computation by performing final operations such as padding.
         * The digest is reset after this call is made.
         *
         * @param input
         *      The output buffer for the computed digest.
         * @param size
         *      The size of the given input buffer.
         * @param offset
         *      The offset into the output buffer to begin storing the digest.
         * @param length
         *      The number of bytes within buf allotted for the digest.
         *
         * @returns the number of bytes placed into buffer.
         *
         * @throws DigestException if an error occurs.
         */
        int digest(unsigned char* input, int size, int offset, int length);

        /**
         * Performs a final update on the digest using the specified array of bytes, then
         * completes the digest computation. That is, this method first calls update(input),
         * passing the input array to the update method, then calls digest().
         *
         * @param input
         *      The input to be updated before the digest is completed.
         * @param size
         *      The length in bytes of the input buffer.
         *
         * @returns the array of bytes for the resulting hash value.
         */
        std::vector<unsigned char> digest(const unsigned char* input, int size);

        /**
         * Performs a final update on the digest using the specified array of bytes, then
         * completes the digest computation. That is, this method first calls update(input),
         * passing the input array to the update method, then calls digest().
         *
         * @param input
         *      The input to be updated before the digest is completed.
         *
         * @returns the array of bytes for the resulting hash value.
         */
        std::vector<unsigned char> digest(const std::vector<unsigned char>& input);

        /**
         * Returns a string that identifies the algorithm, independent of implementation
         * details. The name should be a standard name (such as "SHA", "MD5", etc).
         *
         * @returns the algorithm name.
         */
        std::string getAlgorithmName() const {
            return this->algorithm;
        }

        /**
         * Returns the Provider associated with this MessageDigest.
         *
         * The pointer returned by this method remains the property of the Security framework
         * and should be deleted by the calling application at any time.
         *
         * @return the provider associated with this MessageDigest.
         */
        const Provider* getProvider() const {
            return this->provider;
        }

        /**
         * Returns the length of the digest in bytes, or 0 if this operation is not supported
         * by the provider and the implementation is not cloneable.
         *
         * @returns the digest length in bytes, or 0 if this operation is not supported by the
         *          provider and the implementation is not cloneable.
         */
        int getDigestLength() const;

        /**
         * Returns a clone of this MessageDisgest instance if the MessageDigestSpi in use
         * is cloneable.
         *
         * @returns a clone of this MessageDigest if possible.
         *
         * @throws CloneNotSupportedException if the SPI in use cannot be cloned.
         */
        MessageDigest* clone();

        /**
         * Resets the digest for further use.
         */
        void reset();

        /**
         * Returns a string representation of this message digest object.
         *
         * @returns a string representation of this message digest object.
         */
        std::string toString() const;

        /**
         * Updates the digest using the specified byte.
         *
         * @param input
         *      The byte with which to update the digest.
         */
        void update(unsigned char input);

        /**
         * Updates the digest using the specified array of bytes, starting at the specified offset.
         *
         * @param input
         *      The array of bytes.
         * @param input
         *      The size of the given input buffer.
         * @param offset
         *      The offset to start from in the array of bytes.
         * @param length
         *      The number of bytes to use, starting at offset.
         */
        void update(unsigned char* input, int size, int offset, int len);

        /**
         * Updates the digest using the specified array of bytes.
         *
         * @param input
         *      The array of bytes to use for the update.
         */
        void update(const std::vector<unsigned char>& input);

        /**
         * Update the digest using the specified ByteBuffer. The digest is updated using
         * the input.remaining() bytes starting at input.position(). Upon return, the
         * buffer's position will be equal to its limit; its limit will not have changed.
         *
         * @param input
         *      The input ByteBuffer to use for the update.
         */
        void update(nio::ByteBuffer& input);

    public:

        /**
         * Returns a MessageDigest object that implements the specified digest algorithm.
         *
         * This method traverses the list of registered security Providers, starting with the
         * most preferred Provider. A new MessageDigest object encapsulating the MessageDigestSpi
         * implementation from the first Provider that supports the specified algorithm is returned.
         *
         * Note that the list of registered providers may be retrieved via the
         * Security.getProviders() method.
         *
         * @param algorithm
         *      The name of the algorithm requested. (MD5, SHA-1, etc)
         *
         * @returns a Message Digest object that implements the specified algorithm.
         * @throws NoSuchAlgorithmException if no Provider supports a MessageDigestSpi implementation
         *         for the specified algorithm.
         */
        static MessageDigest* getInstance(const std::string& algorithm);

        /**
         * Compares two digests for equality. Does a simple byte compare.
         *
         * @param digesta
         *      The first digest for comparison.
         * @param digesta
         *      The second digest for comparison.
         *
         * @returns true if the two digests are equal.
         */
        static bool isEqual(const std::vector<unsigned char>& digesta,
                            const std::vector<unsigned char>& digestb);

    };

}}

#endif /* _DECAF_SECURITY_MESSAGEDIGEST_H_ */
