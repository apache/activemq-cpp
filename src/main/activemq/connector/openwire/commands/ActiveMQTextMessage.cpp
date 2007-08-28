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

#include <activemq/io/ByteArrayOutputStream.h>
#include <activemq/io/DataOutputStream.h>

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
        if( getContent().size() <= 4 ) {
            return "";
        }

        return std::string( (const char*)&getContent()[4], getContent().size()-4 );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::setText( const char* msg ) throw( cms::CMSException ) {
    try{
        setText( std::string(msg) );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::setText( const std::string& msg ) throw( cms::CMSException ) {
    try{
        std::vector<unsigned char>& content = getContent();
        content.clear();
        io::ByteArrayOutputStream bos( content );
        io::DataOutputStream dos( &bos );

        dos.writeInt( (int)msg.length() );
        dos.write( (const unsigned char*)msg.c_str(), msg.length() );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}
