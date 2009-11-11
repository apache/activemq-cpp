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
#include <activemq/commands/ActiveMQTextMessage.h>

#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <activemq/wireformat/openwire/utils/OpenwireStringSupport.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::utils;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
ActiveMQTextMessage::ActiveMQTextMessage() :
    ActiveMQMessageTemplate<cms::TextMessage>() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTextMessage::~ActiveMQTextMessage() {
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQTextMessage::getDataStructureType() const {
    return ActiveMQTextMessage::ID_ACTIVEMQTEXTMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTextMessage* ActiveMQTextMessage::cloneDataStructure() const {
    std::auto_ptr<ActiveMQTextMessage> message( new ActiveMQTextMessage() );
    message->copyDataStructure( this );
    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::copyDataStructure( const DataStructure* src ) {
    ActiveMQMessageTemplate<cms::TextMessage>::copyDataStructure( src );
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQTextMessage::toString() const {
    std::ostringstream stream;

    stream << "Begin Class = ActiveMQTextMessage" << std::endl;
    stream << ActiveMQMessageTemplate<cms::TextMessage>::toString();
    stream << "End Class = ActiveMQTextMessage" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTextMessage::equals( const DataStructure* value ) const {
    return ActiveMQMessageTemplate<cms::TextMessage>::equals( value );
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQTextMessage::getText() const throw( cms::CMSException ) {

    try{
        if( getContent().size() <= 4 ) {
            return "";
        }

        decaf::io::ByteArrayInputStream bais( getContent() );
        decaf::io::DataInputStream dataIn( &bais );

        return OpenwireStringSupport::readString( dataIn );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::setText( const char* msg ) throw( cms::CMSException ) {
    try{
        setText( std::string(msg) );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::setText( const std::string& msg ) throw( cms::CMSException ) {
    try{
        failIfReadOnlyBody();
        std::vector<unsigned char>& content = getContent();
        content.clear();
        decaf::io::ByteArrayOutputStream bos( content );
        decaf::io::DataOutputStream dataOut( &bos );

        OpenwireStringSupport::writeString( dataOut, &msg );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
