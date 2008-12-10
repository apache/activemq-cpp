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

#include "MessagePropertyInterceptor.h"

#include <decaf/lang/Integer.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <cms/DeliveryMode.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::utils;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
MessagePropertyInterceptor::MessagePropertyInterceptor(
    commands::Message* message, PrimitiveMap* properties )
        throw( decaf::lang::exceptions::NullPointerException ) {

    if( message == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Message passed was NULL" );
    }

    if( properties == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "PrimitiveMap passed was NULL" );
    }

    this->message = message;
    this->properties = properties;
}

////////////////////////////////////////////////////////////////////////////////
MessagePropertyInterceptor::~MessagePropertyInterceptor() {
}

////////////////////////////////////////////////////////////////////////////////
bool MessagePropertyInterceptor::getBooleanProperty( const std::string& name ) const
	throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXDeliveryCount" || name == "JMSXGroupID" || name == "JMSXGroupSeq" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        }

        return this->properties->getBool( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char MessagePropertyInterceptor::getByteProperty( const std::string& name ) const
    throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXDeliveryCount" || name == "JMSXGroupID" || name == "JMSXGroupSeq" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        }

        return this->properties->getByte( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
double MessagePropertyInterceptor::getDoubleProperty( const std::string& name ) const
    throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXDeliveryCount" || name == "JMSXGroupID" || name == "JMSXGroupSeq" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        }

        return this->properties->getDouble( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
float MessagePropertyInterceptor::getFloatProperty( const std::string& name ) const
    throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXDeliveryCount" || name == "JMSXGroupID" || name == "JMSXGroupSeq" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        }

        return this->properties->getFloat( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
int MessagePropertyInterceptor::getIntProperty( const std::string& name ) const
    throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXGroupID" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        } else if( name == "JMSXDeliveryCount" ) {
            return this->message->getRedeliveryCounter();
        } else if( name == "JMSXGroupSeq" ) {
            return this->message->getGroupSequence();
        }

        return this->properties->getInt( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
long long MessagePropertyInterceptor::getLongProperty( const std::string& name ) const
    throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXGroupID" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        } else if( name == "JMSXDeliveryCount" ) {
            return (long long)this->message->getRedeliveryCounter();
        } else if( name == "JMSXGroupSeq" ) {
            return (long long)this->message->getGroupSequence();
        }

        return this->properties->getLong( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
short MessagePropertyInterceptor::getShortProperty( const std::string& name ) const
    throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXDeliveryCount" || name == "JMSXGroupID" || name == "JMSXGroupSeq" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        }

        return this->properties->getShort( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string MessagePropertyInterceptor::getStringProperty( const std::string& name ) const
    throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXGroupID" ) {
            return this->message->getGroupID();
        } else if( name == "JMSXDeliveryCount" ) {
            return Integer::toString( this->message->getRedeliveryCounter() );
        } else if( name == "JMSXGroupSeq" ) {
            return Integer::toString( this->message->getGroupSequence() );
        }

        return this->properties->getString( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void MessagePropertyInterceptor::setBooleanProperty( const std::string& name, bool value )
	throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXDeliveryCount" || name == "JMSXGroupID" || name == "JMSXGroupSeq" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        }

        this->properties->setBool( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void MessagePropertyInterceptor::setByteProperty( const std::string& name, unsigned char value )
	throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXDeliveryCount" || name == "JMSXGroupID" || name == "JMSXGroupSeq" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        }

        this->properties->setByte( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void MessagePropertyInterceptor::setDoubleProperty( const std::string& name, double value )
	throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXDeliveryCount" || name == "JMSXGroupID" || name == "JMSXGroupSeq" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        }

        this->properties->setDouble( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void MessagePropertyInterceptor::setFloatProperty( const std::string& name, float value )
	throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXDeliveryCount" || name == "JMSXGroupID" || name == "JMSXGroupSeq" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        }

        this->properties->setFloat( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void MessagePropertyInterceptor::setIntProperty( const std::string& name, int value )
    throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXGroupID" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        } else if( name == "JMSXDeliveryCount" ) {
            this->message->setRedeliveryCounter( value );
        } else if( name == "JMSXGroupSeq" ) {
            this->message->setGroupSequence( value );
        }

        this->properties->setInt( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void MessagePropertyInterceptor::setLongProperty( const std::string& name, long long value )
    throw( activemq::exceptions::ActiveMQException ) {

    try{
        this->properties->setLong( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void MessagePropertyInterceptor::setShortProperty( const std::string& name, short value )
    throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXGroupID" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot Convert Reserved Property to this Type." );
        } else if( name == "JMSXDeliveryCount" ) {
            this->message->setRedeliveryCounter( (int)value );
        } else if( name == "JMSXGroupSeq" ) {
            this->message->setGroupSequence( (int)value );
        }

        this->properties->setShort( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void MessagePropertyInterceptor::setStringProperty( const std::string& name,
                                                    const std::string& value )
    throw( activemq::exceptions::ActiveMQException ) {

    try{

        if( name == "JMSXGroupID" ) {
            this->message->setGroupID( value );
        } else if( name == "JMSXDeliveryCount" ) {
            this->message->setRedeliveryCounter( Integer::parseInt( value ) );
        } else if( name == "JMSXGroupSeq" ) {
            this->message->setGroupSequence( Integer::parseInt( value ) );
        }

        this->properties->setString( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}
