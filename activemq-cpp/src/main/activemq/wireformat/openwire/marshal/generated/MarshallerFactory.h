/*
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
#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSAHAL_GENERATED_MARSHALERFACTORY_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSAHAL_GENERATED_MARSHALERFACTORY_H_

//       Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/wireformat/openwire/OpenWireFormat.h>

namespace activemq{
namespace wireformat{
namespace openwire{
namespace marshal{
namespace generated{

    /**
     * Used to create marshallers for a specific version of the wire
     * protocol.
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Groovy scripts
     *         in the activemq-openwire-generator module
     */
    class MarshallerFactory {
    public:

        virtual ~MarshallerFactory() {};

        virtual void configure( OpenWireFormat* format );

    };

}}}}}

#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_GENERATED_MARSHALLERFACTORY_H_*/
