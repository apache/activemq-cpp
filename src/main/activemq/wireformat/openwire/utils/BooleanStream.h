/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_UTILS_BOOLEANSTREAM_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_UTILS_BOOLEANSTREAM_H_

#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace wireformat{
namespace openwire{
namespace utils{

    /**
     * Manages the writing and reading of boolean data streams
     * to and from a data source such as a DataInputStream or
     * DataOutputStream.  The booleans are stored as single bits in the
     * stream, with the stream size pre-pended to the stream when the
     * data is marshalled.
     *
     * The serialized form of the size field can be between 1 and 3 bytes.
     * If the number of used bytes < 64, size=1 byte
     * If the number of used bytes >=64 and < 256 (size of an unsigned byte), size=2 bytes
     * If the number of used bytes >=256, size=3 bytes
     *
     * The high-order 2 bits (128 and 64) of the first byte of the size field are
     * used to encode the information about the number of bytes in the size field.  The
     * only time the first byte will contain a value >=64 is if there are more bytes in
     * the size field.  If the first byte < 64, the value of the byte is simply the size
     * value.  If the first byte = 0xC0, the following unsigned byte is the size field.
     * If the first byte = 0x80, the following short (two bytes) are the size field.
     */
    class AMQCPP_API BooleanStream {
    private:

        // Internal Buffer of data
        std::vector<unsigned char> data;

        // Limit on buffer size
        short arrayLimit;

        // Byte we are on from the buffer
        short arrayPos;

        // Bit we are on in the byte we are on from the buffer
        unsigned char bytePos;

    public:

        BooleanStream();

        virtual ~BooleanStream() throw();

        /**
         * Read a boolean data element from the internal data buffer
         *
         * @returns boolean from the stream
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        bool readBoolean();

        /**
         * Writes a Boolean value to the internal data buffer
         * @param value - boolean data to write.
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        void writeBoolean( bool value );

        /**
         * Marshal the data to a DataOutputStream
         * @param dataOut - Stream to write the data to.
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        void marshal( decaf::io::DataOutputStream* dataOut );

        /**
         * Marshal the data to a STL vector of unsigned chars
         * @param dataOut - reference to a vector to write the data to.
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        void marshal( std::vector< unsigned char >& dataOut );

        /**
         * Unmarshal a Boolean data stream from the Input Stream
         * @param dataIn - Input Stream to read data from.
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        void unmarshal( decaf::io::DataInputStream* dataIn );

        /**
         * Clears to old position markers, data starts at the beginning
         */
        void clear();

        /**
         * Calc the size that data is marshalled to
         * @returns int size of marshalled data.
         */
        int marshalledSize();

    };

}}}}

#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_UTILS_BOOLEANSTREAM_H_*/
