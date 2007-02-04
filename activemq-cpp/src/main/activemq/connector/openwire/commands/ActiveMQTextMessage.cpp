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
#include <activemq/connector/openwire/commands/ActiveMQTextMessage.h>

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQTextMessage::ActiveMQTextMessage() :
    ActiveMQMessageBase<cms::TextMessage>()
{
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTextMessage::~ActiveMQTextMessage()
{
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQTextMessage::getDataStructureType() const
{
    return ActiveMQTextMessage::ID_ACTIVEMQTEXTMESSAGE; 
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQTextMessage::getText() const throw( cms::CMSException ) {

    try{
        return std::string( (const char*)&getContent()[0], getContent().size() ); 
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::setText( const char* msg ) throw( cms::CMSException ) {
    try{
        size_t length = strlen( msg );
        for( size_t i = 0; i < length; ++i ){
            getContent().push_back( msg[i] );
        }
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::setText( const std::string& msg ) throw( cms::CMSException ) {
    try{
        for( size_t i = 0; i < msg.length(); ++i ){
            getContent().push_back( msg[i] );
        }
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}
