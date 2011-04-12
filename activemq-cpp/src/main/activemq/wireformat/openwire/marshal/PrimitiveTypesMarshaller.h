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

#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_PRIMITIVETYPESMARSHALLER_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_PRIMITIVETYPESMARSHALLER_H_

#include <cms/CMSException.h>
#include <activemq/util/Config.h>
#include <activemq/util/PrimitiveValueNode.h>
#include <activemq/util/PrimitiveMap.h>
#include <activemq/util/PrimitiveList.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/IOException.h>
#include <string>

namespace activemq{
namespace wireformat{
namespace openwire{
namespace marshal{

    /**
     * This class wraps the functionality needed to marshal a primitive
     * map to the Openwire Format's expectation of what the map looks like
     * on the wire.
     */
    class AMQCPP_API PrimitiveTypesMarshaller {
    public:

        PrimitiveTypesMarshaller() {}
        virtual ~PrimitiveTypesMarshaller() {}

        /**
         * Marshal a primitive map object to the given byte buffer.
         *
         * @param map
         *      Map to Marshal.
         * @param buffer
         *      The byte buffer to write the marshaled data to.
         *
         * @throws Exception if an error occurs during the marshaling process.
         */
        static void marshal( const util::PrimitiveMap* map, std::vector<unsigned char>& buffer );

        /**
         * Unmarshal a PrimitiveMap from the provided Byte buffer.
         *
         * @param map
         *      The Map to populate with values from the marshaled data.
         * @param buffer
         *      The byte buffer containing the marshaled Map.
         *
         * @throws Exception if an error occurs during the unmarshal process.
         */
        static void unmarshal( util::PrimitiveMap* map, const std::vector<unsigned char>& buffer );

        /**
         * Marshal a primitive list object to the given byte buffer.
         *
         * @param map
         *      The PrimitiveList to Marshal.
         * @param buffer
         *      The byte buffer to write the marshaled data to.
         *
         * @throws Exception if an error occurs during the marshaling process.
         */
        static void marshal( const util::PrimitiveList* list, std::vector<unsigned char>& buffer );

        /**
         * Unmarshal a PrimitiveList from the provided byte buffer.
         *
         * @param map
         *      The List to populate with values from the marshaled data.
         * @param buffer
         *      The byte buffer containing the marshaled Map.
         *
         * @throws Exception if an error occurs during the unmarshal process.
         */
        static void unmarshal( util::PrimitiveList* list, const std::vector<unsigned char>& buffer );

    public:

        /**
         * Marshal a primitive map object to the given DataOutputStream.
         *
         * @param map
         *      Map to Marshal.
         * @param dataOut
         *      Reference to a DataOutputStream to write the marshaled data to.
         *
         * @throws Exception if an error occurs during the marshaling process.
         */
        static void marshalMap( const util::PrimitiveMap* map, decaf::io::DataOutputStream& dataOut );

        /**
         * Unmarshal a PrimitiveMap from the provided DataInputStream.
         *
         * @param dataIn
         *      The DataInputStream instance to read the marshaled PrimitiveMap from.
         *
         * @return a pointer to a newly allocated PrimitiveMap instnace.
         *
         * @throws Exception if an error occurs during the unmarshal process.
         */
        static util::PrimitiveMap* unmarshalMap( decaf::io::DataInputStream& dataIn );

        /**
         * Marshal a PrimitiveList to the given DataOutputStream.
         *
         * @param list
         *      The list object to Marshal
         * @param dataOut
         *      Reference to a DataOutputStream to write the marshaled data to.
         *
         * @throws Exception if an error occurs during the marshaling process.
         */
        static void marshalList( const util::PrimitiveList* list, decaf::io::DataOutputStream& dataOut );

        /**
         * Unmarshal a PrimitiveList from the given DataInputStream.
         *
         * @param dataIn
         *      The DataInputStream instance to read the marshaled PrimitiveList from.
         *
         * @return a pointer to a newly allocated PrimitiveList instnace.
         *
         * @throws Exception if an error occurs during the unmarshal process.
         */
        static util::PrimitiveList* unmarshalList( decaf::io::DataInputStream& dataIn );

    protected:

        /**
         * Marshal a Map of Primitives to the given OutputStream, can result
         * in recursive calls to this method if the map contains maps of maps.
         *
         * @param dataOut - the DataOutputStream to write to
         * @param map - the ValueNode to write.
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        static void marshalPrimitiveMap(
            decaf::io::DataOutputStream& dataOut,
            const decaf::util::Map<std::string, util::PrimitiveValueNode>& map );

        /**
         * Marshal a List of Primitives to the given OutputStream, can result
         * in recursive calls to this method if the list contains lists of lists.
         * @param dataOut - the DataOutputStream to write to
         * @param list - the ValueNode to write.
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        static void marshalPrimitiveList(
            decaf::io::DataOutputStream& dataOut,
            const decaf::util::List<util::PrimitiveValueNode>& list );

        /**
         * Used to Marshal the Primitive types out on the Wire.
         * @param dataOut - the DataOutputStream to write to
         * @param value - the ValueNode to write.
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        static void marshalPrimitive( decaf::io::DataOutputStream& dataOut,
                                      const util::PrimitiveValueNode& value );

        /**
         * Unmarshals a Map of Primitives from the given InputStream, can result
         * in recursive calls to this method if the map contains maps of maps.
         * @param dataIn - DataInputStream to read from.
         * @param map - the map to fill with data.
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        static void unmarshalPrimitiveMap(
            decaf::io::DataInputStream& dataIn, util::PrimitiveMap& map );

        /**
         * Unmarshals a List of Primitives from the given InputStream, can result
         * in recursive calls to this method if the list contains lists of lists.
         * @param dataIn - DataInputStream to read from.
         * @param list - the ValueNode to write.
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        static void unmarshalPrimitiveList(
            decaf::io::DataInputStream& dataIn,
            decaf::util::LinkedList<util::PrimitiveValueNode>& list );

        /**
         * Unmarshals a Primitive Type from the stream, and returns it as a
         * value Node.
         * @param dataIn - DataInputStream to read from.
         * @return a PrimitiveValueNode containing the data.
         *
         * @throws IOException if an I/O error occurs during this operation.
         */
        static util::PrimitiveValueNode unmarshalPrimitive( decaf::io::DataInputStream& dataIn );

    };

}}}}

#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_PRIMITIVETYPESMARSHALLER_H_*/
