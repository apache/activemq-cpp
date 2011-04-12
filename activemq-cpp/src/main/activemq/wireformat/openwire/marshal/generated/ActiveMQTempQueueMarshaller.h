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

#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSAHAL_GENERATED_ACTIVEMQTEMPQUEUEMARSHALLER_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSAHAL_GENERATED_ACTIVEMQTEMPQUEUEMARSHALLER_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/wireformat/openwire/marshal/generated/ActiveMQTempDestinationMarshaller.h>

#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/io/IOException.h>
#include <activemq/util/Config.h>
#include <activemq/commands/DataStructure.h>
#include <activemq/wireformat/openwire/OpenWireFormat.h>
#include <activemq/wireformat/openwire/utils/BooleanStream.h>

namespace activemq{
namespace wireformat{
namespace openwire{
namespace marshal{
namespace generated{

    /**
     * Marshaling code for Open Wire Format for ActiveMQTempQueueMarshaller
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-openwire-generator module
     */
    class AMQCPP_API ActiveMQTempQueueMarshaller : public ActiveMQTempDestinationMarshaller {
    public:

        ActiveMQTempQueueMarshaller() {}
        virtual ~ActiveMQTempQueueMarshaller() {}

        virtual commands::DataStructure* createObject() const;

        virtual unsigned char getDataStructureType() const;

        virtual void tightUnmarshal( OpenWireFormat* wireFormat,
                                     commands::DataStructure* dataStructure,
                                     decaf::io::DataInputStream* dataIn,
                                     utils::BooleanStream* bs );

        virtual int tightMarshal1( OpenWireFormat* wireFormat,
                                   commands::DataStructure* dataStructure,
                                   utils::BooleanStream* bs );

        virtual void tightMarshal2( OpenWireFormat* wireFormat,
                                    commands::DataStructure* dataStructure,
                                    decaf::io::DataOutputStream* dataOut,
                                    utils::BooleanStream* bs );

        virtual void looseUnmarshal( OpenWireFormat* wireFormat,
                                     commands::DataStructure* dataStructure,
                                     decaf::io::DataInputStream* dataIn );

        virtual void looseMarshal( OpenWireFormat* wireFormat,
                                   commands::DataStructure* dataStructure,
                                   decaf::io::DataOutputStream* dataOut );

    };

}}}}}

#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSAHAL_GENERATED_ACTIVEMQTEMPQUEUEMARSHALLER_H_*/

