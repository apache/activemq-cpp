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

#include <activemq/util/PrimitiveMap.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace marshal{

    /**
     * This class wraps the functionality needed to marshal a primitive
     * map to the Openwire Format's expectation of what the map looks like
     * on the wire.
     */
    class PrimitiveMapMarshaller
    {
    public:
    
        PrimitiveMapMarshaller();
        virtual ~PrimitiveMapMarshaller();
        
        /**
         * Static Marshal of a primitive map object
         * @param Map to Marshal
         * @param Reference to a byte array to house the data
         * @throws CMSException
         */
        static void marshal( const util::PrimitiveMap& map, 
                             std::vector<unsigned char>& dest ) throw ( cms::CMSException );        

        /**
         * Static Map Unmarshaler, takes an array of bytes and returns a 
         * new instance of a PrimitiveMap object.  Caller owns the pointer.
         * @param reference to a byte array to read data from.
         * @return newly create PrimitiveMap with unmarshaled data.
         */
        static PrimitiveMap* unmarshal( const std::vector<unsigned char>& src ) 
            throw ( cms::CMSException );
        
    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_PRIMITIVEMAPMARSHALLER_H_*/
