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
#include <activemq/connector/openwire/commands/ActiveMQMapMessage.h>
#include <activemq/connector/openwire/marshal/PrimitiveMapMarshaller.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace activemq::connector::openwire::marshal;

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage::ActiveMQMapMessage() :
    ActiveMQMessageBase<cms::MapMessage>()
{
    this->map = NULL;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage::~ActiveMQMapMessage()
{
    delete map;
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQMapMessage::getDataStructureType() const
{
    return ActiveMQMapMessage::ID_ACTIVEMQMAPMESSAGE; 
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::beforeMarshall( OpenWireFormat* wireFormat AMQCPP_UNUSED ) {

    try{ 
  
        if( map != NULL && !map->isEmpty() ) {        
            // Marshal as Content.
            PrimitiveMapMarshaller::marshal( map, getContent() );        
        } else {
            clearBody();
        }
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
util::PrimitiveMap& ActiveMQMapMessage::getMap() throw ( NullPointerException ) {

    try{ 
        
        if( map == NULL ) {
            
            if( getContent().size() == 0 ){
                map = new PrimitiveMap;
            } else {
                map = PrimitiveMapMarshaller::unmarshal( getContent() );
            }
            
            if( map == NULL ) {
                throw NullPointerException( 
                    __FILE__,
                    __LINE__,
                    "ActiveMQMapMessage::getMap() - All attempts to create a " 
                    "map have fialed." );
            }    
        }
        
        return *map;
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}
