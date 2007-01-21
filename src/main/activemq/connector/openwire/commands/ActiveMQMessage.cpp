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
#include <activemq/connector/openwire/commands/ActiveMQMessage.h>
#include <activemq/connector/openwire/OpenWireFormat.h>
#include <activemq/util/Config.h>

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessage::ActiveMQMessage()
{}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessage::~ActiveMQMessage()
{}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQMessage::getDataStructureType() const {
    return ActiveMQMessage::ID_ACTIVEMQMESSAGE; 
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessage::beforeMarshall( OpenWireFormat* wireFormat ACTIVEMQ_ATTRIBUTE_UNUSED )
{
//    marshalledProperties.clear();
//    if( !properties.empty() )
//    {
//        properties.Marshal( marshalledProperties );
//    }
}
