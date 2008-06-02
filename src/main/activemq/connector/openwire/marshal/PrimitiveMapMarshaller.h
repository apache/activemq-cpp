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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_PRIMITIVEMAPMARSHALLER_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_PRIMITIVEMAPMARSHALLER_H_

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
namespace connector{
namespace openwire{
namespace marshal{

    /**
     * This class wraps the functionality needed to marshal a primitive
     * map to the Openwire Format's expectation of what the map looks like
     * on the wire.
     */
    class AMQCPP_API PrimitiveMapMarshaller {
    public:

        PrimitiveMapMarshaller() {}
        virtual ~PrimitiveMapMarshaller() {}

        /**
         * Static Marshal of a primitive map object
         * @param Map to Marshal
         * @param Reference to a byte array to house the data
         * @throws CMSException
         */
        static void marshal( const util::PrimitiveMap* map,
                             std::vector<unsigned char>& dest )
                                throw ( cms::CMSException );

        /**
         * Static Map Unmarshaler, takes an array of bytes and returns a
         * new instance of a PrimitiveMap object.  Caller owns the pointer.
         * @param reference to a byte array to read data from.
         * @return newly create PrimitiveMap with unmarshaled data.
         */
        static util::PrimitiveMap* unmarshal( const std::vector<unsigned char>& src )
            throw ( cms::CMSException );

        /**
         * Static Map Unmarshaler, takes an array of bytes and returns a
         * new instance of a PrimitiveMap object.  Caller owns the pointer.
         * @param Map to Unmarshal into
         * @param reference to a byte array to read data from.
         */
        static void unmarshal( util::PrimitiveMap* map,
                               const std::vector<unsigned char>& src )
                                    throw ( cms::CMSException );

    protected:

        /**
         * Marshal a Map of Primitives to the given OutputStream, can result
         * in recursive calls to this method if the map contains maps of maps.
         * @param dataOut - the DataOutputStream to write to
         * @param map - the ValueNode to write.
         * @throws IOException
         */
        static void marshalPrimitiveMap(
            decaf::io::DataOutputStream& dataOut,
            const decaf::util::Map<std::string, util::PrimitiveValueNode>& map )
                throw ( decaf::io::IOException );

        /**
         * Marshal a List of Primitives to the given OutputStream, can result
         * in recursive calls to this method if the list contains lists of lists.
         * @param dataOut - the DataOutputStream to write to
         * @param list - the ValueNode to write.
         * @throws IOException
         */
        static void marshalPrimitiveList(
            decaf::io::DataOutputStream& dataOut,
            const decaf::util::List<util::PrimitiveValueNode>& list )
                throw ( decaf::io::IOException );

        /**
         * Used to Marshal the Primitive types out on the Wire.
         * @param dataOut - the DataOutputStream to write to
         * @param value - the ValueNode to write.
         * @throws IOException
         */
        static void marshalPrimitive( decaf::io::DataOutputStream& dataOut,
                                      const util::PrimitiveValueNode& value )
                                        throw ( decaf::io::IOException );

        /**
         * Unmarshals a Map of Primitives from the given InputStream, can result
         * in recursive calls to this method if the map contains maps of maps.
         * @param dataIn - DataInputStream to read from.
         * @param map - the map to fill with data.
         * @throws IOException
         */
        static void unmarshalPrimitiveMap(
            decaf::io::DataInputStream& dataIn, util::PrimitiveMap& map )
                throw ( decaf::io::IOException );

        /**
         * Unmarshals a List of Primitives from the given InputStream, can result
         * in recursive calls to this method if the list contains lists of lists.
         * @param dataIn - DataInputStream to read from.
         * @param list - the ValueNode to write.
         * @throws IOException
         */
        static void unmarshalPrimitiveList(
            decaf::io::DataInputStream& dataIn,
            decaf::util::List<util::PrimitiveValueNode>& list )
                throw ( decaf::io::IOException );

        /**
         * Unmarshals a Primitive Type from the stream, and returns it as a
         * value Node.
         * @param dataIn - DataInputStream to read from.
         * @return a PrimitiveValueNode containing the data.
         * @throws IOException
         */
        static util::PrimitiveValueNode unmarshalPrimitive(
            decaf::io::DataInputStream& dataIn )
                throw ( decaf::io::IOException );

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_PRIMITIVEMAPMARSHALLER_H_*/
