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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_UTILS_BOOLEANSTREAM_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_UTILS_BOOLEANSTREAM_H_

#include <activemq/io/DataInputStream.h>
#include <activemq/io/DataOutputStream.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace utils{

    /**
     * class to manage the writing and reading of boolean data streams
     * to and from a data source such as a DataInputStream or 
     * DataOutputStream.  The booleans are stored as single bits in the
     * stream, with the stream size pre-pended to the stream when the
     * data is marshalled.
     */
    class BooleanStream
    {
    public:
    
        BooleanStream();
        virtual ~BooleanStream();

        /**
         * Read a boolean data element from the internal data buffer
         * @returns boolean from the stream
         */
        bool readBoolean() throw ( io::IOException );

        /**
         * Writes a Boolean value to the internal data buffer
         * @param value - boolean data to write.
         */
        void writeBoolean( bool value ) throw ( io::IOException );

        /**
         * Marshal the data to a DataOutputStream
         * @param dataOut - Stream to write the data to.
         */
        void marshal( io::DataOutputStream* dataOut ) throw ( io::IOException );
        
        /**
         * Marshal the data to a STL vector of unsigned chars
         * @param dataOut - reference to a vector to write the data to.
         */
        void marshal( std::vector< unsigned char >& dataOut );        

        /**
         * Unmarshal a Boolean data stream from the Input Stream
         * @param dataIn - Input Stream to read data from.
         */
        void unmarshal( io::DataInputStream* dataIn ) throw ( io::IOException );

        /**
         * Clears to old position markers, data starts at the beginning
         */
        void clear();
    
        /**
         * Calc the size that data is marshalled to
         * @returns int size of marshalled data.
         */
        int marshalledSize();
        
    private:
    
        // Internal Buffer of data
        std::vector<unsigned char> data;
        
        // Limit on buffer size
        short arrayLimit;
        
        // Byte we are on from the buffer
        short arrayPos;
        
        // Bit we are on in the byte we are on from the buffer
        unsigned char bytePos;
    
    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_UTILS_BOOLEANSTREAM_H_*/
