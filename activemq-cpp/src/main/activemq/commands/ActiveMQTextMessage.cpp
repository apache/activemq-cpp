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
#include <activemq/util/CMSExceptionSupport.h>
#include <cms/CMSException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace activemq::util;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::utils;
using namespace decaf::io;
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

    if( this == src ) {
        return;
    }

    const ActiveMQTextMessage* srcPtr = dynamic_cast<const ActiveMQTextMessage*>( src );

    if( srcPtr == NULL || src == NULL ) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ActiveMQTextMessage::copyDataStructure - src is NULL or invalid" );
    }

    if( srcPtr->text.get() != NULL ) {
        this->text.reset( new std::string( *( srcPtr->text.get() ) ) );
    }

    ActiveMQMessageTemplate<cms::TextMessage>::copyDataStructure( src );
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQTextMessage::toString() const {

    try {

        std::string text = getText();

        if( text != "" && text.length() > 63 ) {

            text = text.substr( 0, 45 ) + "..." + text.substr( text.length() - 12 );
            return ActiveMQMessageTemplate<cms::TextMessage>::toString() + "Text = " + text;
        }

    } catch( cms::CMSException& e ) {
    }

    return ActiveMQMessageTemplate<cms::TextMessage>::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTextMessage::equals( const DataStructure* value ) const {
    return ActiveMQMessageTemplate<cms::TextMessage>::equals( value );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::clearBody() throw( cms::CMSException ) {
    ActiveMQMessageTemplate<cms::TextMessage>::clearBody();
    this->text.reset( NULL );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::beforeMarshal( wireformat::WireFormat* wireFormat )
    throw ( decaf::io::IOException ) {

    ActiveMQMessageTemplate<cms::TextMessage>::beforeMarshal( wireFormat );

    if( this->text.get() != NULL ) {

        ByteArrayOutputStream bytesOut;
        DataOutputStream dataOut( &bytesOut );

        OpenwireStringSupport::writeString( dataOut, this->text.get() );

        dataOut.close();

        this->setContent( bytesOut.toByteArrayRef() );
        this->text.reset( NULL );
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQTextMessage::getSize() const {

    if( this->text.get() != NULL ) {
        unsigned int size = commands::Message::DEFAULT_MESSAGE_SIZE;

        size += (unsigned int)getMarshalledProperties().size();
        size += (unsigned int)this->text->size() * 2;

        return size;
    }

    return ActiveMQMessageTemplate<cms::TextMessage>::getSize();
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQTextMessage::getText() const throw( cms::CMSException ) {

    try{

        if( this->text.get() != NULL ) {
            return *( this->text.get() );
        } else {

            if( this->getContent().size() <= 4 ) {
                return "";
            }

            try {

                decaf::io::ByteArrayInputStream bais( getContent() );
                decaf::io::DataInputStream dataIn( &bais );

                dataIn.close();

                this->text.reset( new std::string( OpenwireStringSupport::readString( dataIn ) ) );

            } catch( IOException& ioe ) {
                throw CMSExceptionSupport::create( ioe );
            }
        }

        return *( this->text.get() );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::setText( const char* msg )
    throw( cms::MessageNotWriteableException, cms::CMSException ) {

    try{
        setText( std::string(msg) );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTextMessage::setText( const std::string& msg )
    throw( cms::MessageNotWriteableException, cms::CMSException ) {

    try{
        failIfReadOnlyBody();
        this->text.reset( new std::string( msg ) );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
