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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_MARSHALAWARE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_MARSHALAWARE_H_

#include <vector> 
#include <activemq/connector/openwire/OpenWireFormat.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace marshal{

    class MarshalAware
    {
    public:
    
        virtual ~MarshalAware() {}
        
        /**
         * Called before marshalling is started to prepare the object to be
         * marshalled.
         * @param wireFormat - the wireformat object to control marshaling
         */
        virtual void BeforeMarshall( OpenWireFormat* wireFormat ) = 0;

        /**
         * Called after marshalling is started to cleanup the object being
         * marshalled.
         * @param wireFormat - the wireformat object to control marshaling
         */
        virtual void AfterMarshall( OpenWireFormat* wireFormat ) = 0;
        
        /**
         * Called before unmarshalling is started to prepare the object to be
         * unmarshalled.
         * @param wireFormat - the wireformat object to control unmarshaling
         */
        virtual void BeforeUnmarshall( OpenWireFormat* wireFormat ) = 0;

        /**
         * Called after unmarshalling is started to cleanup the object being
         * unmarshalled.
         * @param wireFormat - the wireformat object to control unmarshaling
         */
        virtual void AfterUnmarshall( OpenWireFormat* wireFormat ) = 0;
        
        /**
         * Called to set the data to this object that will contain the objects
         * marshalled form.
         * @param wireFormat - the wireformat object to control unmarshaling
         * @param data - vector of object binary data
         */ 
        virtual void SetMarshalledForm( OpenWireFormat* wireFormat, 
                                        std::vector<char>& data ) = 0;

        /**
         * Called to get the data to this object that will contain the objects
         * marshalled form.
         * @param wireFormat - the wireformat object to control unmarshaling
         * @return buffer that holds the objects data.
         */ 
        virtual std::vector<unsigned char> GetMarshalledForm( OpenWireFormat* wireFormat ) = 0;

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_MARSHALAWARE_H_*/
